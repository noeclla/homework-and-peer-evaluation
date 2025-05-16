#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TYPES 10
#define MAX_NAME_LEN 50
#define QUEUE_SIZE 10

typedef enum { MALE, FEMALE } Gender;

typedef struct {
    Gender gender;
    char type[MAX_NAME_LEN];
    char desc[100];
} MakeupType;

typedef struct {
    int id;
    int arrivalTime;
    MakeupType *makeup;
    int duration; // 3~6 min
} Trainee;

// Circular queue for trainees
typedef struct {
    Trainee queue[QUEUE_SIZE];
    int front, rear, count;
} MakeupRoom;

void initRoom(MakeupRoom *room) {
    room->front = 0;
    room->rear = -1;
    room->count = 0;
}

int isFull(MakeupRoom *room) {
    return room->count == QUEUE_SIZE;
}

int isEmpty(MakeupRoom *room) {
    return room->count == 0;
}

int enqueue(MakeupRoom *room, Trainee t) {
    if (isFull(room)) return 0;
    room->rear = (room->rear + 1) % QUEUE_SIZE;
    room->queue[room->rear] = t;
    room->count++;
    return 1;
}

Trainee* peek(MakeupRoom *room) {
    if (isEmpty(room)) return NULL;
    return &room->queue[room->front];
}

int dequeueIfDone(MakeupRoom *room, int currentTime) {
    if (isEmpty(room)) return 0;
    Trainee *t = &room->queue[room->front];
    if (t->arrivalTime + t->duration <= currentTime) {
        room->front = (room->front + 1) % QUEUE_SIZE;
        room->count--;
        return 1;
    }
    return 0;
}

int loadMakeupTypes(const char *filename, MakeupType **types) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("File open error");
        return 0;
    }
    MakeupType *arr = malloc(MAX_TYPES * sizeof(MakeupType));
    if (!arr) { fclose(f); return 0; }

    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), f) && i < MAX_TYPES) {
        char g[10], t[MAX_NAME_LEN], d[100];
        if (sscanf(line, "%[^:]:%[^:]:%[^\n]", g, t, d) != 3) continue;
        arr[i].gender = (strcmp(g, "Male") == 0) ? MALE : FEMALE;
        strncpy(arr[i].type, t, MAX_NAME_LEN-1);
        strncpy(arr[i].desc, d, 99);
        i++;
    }
    fclose(f);
    *types = arr;
    return i;
}

void printStatus(MakeupRoom *room, int currentTime, int totalWaitMale, int totalWaitFemale, int totalWait) {
    printf("\nTime: %d min\n", currentTime);
    if (isEmpty(room)) {
        printf("Makeup room is empty.\n");
    } else {
        Trainee *t = peek(room);
        int wait = currentTime - t->arrivalTime;
        printf("Trainee ID: %d, Makeup: %s, Duration: %d, Wait: %d min\n",
            t->id, t->makeup->type, t->duration, wait);
    }
    printf("Total wait time: %d min (Male: %d, Female: %d)\n", totalWait, totalWaitMale, totalWaitFemale);
}

void testMakeUp() {
    MakeupType *types = NULL;
    int typeCount = loadMakeupTypes("makeup.txt", &types);
    if (typeCount == 0) {
        printf("No makeup types loaded.\n");
        return;
    }

    MakeupRoom room;
    initRoom(&room);

    int currentTime = 0, traineeId = 1;
    int totalWaitMale = 0, totalWaitFemale = 0, totalWait = 0;

    srand((unsigned)time(NULL));

    while (currentTime <= 60) {
        // Remove finished trainee
        while (dequeueIfDone(&room, currentTime));

        // Random chance a new trainee arrives
        int r = rand() % 11;
        if (r < 3 && !isFull(&room)) {
            Trainee t;
            t.id = traineeId++;
            t.arrivalTime = currentTime;
            Gender g = (t.id % 2 == 1) ? MALE : FEMALE;

            // Select random makeup type for gender
            MakeupType *chosen = NULL;
            while (!chosen) {
                int idx = rand() % typeCount;
                if (types[idx].gender == g) chosen = &types[idx];
            }
            t.makeup = chosen;
            t.duration = (rand() % 4) + 3; // 3~6 mins
            enqueue(&room, t);
        }

        // Calculate wait times for trainees in queue
        for (int i = 0; i < room.count; i++) {
            int idx = (room.front + i) % QUEUE_SIZE;
            int wait = currentTime - room.queue[idx].arrivalTime;
            totalWait += wait;
            if (room.queue[idx].id % 2 == 1) totalWaitMale += wait;
            else totalWaitFemale += wait;
        }

        printStatus(&room, currentTime, totalWaitMale, totalWaitFemale, totalWait);

        currentTime++;
        // simulate 1 min per loop
    }

    free(types);
}
