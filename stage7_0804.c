#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 6
#define MAX_NAME 50
#define MAX_DESC 200

typedef struct PsychNode {
    int priority;
    char name[MAX_NAME];
    char desc[MAX_DESC];
    struct PsychNode *next;
} PsychNode;

typedef struct {
    PsychNode *front, *rear;
    int size;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = NULL;
    q->size = 0;
}

void enqueue(Queue *q, PsychNode *node) {
    if (!q->front) {
        q->front = q->rear = node;
        node->next = NULL;
    } else {
        PsychNode *cur = q->front, *prev = NULL;
        while (cur && cur->priority <= node->priority) {
            prev = cur;
            cur = cur->next;
        }
        if (!prev) { node->next = q->front; q->front = node; }
        else { node->next = cur; prev->next = node; }
        if (!cur) q->rear = node;
    }
    q->size++;
}

PsychNode *dequeue(Queue *q) {
    if (!q->front) return NULL;
    PsychNode *node = q->front;
    q->front = node->next;
    if (!q->front) q->rear = NULL;
    node->next = NULL;
    q->size--;
    return node;
}

int loadData(const char *file, PsychNode *arr[]) {
    FILE *f = fopen(file, "r");
    if (!f) { printf("File error\n"); return 0; }
    char line[256];
    int i = 0;
    while (i < MAX_ITEMS && fgets(line, sizeof(line), f)) {
        PsychNode *p = malloc(sizeof(PsychNode));
        p->priority = atoi(line);
        fgets(p->name, MAX_NAME, f);
        fgets(p->desc, MAX_DESC, f);
        p->name[strcspn(p->name, "\r\n")] = 0;
        p->desc[strcspn(p->desc, "\r\n")] = 0;
        p->next = NULL;
        arr[i++] = p;
    }
    fclose(f);
    return i;
}

void printAll(PsychNode *arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("Priority: %d\nName: %s\nDesc: %s\n\n", arr[i]->priority, arr[i]->name, arr[i]->desc);
}

void writeReport(const char *file, Queue *q) {
    if (q->size == 0) { printf("Queue empty\n"); return; }
    PsychNode *arr[MAX_ITEMS];
    int n = 0;
    for (PsychNode *cur = q->front; cur; cur = cur->next) arr[n++] = cur;

    // Sort descending priority
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-1-i; j++)
            if (arr[j]->priority < arr[j+1]->priority) {
                PsychNode *tmp = arr[j]; arr[j] = arr[j+1]; arr[j+1] = tmp;
            }

    FILE *f = fopen(file, "w");
    if (!f) { printf("Write error\n"); return; }
    for (int i = 0; i < n; i++)
        fprintf(f, "Priority: %d\nName: %s\nDesc: %s\n\n", arr[i]->priority, arr[i]->name, arr[i]->desc);
    fclose(f);
    printf("Report saved to %s\n", file);
}

void printReport(const char *file) {
    FILE *f = fopen(file, "r");
    if (!f) { printf("Cannot open %s\n", file); return; }
    printf("\n--- Report ---\n");
    char line[256];
    while (fgets(line, sizeof(line), f)) printf("%s", line);
    fclose(f);
    printf("--------------\n");
}

void freeAll(PsychNode *arr[], int n) {
    for (int i = 0; i < n; i++) free(arr[i]);
}

void analyzFanPsychology() {
    PsychNode *psychArr[MAX_ITEMS];
    int count = loadData("psychology.txt", psychArr);
    if (count == 0) return;

    Queue q; initQueue(&q);
    int choice, prio;

    while (1) {
        printf("\n1.Show all  2.Add by priority  3.Save & show report  0.Exit\nChoose: ");
        if (scanf("%d", &choice) != 1) break;

        switch(choice) {
            case 1:
                printAll(psychArr, count);
                break;
            case 2:
                if (q.size >= count) { printf("Queue full\n"); break; }
                printf("Enter priority: ");
                if (scanf("%d", &prio) != 1) break;

                // find and check duplicates
                int found = 0;
                for (int i = 0; i < count; i++) {
                    if (psychArr[i]->priority == prio) {
                        PsychNode *cur = q.front; int inQ = 0;
                        while(cur) { if (cur->priority == prio) { inQ=1; break; } cur=cur->next; }
                        if (inQ) { printf("Already in queue\n"); found = 1; break; }
                        PsychNode *copy = malloc(sizeof(PsychNode));
                        if (!copy) { perror("malloc"); break; }
                        *copy = *psychArr[i];
                        copy->next = NULL;
                        enqueue(&q, copy);
                        printf("Added %s\n", copy->name);
                        found = 1;
                        break;
                    }
                }
                if (!found) printf("No such priority\n");
                break;
            case 3:
                if (q.size == 0) { printf("Queue empty\n"); break; }
                writeReport("fan_report.txt", &q);
                printReport("fan_report.txt");
                while(q.size) free(dequeue(&q));
                break;
            case 0:
                while(q.size) free(dequeue(&q));
                freeAll(psychArr, count);
                return;
            default:
                printf("Invalid choice\n");
        }
        while(getchar() != '\n'); // flush stdin
    }
}
