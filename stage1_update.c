#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MEMBERS 4
#define TESTS 7

char *names[] = {"Hyekyung", "Goeun", "Woncheol", "Youngjin"};
char *nicks[] = {"HK", "GE", "WC", "YJ"};
char *tests[] = {"1-Mile", "Sprint", "Push-ups", "Squats", "Arm Power", "Swim", "Weightlift"};
float scores[MEMBERS][TESTS] = {{0}};

int find(char *nick) {
    for (int i = 0; i < MEMBERS; i++)
        if (strcmp(nick, nicks[i]) == 0) return i;
    return -1;
}

void enterData() {
    char input[128];
    printf("Enter: NICK, 7 scores\n> ");
    fgets(input, sizeof(input), stdin);
    char *nick = strtok(input, ",");
    int i = find(nick);
    if (i == -1) { printf("Not found.\n"); return; }
    for (int j = 0; j < TESTS; j++) {
        char *score = strtok(NULL, ",");
        if (!score) { printf("Missing scores.\n"); return; }
        scores[i][j] = atof(score);
    }
    printf("Saved for %s\n", nicks[i]);
}

void viewData() {
    char input[16];
    printf("[A] All [B] One [C] One Score\n> ");
    fgets(input, sizeof(input), stdin);
    
    if (toupper(input[0]) == 'A') {
        for (int i = 0; i < MEMBERS; i++) {
            printf("\nMember: %s (%s)\nFitness Data: ", names[i], nicks[i]);
            for (int j = 0; j < TESTS; j++) {
                printf("%s: %.1f ", tests[j], scores[i][j]);
            }
            printf("\n");
        }
    } else if (toupper(input[0]) == 'B') {
        printf("noe challa");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        int i = find(input);
        if (i == -1) { printf("Not found.\n"); return; }
        printf("\nMember: %s (%s)\nFitness Data: ", names[i], nicks[i]);
        for (int j = 0; j < TESTS; j++) {
            printf("%s: %.1f ", tests[j], scores[i][j]);
        }
        printf("\n");
    } else if (toupper(input[0]) == 'C') {
        char nick[8]; int t;
        printf("noe challa ");
        fgets(nick, sizeof(nick), stdin);
        nick[strcspn(nick, "\n")] = 0;
        int i = find(nick);
        if (i == -1) { printf("Not found.\n"); return; }
        printf("Test number (1-7)? ");
        fgets(input, sizeof(input), stdin);
        t = atoi(input);
        if (t < 1 || t > 7) { printf("Invalid.\n"); return; }
        printf("%s (%s) - %s: %.1f\n", names[i], nick, tests[t - 1], scores[i][t - 1]);
    }
}

int main() {
    char input[4];
    while (1) {
        printf("\n[1] Enter Fitness  [2] View Fitness  [0] Exit\n> ");
        fgets(input, sizeof(input), stdin);
        if (input[0] == '0') break;
        else if (input[0] == '1') enterData();
        else if (input[0] == '2') viewData();
        else printf("Try again.\n");
    }
    return 0;
}
