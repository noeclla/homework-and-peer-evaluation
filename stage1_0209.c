#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_WORDS 10
#define WORD_LEN 6
#define RANDOM_NUM 5
#define MIN_LEN 3
#define MAX_LEN 5
#define MIN_SENT 90
#define MAX_SENT 100
#define BUF_SIZE 256

char wordbook[MAX_WORDS][WORD_LEN];
char random1[RANDOM_NUM][WORD_LEN];
char random2[RANDOM_NUM][WORD_LEN];

void inputWords() {
    int i = 0;
    while(i < MAX_WORDS) {
        printf("Enter word %d (3-5 letters): ", i+1);
        fgets(wordbook[i], WORD_LEN, stdin);
        wordbook[i][strcspn(wordbook[i], "\n")] = 0;
        int len = strlen(wordbook[i]);
        if(len < MIN_LEN || len > MAX_LEN) {
            printf("Invalid length.\n");
            continue;
        }
        i++;
    }
}

int inSet(char arr[RANDOM_NUM][WORD_LEN], char *w, int size) {
    for(int i=0; i<size; i++)
        if(strcmp(arr[i], w) == 0) return 1;
    return 0;
}

void getRandomWords(char dest[RANDOM_NUM][WORD_LEN], char avoid[RANDOM_NUM][WORD_LEN], int checkOverlap) {
    int c = 0;
    while(c < RANDOM_NUM) {
        int r = rand() % MAX_WORDS;
        if(inSet(dest, wordbook[r], c)) continue;
        if(checkOverlap) {
            int overlap = 0;
            for(int i=0; i<c; i++) {
                for(int j=0; j<RANDOM_NUM; j++) {
                    if(strcmp(dest[i], avoid[j]) == 0) overlap++;
                }
            }
            if(overlap >= 2) continue;
            if(inSet(avoid, wordbook[r], RANDOM_NUM)) continue;
        }
        strcpy(dest[c++], wordbook[r]);
    }
}

void printWords(char arr[RANDOM_NUM][WORD_LEN]) {
    printf("Random words: ");
    for(int i=0; i<RANDOM_NUM; i++) printf("%s ", arr[i]);
    printf("\n");
}

int countUsed(char *sent, char arr[RANDOM_NUM][WORD_LEN]) {
    int cnt = 0;
    for(int i=0; i<RANDOM_NUM; i++)
        if(strstr(sent, arr[i])) cnt++;
    return cnt;
}

int countRepeats(char *sent, char arr[RANDOM_NUM][WORD_LEN]) {
    int rep = 0;
    for(int i=0; i<RANDOM_NUM; i++) {
        char *p = sent;
        int wc = 0;
        while((p = strstr(p, arr[i])) != NULL) {
            wc++;
            p += strlen(arr[i]);
        }
        if(wc > 1) rep += wc - 1;
    }
    return rep;
}

void writeRandomSentence() {
    inputWords();
    srand(time(NULL));
    getRandomWords(random1, NULL, 0);
    printWords(random1);

    int tries = 0;
    while(tries < 2) {
        printf("New random words? (y/n): ");
        char c = getchar();
        while(getchar() != '\n');
        if(c == 'y' && tries == 0) {
            getRandomWords(random2, random1, 1);
            printWords(random2);
            memcpy(random1, random2, sizeof(random2));
            tries++;
        } else break;
    }

    char sentence[BUF_SIZE];
    while(1) {
        printf("Write a story (90-100 chars):\n");
        fgets(sentence, BUF_SIZE, stdin);
        sentence[strcspn(sentence, "\n")] = 0;
        int len = strlen(sentence);
        if(len < MIN_SENT || len > MAX_SENT) {
            printf("Length out of range.\n");
            continue;
        }
        break;
    }

    printf("Used words: %d\n", countUsed(sentence, random1));
    printf("Characters: %d\n", (int)strlen(sentence));
    printf("Repeated words: %d\n", countRepeats(sentence, random1));
}
