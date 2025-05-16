#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 64
#define MAX_LINE_LEN 512
#define MAX_KEYWORDS 1000

char positiveKeywords[MAX_KEYWORDS][MAX_WORD_LEN];
int positiveCount = 0;
char negativeKeywords[MAX_KEYWORDS][MAX_WORD_LEN];
int negativeCount = 0;

void preprocessBadChar(const char *pattern, int badCharTable[256]) {
    int m = (int)strlen(pattern);
    for (int i = 0; i < 256; i++)
        badCharTable[i] = -1;
    for (int i = 0; i < m; i++)
        badCharTable[(unsigned char)pattern[i]] = i;
}

int boyerMooreSearch(const char *text, const char *pattern) {
    int n = (int)strlen(text);
    int m = (int)strlen(pattern);
    if (m == 0) return 0; // empty pattern matches at start

    int badCharTable[256];
    preprocessBadChar(pattern, badCharTable);

    int s = 0; // shift of the pattern over text
    while (s <= n - m) {
        int j = m - 1;

        while (j >= 0 && pattern[j] == text[s + j])
            j--;

        if (j < 0) return s; // found at index s
        else {
            int badCharShift = j - badCharTable[(unsigned char)text[s + j]];
            s += (badCharShift > 1) ? badCharShift : 1;
        }
    }
    return -1; // not found
}

int loadKeywords(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    positiveCount = 0;
    negativeCount = 0;

    char neg[MAX_WORD_LEN], pos[MAX_WORD_LEN];
    while (fscanf(fp, "%63s %63s", neg, pos) == 2) {
        if (negativeCount < MAX_KEYWORDS)
            strncpy(negativeKeywords[negativeCount++], neg, MAX_WORD_LEN - 1);
        if (positiveCount < MAX_KEYWORDS)
            strncpy(positiveKeywords[positiveCount++], pos, MAX_WORD_LEN - 1);
    }
    fclose(fp);
    return 1;
}

int containsKeyword(const char *line, char keywords[][MAX_WORD_LEN], int count) {
    for (int i = 0; i < count; i++) {
        if (boyerMooreSearch(line, keywords[i]) >= 0)
            return 1;
    }
    return 0;
}

void filterComment(void) {
    char keywordsFile[128];
    char commentsFile[128];

    printf("Enter keyword file name (bad_good_words.txt): ");
    if (!fgets(keywordsFile, sizeof(keywordsFile), stdin)) return;
    keywordsFile[strcspn(keywordsFile, "\n")] = 0;

    printf("Enter fan comments file name (fan_response.txt): ");
    if (!fgets(commentsFile, sizeof(commentsFile), stdin)) return;
    commentsFile[strcspn(commentsFile, "\n")] = 0;

    if (!loadKeywords(keywordsFile)) {
        printf("Failed to load keywords from file.\n");
        return;
    }
    printf("Loaded %d negative and %d positive keywords.\n", negativeCount, positiveCount);

    FILE *fp = fopen(commentsFile, "r");
    if (!fp) {
        printf("Failed to open fan comments file.\n");
        return;
    }

    char line[MAX_LINE_LEN];
    printf("\n=== Comments with NO negative keywords ===\n");
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        if (!containsKeyword(line, negativeKeywords, negativeCount)) {
            printf("%s\n", line);
        }
    }

    printf("\n=== Comments WITH positive keywords ===\n");
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        if (containsKeyword(line, positiveKeywords, positiveCount)) {
            printf("%s\n", line);
        }
    }

    printf("\n=== Comments with NEITHER positive NOR negative keywords (Bonus) ===\n");
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        if (!containsKeyword(line, negativeKeywords, negativeCount) && !containsKeyword(line, positiveKeywords, positiveCount)) {
            printf("%s\n", line);
        }
    }

    fclose(fp);
}
