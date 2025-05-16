#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(__GNUC__) || __GNUC__ != 9
#error "Compiler version mismatch. Compilation cannot proceed."
#endif

#include "stage8.h"


typedef struct BadHabitNode {
    char habit[128];
    struct BadHabitNode *next;
} BadHabitNode;

static BadHabitNode *badHabitTop = NULL;

void pushBadHabit(const char *habit) {
    BadHabitNode *node = (BadHabitNode*)malloc(sizeof(BadHabitNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for bad habit\n");
        exit(EXIT_FAILURE);
    }
    strncpy(node->habit, habit, 127);
    node->habit[127] = '\0';
    node->next = badHabitTop;
    badHabitTop = node;
}

int popBadHabit() {
    if (!badHabitTop) return 0;
    BadHabitNode *temp = badHabitTop;
    badHabitTop = badHabitTop->next;
    free(temp);
    return 1;
}

void printBadHabits() {
    printf("Bad habits (most recent on top):\n");
    if (!badHabitTop) {
        printf("  None\n");
        return;
    }
    for (BadHabitNode *cur = badHabitTop; cur != NULL; cur = cur->next) {
        printf("  - %s\n", cur->habit);
    }
}

// --- Queue for Good Habits ---

typedef struct GoodHabitNode {
    char habit[128];
    struct GoodHabitNode *next;
} GoodHabitNode;

static GoodHabitNode *goodHabitFront = NULL;
static GoodHabitNode *goodHabitRear = NULL;

void enqueueGoodHabit(const char *habit) {
    GoodHabitNode *node = (GoodHabitNode*)malloc(sizeof(GoodHabitNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for good habit\n");
        exit(EXIT_FAILURE);
    }
    strncpy(node->habit, habit, 127);
    node->habit[127] = '\0';
    node->next = NULL;
    if (!goodHabitRear) {
        goodHabitFront = goodHabitRear = node;
    } else {
        goodHabitRear->next = node;
        goodHabitRear = node;
    }
}

int dequeueGoodHabit() {
    if (!goodHabitFront) return 0;
    GoodHabitNode *temp = goodHabitFront;
    goodHabitFront = goodHabitFront->next;
    if (!goodHabitFront) goodHabitRear = NULL;
    free(temp);
    return 1;
}

void printGoodHabits() {
    printf("Good habits (oldest first):\n");
    if (!goodHabitFront) {
        printf("  None\n");
        return;
    }
    for (GoodHabitNode *cur = goodHabitFront; cur != NULL; cur = cur->next) {
        printf("  - %s\n", cur->habit);
    }
}

void loadHabitsFromFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    char line[256];
    enum { NONE, BAD, GOOD } mode = NONE;

    while (fgets(line, sizeof(line), fp)) {
        // Remove newline chars
        line[strcspn(line, "\r\n")] = 0;

        if (strcmp(line, "[Bad Habits]") == 0) {
            mode = BAD;
        } else if (strcmp(line, "[Good Habits]") == 0) {
            mode = GOOD;
        } else if (line[0] != '\0') {
            if (mode == BAD) {
                pushBadHabit(line);
            } else if (mode == GOOD) {
                enqueueGoodHabit(line);
            }
        }
    }
    fclose(fp);
}


void promoteGoodHabit(void) {
    loadHabitsFromFile("habit.txt");

    int choice;
    do {
        printf("\n=== Habit Management Menu ===\n");
        printf("1. Remove most recent bad habit (stack pop)\n");
        printf("2. Reveal oldest good habit (queue dequeue)\n");
        printf("0. Exit\n");
        printf("Select an option: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');  // flush input
            printf("Invalid input.\n");
            continue;
        }

        switch(choice) {
            case 1:
                if (popBadHabit()) {
                    printf("Removed the most recent bad habit.\n");
                } else {
                    printf("No bad habits to remove.\n");
                }
                printBadHabits();
                break;

            case 2:
                if (dequeueGoodHabit()) {
                    printf("Revealed the oldest good habit.\n");
                } else {
                    printf("No good habits left.\n");
                }
                printGoodHabits();
                break;

            case 0:
                printf("Exiting the program.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    while(popBadHabit());
    while(dequeueGoodHabit());
}
