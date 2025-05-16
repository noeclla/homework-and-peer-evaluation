#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEMBERS_COUNT 5
#define WORDS_PER_LEVEL 10
#define TOTAL_WORDS 30
#define MAX_ANSWER_LEN 100

typedef struct {
    char word[50];
    char description[200];
    int level;  
} WordCard;

typedef struct {
    char name[30];
    int level;
    int score;
    int passStatus; 
    int passDate;  
} MyChallenge;

const char *members[MEMBERS_COUNT] = {
    "Alice", "Bob", "Charlie", "Diana", "Evan"
};

WordCard dictionary[TOTAL_WORDS];
MyChallenge challenges[MEMBERS_COUNT];
int selectedPlayers[MEMBERS_COUNT];
int selectedCount = 0;

void initDictionary() {
    for (int i = 0; i < TOTAL_WORDS; i++) {
        sprintf(dictionary[i].word, "Word%d", i + 1);
        sprintf(dictionary[i].description, "Description for word %d", i + 1);
        dictionary[i].level = i / WORDS_PER_LEVEL + 1;
    }
}

int randomInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

int selectMember() {
    if (selectedCount == MEMBERS_COUNT) return -1;
    int idx;
    do {
        idx = randomInRange(0, MEMBERS_COUNT - 1);
        int used = 0;
        for (int i = 0; i < selectedCount; i++) {
            if (selectedPlayers[i] == idx) {
                used = 1; break;
            }
        }
        if (!used) {
            selectedPlayers[selectedCount++] = idx;
            return idx;
        }
    } while (1);
}

void challengeHighLevel(int playerIndex) {
    int correct = 0;
    printf("Challenge Highest Level for %s\n", members[playerIndex]);
    for (int i = 0; i < 5; i++) {
        int wordIndex;
        do {
            wordIndex = randomInRange(20, 29);
        } while (wordIndex < 20 || wordIndex >= 30);
        printf("Describe: %s\nYour answer: ", dictionary[wordIndex].description);
        char answer[MAX_ANSWER_LEN];
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0;
        if (strcasecmp(answer, dictionary[wordIndex].word) == 0) correct++;
    }
    if (correct == 5) {
        challenges[playerIndex].passStatus = 4; 
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        challenges[playerIndex].passDate = (tm.tm_year + 1900)*10000 + (tm.tm_mon+1)*100 + tm.tm_mday;
        printf("You passed the highest challenge!\n");
    } else {
        printf("Challenge failed. All players must retry.\n");
    }
}

void challengeStepByStep(int playerIndex) {
    int currentLevel = challenges[playerIndex].level;
    if (currentLevel > 3) {
        printf("You already passed all levels!\n");
        return;
    }
    int correct = 0;
    printf("Step by Step Challenge Level %d for %s\n", currentLevel, members[playerIndex]);
    int start = (currentLevel - 1) * WORDS_PER_LEVEL;
    for (int i = 0; i < 3; i++) {
        int wordIndex = start + i;
        printf("Describe: %s\nYour answer: ", dictionary[wordIndex].description);
        char answer[MAX_ANSWER_LEN];
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0;
        if (strcasecmp(answer, dictionary[wordIndex].word) == 0) correct++;
    }
    if (correct >= 2) {
        printf("You passed level %d!\n", currentLevel);
        challenges[playerIndex].level++;
        if (challenges[playerIndex].level > 3) {
            challenges[playerIndex].passStatus = 4;
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            challenges[playerIndex].passDate = (tm.tm_year + 1900)*10000 + (tm.tm_mon+1)*100 + tm.tm_mday;
            printf("Final pass achieved! Congratulations!\n");
        }
    } else {
        printf("Challenge failed. All players must retry.\n");
    }
}

void doDictionaryGame() {
    if (selectedCount == MEMBERS_COUNT) {
        printf("All players completed the game. Restarting selection...\n");
        selectedCount = 0;
    }
    int playerIndex = selectMember();
    if (playerIndex == -1) {
        printf("No player available.\n");
        return;
    }
    printf("Selected Player: %s\n", members[playerIndex]);
    printf("Choose option:\n1. Challenge Highest Difficulty\n2. Challenge Step by Step\nChoice: ");
    int choice;
    scanf("%d%*c", &choice);
    if (choice == 1) challengeHighLevel(playerIndex);
    else if (choice == 2) challengeStepByStep(playerIndex);
    else printf("Invalid choice.\n");
}

int main() {
    srand(time(NULL));
    initDictionary();
    memset(challenges, 0, sizeof(challenges));
    for (int i = 0; i < MEMBERS_COUNT; i++) {
        challenges[i].level = 1;
        strncpy(challenges[i].name, members[i], sizeof(challenges[i].name));
    }
    while (1) {
        printf("\n=== Dictionary Game Menu ===\n");
        printf("1. Play Dictionary Game\n");
        printf("2. Exit\nChoice: ");
        int mainChoice;
        scanf("%d%*c", &mainChoice);
        if (mainChoice == 1) doDictionaryGame();
        else break;
    }
    return 0;
}
