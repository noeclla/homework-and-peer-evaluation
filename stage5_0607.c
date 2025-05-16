#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 20
#define LINE 256

typedef struct {
    char kr[64];
    char en[64];
} Word;

typedef struct {
    Word base;
    Word similar;
    char body[128];
} Emotion;

Emotion emotions[MAX];
int count = 0;

void trim(char* s) {
    char* end;
    while(*s == ' ' || *s == '\t') s++;
    end = s + strlen(s) - 1;
    while(end > s && (*end == '\n' || *end == ' ')) *end-- = 0;
}

void loadYaml(const char* file) {
    FILE* f = fopen(file, "r");
    if(!f) { printf("Can't open %s\n", file); return; }
    char line[LINE], state = 0;
    Emotion e = {0};

    while(fgets(line, LINE, f)) {
        trim(line);
        if(strstr(line, "기본 감정:")) state=1;
        else if(strstr(line, "유사 감정:")) state=2;
        else if(strstr(line, "몸 동작:")) state=3;
        else if(strstr(line, "한국어:")) {
            char* v = strchr(line, ':')+1; trim(v);
            if(state==1) strcpy(e.base.kr, v);
            else if(state==2) strcpy(e.similar.kr, v);
        }
        else if(strstr(line, "영어:")) {
            char* v = strchr(line, ':')+1; trim(v);
            if(state==1) strcpy(e.base.en, v);
            else if(state==2) strcpy(e.similar.en, v);
        }
        else if(strstr(line, "표현:")) {
            char* v = strchr(line, ':')+1; trim(v);
            strcpy(e.body, v);
            emotions[count++] = e;
        }
    }
    fclose(f);
    printf("Loaded %d emotions.\n", count);
}

void practice() {
    if(count==0) { printf("No data loaded.\n"); return; }
    int correct=0; char ans[64];
    srand((unsigned)time(NULL));

    while(correct<5) {
        int i = rand()%count;
        printf("What is the English word for '%s'? ", emotions[i].base.kr);
        fgets(ans, sizeof(ans), stdin);
        ans[strcspn(ans, "\n")] = 0;
        if(strcasecmp(ans, emotions[i].base.en)==0) {
            printf("Correct!\n");
            correct++;
        } else {
            printf("Wrong. Correct: %s\n", emotions[i].base.en);
        }
    }

    printf("\nYou got 5 correct!\n");
    for(int i=0; i<count; i++) {
        printf("\n%s(%s) ~ %s(%s)\nBody: %s\n",
            emotions[i].base.kr, emotions[i].base.en,
            emotions[i].similar.kr, emotions[i].similar.en,
            emotions[i].body);
    }

    // Save JSON
    FILE* j = fopen("emotion.json", "w");
    if(!j) { printf("JSON save failed.\n"); return; }
    fprintf(j, "[\n");
    for(int i=0; i<count; i++) {
        fprintf(j,
            "  {\n"
            "    \"base_emotion\": {\"korean\": \"%s\", \"english\": \"%s\"},\n"
            "    \"similar_emotion\": {\"korean\": \"%s\", \"english\": \"%s\"},\n"
            "    \"body_movement\": {\"expression\": \"%s\"}\n"
            "  }%s\n",
            emotions[i].base.kr, emotions[i].base.en,
            emotions[i].similar.kr, emotions[i].similar.en,
            emotions[i].body,
            (i == count-1) ? "" : ",");
    }
    fprintf(j, "]\n");
    fclose(j);
    printf("Saved emotion.json\n");
}

int main() {
    loadYaml("emotion.yaml");
    practice();
    return 0;
}
