#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LEN 20
#define MAX_LINES 20
#define MAX_LINE_LEN 256
#define MAX_POOL 1000

char wordPool[MAX_POOL][MAX_WORD_LEN];
int poolSize = 0;

int loadWords(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    poolSize = 0;
    while (fscanf(f, "%19s", wordPool[poolSize]) == 1) poolSize++;
    fclose(f);
    return poolSize;
}

void pickWords(char picks[][MAX_WORD_LEN], int count, int minLen, int maxLen) {
    srand((unsigned)time(NULL));
    int picked = 0;
    while (picked < count) {
        int i = rand() % poolSize;
        int len = strlen(wordPool[i]);
        if (len < minLen || len > maxLen) continue;

        int duplicate = 0;
        for (int j = 0; j < picked; j++)
            if (strcmp(picks[j], wordPool[i]) == 0)
                duplicate = 1;

        if (!duplicate) {
            strcpy(picks[picked], wordPool[i]);
            picked++;
        }
    }
}

int main() {
    if (!loadWords("writing_words.txt")) {
        printf("Failed to load words.\n");
        return 1;
    }

    int wordCount, minLen, maxLen;
    printf("Enter number of words (10-20): ");
    scanf("%d", &wordCount);
    printf("Enter min and max word length (2-7): ");
    scanf("%d %d", &minLen, &maxLen);
    getchar(); // clear newline

    char picks[wordCount][MAX_WORD_LEN];
    pickWords(picks, wordCount, minLen, maxLen);

    printf("Use these words in your writing:\n");
    for (int i = 0; i < wordCount; i++)
        printf("- %s\n", picks[i]);

    char *lines[wordCount];
    for (int i = 0; i < wordCount; i++)
        lines[i] = malloc(MAX_LINE_LEN);

    while (1) {
        printf("Write %d lines:\n", wordCount);
        for (int i = 0; i < wordCount; i++) {
            fgets(lines[i], MAX_LINE_LEN, stdin);
            lines[i][strcspn(lines[i], "\n")] = 0; // remove newline
        }

        // Check usage of each word in lines (simple version)
        int used[wordCount];
        for (int i = 0; i < wordCount; i++) used[i] = 0;

        for (int i = 0; i < wordCount; i++) {
            for (int w = 0; w < wordCount; w++) {
                if (strstr(lines[i], picks[w]) != NULL)
                    used[w] = 1;
            }
        }

        int allUsed = 1;
        for (int i = 0; i < wordCount; i++) {
            if (!used[i]) allUsed = 0;
        }

        if (allUsed) {
            printf("All words used at least once! Saving...\n");
            FILE *f = fopen("camelcase.txt", "w");
            for (int i = 0; i < wordCount; i++) fprintf(f, "%s\n", lines[i]);
            fclose(f);
            break;
        } else {
            printf("Not all words used. Please rewrite.\n");
        }
    }

    for (int i = 0; i < wordCount; i++) free(lines[i]);
    return 0;
}
