#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MEMBERS 4
#define MAX_DAYS 6
#define MAX_EXERCISES 2

const char *names[MAX_MEMBERS] = {"Hyekyung", "Goeun", "Woncheol", "Youngjin"};
const char *nicks[MAX_MEMBERS] = {"HK", "GE", "WC", "YJ"};

const char *exercise_types[][3] = {
    {"Running", "Cycling", "Fast Walking"},
    {"Push-ups", "Squats"},
    {"Leg Press", "Leg Curl"},
    {"Pull-ups", "Chin-ups"},
    {"Plank", "Crunches"}
};

char *member_routine[MAX_MEMBERS][MAX_DAYS][MAX_EXERCISES] = {NULL};

void display_members() {
    printf("\n=== Members ===\n");
    for (int i = 0; i < MAX_MEMBERS; i++)
        printf("%d. %s (%s)\n", i + 1, names[i], nicks[i]);
    printf("\n");
}

void setExerciseRoutine() {
    int member_idx, day;
    display_members();
    printf("Choose member (1-%d): ", MAX_MEMBERS);
    scanf("%d", &member_idx);
    member_idx--; if (member_idx < 0 || member_idx >= MAX_MEMBERS) return;

    for (day = 0; day < MAX_DAYS; day++) {
        printf("\nDay %d exercises (1-5):\n", day + 1);
        for (int i = 0; i < 2; i++) {
            int type = (i == 0) ? 0 : rand() % 4 + 1;
            printf("%d. %s\n", i + 1, exercise_types[type][rand() % 2]);
            member_routine[member_idx][day][i] = (char *)exercise_types[type][rand() % 2];
        }
    }
    printf("Routine set for %s.\n", nicks[member_idx]);
}

void getExerciseRoutine() {
    int member_idx;
    char input[64];
    display_members();
    printf("Enter member nickname: ");
    scanf("%s", input);

    for (member_idx = 0; member_idx < MAX_MEMBERS; member_idx++)
        if (strcmp(input, nicks[member_idx]) == 0) break;

    if (member_idx == MAX_MEMBERS) return;
    printf("\n=== %s's Routine ===\n", names[member_idx]);
    for (int day = 0; day < MAX_DAYS; day++) {
        printf("Day %d: ", day + 1);
        for (int i = 0; i < MAX_EXERCISES; i++)
            printf("%s ", member_routine[member_idx][day][i]);
        printf("\n");
    }
}

void main_menu() {
    int choice;
    while (1) {
        printf("\n=== Main Menu ===\n1. Set Routine\n2. View Routine\n0. Exit\nChoice: ");
        scanf("%d", &choice);
        if (choice == 0) break;
        else if (choice == 1) setExerciseRoutine();
        else if (choice == 2) getExerciseRoutine();
        else printf("Invalid choice. Try again.\n");
    }
}

int main() {
    main_menu();
    return 0;
}
