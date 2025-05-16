#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCESSORY_NAME 64
#define MAX_ACCESSORY_USE 128
#define MAX_LINE_LEN 256
#define QUEUE_SIZE 6

typedef struct {
    char *type;   // 액세서리 종류 (예: Necklace)
    char *usage;  // 액세서리 용도 설명
} Accessory;

typedef struct {
    Accessory *items[QUEUE_SIZE];
    int front;
    int rear;
    int count;
} CircularQueue;

void initQueue(CircularQueue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int isEmpty(CircularQueue *q) {
    return q->count == 0;
}

int isFull(CircularQueue *q) {
    return q->count == QUEUE_SIZE;
}

int enqueue(CircularQueue *q, Accessory *acc) {
    if (isFull(q)) {
        printf("Queue is full! Cannot add more accessories.\n");
        return 0;
    }
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->items[q->rear] = acc;
    q->count++;
    printf("Accessory '%s' added to queue.\n", acc->type);
    return 1;
}

void printQueue(CircularQueue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty.\n");
        return;
    }
    printf("\n-- Accessories in Circular Queue --\n");
    int idx = q->front;
    for (int i = 0; i < q->count; i++) {
        Accessory *acc = q->items[idx];
        printf("%d. %s : %s\n", i + 1, acc->type, acc->usage);
        idx = (idx + 1) % QUEUE_SIZE;
    }
}

void freeAccessories(Accessory *arr, int n) {
    for (int i = 0; i < n; i++) {
        free(arr[i].type);
        free(arr[i].usage);
    }
    free(arr);
}

int loadAccessories(const char *filename, Accessory **arr) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open accessory file");
        return 0;
    }

    int capacity = 6;
    int count = 0;
    Accessory *temp = malloc(sizeof(Accessory) * capacity);
    if (!temp) {
        fclose(fp);
        printf("Memory allocation failed\n");
        return 0;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0;

        if (strlen(line) == 0) continue;

        // 콜론(:) 기준 분리
        char *sep = strchr(line, ':');
        if (!sep) continue;

        *sep = '\0';
        char *type = line;
        char *usage = sep + 1;

        if (count == capacity) {
            capacity *= 2;
            Accessory *newArr = realloc(temp, sizeof(Accessory) * capacity);
            if (!newArr) {
                freeAccessories(temp, count);
                fclose(fp);
                printf("Memory reallocation failed\n");
                return 0;
            }
            temp = newArr;
        }

        temp[count].type = strdup(type);
        temp[count].usage = strdup(usage);
        count++;
    }
    fclose(fp);
    *arr = temp;
    return count;
}

void applyAccessory() {
    Accessory *accessories = NULL;
    int total = loadAccessories("accessory.txt", &accessories);
    if (total == 0) {
        printf("No accessories loaded.\n");
        return;
    }

    CircularQueue queue;
    initQueue(&queue);

    printf("Loaded %d accessories.\n", total);
    printf("Select at least 4 accessories to add to the circular queue.\n");

    int selectedCount = 0;
    while (selectedCount < 4 || selectedCount < QUEUE_SIZE) {
        printf("\nAvailable accessories:\n");
        for (int i = 0; i < total; i++) {
            printf("%d) %s : %s\n", i + 1, accessories[i].type, accessories[i].usage);
        }

        printf("Enter accessory number to add to queue (0 to finish): ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // 입력 버퍼 클리어
            printf("Invalid input. Try again.\n");
            continue;
        }
        while(getchar() != '\n');

        if (choice == 0) {
            if (selectedCount >= 4) break;
            else {
                printf("Please add at least 4 accessories.\n");
                continue;
            }
        }

        if (choice < 1 || choice > total) {
            printf("Choice out of range. Try again.\n");
            continue;
        }

        if (isFull(&queue)) {
            printf("Queue is full. Cannot add more.\n");
            break;
        }

        if (enqueue(&queue, &accessories[choice - 1])) {
            selectedCount++;
        }
    }

    printQueue(&queue);

    if (isEmpty(&queue)) {
        printf("No accessories selected.\n");
    } else if (isFull(&queue)) {
        printf("Queue is full with %d accessories.\n", queue.count);
    } else {
        printf("Queue has %d accessories (not full yet).\n", queue.count);
    }

    freeAccessories(accessories, total);
}
