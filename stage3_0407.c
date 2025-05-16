#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SENT 10
#define INIT_SENT 5
#define MAX_MEM 5
#define MAX_LEN 100

typedef struct {
    char korean[MAX_LEN];
    char roman[MAX_LEN];
    char inputter[30];
} Sentence;

typedef struct {
    char name[30];
    char nationality[20];
} Member;

typedef struct {
    char name[30];
    int score;
    int maxMatch;
    int minMatch;
} Result;

Sentence sentences[MAX_SENT];
Member members[MAX_MEM];
Result results[MAX_MEM];
int sentCount = INIT_SENT;

void initData() {
    strcpy(members[0].name, "Ariel"); strcpy(members[0].nationality, "Korean");
    strcpy(members[1].name, "Bob"); strcpy(members[1].nationality, "American");
    strcpy(members[2].name, "Cindy"); strcpy(members[2].nationality, "Chinese");
    strcpy(members[3].name, "Diana"); strcpy(members[3].nationality, "Korean");
    strcpy(members[4].name, "Evan"); strcpy(members[4].nationality, "Canadian");

    strcpy(sentences[0].korean, "안녕하세요"); strcpy(sentences[0].roman, "annyeonghaseyo");
    strcpy(sentences[1].korean, "감사합니다"); strcpy(sentences[1].roman, "gamsahamnida");
    strcpy(sentences[2].korean, "사랑해요"); strcpy(sentences[2].roman, "saranghaeyo");
    strcpy(sentences[3].korean, "배고파요"); strcpy(sentences[3].roman, "baegopayo");
    strcpy(sentences[4].korean, "잘 지내요"); strcpy(sentences[4].roman, "jal jinaeyo");
}

int isKorean(const char *name) {
    for(int i=0; i<MAX_MEM; i++)
        if(strcmp(members[i].name, name)==0 && strcmp(members[i].nationality,"Korean")==0)
            return 1;
    return 0;
}

int isForeign(const char *name) {
    for(int i=0; i<MAX_MEM; i++)
        if(strcmp(members[i].name, name)==0 && strcmp(members[i].nationality,"Korean")!=0)
            return 1;
    return 0;
}

void inputSentences() {
    if(sentCount >= MAX_SENT) {
        printf("Already have 10 sentences.\n");
        return;
    }

    char inputter[30];
    printf("Enter your name (Korean member only): ");
    fgets(inputter, sizeof(inputter), stdin);
    inputter[strcspn(inputter, "\n")] = 0;

    if(!isKorean(inputter)) {
        printf("Not a Korean member.\n");
        return;
    }

    for(; sentCount < MAX_SENT; sentCount++) {
        printf("Korean sentence #%d: ", sentCount+1);
        fgets(sentences[sentCount].korean, MAX_LEN, stdin);
        sentences[sentCount].korean[strcspn(sentences[sentCount].korean, "\n")] = 0;

        printf("Romanization #%d: ", sentCount+1);
        fgets(sentences[sentCount].roman, MAX_LEN, stdin);
        sentences[sentCount].roman[strcspn(sentences[sentCount].roman, "\n")] = 0;

        strcpy(sentences[sentCount].inputter, inputter);
        if(sentCount == MAX_SENT-1) break;
    }
}

int wordMatch(char *answer, char *correct) {
    int matches = 0;
    char aCopy[MAX_LEN], cCopy[MAX_LEN];
    strcpy(aCopy, answer); strcpy(cCopy, correct);

    char *aTok = strtok(aCopy, " ");
    while(aTok) {
        char *cTok = strtok(cCopy, " ");
        while(cTok) {
            if(strcmp(aTok,cTok)==0) { matches++; break; }
            cTok = strtok(NULL," ");
        }
        aTok = strtok(NULL, " ");
    }
    return matches;
}

void evalAnswers(char *answer, char *correct, int *score, int *max, int *min) {
    int totalWords = 0;
    char temp[MAX_LEN];
    strcpy(temp, correct);
    for(char *tok=strtok(temp," "); tok; tok=strtok(NULL," ")) totalWords++;

    if(totalWords == 0) totalWords = 1;

    int matched = wordMatch(answer, correct);
    int percent = matched * 100 / totalWords;

    if(percent > *max) *max = percent;
    if(percent < *min) *min = percent;

    *score += matched * (20 / totalWords);
}

void playGame(int idx) {
    int score=0, max=0, min=100;
    int order[INIT_SENT];
    for(int i=0;i<INIT_SENT;i++) order[i]=i;

    for(int i=INIT_SENT-1;i>0;i--) {
        int j = rand() % (i+1);
        int t = order[i]; order[i]=order[j]; order[j]=t;
    }

    printf("\nDictation for %s:\n", members[idx].name);
    for(int i=0; i<INIT_SENT; i++) {
        printf("Type Korean for: %s\n> ", sentences[order[i]].roman);
        char answer[MAX_LEN];
        fgets(answer, MAX_LEN, stdin);
        answer[strcspn(answer, "\n")] = 0;
        evalAnswers(answer, sentences[order[i]].korean, &score, &max, &min);
    }

    results[idx].score = score;
    results[idx].maxMatch = max;
    results[idx].minMatch = min;
    strcpy(results[idx].name, members[idx].name);
}

int cmp(const void *a, const void *b) {
    Result *r1=(Result*)a, *r2=(Result*)b;
    return r2->score - r1->score; 
}

void printResults() {
    printf("\n--- Results ---\n");
    qsort(results, MAX_MEM, sizeof(Result), cmp);
    for(int i=0; i<MAX_MEM; i++) {
        if(results[i].name[0] == 0) continue;
        printf("%s: Score=%d, MaxMatch=%d%%, MinMatch=%d%%\n",
            results[i].name, results[i].score, results[i].maxMatch, results[i].minMatch);
    }
}

void doDictation() {
    inputSentences();

    for(int i=0;i<MAX_MEM;i++) {
        if(isForeign(members[i].name))
            playGame(i);
    }

    printResults();
}

int main() {
    srand((unsigned)time(NULL));
    initData();
    initSentences();
    doDictation();
    return 0;
}
