#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PATTERNS 6
#define MAX_SONGS 4
#define MAX_LEN 50
#define QUIZ_COUNT 4
#define SUB_LEN 10

typedef struct {
    char name[MAX_LEN];
    char desc[256];
} Pattern;

typedef struct Node {
    char name[MAX_LEN];
    struct Node *next;
} Node;

typedef struct {
    char songName[MAX_LEN];
    Node *head;
} Song;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Load patterns from file
int loadPatterns(const char *file, Pattern pats[]) {
    FILE *f = fopen(file, "r");
    if (!f) return 0;
    for (int i=0; i<MAX_PATTERNS; i++) {
        if (!fgets(pats[i].name, MAX_LEN, f)) break;
        pats[i].name[strcspn(pats[i].name,"\r\n")] = 0;
        if (!fgets(pats[i].desc, 256, f)) break;
        pats[i].desc[strcspn(pats[i].desc,"\r\n")] = 0;
    }
    fclose(f);
    return 1;
}

// Get random substring (no spaces) from description
void randomSubstr(const char *src, char *dest) {
    int len = strlen(src);
    int tries = 100;
    while (tries--) {
        int start = rand() % len, c=0, i=start;
        while (c < SUB_LEN && i < len) {
            if (src[i] != ' ') dest[c++] = src[i];
            i++;
        }
        if (c == SUB_LEN) {
            dest[c] = 0;
            return;
        }
    }
    strncpy(dest, src, SUB_LEN);
    dest[SUB_LEN] = 0;
}

// Quiz user on pattern names
int quiz(Pattern pats[]) {
    int score = 0;
    char input[MAX_LEN], hint[SUB_LEN+1];
    for (int i=0; i<QUIZ_COUNT; i++) {
        int idx = rand() % MAX_PATTERNS;
        randomSubstr(pats[idx].desc, hint);
        printf("\nDesc: %s\nHint: %s\nYour answer: ", pats[idx].desc, hint);
        scanf(" %49[^\n]", input);
        if (strcmp(input, pats[idx].name) == 0) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Nope! Correct: %s\n", pats[idx].name);
        }
    }
    printf("Score: %d/%d\n", score, QUIZ_COUNT);
    return score;
}

// Load song trees from CSV
int loadSongs(const char *file, Song songs[]) {
    FILE *f = fopen(file, "r");
    if (!f) return 0;
    char line[256];
    int curSong = -1;
    Node *last = NULL;
    for (int i=0; i<MAX_SONGS; i++) {
        songs[i].head = NULL;
        songs[i].songName[0] = 0;
    }
    while (fgets(line, 256, f)) {
        char *token = strtok(line, ",");
        if (!token) continue;
        if (curSong == -1 || strcmp(token, songs[curSong].songName) != 0) {
            curSong++;
            if (curSong >= MAX_SONGS) break;
            strcpy(songs[curSong].songName, token);
            songs[curSong].head = NULL;
            last = NULL;
            token = strtok(NULL, ",");
        }
        if (token) {
            Node *node = malloc(sizeof(Node));
            strcpy(node->name, token);
            node->next = NULL;
            if (!last) songs[curSong].head = node;
            else last->next = node;
            last = node;
        }
    }
    fclose(f);
    return 1;
}

// Print all song patterns
void printSongs(Song songs[]) {
    printf("\nSongs & their choreography patterns:\n");
    for (int i=0; i<MAX_SONGS; i++) {
        printf("%d. %s\n", i+1, songs[i].songName);
        Node *cur = songs[i].head;
        int n = 1;
        while(cur) {
            printf("   %d) %s\n", n++, cur->name);
            cur = cur->next;
        }
    }
}

// Practice a selected song
void practiceSong(Song *song) {
    if (!song || !song->head) return;
    Node *cur = song->head->next;
    char input[MAX_LEN];
    printf("\nPractice '%s'\nFirst pattern: %s\n", song->songName, song->head->name);
    while (cur) {
        printf("Next pattern? ");
        scanf(" %49[^\n]", input);
        if (strcmp(input, cur->name) != 0) {
            printf("Wrong! Returning to menu.\n");
            return;
        }
        cur = cur->next;
    }
    printf("Great! You finished all patterns!\n");
}

// Main function
void learnDancePattern() {
    Pattern pats[MAX_PATTERNS];
    Song songs[MAX_SONGS];
    srand((unsigned)time(NULL));

    if (!loadPatterns("dance_pattern.txt", pats)) {
        printf("Can't read dance patterns.\n");
        return;
    }
    int score = quiz(pats);
    if (score < 3) {
        printf("Need 3+ correct to continue.\n");
        return;
    }
    if (!loadSongs("analyz_dance-pattern.csv", songs)) {
        printf("Can't read song analysis.\n");
        return;
    }
    printSongs(songs);

    int choice;
    printf("\nChoose a song to practice (1-%d) or 0 to exit: ", MAX_SONGS);
    scanf("%d", &choice);
    if (choice < 1 || choice > MAX_SONGS) return;

    clearScreen();
    practiceSong(&songs[choice - 1]);

    // Free song nodes here (not shown)
}
