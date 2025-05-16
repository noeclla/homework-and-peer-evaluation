#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DANCES 6
#define MAX_STEPS 20
#define MAX_MEMBERS 10
#define STEP_NAME_LEN 30
#define DANCE_NAME_LEN 50
#define NICKNAME_LEN 20

typedef struct {
    char korean[DANCE_NAME_LEN];
    char english[DANCE_NAME_LEN];
    char steps[MAX_STEPS][STEP_NAME_LEN];
    int stepCount;
} Dance;

typedef struct {
    char name[30];
    char nickname[NICKNAME_LEN];
    int danceScore;
} Member;

Dance dances[MAX_DANCES];
Member members[MAX_MEMBERS] = {
    {"Alice Kim", "alice", 0},
    {"Bob Lee", "bob", 0},
    // add more members as needed
};

void shuffleSteps(char steps[][STEP_NAME_LEN], int count) {
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[STEP_NAME_LEN];
        strcpy(temp, steps[i]);
        strcpy(steps[i], steps[j]);
        strcpy(steps[j], temp);
    }
}

int compareSteps(char input[][STEP_NAME_LEN], char correct[][STEP_NAME_LEN], int count) {
    int exact = 1, allPresent = 1, anyMatch = 0;
    // Check exact order
    for (int i = 0; i < count; i++) {
        if (strcmp(input[i], correct[i]) != 0) {
            exact = 0;
            break;
        }
    }
    if (exact) return 100;
    // Check all steps present (ignore order)
    for (int i = 0; i < count; i++) {
        int found = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(input[i], correct[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) allPresent = 0;
        else anyMatch = 1;
    }
    if (allPresent) return 50;
    if (anyMatch) return 20;
    return 0;
}

int findMemberByNickname(char *nickname) {
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (strcmp(members[i].nickname, nickname) == 0) return i;
    }
    return -1;
}

void learnDanceStep() {
    char inputNick[NICKNAME_LEN];
    int memberIndex = -1, tries = 0;

    while (tries < 3) {
        printf("Enter member nickname: ");
        scanf("%s", inputNick);
        memberIndex = findMemberByNickname(inputNick);
        if (memberIndex != -1) break;
        tries++;
    }
    if (memberIndex == -1) {
        printf("Nickname not found. Returning to menu.\n");
        return;
    }

    // TODO: Load dances from file dance_step.txt
    // For now, dummy data:
    strcpy(dances[0].english, "Basic Step 1");
    dances[0].stepCount = 3;
    strcpy(dances[0].steps[0], "Step1");
    strcpy(dances[0].steps[1], "Step2");
    strcpy(dances[0].steps[2], "Step3");

    // Randomly pick a dance
    srand(time(NULL));
    int danceIdx = rand() % MAX_DANCES;

    // Shuffle steps
    char shuffled[MAX_STEPS][STEP_NAME_LEN];
    for (int i = 0; i < dances[danceIdx].stepCount; i++) {
        strcpy(shuffled[i], dances[danceIdx].steps[i]);
    }
    shuffleSteps(shuffled, dances[danceIdx].stepCount);

    // Show dance name and steps (bonus: for 10 seconds, then clear)
    printf("Dance: %s\n", dances[danceIdx].english);
    for (int i = 0; i < dances[danceIdx].stepCount; i++) {
        printf("%s ", dances[danceIdx].steps[i]);
    }
    printf("\n");
    // sleep(10); system("clear"); // implement sleep and clear for your platform

    // Show shuffled steps
    printf("Shuffled steps: ");
    for (int i = 0; i < dances[danceIdx].stepCount; i++) {
        printf("%s ", shuffled[i]);
    }
    printf("\n");

    // Get user input
    char userInput[MAX_STEPS][STEP_NAME_LEN];
    printf("Enter steps in order:\n");
    for (int i = 0; i < dances[danceIdx].stepCount; i++) {
        scanf("%s", userInput[i]);
    }

    // Score user input
    int score = compareSteps(userInput, dances[danceIdx].steps, dances[danceIdx].stepCount);
    printf("Correct steps: ");
    for (int i = 0; i < dances[danceIdx].stepCount; i++) {
        printf("%s ", dances[danceIdx].steps[i]);
    }
    printf("\nYour input: ");
    for (int i = 0; i < dances[danceIdx].stepCount; i++) {
        printf("%s ", userInput[i]);
    }
    printf("\nScore: %d\n", score);

    members[memberIndex].danceScore = score; // Save score
}

int main() {
    learnDanceStep();
    return 0;
}
