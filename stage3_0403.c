#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 50
#define MAX_WORDS 10
#define QUESTIONS 5
#define MAX_TRAINEES 10

typedef struct {
    char eng[100];
    char kor[100];
} EngKor;

typedef struct {
    char nickname[MAX_NAME];
    int score;
} TraineeScore;

EngKor sentences[10] = {
    {"I eat rice every day", "나는 매일 밥을 먹는다"},
    {"She likes to read books", "그녀는 책을 읽는 것을 좋아한다"},
    {"We are going to school", "우리는 학교에 간다"},
    {"He plays soccer well", "그는 축구를 잘 한다"},
    {"They watch movies together", "그들은 함께 영화를 본다"},
    {"The weather is very cold", "날씨가 매우 춥다"},
    {"I want to learn Korean", "나는 한국어를 배우고 싶다"},
    {"My friend lives in Seoul", "내 친구는 서울에 산다"},
    {"We study hard every day", "우리는 매일 열심히 공부한다"},
    {"She cooks delicious food", "그녀는 맛있는 음식을 요리한다"}
};

TraineeScore scores[MAX_TRAINEES] = {
    {"Alex",0},{"Bob",0},{"Cindy",0},{"Diana",0},{"Ethan",0},
    {"Fiona",0},{"George",0},{"Helen",0},{"Ian",0},{"Jane",0}
};

int scoreCount=0;

void flush() { while(getchar()!='\n'); }

void selectionSort(char arr[][20], int n) {
    int i,j,min_idx;
    char temp[20];
    for(i=0; i<n-1; i++) {
        min_idx = i;
        for(j=i+1; j<n; j++) {
            if(strcmp(arr[j], arr[min_idx]) < 0) min_idx = j;
        }
        if(min_idx != i) {
            strcpy(temp, arr[i]);
            strcpy(arr[i], arr[min_idx]);
            strcpy(arr[min_idx], temp);
        }
    }
}

void quickSort(char arr[][20], int low, int high) {
    if(low >= high) return;
    int i = low, j = high;
    char pivot[20];
    strcpy(pivot, arr[low]);
    while(i<j) {
        while(i<j && strcmp(arr[j], pivot)>=0) j--;
        strcpy(arr[i], arr[j]);
        while(i<j && strcmp(arr[i], pivot)<=0) i++;
        strcpy(arr[j], arr[i]);
    }
    strcpy(arr[i], pivot);
    quickSort(arr, low, i-1);
    quickSort(arr, i+1, high);
}

void shuffleWords(char words[][20], int n) {
    for(int i=n-1; i>0; i--) {
        int j = rand() % (i+1);
        char temp[20];
        strcpy(temp, words[i]);
        strcpy(words[i], words[j]);
        strcpy(words[j], temp);
    }
    selectionSort(words, n); // change to quickSort(words,0,n-1); for bonus
}

int findTrainee(char *name) {
    for(int i=0;i<MAX_TRAINEES;i++)
        if(strcmp(scores[i].nickname,name)==0)
            return i;
    return -1;
}

void composeSentences(char *name) {
    int indices[QUESTIONS], used[10]={0}, qcount=0;
    while(qcount<QUESTIONS) {
        int r=rand()%10;
        if(!used[r]) { indices[qcount++]=r; used[r]=1; }
    }
    int total=0;
    for(int q=0;q<QUESTIONS;q++) {
        EngKor *s = &sentences[indices[q]];
        char korWords[20][20];
        int wordCount=0;
        char *token = strtok(s->kor, " ");
        while(token) {
            strcpy(korWords[wordCount++], token);
            token = strtok(NULL, " ");
        }
        shuffleWords(korWords, wordCount);

        printf("\nEnglish: %s\n", s->eng);
        printf("Arrange Korean words in order separated by spaces:\n");
        for(int i=0;i<wordCount;i++) printf("%s ", korWords[i]);
        printf("\nYour answer: ");

        char answer[200];
        fgets(answer, 200, stdin);
        answer[strcspn(answer,"\n")]=0;

        if(strcmp(answer, s->kor)==0) {
            printf("Correct!\n");
            total+=20;
        } else {
            printf("Wrong! Correct: %s\n", s->kor);
        }
    }
    int idx = findTrainee(name);
    if(idx>=0) scores[idx].score += total;
    else printf("Nickname not found.\n");
    printf("Your total score: %d/100\n", total);
}

void printScore(char *name) {
    int idx = findTrainee(name);
    if(idx<0) {
        printf("Nickname not found.\n");
        return;
    }
    if(scores[idx].score==0) {
        printf("You have not played yet.\n");
        return;
    }
    printf("\n%s's total score: %d\n", name, scores[idx].score);

    // Sort by score descending for ranking
    TraineeScore copy[MAX_TRAINEES];
    memcpy(copy, scores, sizeof(scores));
    for(int i=0;i<MAX_TRAINEES-1;i++) {
        for(int j=i+1;j<MAX_TRAINEES;j++) {
            if(copy[j].score > copy[i].score) {
                TraineeScore temp = copy[i];
                copy[i] = copy[j];
                copy[j] = temp;
            }
        }
    }
    printf("Rankings:\n");
    for(int i=0;i<MAX_TRAINEES;i++) {
        printf("%d. %s - %d\n", i+1, copy[i].nickname, copy[i].score);
    }
}

void learnKoreanGrammar() {
    char nickname[MAX_NAME];
    int tries=0, found=0;
    while(tries<3 && !found) {
        printf("Enter your nickname: ");
        fgets(nickname, MAX_NAME, stdin);
        nickname[strcspn(nickname,"\n")]=0;
        if(findTrainee(nickname)>=0) found=1;
        else {
            printf("Nickname not found. Try again.\n");
            tries++;
        }
    }
    if(!found) return;

    int played=0;
    while(1) {
        printf("\n1. Compose Korean Sentences\n2. Check Scores\n0. Exit\nChoose: ");
        int ch; scanf("%d", &ch); flush();
        if(ch==0) break;
        if(ch==1) {
            composeSentences(nickname);
            played=1;
        } else if(ch==2) {
            if(!played) printf("Play first to check scores.\n");
            else printScore(nickname);
        } else printf("Invalid choice.\n");
    }
}

int main() {
    srand(time(NULL));
    learnKoreanGrammar();
    return 0;
}
