#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATTERNS 50
#define MAX_LINE_LEN 512
#define MAX_JSON_SIZE 4096

typedef struct {
    int id;
    char name[100];
    char description[300];
} BreathingPattern;

BreathingPattern breathingPatterns[MAX_PATTERNS];
int patternCount = 0;

void trim(char *str) {
    char *end;
    while (*str == ' ' || *str == '\t') str++;
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) *end-- = '\0';
}

void parseJsonFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("[ERROR] Cannot open %s\n", filename);
        return;
    }

    char buffer[MAX_JSON_SIZE];
    fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    char *ptr = buffer;
    while ((ptr = strstr(ptr, "{\"id\"")) != NULL) {
        BreathingPattern bp;
        sscanf(ptr, "{\"id\": %d, \"name\": \"%99[^"]\", \"description\": \"%299[^"]\"", &bp.id, bp.name, bp.description);
        breathingPatterns[patternCount++] = bp;
        ptr++;
    }
}

void showPatternList() {
    printf("\n[Breathing Patterns]\n");
    for (int i = 0; i < patternCount; i++) {
        printf("%d. %s\n", breathingPatterns[i].id, breathingPatterns[i].name);
    }
    printf("\nType pattern ID to learn more, or 'q' to quit.\n");
}

void saveFavoritePattern(BreathingPattern *bp) {
    FILE *file = fopen("mymindfulness.json", "a");
    if (!file) {
        file = fopen("mymindfulness.json", "w");
        if (!file) {
            printf("[ERROR] Cannot create mymindfulness.json\n");
            return;
        }
    }
    fprintf(file, "{\"id\": %d, \"name\": \"%s\", \"how-to\": \"%s\"}\n", bp->id, bp->name, bp->description);
    fclose(file);
    printf("Saved as favorite.\n");
}

void guideMindfulnessBreathing() {
    parseJsonFile("mindfulness_breathing.json");
    if (patternCount == 0) {
        printf("No breathing patterns loaded.\n");
        return;
    }

    char input[10];
    while (1) {
        showPatternList();
        printf("\nEnter ID (or 'q' to quit): ");
        fgets(input, sizeof(input), stdin);
        trim(input);

        if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0) break;

        int id = atoi(input);
        int found = 0;
        for (int i = 0; i < patternCount; i++) {
            if (breathingPatterns[i].id == id) {
                printf("\n[%s]\n%s\n", breathingPatterns[i].name, breathingPatterns[i].description);
                found = 1;
                printf("\nSave as favorite? (y/n): ");
                fgets(input, sizeof(input), stdin);
                trim(input);
                if (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0) {
                    saveFavoritePattern(&breathingPatterns[i]);
                }
                break;
            }
        }
        if (!found) printf("[ERROR] Pattern ID not found.\n");
    }
}

int main() {
    guideMindfulnessBreathing();
    return 0;
}
