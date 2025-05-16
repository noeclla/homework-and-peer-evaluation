#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "stage8.h"

#if !defined(__GNUC__) || __GNUC__ != 9
#error "Compiler version mismatch. Compilation cannot proceed."
#endif

#define MAX_MANAGERS 10
#define TOTAL_FANS 1000
#define TOTAL_SEATS 42

typedef struct SeatNode {
    int seatNo, fanID;
    struct SeatNode *next;
} SeatNode;

static SeatNode *head = NULL;
static int numManagers = 4;
static int managers[MAX_MANAGERS];
static int fans[TOTAL_SEATS]; // 38 fans + managers

static int isSelected(int id, int count) {
    for (int i = 0; i < count; i++) if (fans[i] == id) return 1;
    return 0;
}

static void insertNode(int seatNo, int fanID) {
    SeatNode *n = malloc(sizeof(SeatNode));
    assert(n);
    n->seatNo = seatNo; n->fanID = fanID; n->next = NULL;

    if (!head || seatNo < head->seatNo) {
        n->next = head; head = n;
        return;
    }
    SeatNode *cur = head;
    while (cur->next && cur->next->seatNo < seatNo) cur = cur->next;
    n->next = cur->next; cur->next = n;
}

void openDemoConcert(void) {
    printf("Managers (1-%d): ", MAX_MANAGERS);
    if (scanf("%d", &numManagers) != 1 || numManagers < 1 || numManagers > MAX_MANAGERS) numManagers = 4;
    while(getchar() != '\n');

    srand((unsigned)time(NULL));

    for (int i = 0; i < numManagers; i++) fans[i] = i + 1;

    int count = numManagers;
    while (count < TOTAL_SEATS) {
        int id = (rand() % (TOTAL_FANS - numManagers)) + numManagers + 1;
        if (!isSelected(id, count)) fans[count++] = id;
    }

    for (int i = 0; i < numManagers; i++) insertNode(i + 1, fans[i]);

    for (int i = numManagers; i < TOTAL_SEATS; i++) {
        int j = i + rand() % (TOTAL_SEATS - i);
        int tmp = fans[i]; fans[i] = fans[j]; fans[j] = tmp;
    }
    for (int i = numManagers; i < TOTAL_SEATS; i++) insertNode(i + 1, fans[i]);

    printf("\nConcert Seating:\n");
    int seatMap[TOTAL_SEATS + 1] = {0};
    for (SeatNode *cur = head; cur; cur = cur->next) seatMap[cur->seatNo] = cur->fanID;

    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            int s = r * 7 + c + 1;
            printf("[%04d] ", seatMap[s]);
        }
        printf("\n");
    }

    while (head) {
        SeatNode *t = head;
        head = head->next;
        free(t);
    }
}
