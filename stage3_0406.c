#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define REGIONS 4
#define WORDS 5
#define TOTAL (REGIONS * WORDS)
#define MEMBERS 5

typedef struct {
    char dialect[30];
    char region[20];
    char standard[30];
} Dialect;

typedef struct {
    char groupName[30];
    char learnerName[30];
    int scoreStd;
    int scoreDialect;
    char gradeStd;
    char gradeDialect;
} Achievement;

typedef struct {
    char groupName[30];
    char memberName[30];
} Member;

Dialect dialects[TOTAL];
Member members[MEMBERS];
Achievement achievements[MEMBERS];

void initData() {
    const char *regions[REGIONS] = {"Seoul", "Busan", "Jeju", "Gyeongsang"};
    for (int i = 0; i < REGIONS; i++)
        for (int j = 0; j < WORDS; j++) {
            int idx = i * WORDS + j;
            snprintf(dialects[idx].region, 20, "%s", regions[i]);
            snprintf(dialects[idx].dialect, 30, "dialect%d_%s", j+1, regions[i]);
            snprintf(dialects[idx].standard, 30, "standard%d", j+1);
        }

    const char *names[MEMBERS] = {"Alice", "Bob", "Charlie", "Diana", "Evan"};
    const char *group = "Milize";
    for (int i = 0; i < MEMBERS; i++) {
        strcpy(members[i].groupName, group);
        strcpy(members[i].memberName, names[i]);
    }

    for (int i = 0; i < MEMBERS; i++) {
        strcpy(achievements[i].groupName, members[i].groupName);
        strcpy(achievements[i].learnerName, members[i].memberName);
        achievements[i].scoreStd = 0;
        achievements[i].scoreDialect = 0;
        achievements[i].gradeStd = 'F';
        achievements[i].gradeDialect = 'F';
    }
}

int randomRegion() {
    return rand() % REGIONS;
}

char getGrade(int score) {
    if (score >= 90) return 'A';
    else if (score >= 80) return 'B';
    else if (score >= 70) return 'C';
    return 'F';
}

void guessStdWord(int learner) {
    int r1 = randomRegion();
    int r2;
    do { r2 = randomRegion(); } while (r2 == r1);

    int correct = 0;
    printf("Guess standard words from dialects of %s and %s:\n",
           dialects[r1 * WORDS].region, dialects[r2 * WORDS].region);

    for (int i = 0; i < WORDS; i++) {
        for (int r = 0; r < 2; r++) {
            int idx = (r == 0 ? r1 : r2) * WORDS + i;
            printf("Dialect '%s' -> Standard? ", dialects[idx].dialect);
            char ans[30];
            fgets(ans, sizeof(ans), stdin);
            ans[strcspn(ans, "\n")] = 0;
            if (strcmp(ans, dialects[idx].standard) == 0) correct++;
        }
    }
    achievements[learner].scoreStd = correct * 100 / (WORDS * 2);
    achievements[learner].gradeStd = getGrade(achievements[learner].scoreStd);
}

void guessDialectWord(int learner) {
    int correct = 0;
    printf("Guess dialect words for standard words:\n");
    for (int i = 0; i < WORDS * 2; i++) {
        int r = randomRegion();
        int idx = r * WORDS + (i % WORDS);
        printf("Standard '%s', Region '%s' -> Dialect? ", dialects[idx].standard, dialects[idx].region);
        char ans[30];
        fgets(ans, sizeof(ans), stdin);
        ans[strcspn(ans, "\n")] = 0;
        if (strcmp(ans, dialects[idx].dialect) == 0) correct++;
    }
    achievements[learner].scoreDialect = correct * 100 / (WORDS * 2);
    achievements[learner].gradeDialect = getGrade(achievements[learner].scoreDialect);
}

void printResults(int learner) {
    char *msgA = "You are a %s expert.";
    char *msgB = "Your %s skill is excellent.";
    char *msgC = "You are familiar with %s.";
    char *msgF = "Study more and try again.";

    printf("\nResults for %s:\n", achievements[learner].learnerName);

    char *typeStd = "standard language";
    char *typeDialect = "dialect";

    switch (achievements[learner].gradeStd) {
        case 'A': printf(msgA, typeStd); break;
        case 'B': printf(msgB, typeStd); break;
        case 'C': printf(msgC, typeStd); break;
        default: printf(msgF); break;
    }
    printf("\n");

    switch (achievements[learner].gradeDialect) {
        case 'A': printf(msgA, typeDialect); break;
        case 'B': printf(msgB, typeDialect); break;
        case 'C': printf(msgC, typeDialect); break;
        default: printf(msgF); break;
    }
    printf("\n");
}

void learnDialect() {
    printf("Choose your learner:\n");
    for (int i = 0; i < MEMBERS; i++)
        printf("%d. %s\n", i + 1, members[i].memberName);

    int choice;
    scanf("%d%*c", &choice);
    if (choice < 1 || choice > MEMBERS) {
        printf("Invalid choice.\n");
        return;
    }
    int learner = choice - 1;

    guessStdWord(learner);
    guessDialectWord(learner);
    printResults(learner);
}

int main() {
    srand((unsigned)time(NULL));
    initData();
    learnDialect();
    return 0;
}
