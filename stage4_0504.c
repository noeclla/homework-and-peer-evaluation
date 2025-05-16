#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SONG_COUNT 10
#define GENRE_COUNT 10
#define MAX_LEN 50

typedef struct {
    char title[MAX_LEN];
    char genre[MAX_LEN];
    char singer[MAX_LEN];
} Song;

const char *genres[GENRE_COUNT] = {
    "Dance Pop", "Ballad", "Hip-hop", "R&B", "Rock",
    "Electronica", "Trot", "Reggae", "Fusion", "Band"
};

Song songs[SONG_COUNT] = {
    {"Call Me Baby", "Dance Pop", "EXO"},
    {"Through the Night", "Ballad", "IU"},
    {"Mic Drop", "Hip-hop", "BTS"},
    {"Eyes, Nose, Lips", "R&B", "Taeyang"},
    {"Shoot Me", "Rock", "Day6"},
    {"Electric Shock", "Electronica", "f(x)"},
    {"Love Tonight", "Trot", "Hong Jin-young"},
    {"Island", "Reggae", "WINNER"},
    {"Dumb Litty", "Fusion", "KARD"},
    {"I'm Sorry", "Band", "CNBLUE"}
};

typedef int (*CheckFunc)(const char *, const char *);
int checkGenre(const char *a, const char *b) { return strcmp(a, b) == 0; }

void printHint(const char *s) {
    int len = strlen(s), n = len <= 3 ? 1 : (len <= 5 ? 2 : 3);
    printf("Hint: Singer starts with '%.*s'\n", n, s);
}

void clearBuf(void) { while(getchar()!='\n'); }

void learnSongGenre(void) {
    srand((unsigned)time(NULL));
    int tries = 2, hintUsed = 0;
    char input[MAX_LEN];
    int idx = rand() % SONG_COUNT;
    Song *s = &songs[idx];
    CheckFunc cf = checkGenre;

    printf("Song: %s\n", s->title);

    while(tries--) {
        printf("Guess the genre: ");
        if(!fgets(input, sizeof(input), stdin)) return;
        input[strcspn(input, "\n")] = 0;
        if(cf(s->genre, input)) {
            printf("Correct! 🎉\n");
            return;
        }
        if(!hintUsed && tries > 0) {
            char c; 
            printf("Wrong! Want a hint? (y/n): ");
            c = getchar(); clearBuf();
            if(c=='y' || c=='Y') { printHint(s->singer); hintUsed=1; tries++; }
        } 
        else printf("Wrong! Tries left: %d\n", tries);
    }
    printf("No more tries! The correct genre: %s\n", s->genre);
}
