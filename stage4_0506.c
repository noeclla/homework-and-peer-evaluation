#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_MASK 10
#define QUIZ_COUNT 5
#define MAX_TITLE 50

typedef struct {
    int id;
    char maskName[20];
    char songName[MAX_TITLE];
} MaskSong;

MaskSong maskSongs[MAX_MASK] = {
    {1, "BearPaw", "Hold The End Of This Night"},
    {2, "JooYoonBal", "Short Hair"},
    {3, "Dalmatian", "My Old Story"},
    {4, "ThunderTiger", "Love Affair"},
    {5, "FrogPrince", "Tired Day"},
    {6, "Turtle", "Love Her"},
    {7, "Elephant", "First Impression"},
    {8, "Vampire", "Can't Know Now"},
    {9, "Hamster", "Listen To Me"},
    {10, "Dolphin", "Unbloomed Flower"}
};

// Utility: remove spaces, to lowercase
void normalize(char *dest, const char *src) {
    int j = 0;
    for (int i = 0; src[i]; i++) {
        if (!isspace((unsigned char)src[i])) {
            dest[j++] = tolower((unsigned char)src[i]);
        }
    }
    dest[j] = '\0';
}

void splashScreen(void) {
    printf("=== Masked Stage Practice ===\n");
    printf("You will see 5 masks in hex code.\n");
    printf("Type the matching song title for each mask.\n");
    printf("Ignore spaces when typing titles.\n");
    printf("You have 1 minute total. Bonus if under 30 seconds!\n\n");
}

void prepareMaskedStage(void) {
    srand((unsigned)time(NULL));
    splashScreen();

    int chosenIndices[QUIZ_COUNT];
    int totalScore = 0;
    time_t startTime = time(NULL);

    // pick 5 unique random indices
    for (int i = 0; i < QUIZ_COUNT; i++) {
        int r;
        int unique;
        do {
            r = rand() % MAX_MASK;
            unique = 1;
            for (int j = 0; j < i; j++) {
                if (chosenIndices[j] == r) unique = 0;
            }
        } while (!unique);
        chosenIndices[i] = r;
    }

    char input[100], normInput[100], normAnswer[100];
    for (int i = 0; i < QUIZ_COUNT; i++) {
        MaskSong *ms = &maskSongs[chosenIndices[i]];
        printf("Mask code: 0x%X\n", ms->id);

        printf("Enter matching song title: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;  // remove newline

        normalize(normInput, input);
        normalize(normAnswer, ms->songName);

        if (strcmp(normInput, normAnswer) == 0) {
            printf("Correct! +10 points\n");
            totalScore += 10;
        } else {
            printf("Wrong! Correct answer was \"%s\"\n", ms->songName);
        }
    }

    time_t elapsed = (time(NULL) - startTime);
    printf("\nTotal points from answers: %d\n", totalScore);

    // Bonus scoring
    if (elapsed <= 60) {
        totalScore += 10;
        printf("Within 1 minute! +10 bonus points\n");
        if (elapsed <= 30) {
            totalScore += 5;
            printf("Within 30 seconds! +5 extra bonus points\n");
        }
    } else {
        totalScore += 5;
        printf("Over 1 minute! +5 bonus points\n");
    }

    printf("Final Score: %d\n", totalScore);
}

