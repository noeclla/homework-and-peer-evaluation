#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_EMOTIONS 10
#define MAX_LINE 256

typedef struct {
    char korean[64];
    char english[64];
} Word;

typedef struct {
    Word base;      // Base emotion
    Word similar;   // Similar emotion
    char body[256]; // Body movement description
} Emotion;

Emotion emotions[MAX_EMOTIONS];
int emotionCount = 0;

void trim(char *s) {
    // Trim leading/trailing spaces/newlines
    char *end;
    while(*s == ' ' || *s == '\t') s++;
    end = s + strlen(s) - 1;
    while(end > s && (*end == ' ' || *end == '\n')) *end-- = 0;
}

void loadEmotionsFromYaml(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Cannot open %s\n", filename);
        return;
    }

    char line[MAX_LINE];
    Emotion e = {0};
    int section = 0; // 1=base, 2=similar, 3=body

    while(fgets(line, sizeof(line), fp)) {
        trim(line);
        if (strstr(line, "기본 감정:")) section = 1;
        else if (strstr(line, "유사 감정:")) section = 2;
        else if (strstr(line, "몸 동작:")) section = 3;
        else if (strstr(line, "한국어:")) {
            char *val = strchr(line, ':') + 1;
            trim(val);
            if(section == 1) strcpy(e.base.korean, val);
            else if(section == 2) strcpy(e.similar.korean, val);
        }
        else if (strstr(line, "영어:")) {
            char *val = strchr(line, ':') + 1;
            trim(val);
            if(section == 1) strcpy(e.base.english, val);
            else if(section == 2) strcpy(e.similar.english, val);
        }
        else if (strstr(line, "표현:")) {
            char *val = strchr(line, ':') + 1;
            trim(val);
            strcpy(e.body, val);
            if(emotionCount < MAX_EMOTIONS) emotions[emotionCount++] = e;
            e = (Emotion){0}; // reset for next
        }
    }

    fclose(fp);
    printf("Loaded %d emotions.\n", emotionCount);
}

void quizUser() {
    if(emotionCount == 0) {
        printf("No emotions loaded.\n");
        return;
    }

    srand((unsigned int)time(NULL));
    int correct = 0;

    while(correct < 5) {
        int idx = rand() % emotionCount;
        char answer[64];

        printf("What is the English word for '%s'? ", emotions[idx].base.korean);
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0; // remove newline

        if(strcasecmp(answer, emotions[idx].base.english) == 0) {
            printf("Correct!\n");
            correct++;
        } else {
            printf("Wrong! Correct answer: %s\n", emotions[idx].base.english);
        }
    }

    printf("\nCongrats! You answered 5 correctly.\n");

    for(int i=0; i<emotionCount; i++) {
        printf("\n%s(%s) ~ %s(%s)\nBody movement: %s\n",
            emotions[i].base.korean, emotions[i].base.english,
            emotions[i].similar.korean, emotions[i].similar.english,
            emotions[i].body);
    }
}

void saveJson(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Failed to save JSON.\n");
        return;
    }

    fprintf(fp, "[\n");
    for(int i=0; i<emotionCount; i++) {
        fprintf(fp,
            "  {\n"
            "    \"base_emotion\": {\"korean\": \"%s\", \"english\": \"%s\"},\n"
            "    \"similar_emotion\": {\"korean\": \"%s\", \"english\": \"%s\"},\n"
            "    \"body_movement\": {\"expression\": \"%s\"}\n"
            "  }%s\n",
            emotions[i].base.korean, emotions[i].base.english,
            emotions[i].similar.korean, emotions[i].similar.english,
            emotions[i].body,
            (i == emotionCount-1) ? "" : ",");
    }
    fprintf(fp, "]\n");
    fclose(fp);

    printf("Saved emotions to %s\n", filename);
}

int main() {
    loadEmotionsFromYaml("emotion.yaml");
    quizUser();
    saveJson("emotion.json");
    return 0;
}
