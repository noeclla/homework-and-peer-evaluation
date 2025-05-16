#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "stage8.h"

#if !defined(__GNUC__) || __GNUC__ != 9
#error "Compiler version mismatch. Compilation cannot proceed."
#endif

#define MAX_QUEUE 10

typedef struct Node {
    int val;
    struct Node *next;
} Node;

static int queue[MAX_QUEUE];
static int front = 0, rear = 0, count = 0;
static Node *head = NULL;

int isFull() { return count == MAX_QUEUE; }
int isEmpty() { return count == 0; }

void enqueue(int val) {
    if (isFull()) {
        printf("Queue is full. Cannot enqueue %d\n", val);
        return;
    }
    queue[rear] = val;
    rear = (rear + 1) % MAX_QUEUE;
    count++;
}

int dequeue() {
    if (isEmpty()) return -1;
    int val = queue[front];
    front = (front + 1) % MAX_QUEUE;
    count--;
    return val;
}

void printQueue() {
    printf("Queue: ");
    for (int i = 0, idx = front; i < count; i++, idx = (idx + 1) % MAX_QUEUE)
        printf("%d ", queue[idx]);
    printf("\n");
}

void addNode(int val) {
    Node *n = malloc(sizeof(Node));
    assert(n);
    n->val = val; n->next = NULL;
    if (!head) {
        head = n;
        return;
    }
    Node *cur = head;
    while (cur->next) cur = cur->next;
    cur->next = n;
}

int countRangePairs() {
    int pairs = 0;
    for (Node *cur = head; cur && cur->next; cur = cur->next) {
        int s = cur->val + cur->next->val;
        if (s >= 24 && s <= 42) pairs++;
    }
    return pairs;
}

void freeList() {
    while (head) {
        Node *t = head;
        head = head->next;
        free(t);
    }
}

void bufferMsgSpeed(void) {
    int n;
    printf("Enter number of messages: ");
    if (scanf("%d", &n) != 1 || n < 1) return;
    while (getchar() != '\n');

    srand((unsigned)time(NULL));
    freeList();
    front = rear = count = 0;

    for (int i = 0; i < n; i++) {
        int num = rand() % 100 + 1;

        if (num % 5 == 0) {
            printf("Enqueue good message: %d\n", num);
            enqueue(num);
            printQueue();
        }

        if (num % 10 == 0) {
            int val = dequeue();
            if (val != -1) {
                printf("Dequeue message: %d\n", val);
                addNode(val);
                printQueue();
            } else {
                printf("Queue empty, cannot dequeue\n");
            }
        }

        if (isFull()) {
            printf("Queue is full at iteration %d\n", i + 1);
        }
    }

    int pairs = countRangePairs();
    printf("Pairs of adjacent nodes with sum in [24..42]: %d\n", pairs);

    freeList();
}
