#include <stdio.h>
#include <stdlib.h>

typedef struct Point {
    int x, y;
} Point;

typedef struct Light {
    int id;
    Point p1, p2; // diagonal corners of rectangular area
} Light;

#define MAX_LIGHTS 20
Light lights[MAX_LIGHTS];
int lightCount = 5;

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

// Check if point inside rectangle formed by p1 and p2
int inside(Point p, Light l) {
    int xmin = min(l.p1.x, l.p2.x);
    int xmax = max(l.p1.x, l.p2.x);
    int ymin = min(l.p1.y, l.p2.y);
    int ymax = max(l.p1.y, l.p2.y);
    return (p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax);
}

void printRange() {
    int xmin=lights[0].p1.x, xmax=lights[0].p1.x;
    int ymin=lights[0].p1.y, ymax=lights[0].p1.y;
    for(int i=0; i<lightCount; i++) {
        xmin = min(xmin, min(lights[i].p1.x, lights[i].p2.x));
        xmax = max(xmax, max(lights[i].p1.x, lights[i].p2.x));
        ymin = min(ymin, min(lights[i].p1.y, lights[i].p2.y));
        ymax = max(ymax, max(lights[i].p1.y, lights[i].p2.y));
    }
    printf("Position input range: (%d,%d) ~ (%d,%d)\n", xmin, ymin, xmax, ymax);
}

void handleLighting() {
    printRange();
    printf("Enter your position (x,y): ");
    Point pos;
    if(scanf("%d,%d", &pos.x, &pos.y) != 2) {
        printf("Invalid input.\n");
        return;
    }
    printf("Lights affecting your position:\n");
    int found = 0;
    for(int i=0; i<lightCount; i++) {
        if(inside(pos, lights[i])) {
            found = 1;
            printf("Light %d: (%d,%d) - (%d,%d)\n", lights[i].id,
                   lights[i].p1.x, lights[i].p1.y, lights[i].p2.x, lights[i].p2.y);
        }
    }
    if(!found) printf("None\n");

    // Bonus: add new lights
    printf("Do you want to add new lights? (y/n): ");
    char c; getchar(); c=getchar();
    while(c == 'y' || c == 'Y') {
        if(lightCount >= MAX_LIGHTS) {
            printf("Max lights reached.\n");
            break;
        }
        Light nl;
        nl.id = lightCount + 1;
        printf("Enter new light diagonal points x1,y1,x2,y2: ");
        if(scanf("%d,%d,%d,%d", &nl.p1.x, &nl.p1.y, &nl.p2.x, &nl.p2.y) != 4) {
            printf("Invalid input.\n");
            break;
        }
        lights[lightCount++] = nl;
        printf("Added Light %d.\nAdd another? (y/n): ", nl.id);
        getchar(); c=getchar();
    }
}

int main() {
    lights[0] = (Light){1, {1,8}, {4,5}};
    lights[1] = (Light){2, {3,2}, {13,6}};
    lights[2] = (Light){3, {7,10}, {12,7}};
    lights[3] = (Light){4, {8,4}, {5,8}};
    lights[4] = (Light){5, {9,1}, {2,5}};

    handleLighting();
    return 0;
}
