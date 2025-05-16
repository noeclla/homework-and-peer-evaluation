#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 50
#define QUESTIONS 5

typedef struct {
    char hangul[4];
    char engAlpha[3];
    char engPron[10];
    char type; // C or V
} Jamo;

typedef struct {
    char name[MAX_NAME];
    char challenge[3];
    int score;
    int attempts;
} Result;

Jamo consonants[] = {
    {"ㄱ","g","giyeok",'C'}, {"ㄴ","n","nieun",'C'}, {"ㄷ","d","digeut",'C'},
    {"ㄹ","r","rieul",'C'}, {"ㅁ","m","mieum",'C'}, {"ㅂ","b","bieup",'C'},
    {"ㅅ","s","siot",'C'}, {"ㅇ","ng","ieung",'C'}, {"ㅈ","j","jieut",'C'},
    {"ㅊ","ch","chieut",'C'}, {"ㅋ","k","kieuk",'C'}, {"ㅌ","t","tieut",'C'},
    {"ㅍ","p","pieup",'C'}, {"ㅎ","h","hieut",'C'}
};
Jamo vowels[] = {
    {"ㅏ","a","ah",'V'}, {"ㅑ","ya","yah",'V'}, {"ㅓ","eo","aw",'V'},
    {"ㅕ","yeo","yaw",'V'}, {"ㅗ","o","oh",'V'}, {"ㅛ","yo","yoh",'V'},
    {"ㅜ","u","oo",'V'}, {"ㅠ","yu","yoo",'V'}, {"ㅡ","eu","eu",'V'},
    {"ㅣ","i","ee",'V'}
};

Result results[100];
int resCount = 0;

void flush() { while(getchar()!='\n'); }

void randIndices(int *arr, int n, int max) {
    int used[max]; for(int i=0;i<max;i++) used[i]=0;
    int c=0;
    while(c<n) {
        int r=rand()%max;
        if(!used[r]) { used[r]=1; arr[c++]=r; }
    }
}

void korToEng(char *name, Jamo *list, int size, char t) {
    int idxs[QUESTIONS];
    randIndices(idxs, QUESTIONS, size);
    int score=0;
    printf("Hangul to English (%s):\n", (t=='C')?"Consonants":"Vowels");
    for(int i=0;i<QUESTIONS;i++) {
        char alpha[10], pron[20];
        printf("%d. '%s' -> ", i+1, list[idxs[i]].hangul);
        scanf("%9[^,],%19s", alpha, pron); flush();
        if(strcmp(alpha,list[idxs[i]].engAlpha)==0 && strcmp(pron,list[idxs[i]].engPron)==0) {
            printf("Right!\n"); score+=10;
        } else printf("Wrong! %s,%s\n",list[idxs[i]].engAlpha,list[idxs[i]].engPron);
    }
    printf("Score: %d/50\n",score);
    if(resCount<100) {
        strcpy(results[resCount].name, name);
        strcpy(results[resCount].challenge, "KE");
        results[resCount].score = score;
        results[resCount].attempts++;
        resCount++;
    }
}

void engToKor(char *name, Jamo *list, int size, char t) {
    int idxs[QUESTIONS];
    randIndices(idxs, QUESTIONS, size);
    int score=0;
    printf("English to Hangul (%s):\n", (t=='C')?"Consonants":"Vowels");
    for(int i=0;i<QUESTIONS;i++) {
        char input[4];
        printf("%d. '%s,%s' -> ", i+1, list[idxs[i]].engAlpha, list[idxs[i]].engPron);
        scanf("%3s", input); flush();
        if(strcmp(input,list[idxs[i]].hangul)==0) {
            printf("Right!\n"); score+=10;
        } else printf("Wrong! %s\n",list[idxs[i]].hangul);
    }
    printf("Score: %d/50\n",score);
    if(resCount<100) {
        strcpy(results[resCount].name, name);
        strcpy(results[resCount].challenge, "EK");
        results[resCount].score = score;
        results[resCount].attempts++;
        resCount++;
    }
}

int cmp(const void *a, const void *b) {
    return strcmp(((Result*)a)->challenge, ((Result*)b)->challenge);
}

void showResults() {
    if(resCount==0) { printf("No results.\n"); return; }
    qsort(results, resCount, sizeof(Result), cmp);
    printf("Results:\n");
    for(int i=0;i<resCount;i++) {
        printf("%s | %s | Score: %d | Attempts: %d\n",
            results[i].name, results[i].challenge, results[i].score, results[i].attempts);
    }
}

void learnHangul() {
    char name[MAX_NAME];
    printf("Your name: ");
    fgets(name, MAX_NAME, stdin);
    name[strcspn(name,"\n")]=0;
    if(strlen(name)==0) return;

    while(1) {
        printf("\n1. Hangul to English\n2. English to Hangul\n3. View Results\n0. Exit\nChoice: ");
        int c; scanf("%d",&c); flush();
        if(c==0) break;
        if(c==1 || c==2) {
            printf("1. Consonants 2. Vowels: ");
            int t; scanf("%d",&t); flush();
            if(t!=1 && t!=2) continue;
            if(c==1) korToEng(name, t==1?consonants:vowels, t==1?14:10, t==1?'C':'V');
            else engToKor(name, t==1?consonants:vowels, t==1?14:10, t==1?'C':'V');
        } else if(c==3) showResults();
        else printf("Invalid\n");
    }
}

int main() {
    srand(time(NULL));
    learnHangul();
    return 0;
}
