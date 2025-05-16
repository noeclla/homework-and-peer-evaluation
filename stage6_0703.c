#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 64
#define MAX_REASON 128
#define DEQUE_SIZE 5

typedef struct {
    char *style;
    char *reason;
} Fashion;

typedef struct {
    char firstname[MAX_NAME];
    char lastname[MAX_NAME];
    char nickname[MAX_NAME];
} Member;

typedef struct {
    Fashion *items[DEQUE_SIZE];
    int front, rear, count;
} CircularDeque;

void initDeque(CircularDeque *d) {
    d->front = 0; d->rear = -1; d->count = 0;
}
int isEmpty(CircularDeque *d) { return d->count == 0; }
int isFull(CircularDeque *d) { return d->count == DEQUE_SIZE; }
int pushFront(CircularDeque *d, Fashion *f) {
    if (isFull(d)) { printf("Deque is full!\n"); return 0; }
    d->front = (d->front - 1 + DEQUE_SIZE) % DEQUE_SIZE;
    d->items[d->front] = f; d->count++; return 1;
}
int pushRear(CircularDeque *d, Fashion *f) {
    if (isFull(d)) { printf("Deque is full!\n"); return 0; }
    d->rear = (d->rear + 1) % DEQUE_SIZE;
    d->items[d->rear] = f; d->count++; return 1;
}
void printDeque(CircularDeque *d) {
    printf("\nDeque filled! Styles:\n");
    for (int i=0, idx=d->front; i<d->count; i++) {
        Fashion *f = d->items[idx];
        printf("- %s: %s\n", f->style, f->reason);
        idx = (idx + 1) % DEQUE_SIZE;
    }
}

void clearInput() { while(getchar() != '\n'); }

int readFashion(const char *file, Fashion **male, int *mCount,
                Fashion **female, int *fCount) {
    FILE *f = fopen(file, "r");
    if (!f) { printf("Cannot open %s\n", file); return 0; }
    char line[256]; int mode=0, mCap=5, fCap=5;
    *male = malloc(sizeof(Fashion)*mCap);
    *female = malloc(sizeof(Fashion)*fCap);
    *mCount = 0; *fCount = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Male", 4) == 0) { mode=1; continue; }
        if (strncmp(line, "Female", 6) == 0) { mode=2; continue; }
        if (strncmp(line, "---", 3) == 0) { mode=0; continue; }
        if (mode == 0) continue;

        char *sep = strchr(line, ':');
        if (!sep) continue;
        *sep = 0;
        char *style = strdup(line);
        char *reason = strdup(sep+1);
        reason[strcspn(reason, "\r\n")] = 0;

        if (mode == 1) {
            if (*mCount == mCap) {
                mCap *= 2;
                *male = realloc(*male, sizeof(Fashion)*mCap);
            }
            (*male)[(*mCount)++] = (Fashion){style, reason};
        } else if (mode == 2) {
            if (*fCount == fCap) {
                fCap *= 2;
                *female = realloc(*female, sizeof(Fashion)*fCap);
            }
            (*female)[(*fCount)++] = (Fashion){style, reason};
        } else {
            free(style); free(reason);
        }
    }
    fclose(f);
    return 1;
}

void freeFashion(Fashion *arr, int n) {
    for (int i=0; i<n; i++) {
        free(arr[i].style);
        free(arr[i].reason);
    }
    free(arr);
}

int readMembers(const char *file, Member **mems) {
    FILE *f = fopen(file, "r");
    if (!f) { printf("Cannot open %s\n", file); return 0; }
    int cap=10, count=0;
    *mems = malloc(sizeof(Member)*cap);
    char line[256];
    while(fgets(line, sizeof(line), f)) {
        if (count == cap) {
            cap *= 2;
            *mems = realloc(*mems, sizeof(Member)*cap);
        }
        char *tok = strtok(line, ",");
        if (!tok) continue;
        strncpy((*mems)[count].firstname, tok, MAX_NAME-1);
        tok = strtok(NULL, ",");
        if (!tok) continue;
        strncpy((*mems)[count].lastname, tok, MAX_NAME-1);
        tok = strtok(NULL, ",\r\n");
        if (!tok) continue;
        strncpy((*mems)[count].nickname, tok, MAX_NAME-1);
        count++;
    }
    fclose(f);
    return count;
}

Member* findMember(Member *arr, int n, const char *nick) {
    for (int i=0; i<n; i++)
        if (strcmp(arr[i].nickname, nick) == 0) return &arr[i];
    return NULL;
}

void understandFashion() {
    srand(time(NULL));

    Fashion *maleFash = NULL, *femaleFash = NULL;
    int mCount = 0, fCount = 0;
    if (!readFashion("fashion.txt", &maleFash, &mCount, &femaleFash, &fCount))
        return;

    Member *members = NULL;
    int memCount = readMembers("milliways_member.csv", &members);
    if (memCount == 0) {
        printf("No members found.\n");
        freeFashion(maleFash, mCount);
        freeFashion(femaleFash, fCount);
        return;
    }

    CircularDeque deque;
    initDeque(&deque);

    while (!isFull(&deque)) {
        printf("\nEnter member nickname (or 'exit' to quit): ");
        char nick[MAX_NAME];
        if (!fgets(nick, sizeof(nick), stdin)) break;
        nick[strcspn(nick, "\r\n")] = 0;
        if (strcmp(nick, "exit") == 0) break;

        Member *m = findMember(members, memCount, nick);
        if (!m) {
            printf("Nickname not found. Try again.\n");
            continue;
        }

        char g;
        do {
            printf("Is %s male or female? (m/f): ", m->nickname);
            g = getchar();
            clearInput();
        } while (g != 'm' && g != 'M' && g != 'f' && g != 'F');

        int gender = (g == 'm' || g == 'M') ? 1 : 2;
        Fashion *arr = (gender == 1) ? maleFash : femaleFash;
        int count = (gender == 1) ? mCount : fCount;
        int center = rand() % count;

        printf("Random style: %s - %s\n", arr[center].style, arr[center].reason);

        char pos;
        do {
            printf("Add to front (f) or rear (r)? ");
            pos = getchar();
            clearInput();
        } while (pos != 'f' && pos != 'F' && pos != 'r' && pos != 'R');

        if (pos == 'f' || pos == 'F') {
            pushFront(&deque, &arr[center]);
        } else {
            pushRear(&deque, &arr[center]);
        }

        for (int i = center - 1; i >= 0 && !isFull(&deque); i--) {
            if (pos == 'f' || pos == 'F')
                pushFront(&deque, &arr[i]);
            else
                pushRear(&deque, &arr[i]);
        }
        for (int i = center + 1; i < count && !isFull(&deque); i++) {
            if (pos == 'f' || pos == 'F')
                pushRear(&deque, &arr[i]);
            else
                pushFront(&deque, &arr[i]);
        }

        printf("Deque size: %d/%d\n", deque.count, DEQUE_SIZE);
    }

    printDeque(&deque);
    printf("Congrats! Deque fully filled.\n");

    freeFashion(maleFash, mCount);
    freeFashion(femaleFash, fCount);
    free(members);
}
