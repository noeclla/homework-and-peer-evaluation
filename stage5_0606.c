#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STYLE 50
#define MAX_LEN 256

typedef struct {
    char style[MAX_LEN];
    char song[MAX_LEN];
    char description[MAX_LEN];
} DanceStyle;

DanceStyle styles[MAX_STYLE];
int styleCount = 0;

void trim(char *str) {
    char *start = str;
    while (*start == ' ' || *start == '\t' || *start == '\n') start++;
    memmove(str, start, strlen(start) + 1);
    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n')) *end-- = '\0';
}

void parseHTML(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Cannot open file: %s\n", filename);
        return;
    }
    char line[MAX_LEN];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "<style>") != NULL) {
            fgets(styles[styleCount].style, MAX_LEN, fp);
            trim(styles[styleCount].style);
        } else if (strstr(line, "<song>") != NULL) {
            fgets(styles[styleCount].song, MAX_LEN, fp);
            trim(styles[styleCount].song);
        } else if (strstr(line, "<desc>") != NULL) {
            fgets(styles[styleCount].description, MAX_LEN, fp);
            trim(styles[styleCount].description);
            styleCount++;
        }
    }
    fclose(fp);
}

void quizUser() {
    int correct = 0;
    int total = styleCount;
    srand(time(NULL));
    for (int i = 0; i < total; i++) {
        int index = rand() % total;
        printf("\nWhich style matches the song: %s\n", styles[index].song);
        printf("Your answer: ");
        char input[MAX_LEN];
        fgets(input, MAX_LEN, stdin);
        trim(input);
        if (strcmp(input, styles[index].style) == 0) {
            printf("Correct!\n");
            correct++;
        } else {
            printf("Incorrect. Correct answer: %s\n", styles[index].style);
        }
    }
    printf("\nTotal correct: %d out of %d\n", correct, total);
}

void exportYAML(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Failed to write YAML file.\n");
        return;
    }
    fprintf(fp, "danceStyles:\n");
    for (int i = 0; i < styleCount; i++) {
        fprintf(fp, "  - 안무_스타일: %s\n", styles[i].style);
        fprintf(fp, "    노래: %s\n", styles[i].song);
        fprintf(fp, "    설명: %s\n", styles[i].description);
    }
    fclose(fp);
    printf("Exported to YAML: %s\n", filename);
}

void learnDanceStyle() {
    parseHTML("dance_style.html");
    if (styleCount == 0) {
        printf("No dance styles loaded.\n");
        return;
    }
    quizUser();

    char choice[MAX_LEN];
    printf("\nExport data to YAML? (y/n): ");
    fgets(choice, MAX_LEN, stdin);
    if (choice[0] == 'y' || choice[0] == 'Y') {
        exportYAML("dance_style.yaml");
    }
}
