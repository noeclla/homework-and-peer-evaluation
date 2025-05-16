#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#else
#include <unistd.h>
#endif

typedef struct Pose {
    char name[50];
    char description[200];
    struct Pose *prev;
    struct Pose *next;
} Pose;

Pose* loadPoses(const char *filename, int *count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        return NULL;
    }
    Pose *head = NULL, *tail = NULL;
    char line[256];
    *count = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Read name
        line[strcspn(line, "\r\n")] = 0;  // trim newline
        if (strlen(line) == 0) continue;
        Pose *newPose = malloc(sizeof(Pose));
        if (!newPose) {
            printf("Memory allocation error\n");
            fclose(fp);
            return NULL;
        }
        strncpy(newPose->name, line, sizeof(newPose->name)-1);
        newPose->name[sizeof(newPose->name)-1] = '\0';

        if (!fgets(line, sizeof(line), fp)) {
            free(newPose);
            break;
        }
        line[strcspn(line, "\r\n")] = 0;
        strncpy(newPose->description, line, sizeof(newPose->description)-1);
        newPose->description[sizeof(newPose->description)-1] = '\0';

        newPose->prev = tail;
        newPose->next = NULL;
        if (tail) tail->next = newPose;
        else head = newPose;
        tail = newPose;
        (*count)++;
    }
    fclose(fp);
    return head;
}

void freePoses(Pose *head) {
    while (head) {
        Pose *tmp = head;
        head = head->next;
        free(tmp);
    }
}

int containsPose(Pose *head, const char *name) {
    for (Pose *p = head; p; p = p->next) {
        if (strcmp(p->name, name) == 0) return 1;
    }
    return 0;
}

Pose* selectRandomPoses(Pose *allPoses, int total, int selectCount) {
    if (selectCount > total) return NULL;
    Pose *selectedHead = NULL, *selectedTail = NULL;

    srand((unsigned)time(NULL));
    while (selectCount > 0) {
        int r = rand() % total;
        Pose *p = allPoses;
        for (int i = 0; i < r; i++) p = p->next;
        if (containsPose(selectedHead, p->name)) continue;

        Pose *newNode = malloc(sizeof(Pose));
        if (!newNode) {
            freePoses(selectedHead);
            return NULL;
        }
        *newNode = *p;
        newNode->prev = selectedTail;
        newNode->next = NULL;
        if (selectedTail) selectedTail->next = newNode;
        else selectedHead = newNode;
        selectedTail = newNode;
        selectCount--;
    }
    return selectedHead;
}

void printPose(const Pose *p) {
    printf("Pose: %s\nDescription: %s\n", p->name, p->description);
}

int askYesNo(const char *prompt) {
    char answer[10];
    printf("%s (yes/no): ", prompt);
    if (fgets(answer, sizeof(answer), stdin)) {
        for (char *c = answer; *c; c++) *c = (char)tolower(*c);
        if (strncmp(answer, "yes", 3) == 0) return 1;
    }
    return 0;
}

void practicePoses(Pose *head, int count) {
    if (!head) return;
    printf("\nStarting pose practice...\n");
    do {
        for (int round = 1; round <= 2; round++) {
            printf("\nRound %d\n", round);
            for (Pose *p = head; p; p = p->next) {
                printPose(p);
                printf("Hold this pose for 30 seconds...\n");
                sleep(3); 
            }
        }
    } while (askYesNo("Practice again?"));
    printf("Practice finished.\n");
}

int main(void) {
    int totalPoses = 0;
    Pose *allPoses = loadPoses("pose.txt", &totalPoses);
    if (!allPoses) return 1;

    int poseCount;
    printf("Enter number of poses to practice (3-5): ");
    if (scanf("%d", &poseCount) != 1 || poseCount < 3 || poseCount > 5) {
        printf("Invalid input.\n");
        freePoses(allPoses);
        return 1;
    }
    while (getchar() != '\n'); // clear newline

    Pose *practiceList = selectRandomPoses(allPoses, totalPoses, poseCount);
    if (!practiceList) {
        printf("Failed to select poses.\n");
        freePoses(allPoses);
        return 1;
    }

    practicePoses(practiceList, poseCount);

    freePoses(allPoses);
    freePoses(practiceList);
    return 0;
}

