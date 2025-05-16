#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AIRPORT_CODE_LEN 3
#define INITIAL_CAPACITY 10

typedef struct {
    char from[AIRPORT_CODE_LEN+1];
    char to[AIRPORT_CODE_LEN+1];
    int used; 
} Ticket;

typedef struct {
    Ticket *arr;
    int size;
    int capacity;
} Tickets;

typedef struct {
    char **arr;
    int size;
    int capacity;
} TravelPath;

void initTravelPath(TravelPath *tp) {
    tp->capacity = INITIAL_CAPACITY;
    tp->size = 0;
    tp->arr = malloc(sizeof(char*) * tp->capacity);
    if (!tp->arr) {
        perror("malloc failed");
        exit(1);
    }
}

void addTravelPath(TravelPath *tp, const char *airport) {
    if (tp->size == tp->capacity) {
        tp->capacity *= 2;
        char **tmp = realloc(tp->arr, sizeof(char*) * tp->capacity);
        if (!tmp) {
            perror("realloc failed");
            exit(1);
        }
        tp->arr = tmp;
    }
    tp->arr[tp->size] = malloc(strlen(airport)+1);
    if (!tp->arr[tp->size]) {
        perror("malloc failed");
        exit(1);
    }
    strcpy(tp->arr[tp->size], airport);
    tp->size++;
}

void freeTravelPath(TravelPath *tp) {
    for (int i=0; i < tp->size; i++) {
        free(tp->arr[i]);
    }
    free(tp->arr);
    tp->arr = NULL;
    tp->size = tp->capacity = 0;
}

void initTickets(Tickets *t) {
    t->capacity = INITIAL_CAPACITY;
    t->size = 0;
    t->arr = malloc(sizeof(Ticket)*t->capacity);
    if (!t->arr) {
        perror("malloc failed");
        exit(1);
    }
}

void addTicket(Tickets *t, const char *from, const char *to) {
    if (t->size == t->capacity) {
        t->capacity *= 2;
        Ticket *tmp = realloc(t->arr, sizeof(Ticket)*t->capacity);
        if (!tmp) {
            perror("realloc failed");
            exit(1);
        }
        t->arr = tmp;
    }
    strncpy(t->arr[t->size].from, from, AIRPORT_CODE_LEN);
    t->arr[t->size].from[AIRPORT_CODE_LEN] = '\0';
    strncpy(t->arr[t->size].to, to, AIRPORT_CODE_LEN);
    t->arr[t->size].to[AIRPORT_CODE_LEN] = '\0';
    t->arr[t->size].used = 0;
    t->size++;
}

void freeTickets(Tickets *t) {
    free(t->arr);
    t->arr = NULL;
    t->size = t->capacity = 0;
}

int cmpTickets(const void *a, const void *b) {
    const Ticket *t1 = (const Ticket*)a;
    const Ticket *t2 = (const Ticket*)b;
    int c = strcmp(t1->from, t2->from);
    if (c != 0) return c;
    return strcmp(t1->to, t2->to);
}

int dfs(Tickets *tickets, TravelPath *path, int usedCount, int total) {
    if (usedCount == total) return 1; 

    char *current = path->arr[path->size-1];

    for (int i = 0; i < tickets->size; i++) {
        if (!tickets->arr[i].used && strcmp(tickets->arr[i].from, current) == 0) {
            tickets->arr[i].used = 1;
            addTravelPath(path, tickets->arr[i].to);

            if (dfs(tickets, path, usedCount+1, total))
                return 1; 

            tickets->arr[i].used = 0;
            free(path->arr[path->size-1]);
            path->size--;
        }
    }
    return 0;
}

int loadTicketsFromFile(const char *filename, Tickets *tickets) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open tickets file");
        return 0;
    }
    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        char from[AIRPORT_CODE_LEN+1], to[AIRPORT_CODE_LEN+1];
        if (sscanf(line, "%3[^,],%3s", from, to) == 2) {
            addTicket(tickets, from, to);
        }
    }
    fclose(fp);
    return 1;
}

void printTravelPath(TravelPath *path) {
    printf("Travel Path:\n");
    for (int i = 0; i < path->size; i++) {
        printf("%s", path->arr[i]);
        if (i != path->size -1) printf(" -> ");
    }
    printf("\n");
}

void planOverseasTraining() {
    Tickets tickets;
    TravelPath path;
    initTickets(&tickets);
    initTravelPath(&path);

    if (!loadTicketsFromFile("tickets.txt", &tickets)) {
        freeTickets(&tickets);
        freeTravelPath(&path);
        return;
    }

    qsort(tickets.arr, tickets.size, sizeof(Ticket), cmpTickets);

    addTravelPath(&path, "ICN");

    if (dfs(&tickets, &path, 0, tickets.size)) {
        printTravelPath(&path);
    } else {
        printf("No valid travel path found.\n");
    }

    freeTickets(&tickets);
    freeTravelPath(&path);
}
