#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 512
#define MAX_NAME 100
#define MAX_DETAIL 1000

typedef struct Subjects {
    char name[MAX_NAME];
    char details[MAX_DETAIL];
    struct Subjects *next;
} Subjects;

Subjects *head = NULL;

const char *subjectNames[] = {
    "Acting Theory",
    "Stage Theory",
    "Script Analysis",
    "Dialogue Interpretation and Emotional Expression",
    "Character Development",
    "Storytelling Theory",
    "Stage Movement and Poses"
};

#define SUBJECT_COUNT (sizeof(subjectNames)/sizeof(subjectNames[0]))

void initSubjectsList() {
    Subjects *prev = NULL;
    for(int i=0; i<SUBJECT_COUNT; i++) {
        Subjects *node = (Subjects*)malloc(sizeof(Subjects));
        if(!node) {
            printf("Memory allocation failed\n");
            exit(1);
        }
        strncpy(node->name, subjectNames[i], MAX_NAME-1);
        node->name[MAX_NAME-1] = '\0';
        node->details[0] = '\0';
        node->next = NULL;

        if(prev == NULL) {
            head = node;
        } else {
            prev->next = node;
        }
        prev = node;
    }
}

void UpdateSubjectInfo(char *line) {
    Subjects *cur = head;
    while(cur) {
        // If subject name found anywhere in the line, append the line to details
        if(strstr(line, cur->name) != NULL) {
            // Append line with a space if details not empty
            if(cur->details[0] != '\0') {
                strncat(cur->details, " ", MAX_DETAIL - strlen(cur->details) -1);
            }
            strncat(cur->details, line, MAX_DETAIL - strlen(cur->details) -1);
            break;  // assume one subject per line
        }
        cur = cur->next;
    }
}

int LoadContentFile() {
    FILE *fp = fopen("perform_stage.txt", "r");
    if(!fp) {
        printf("Error: Cannot open perform_stage.txt\n");
        return 0;
    }

    char line[MAX_LINE];
    while(fgets(line, sizeof(line), fp)) {
        // Remove newline
        size_t len = strlen(line);
        if(len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[len-1] = '\0';

        if(strlen(line) == 0) continue; // skip blank lines

        UpdateSubjectInfo(line);
    }
    fclose(fp);
    return 1;
}

void PrintSubjectList() {
    Subjects *cur = head;
    int idx = 1;
    while(cur) {
        printf("%d. %s\n", idx++, cur->name);
        char *p = cur->details;
        while(*p) {
            putchar(*p);
            if(*p == '.') putchar('\n');
            p++;
        }
        putchar('\n');
        cur = cur->next;
    }
}

void insertSubject(const char *name, const char *details, int pos) {
    Subjects *node = (Subjects*)malloc(sizeof(Subjects));
    if(!node) {
        printf("Memory allocation failed\n");
        return;
    }
    strncpy(node->name, name, MAX_NAME-1);
    node->name[MAX_NAME-1] = '\0';
    strncpy(node->details, details, MAX_DETAIL-1);
    node->details[MAX_DETAIL-1] = '\0';
    node->next = NULL;

    if(pos <= 1 || head == NULL) { // Insert at head
        node->next = head;
        head = node;
        return;
    }

    Subjects *cur = head;
    int i = 1;
    while(cur->next && i < pos -1) {
        cur = cur->next;
        i++;
    }
    node->next = cur->next;
    cur->next = node;
}

void removeSubject(int pos) {
    if(head == NULL) return;
    if(pos <= 1) { // remove head
        Subjects *tmp = head;
        head = head->next;
        free(tmp);
        return;
    }
    Subjects *cur = head;
    int i=1;
    while(cur->next && i < pos -1) {
        cur = cur->next;
        i++;
    }
    if(cur->next) {
        Subjects *tmp = cur->next;
        cur->next = tmp->next;
        free(tmp);
    }
}

void learnPerformanceStageTheory() {
    initSubjectsList();

    if(!LoadContentFile()) return;

    printf("\n--- Acting and Stage Theory Subjects ---\n");
    PrintSubjectList();

    printf("\n--- After inserting a new subject at position 3 ---\n");
    insertSubject("Improvisation Techniques", "Learn to adapt quickly on stage.", 3);
    PrintSubjectList();

    printf("\n--- After removing subject at position 2 ---\n");
    removeSubject(2);
    PrintSubjectList();
}

int main() {
    learnPerformanceStageTheory();
    return 0;
}
