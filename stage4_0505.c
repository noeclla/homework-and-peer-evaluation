#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 30
#define TUTOR_COUNT 10
#define MEMBER_COUNT 7

typedef struct {
    char nickname[MAX_NAME];
    char skill[100];
} Tutor;

typedef struct {
    char name[MAX_NAME];
    Tutor *assignedTutor;
} Member;

Tutor tutors[TUTOR_COUNT] = {
    {"Aria", "Wide vocal range, powerful voice, versatile genres"},
    {"Retro", "Masters old & modern styles"},
    {"Magic", "Magical emotion through music"},
    {"Symphony", "Orchestra-like harmony"},
    {"Siren", "Mysterious, emotional journey"},
    {"Rune", "Creative imagination through music"},
    {"Future", "Futuristic & innovative guidance"},
    {"Massive", "Strong voice encouraging students"},
    {"Echo", "Reflects emotions and messages"},
    {"Nova", "Dynamic vocal techniques"}
};

Member members[MEMBER_COUNT] = {
    {"Ariel", NULL},
    {"Milly", NULL},
    {"Woncheol", NULL},
    {"Yejin", NULL},
    {"Jisoo", NULL},
    {"Haneul", NULL},
    {"Soojin", NULL}
};

void assignRandomTutor(Member *m, Tutor **tutorPool, int poolSize) {
    if (poolSize == 0) {
        m->assignedTutor = NULL;
        return;
    }
    int r = rand() % poolSize;
    m->assignedTutor = tutorPool[r];
}

void printMembers(void) {
    printf("Members and assigned tutors:\n");
    for (int i=0; i<MEMBER_COUNT; i++) {
        printf("%s : %s\n", members[i].name, 
               members[i].assignedTutor ? members[i].assignedTutor->nickname : "None");
    }
}

void removeTutorRandomly(Tutor **tutorPool, int *poolSize) {
    if (*poolSize == 0) return;
    int r = rand() % (*poolSize);
    printf("Tutor %s left.\n", tutorPool[r]->nickname);
    for (int i = r; i < *poolSize - 1; i++) {
        tutorPool[i] = tutorPool[i+1];
    }
    (*poolSize)--;
}

int rps(int a, int b) {
    if (a == b) return 0;       // draw
    if ((a==1 && b==3) || (a==2 && b==1) || (a==3 && b==2)) return 1;  // a wins
    return -1;                  // b wins
}

void adminVocalTuition(void) {
    srand((unsigned)time(NULL));
    Tutor *tutorPool[TUTOR_COUNT];
    int tutorPoolSize = TUTOR_COUNT;

    // init tutor pool pointers
    for (int i=0; i<TUTOR_COUNT; i++)
        tutorPool[i] = &tutors[i];

    // assign tutors randomly to members (unique)
    for (int i=0; i<MEMBER_COUNT; i++) {
        int r = rand() % tutorPoolSize;
        members[i].assignedTutor = tutorPool[r];
        // remove from pool
        for (int j=r; j<tutorPoolSize-1; j++) tutorPool[j] = tutorPool[j+1];
        tutorPoolSize--;
    }

    printMembers();

    // one tutor leaves randomly
    // Find tutor in pool and remove
    // Rebuild tutor pool from assigned tutors + unassigned tutors
    Tutor *assignedTutors[MEMBER_COUNT];
    int assignedCount = 0;
    for (int i=0; i<MEMBER_COUNT; i++)
        if (members[i].assignedTutor) assignedTutors[assignedCount++] = members[i].assignedTutor;

    removeTutorRandomly(assignedTutors, &assignedCount);

    // Reset tutor assignments for members with lost tutor
    for (int i=0; i<MEMBER_COUNT; i++) {
        int found = 0;
        for (int j=0; j<assignedCount; j++) {
            if (members[i].assignedTutor == assignedTutors[j]) {
                found = 1;
                break;
            }
        }
        if (!found) members[i].assignedTutor = NULL;
    }

    // Rebuild tutorPool with assigned tutors (no duplicates)
    tutorPoolSize = assignedCount;
    for (int i=0; i<assignedCount; i++)
        tutorPool[i] = assignedTutors[i];

    // Add missing tutors back to pool from original list
    for (int i=0; i<TUTOR_COUNT; i++) {
        int used = 0;
        for (int j=0; j<tutorPoolSize; j++)
            if (&tutors[i] == tutorPool[j]) used = 1;
        if (!used) tutorPool[tutorPoolSize++] = &tutors[i];
    }

    // Assign tutors to members missing one
    for (int i=0; i<MEMBER_COUNT; i++) {
        if (members[i].assignedTutor == NULL) {
            assignRandomTutor(&members[i], tutorPool, tutorPoolSize);
            // Remove assigned tutor from pool
            for (int j=0; j<tutorPoolSize; j++) {
                if (tutorPool[j] == members[i].assignedTutor) {
                    for (int k=j; k<tutorPoolSize-1; k++)
                        tutorPool[k] = tutorPool[k+1];
                    tutorPoolSize--;
                    break;
                }
            }
        }
    }

    printMembers();

    // Bonus
    printf("Tutor swap by Rock-Paper-Scissors not implemented.\n");
}
