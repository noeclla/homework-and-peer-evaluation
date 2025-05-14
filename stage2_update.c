#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRAUMAS 8
#define MAX_RESPONSES 24
#define MAX_LEN 101

typedef struct {
    char nickname[8];
    int age;
    char description[256];
} Trauma;

typedef struct {
    int id;
    char question[128];
} CounselingQuestion;

typedef struct {
    char nickname[8];
    char question[128];
    char response[MAX_LEN];
} CounselingResponse;

Trauma traumaList[MAX_TRAUMAS];
int traumaCount = 0;
CounselingResponse responses[MAX_RESPONSES];
int responseCount = 0;

const CounselingQuestion questions[5] = {
    {1, "In what situations have you experienced this trauma?"},
    {2, "How has this situation affected your daily life and emotions?"},
    {3, "How have you tried to overcome this trauma?"},
    {4, "What emotions do you associate with this trauma?"},
    {5, "What kind of support do you think is necessary to overcome this trauma?"}
};

void inputTrauma() {
    char nickname[8];
    int age;
    char desc[256];

    while (1) {
        printf("Enter nickname (or '0' to return): ");
        scanf("%s", nickname);
        if (nickname[0] == '0') break;

        int idx = -1;
        for (int i = 0; i < MAX_TRAUMAS; i++)
            if (strcmp(traumaList[i].nickname, nickname) == 0) { idx = i; break; }

        (idx != -1) ? printf("Trauma already recorded.\n")
                    : ({ 
                        printf("Age: "); scanf("%d", &age);
                        printf("Describe trauma: "); getchar(); fgets(desc, sizeof(desc), stdin);
                        desc[strcspn(desc, "\n")] = 0;

                        strcpy(traumaList[traumaCount].nickname, nickname);
                        traumaList[traumaCount].age = age;
                        strcpy(traumaList[traumaCount].description, desc);
                        traumaCount++;
                    });
    }
}

void counselingSession() {
    if (traumaCount == 0) { printf("No trauma data available.\n"); return; }

    printf("Available members:\n");
    for (int i = 0; i < traumaCount; i++)
        printf("- %s\n", traumaList[i].nickname);

    char nickname[8];
    printf("Select nickname: ");
    scanf("%s", nickname);

    int valid = 0;
    for (int i = 0; i < traumaCount; i++)
        if (strcmp(traumaList[i].nickname, nickname) == 0) valid = 1;
    if (!valid) { printf("Not found.\n"); return; }

    int used[5] = {0}, asked = 0;
    while (asked < 3) {
        int q;
        do { q = rand() % 5; } while (used[q]);
        used[q] = 1;

        char ans[MAX_LEN];
        do {
            printf("Q%d: %s\n> ", questions[q].id, questions[q].question);
            getchar(); fgets(ans, sizeof(ans), stdin);
            ans[strcspn(ans, "\n")] = 0;
        } while (strlen(ans) == 0 || strlen(ans) > 100);

        strcpy(responses[responseCount].nickname, nickname);
        strcpy(responses[responseCount].question, questions[q].question);
        strcpy(responses[responseCount].response, ans);
        responseCount++;
        asked++;
    }

    printf("Session complete for %s.\n", nickname);
}

void showCounselingResults() {
    printf("\n--- Counseling Results ---\n");
    for (int i = 0; i < traumaCount; i++) {
        printf("\nNickname: %s\n", traumaList[i].nickname);
        printf("Trauma: %s\n", traumaList[i].description);
        for (int j = 0; j < responseCount; j++)
            if (strcmp(responses[j].nickname, traumaList[i].nickname) == 0)
                printf("Q: %s\nA: %s\n", responses[j].question, responses[j].response);
    }
}

void overcomeTrauma() {
    char choice[4];
    while (1) {
        printf("\n[B. Trauma Management]\n");
        printf("[1] Input Trauma\n[2] Counseling Session\n[3] View Results\n[0] Back\n> ");
        fgets(choice, sizeof(choice), stdin);
        switch (choice[0]) {
            case '1': inputTrauma(); break;
            case '2': counselingSession(); break;
            case '3': showCounselingResults(); break;
            case '0': return;
            default: printf("Invalid choice.\n");
        }
    }
}

int main() {
    srand((unsigned)time(NULL));
    char input[4];
    while (1) {
        printf("\n=== Magrathea Training System ===\n");
        printf("[1] Trauma Management\n[0] Exit\n> ");
        fgets(input, sizeof(input), stdin);
        if (input[0] == '0') break;
        else if (input[0] == '1') overcomeTrauma();
        else printf("Invalid input. Try again.\n");
    }
    return 0;
}
