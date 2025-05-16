#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MEMBERS 100
#define MAX_GROUPS 10
#define MAX_NAME_LEN 50
#define MAX_RECORDS_PER_DAY 3

typedef struct {
    int bpm;           
    char rhythm[5];   
} TempoRhythm;

typedef struct {
    char memberName[MAX_NAME_LEN];
    char testDate[11];     
    int targetBeats;
    double elapsedTime;    
} BeatTestRecord;

typedef struct {
    char groupName[MAX_NAME_LEN];
    char members[MAX_MEMBERS][MAX_NAME_LEN];
    int memberCount;
} Group;

TempoRhythm tempoRhythms[] = {
    {60, "2/4"},
    {60, "3/4"},
    {60, "4/4"},
    {60, "6/8"},
    {80, "2/4"},
    {80, "3/4"},
    {80, "4/4"},
    {80, "6/8"},
    {120, "2/4"},
    {120, "3/4"},
    {120, "4/4"},
    {120, "6/8"}
};
const int tempoRhythmsCount = sizeof(tempoRhythms) / sizeof(TempoRhythm);

int loadGroups(const char *filename, Group groups[], int *groupCount) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    char line[256];
    *groupCount = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (*groupCount >= MAX_GROUPS) break;
        line[strcspn(line, "\r\n")] = 0;
        char *token = strtok(line, ",");
        if (!token) continue;
        strcpy(groups[*groupCount].groupName, token);

        groups[*groupCount].memberCount = 0;
        while ((token = strtok(NULL, ",")) != NULL) {
            if (groups[*groupCount].memberCount >= MAX_MEMBERS) break;
            strcpy(groups[*groupCount].members[groups[*groupCount].memberCount++], token);
        }
        (*groupCount)++;
    }
    fclose(fp);
    return 1;
}

int inputTrainee(const Group *group, char *memberName) {
    int tries = 3;
    char input[MAX_NAME_LEN];

    while (tries--) {
        printf("Enter member name from group %s: ", group->groupName);
        if (scanf(" %49[^\n]", input) != 1) return 0;

        for (int i = 0; i < group->memberCount; i++) {
            if (strcmp(input, group->members[i]) == 0) {
                strcpy(memberName, input);
                return 1;
            }
        }
        printf("No matching member. Try again.\n");
    }
    return 0; 
}

void getCurrentDate(char *dateStr) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(dateStr, "%02d%02d%02d%02d", 
            (tm->tm_year + 1900) % 100, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour);
}

void writeFile(const BeatTestRecord *record) {
    FILE *fp = fopen("beat_test_result.txt", "a");
    if (!fp) {
        printf("Error opening result file.\n");
        return;
    }
    fprintf(fp, "%s,%s,%d,%.2f\n", record->memberName, record->testDate, record->targetBeats, record->elapsedTime);
    fclose(fp);
}

void printResult(const BeatTestRecord *record) {
    double expectedTime = (60.0 / record->targetBeats) * record->targetBeats; 
    double diff = record->elapsedTime - expectedTime;
    printf("Target beats: %d, Time elapsed: %.2f sec\n", record->targetBeats, record->elapsedTime);
    if (diff > 0) printf("You were %.2f seconds too slow.\n", diff);
    else if (diff < 0) printf("You were %.2f seconds too fast.\n", -diff);
    else printf("Perfect timing!\n");
}

void testBeat(const char *memberName) {
    static int dailyTests = 0;
    if (dailyTests >= MAX_RECORDS_PER_DAY) {
        printf("You can test only 3 times per day.\n");
        return;
    }

    srand((unsigned)time(NULL));
    int idx = rand() % tempoRhythmsCount;
    TempoRhythm tr = tempoRhythms[idx];

    int targetBeats = (tr.bpm * 60) / 60; 

    printf("\nTempo: %d BPM, Rhythm: %s\n", tr.bpm, tr.rhythm);
    printf("Count internally for %d beats.\n", targetBeats);

    printf("Start test? (y/n): ");
    char c; scanf(" %c", &c);
    if (c != 'y' && c != 'Y') {
        printf("Test cancelled.\n");
        return;
    }

    printf("Press ENTER when done counting the beats...\n");
    clock_t start = clock();
    while (getchar() != '\n'); 
    getchar(); 
    clock_t end = clock();

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    BeatTestRecord record;
    strcpy(record.memberName, memberName);
    getCurrentDate(record.testDate);
    record.targetBeats = targetBeats;
    record.elapsedTime = elapsed;

    writeFile(&record);
    printResult(&record);

    dailyTests++;
}

void improveSenseOfBeat() {
    Group groups[MAX_GROUPS];
    int groupCount = 0;

    if (!loadGroups("newtrainees.csv", groups, &groupCount)) {
        printf("Failed to load groups.\n");
        return;
    }

    printf("Enter group name:\n");
    char groupName[MAX_NAME_LEN];
    scanf(" %49[^\n]", groupName);

    int groupIdx = -1;
    for (int i = 0; i < groupCount; i++) {
        if (strcmp(groupName, groups[i].groupName) == 0) {
            groupIdx = i;
            break;
        }
    }
    if (groupIdx == -1) {
        printf("No such group.\n");
        return;
    }

    printf("Members in group %s:\n", groups[groupIdx].groupName);
    for (int i = 0; i < groups[groupIdx].memberCount; i++) {
        printf("%d. %s\n", i + 1, groups[groupIdx].members[i]);
    }

    char memberName[MAX_NAME_LEN];
    if (!inputTrainee(&groups[groupIdx], memberName)) {
        printf("Failed to input valid member name.\n");
        return;
    }

    testBeat(memberName);
}
