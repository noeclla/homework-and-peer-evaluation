// stage2.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MEMBERS 5
#define MAX_TEXT 41
#define MAX_QUESTIONS 7

const char *memberList[MAX_MEMBERS][2] = {
    {"Arthur", "arthur"},
    {"Goeun", "goeun"},
    {"Mira", "mira"},
    {"Jake", "jake"},
    {"Nari", "nari"}
};

const char *emotionQuestions[MAX_QUESTIONS] = {
    "What happened recently and how did you feel?",
    "How have you felt over the past few days?",
    "How do you feel now? (joy, sadness, anger, anxiety, etc.)",
    "In what situations do you feel emotions more strongly?",
    "Have there been emotional changes or stress factors recently?",
    "Do you have physical signs of emotion? (tears, laughter, fast heartbeat, etc.)",
    "What strategies do you have for handling emotions?"
};

typedef struct {
    char nickname[20];
    char entries[MAX_QUESTIONS][MAX_TEXT];
} EmotionDiary;

EmotionDiary diaries[MAX_MEMBERS];
int diaryCount = 0;

void inputText(char *buffer, int max) {
    int len = 0;
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF && len < max - 1) {
        buffer[len++] = (char)ch;
    }
    buffer[len] = '\0';
}

int stringCompare(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == *b;
}

int findMemberIndex(const char *nick) {
    for (int i = 0; i < MAX_MEMBERS; i++) {
        const char *m = memberList[i][1];
        int j = 0;
        int match = 1;
        while (nick[j] && m[j]) {
            if (nick[j] != m[j]) {
                match = 0;
                break;
            }
            j++;
        }
        if (match && !nick[j] && !m[j]) return i;
    }
    return -1;
}

void printEmotionDiary(const char *nickname) {
    int idx = findMemberIndex(nickname);
    if (idx < 0) return;

    printf("\nEmotion Diary for %s (%s):\n", memberList[idx][0], memberList[idx][1]);

    for (int i = 0; i < MAX_QUESTIONS; i++) {
        if (stringCompare(diaries[idx].entries[i], "TBD")) continue;
        printf("Q%d. %s\nA: %s\n", i + 1, emotionQuestions[i], diaries[idx].entries[i]);
    }
}

void writeEmotionDiary(const char *nickname) {
    int idx = findMemberIndex(nickname);
    if (idx < 0) return;

    for (int i = 0; i < MAX_QUESTIONS; i++) {
        printf("\nQ%d. %s\n> ", i + 1, emotionQuestions[i]);
        inputText(diaries[idx].entries[i], MAX_TEXT);
        int len = 0;
        while (diaries[idx].entries[i][len]) len++;
        if (len < 20 || len > 40) {
            printf("Invalid input. Saved as TBD.\n");
            diaries[idx].entries[i][0] = 'T';
            diaries[idx].entries[i][1] = 'B';
            diaries[idx].entries[i][2] = 'D';
            diaries[idx].entries[i][3] = '\0';
        }
    }

    printf("\n1. Print Diary\n2. Back\n> ");
    char opt;
    scanf(" %c", &opt);
    getchar();
    if (opt == '1') printEmotionDiary(nickname);
}

void logEmotionDiary() {
    printf("\n[Emotion Diary]\n");

    printf("Available Members:\n");
    for (int i = 0; i < MAX_MEMBERS; i++) {
        const char *nick = memberList[i][1];
        int len = 0;
        while (nick[len]) len++;
        printf("- %c...%c\n", nick[0], nick[len - 1]);
    }

    char input[20];
    int tries = 0;
    int idx = -1;
    do {
        printf("\nEnter nickname: ");
        inputText(input, 20);
        idx = findMemberIndex(input);
        if (idx < 0) tries++;
    } while (idx < 0 && tries < 2);

    if (idx < 0) {
        printf("Too many failed attempts. Exiting.\n");
        exit(1);
    }

    printf("\nWelcome %s!\n1. Write Diary\n2. View Diary\n> ", memberList[idx][0]);
    char choice;
    scanf(" %c", &choice);
    getchar();

    if (choice == '1') writeEmotionDiary(input);
    else if (choice == '2') printEmotionDiary(input);
}
