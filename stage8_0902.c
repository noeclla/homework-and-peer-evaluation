#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PHRASES 20
#define MAX_LINE_LEN 256
#define MAX_RESULTS 100

// External variables for phrases and results
const char *phrases[MAX_PHRASES] = {
    "항상", "절대", "모든 사람", "모든 것", "대부분", "상당수",
    "가끔", "종종", "아마도", "아마", "정확하지는 않지만",
    "싫지는 않지만", "찐이야", "버려"
};
int phrase_count = 14;

typedef struct {
    int line_no;
    char line[MAX_LINE_LEN];
} Result;

Result results[MAX_RESULTS];
int result_count = 0;

// KMP prefix function
void computeLPSArray(const char *pat, int M, int *lps) {
    int len = 0, i = 1;
    lps[0] = 0;
    while (i < M) {
        if (pat[i] == pat[len]) {
            len++; lps[i] = len; i++;
        } else {
            if (len != 0) len = lps[len - 1];
            else lps[i++] = 0;
        }
    }
}

// KMP search
int KMPSearch(const char *pat, const char *txt) {
    int M = strlen(pat);
    int N = strlen(txt);
    int *lps = (int*)malloc(sizeof(int)*M);
    if (!lps) return 0;
    computeLPSArray(pat, M, lps);
    int i = 0, j = 0;
    while (i < N) {
        if (pat[j] == txt[i]) { i++; j++; }
        if (j == M) {
            free(lps);
            return 1; // found
        } else if (i < N && pat[j] != txt[i]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    free(lps);
    return 0; // not found
}

// Brute-force search
int BFSearch(const char *pat, const char *txt) {
    int M = strlen(pat);
    int N = strlen(txt);
    for (int i = 0; i <= N - M; i++) {
        int j;
        for (j = 0; j < M; j++)
            if (txt[i+j] != pat[j]) break;
        if (j == M) return 1;
    }
    return 0;
}

void writingManners(void) {
    char filename[128];
    printf("Enter text filename to check: ");
    if (!fgets(filename, sizeof(filename), stdin)) return;
    filename[strcspn(filename, "\n")] = 0;

    int algo_choice = 1;
    printf("Select search algorithm: 1. KMP (default) 2. Brute Force\nChoice: ");
    if (scanf("%d", &algo_choice) != 1) algo_choice = 1;
    while (getchar() != '\n');

    int (*searchFunc)(const char*, const char*) = (algo_choice == 2) ? BFSearch : KMPSearch;

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file.\n");
        return;
    }

    char line[MAX_LINE_LEN];
    int line_no = 0;
    result_count = 0;

    while (fgets(line, sizeof(line), fp)) {
        line_no++;
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        for (int i = 0; i < phrase_count; i++) {
            if (searchFunc(phrases[i], line)) {
                if (result_count < MAX_RESULTS) {
                    results[result_count].line_no = line_no;
                    strncpy(results[result_count].line, line, MAX_LINE_LEN-1);
                    results[result_count].line[MAX_LINE_LEN-1] = 0;
                    result_count++;
                }
                break; // no need to check other phrases for this line
            }
        }
    }
    fclose(fp);

    if (result_count == 0) {
        printf("No problematic expressions found.\n");
    } else {
        printf("\nFound problematic expressions:\n");
        for (int i = 0; i < result_count; i++) {
            printf("Line %d: %s\n", results[i].line_no, results[i].line);
        }
    }
}

int main(void) {
    writingManners();
    return 0;
}
