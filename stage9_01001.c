#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_COMPOSERS 3
#define MAX_TITLE 50
#define MAX_DESC 100

typedef struct Song {
    char title[MAX_TITLE];
    char desc[MAX_DESC];
    int priority;
    struct Song *next;
} Song;

typedef struct {
    char name[30];
    Song *allSongs;
    Song *selected;
    int total;
} Composer;

Composer composers[NUM_COMPOSERS] = {0};

void addSong(Composer *c, const char *title, const char *desc) {
    Song *s = malloc(sizeof(Song));
    strncpy(s->title, title, MAX_TITLE);
    strncpy(s->desc, desc, MAX_DESC);
    s->priority = 0;
    s->next = c->allSongs;
    c->allSongs = s;
    c->total++;
}

void readSongs() {
    FILE *f = fopen("music.txt", "r");
    if (!f) { perror("music.txt"); exit(1); }
    char line[200], comp[30], title[50], desc[100];
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%29[^|]|%49[^|]|%99[^\n]", comp, title, desc) == 3) {
            int i;
            for (i = 0; i < NUM_COMPOSERS; i++) {
                if (strcmp(composers[i].name, comp) == 0 || composers[i].name[0] == 0) {
                    if (composers[i].name[0] == 0)
                        strcpy(composers[i].name, comp);
                    addSong(&composers[i], title, desc);
                    break;
                }
            }
        }
    }
    fclose(f);
}

Song* copySong(Song *src) {
    Song *s = malloc(sizeof(Song));
    *s = *src;
    s->next = NULL;
    return s;
}

// Select first N songs per composer (simple: pick first N, max 2 consecutive)
void selectSongs(int picks[]) {
    int globalPri = 1;
    for (int i = 0; i < NUM_COMPOSERS; i++) {
        Song *cur = composers[i].allSongs;
        int count = 0, cons = 0;
        while (cur && count < picks[i]) {
            // check consecutive pick limit
            if (composers[i].selected && composers[i].selected->priority == globalPri - 1 && cons == 2) break;
            Song *sel = copySong(cur);
            sel->priority = globalPri++;
            sel->next = composers[i].selected;
            composers[i].selected = sel;
            count++;
            cons++;
            cur = cur->next;
        }
    }
}

// Merge and sort selected songs by priority ascending
Song* mergeAndSort() {
    // Collect all selected songs in array
    int total = 0;
    for (int i = 0; i < NUM_COMPOSERS; i++) {
        Song *cur = composers[i].selected;
        while (cur) { total++; cur = cur->next; }
    }
    Song **arr = malloc(sizeof(Song*) * total);
    int idx = 0;
    for (int i = 0; i < NUM_COMPOSERS; i++) {
        Song *cur = composers[i].selected;
        while (cur) { arr[idx++] = cur; cur = cur->next; }
    }
    // Sort by priority ascending (simple bubble sort)
    for (int i = 0; i < total-1; i++) {
        for (int j = 0; j < total-1-i; j++) {
            if (arr[j]->priority > arr[j+1]->priority) {
                Song *tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
    // Link list
    for (int i = 0; i < total-1; i++) arr[i]->next = arr[i+1];
    if (total > 0) arr[total-1]->next = NULL;
    Song *head = (total > 0) ? arr[0] : NULL;
    free(arr);
    return head;
}

void printAlbum(Song *head) {
    int num = 1;
    while (head) {
        printf("%d. %s - %s\n", num++, head->title, head->desc);
        head = head->next;
    }
}

void makeDebutAlbum() {
    readSongs();
    int picks[NUM_COMPOSERS] = {3,3,3}; // hardcoded picks
    selectSongs(picks);
    Song *album = mergeAndSort();
    printAlbum(album);
}

int main() {
    makeDebutAlbum();
    return 0;
}
