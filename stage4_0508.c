#include <stdio.h>

#define MAX_PARTS 7
#define MAX_LINES 50
#define MAX_MEMBERS 5
#define MAX_LINES_PER_PART 20

const char *PARTS[MAX_PARTS] = {"Intro", "Verse 1", "Pre-Chorus", "Chorus", "Verse 2", "Bridge", "Final Chorus"};

const char *LYRICS[] = {
    "I miss you", "This word is...", "Too much", "Too much", "I miss you", 
    "Time is cruel", "I hate us", "No more seeing faces", "It's hard now", NULL
};

typedef struct {
    int lines[MAX_LINES_PER_PART];
    int count;
} Part;

typedef struct {
    char name[30];
    int partIndex;
    int lines[MAX_LINES_PER_PART];
    int lineCount;
} Member;

void printLyrics() {
    printf("Song Lyrics:\n");
    for (int i=0; LYRICS[i] != NULL; i++) {
        printf("[%d] %s\n", i, LYRICS[i]);
    }
}

void analyzeSong(Part parts[]) {
    for (int p=0; p<MAX_PARTS; p++) {
        printf("Assign lines to %s (-1 to stop): ", PARTS[p]);
        parts[p].count = 0;
        while (1) {
            int line;
            scanf("%d", &line);
            if (line == -1) break;
            parts[p].lines[parts[p].count++] = line;
        }
    }
}

void assignMembers(Member members[], int count) {
    for (int i=0; i<count; i++) {
        printf("Member %s, choose part (0-%d): ", members[i].name, MAX_PARTS-1);
        scanf("%d", &members[i].partIndex);
        members[i].lineCount = 0;
        printf("Enter lines for %s (-1 to stop): ", members[i].name);
        while(1) {
            int line;
            scanf("%d", &line);
            if(line == -1) break;
            members[i].lines[members[i].lineCount++] = line;
        }
    }
}

void printAnalysis(Part parts[]) {
    printf("Song Structure:\n");
    for (int p=0; p<MAX_PARTS; p++) {
        printf("%s:\n", PARTS[p]);
        for (int i=0; i<parts[p].count; i++) {
            printf(" %d: %s\n", parts[p].lines[i], LYRICS[parts[p].lines[i]]);
        }
    }
}

int main() {
    Part parts[MAX_PARTS] = {0};
    Member members[MAX_MEMBERS] = { {"Alice", -1}, {"Bob", -1} };
    int memberCount = 2;

    printLyrics();
    analyzeSong(parts);
    assignMembers(members, memberCount);
    printAnalysis(parts);

    return 0;
}
