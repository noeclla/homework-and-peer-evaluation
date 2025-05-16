#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MAX_LINES 100
#define MAX_LEN 256

typedef struct {
    char lyric[MAX_LEN];
    char concept[MAX_LEN];
    char symbol[MAX_LEN];
} SongWithDance;

typedef struct {
    char lyric[MAX_LEN];
    char answer[MAX_LEN];
} MyAnswer;

SongWithDance database[MAX_LINES];
MyAnswer userAnswers[MAX_LINES];
int totalLines = 0;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void sleepSeconds(int sec) {
#ifdef _WIN32
    Sleep(sec * 1000);
#else
    sleep(sec);
#endif
}

void LoadFile() {
    FILE *fp = fopen("song-with-dance.txt", "r");
    if (!fp) {
        perror("File open error");
        exit(1);
    }
    char line[MAX_LEN];
    while (fgets(line, sizeof(line), fp) && totalLines < MAX_LINES) {
        char *start_concept = strchr(line, '(');
        char *end_concept = strchr(line, ')');
        char *start_symbol = strchr(line, '{');
        char *end_symbol = strchr(line, '}');
        if (start_concept && end_concept && start_symbol && end_symbol) {
            *start_concept = '\0';
            strcpy(database[totalLines].lyric, line);
            *end_concept = '\0';
            strcpy(database[totalLines].concept, start_concept + 1);
            *end_symbol = '\0';
            strcpy(database[totalLines].symbol, start_symbol + 1);
            totalLines++;
        }
    }
    fclose(fp);
}

void DisplayHint(int duration) {
    for (int i = 0; i < totalLines; i++) {
        printf("[%d] %s (%s) {%s}\n", i + 1, database[i].lyric, database[i].concept, database[i].symbol);
    }
    printf("\n-- Displaying for %d seconds...\n", duration);
    sleepSeconds(duration);
    clearScreen();
}

void InputDanceSymbol() {
    srand(time(NULL));
    for (int i = 0; i < totalLines; i++) {
        int idx = rand() % totalLines;
        printf("Lyric: %s\nYour Dance Symbol: ", database[idx].lyric);
        fgets(userAnswers[i].answer, MAX_LEN, stdin);
        userAnswers[i].answer[strcspn(userAnswers[i].answer, "\n")] = 0;
        strcpy(userAnswers[i].lyric, database[idx].lyric);
    }
}

void EvalMatchingRatio() {
    int match = 0;
    printf("\nEvaluation Result:\n");
    for (int i = 0; i < totalLines; i++) {
        for (int j = 0; j < totalLines; j++) {
            if (strcmp(userAnswers[i].lyric, database[j].lyric) == 0) {
                if (strcmp(userAnswers[i].answer, database[j].symbol) == 0) {
                    match++;
                } else {
                    printf("Not matched: %s\n", userAnswers[i].lyric);
                }
                break;
            }
        }
    }
    int percent = (match * 100) / totalLines;
    printf("\nYour choreo match rate: %d%%\n", percent);
}

void RememberSongWithDance() {
    LoadFile();
    int duration;
    printf("Enter display duration for hint (seconds): ");
    scanf("%d", &duration);
    getchar(); // flush newline
    DisplayHint(duration);
    InputDanceSymbol();
    EvalMatchingRatio();
}
