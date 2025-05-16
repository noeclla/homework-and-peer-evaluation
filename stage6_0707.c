#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTENT_LEN 100
#define QUEUE_SIZE 20

typedef struct {
    int id;
    char content[MAX_CONTENT_LEN];
} Bucket;

typedef struct {
    Bucket *arr;
    int size;
    int capacity;
} BucketList;

typedef struct {
    Bucket queue[QUEUE_SIZE];
    int front, rear, count;
} BucketQueue;

void initBucketList(BucketList *list) {
    list->capacity = 10;
    list->size = 0;
    list->arr = malloc(sizeof(Bucket) * list->capacity);
}

void addBucket(BucketList *list, Bucket b) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        Bucket *tmp = realloc(list->arr, sizeof(Bucket) * list->capacity);
        if (!tmp) {
            perror("Memory realloc failed");
            free(list->arr);
            exit(1);
        }
        list->arr = tmp;
    }
    list->arr[list->size++] = b;
}

void freeBucketList(BucketList *list) {
    free(list->arr);
    list->arr = NULL;
    list->size = list->capacity = 0;
}

void initQueue(BucketQueue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int isQueueFull(BucketQueue *q) {
    return q->count == QUEUE_SIZE;
}

int isQueueEmpty(BucketQueue *q) {
    return q->count == 0;
}

void enqueue(BucketQueue *q, Bucket b) {
    if (isQueueFull(q)) {
        printf("Completed bucket queue is full!\n");
        return;
    }
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->queue[q->rear] = b;
    q->count++;
}

Bucket dequeue(BucketQueue *q) {
    Bucket empty = {0,""};
    if (isQueueEmpty(q)) return empty;
    Bucket b = q->queue[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;
    return b;
}

// Load bucket list from CSV file (format: ID,Content)
int loadBuckets(const char *filename, BucketList *list) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Failed to open bucket list file");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        Bucket b;
        char *token = strtok(line, ",");
        if (!token) continue;
        b.id = atoi(token);
        token = strtok(NULL, "\n");
        if (!token) continue;
        strncpy(b.content, token, MAX_CONTENT_LEN-1);
        b.content[MAX_CONTENT_LEN-1] = '\0';
        addBucket(list, b);
    }
    fclose(f);
    return 1;
}

// Print bucket list
void printBuckets(BucketList *list) {
    if (list->size == 0) {
        printf("No remaining buckets.\n");
        return;
    }
    printf("Remaining Buckets:\n");
    for (int i = 0; i < list->size; i++) {
        printf("ID: %d, Content: %s\n", list->arr[i].id, list->arr[i].content);
    }
}

// Remove bucket by ID, return bucket removed or NULL if not found
int removeBucketById(BucketList *list, int id, Bucket *removed) {
    for (int i = 0; i < list->size; i++) {
        if (list->arr[i].id == id) {
            *removed = list->arr[i];
            for (int j = i; j < list->size -1; j++) {
                list->arr[j] = list->arr[j+1];
            }
            list->size--;
            return 1;
        }
    }
    return 0;
}

// Print completed buckets queue
void printCompletedBuckets(BucketQueue *q) {
    if (isQueueEmpty(q)) {
        printf("No completed buckets yet.\n");
        return;
    }
    printf("Completed Buckets:\n");
    for (int i = 0; i < q->count; i++) {
        int idx = (q->front + i) % QUEUE_SIZE;
        printf("ID: %d, Content: %s\n", q->queue[idx].id, q->queue[idx].content);
    }
}

// Linear search by ID or partial content
void searchBuckets(BucketList *list, const char *query) {
    int found = 0;
    printf("Search results for '%s':\n", query);
    for (int i = 0; i < list->size; i++) {
        char idStr[20];
        sprintf(idStr, "%d", list->arr[i].id);
        if (strstr(idStr, query) || strstr(list->arr[i].content, query)) {
            printf("ID: %d, Content: %s\n", list->arr[i].id, list->arr[i].content);
            found = 1;
        }
    }
    if (!found) printf("No matching bucket found.\n");
}

// Main bucket list management function
void makeBucketList() {
    BucketList list;
    BucketQueue completed;
    initBucketList(&list);
    initQueue(&completed);

    if (!loadBuckets("buckets.csv", &list)) return;

    while (list.size > 0) {
        printBuckets(&list);
        printCompletedBuckets(&completed);

        printf("Enter completed bucket ID (or 0 to exit, -1 to search): ");
        int id;
        if (scanf("%d", &id) != 1) break;
        getchar(); // consume newline

        if (id == 0) break;

        if (id == -1) {
            char query[50];
            printf("Enter search keyword (ID or text): ");
            fgets(query, sizeof(query), stdin);
            query[strcspn(query, "\n")] = 0; // remove newline
            searchBuckets(&list, query);
            continue;
        }

        Bucket done;
        if (removeBucketById(&list, id, &done)) {
            enqueue(&completed, done);
            printf("Bucket ID %d marked as completed.\n", id);
        } else {
            printf("Bucket ID %d not found.\n", id);
        }
    }

    printf("All buckets completed or exit selected.\n");
    printCompletedBuckets(&completed);
    freeBucketList(&list);
}
