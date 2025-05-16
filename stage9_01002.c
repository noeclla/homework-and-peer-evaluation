#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct City {
    char name[20];
    int x, y;
} City;

typedef struct Route {
    City *path[5];
    double dist;
    struct Route *next;
} Route;

City cities[] = {
    {"Seoul", 12697, 3756},
    {"Gumi", 12834, 3611},
    {"Daejeon", 12738, 3635},
    {"Chuncheon", 12773, 3788},
    {"Busan", 12907, 3518}
};

#define N 3  // number of intermediate cities

int permutations[6][3] = {
    {1,2,3}, {1,3,2}, {2,1,3},
    {2,3,1}, {3,1,2}, {3,2,1}
};

double dist(City *a, City *b) {
    double dx = a->x - b->x, dy = a->y - b->y;
    return sqrt(dx*dx + dy*dy);
}

void addRoute(Route **head, City *p[]) {
    Route *r = malloc(sizeof(Route));
    memcpy(r->path, p, sizeof(City*)*5);
    r->dist = 0;
    for(int i=0; i<4; i++)
        r->dist += dist(r->path[i], r->path[i+1]);
    r->next = *head;
    *head = r;
}

void sortRoutes(Route **head) {
    if (!*head) return;
    Route *i, *j;
    for(i=*head; i; i=i->next) {
        for(j=i->next; j; j=j->next) {
            if(i->dist > j->dist) {
                City *tmpPath[5];
                double tmpDist = i->dist;
                memcpy(tmpPath, i->path, sizeof(City*)*5);
                memcpy(i->path, j->path, sizeof(City*)*5);
                memcpy(j->path, tmpPath, sizeof(City*)*5);
                i->dist = j->dist;
                j->dist = tmpDist;
            }
        }
    }
}

void printRoutes(Route *head) {
    int count = 1;
    for(Route *r = head; r; r = r->next, count++) {
        printf("%d. ", count);
        for(int i=0; i<5; i++) {
            printf("%s", r->path[i]->name);
            if(i<4) printf(" -> ");
        }
        printf("\n   ");
        for(int i=0; i<4; i++) {
            printf("%.1f", dist(r->path[i], r->path[i+1]));
            if(i<3) printf(" + ");
        }
        printf(" : %.1f\n", r->dist);
    }
}

void makeOptimalPath() {
    Route *routes = NULL;
    City *path[5];
    path[0] = &cities[0]; // Seoul
    path[4] = &cities[4]; // Busan

    for(int i=0; i<6; i++) {
        for(int j=0; j<3; j++)
            path[j+1] = &cities[permutations[i][j]];
        addRoute(&routes, path);
    }
    sortRoutes(&routes);
    printRoutes(routes);

    // free list
    while(routes) {
        Route *tmp = routes;
        routes = routes->next;
        free(tmp);
    }
}

int main() {
    makeOptimalPath();
    return 0;
}
