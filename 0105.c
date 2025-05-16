
#include <stdlib.h>
#include <string.h>

#define NUM_CANDIDATES 6
#define NUM_CATEGORIES 5

typedef struct {
    char name[50];
    int id;
} Candidate;

Candidate candidates[NUM_CANDIDATES] = {
    {"Jiyeon Park", 100001},
    {"Ethan Smith", 100002},
    {"Helena Silva", 100003},
    {"Liam Wilson", 100004},
    {"Soojin Lee", 100005},
    {"Carlos Gomez", 100006}
};

int scores[NUM_CANDIDATES][NUM_CATEGORIES + 2]; // [ID, 5 Scores, Total]

const char *categories[NUM_CATEGORIES] = {
    "Music", "Dance", "Vocal", "Visual", "Expression"
};

void input_scores() {
    char judge[50], area[50];
    int s;

    printf(">> Judge Name: "); scanf(" %[^\n]", judge);
    printf(">> Expertise : "); scanf(" %[^\n]", area);

    for (int i = 0; i < NUM_CANDIDATES; i++) {
        printf("\nCandidate: %s\n", candidates[i].name);
        scores[i][0] = candidates[i].id;

        for (int j = 0; j < NUM_CATEGORIES; j++) {
            do {
                printf("%s (10-100): ", categories[j]);
                scanf("%d", &s);
            } while (s < 10 || s > 100);
            scores[i][j + 1] = s;
        }
    }
}

void review_scores() {
    printf("\n-- Review Your Scores --\n");
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        printf("%s: ", candidates[i].name);
        for (int j = 1; j <= NUM_CATEGORIES; j++)
            printf("%d ", scores[i][j]);
        printf("\n");
    }
}

void modify_scores() {
    int id, found = 0, s;

    while (1) {
        printf("Modify scores? (Y/N): ");
        char ch; scanf(" %c", &ch);
        if (ch == 'N' || ch == 'n') break;

        printf("Candidate ID: "); scanf("%d", &id);
        for (int i = 0; i < NUM_CANDIDATES; i++) {
            if (candidates[i].id == id) {
                found = 1;
                for (int j = 0; j < NUM_CATEGORIES; j++) {
                    do {
                        printf("%s (10-100): ", categories[j]);
                        scanf("%d", &s);
                    } while (s < 10 || s > 100);
                    scores[i][j + 1] = s;
                }
                break;
            }
        }
        if (!found) printf("ID not found.\n");
    }
}

void calculate_totals() {
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        int sum = 0;
        for (int j = 1; j <= NUM_CATEGORIES; j++)
            sum += scores[i][j];
        scores[i][NUM_CATEGORIES + 1] = sum;
    }
}

void show_top4() {
    int order[NUM_CANDIDATES], temp;
    for (int i = 0; i < NUM_CANDIDATES; i++) order[i] = i;

    for (int i = 0; i < NUM_CANDIDATES - 1; i++)
        for (int j = i + 1; j < NUM_CANDIDATES; j++)
            if (scores[order[j]][NUM_CATEGORIES + 1] > scores[order[i]][NUM_CATEGORIES + 1]) {
                temp = order[i]; order[i] = order[j]; order[j] = temp;
            }

    printf("\n==== Final Selection ====\n");
    for (int i = 0; i < 4; i++)
        printf("%d. %s\n", i + 1, candidates[order[i]].name);
}

int main() {
    char submit;
    while (1) {
        input_scores();
        review_scores();

        printf("Submit scores? (Y/N): ");
        scanf(" %c", &submit);

        if (submit == 'Y' || submit == 'y') break;
        modify_scores();
    }

    printf("\n***Final submission completed.***\n");
    calculate_totals();
    show_top4();

    return 0;
}
