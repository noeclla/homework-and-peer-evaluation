#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OFFICIAL 5
#define MAX_SONGS 10
#define MAX_STR 100

typedef struct Schedule {
    char country[MAX_STR];
    char region[MAX_STR];
    char date[11];    // YYYY-MM-DD + '\0'
    char time[6];     // HH:MM + '\0'
    char performanceName[MAX_STR];
    int songCount;
    char songs[MAX_SONGS][MAX_STR];
    struct Schedule *next;
} Schedule;

// Official stages list
const char *overseasStages[MAX_OFFICIAL] = {"Billboard", "Grammy", "GMA", "MTV VMAs", "AMA"};
const char *domesticStages[MAX_OFFICIAL] = {"Music Bank", "Inkigayo", "M Countdown", "Show! Champion", "Simply K-Pop"};

static Schedule *head = NULL;

// Compare dates lex order YYYY-MM-DD (string compare works)
static int dateCmp(const char *d1, const char *d2) {
    return strcmp(d1, d2);
}

// Insert schedule sorted by date and time
static void insertSchedule(Schedule *newSch) {
    if (!head || dateCmp(newSch->date, head->date) < 0 ||
        (dateCmp(newSch->date, head->date) == 0 && strcmp(newSch->time, head->time) < 0)) {
        newSch->next = head;
        head = newSch;
        return;
    }
    Schedule *curr = head;
    while (curr->next &&
          (dateCmp(newSch->date, curr->next->date) > 0 ||
          (dateCmp(newSch->date, curr->next->date) == 0 && strcmp(newSch->time, curr->next->time) >= 0))) {
        curr = curr->next;
    }
    newSch->next = curr->next;
    curr->next = newSch;
}

// Print a single schedule with songs
static void printSchedule(const Schedule *sch) {
    printf("Date: %s Time: %s\n", sch->date, sch->time);
    printf("Country: %s Region: %s\n", sch->country, sch->region);
    printf("Performance: %s\n", sch->performanceName);
    printf("Songs:\n");
    for (int i = 0; i < sch->songCount; i++) {
        printf("  - %s\n", sch->songs[i]);
    }
    printf("-------------------------\n");
}

// Add a schedule interactively
static void addSchedule(void) {
    Schedule *newSch = malloc(sizeof(Schedule));
    if (!newSch) {
        printf("Memory allocation failed.\n");
        return;
    }
    newSch->next = NULL;

    printf("Enter country: ");
    scanf(" %99[^\n]", newSch->country);

    printf("Enter region: ");
    scanf(" %99[^\n]", newSch->region);

    printf("Enter date (YYYY-MM-DD): ");
    scanf(" %10s", newSch->date);

    printf("Enter time (HH:MM): ");
    scanf(" %5s", newSch->time);

    int cat;
    printf("Select performance category:\n");
    printf("1. Self Concert\n2. Overseas Official Stage\n3. Domestic Official Stage\nChoice: ");
    scanf("%d", &cat);
    getchar(); // consume newline

    if (cat == 1) {
        printf("Enter performance name: ");
        fgets(newSch->performanceName, MAX_STR, stdin);
        size_t len = strlen(newSch->performanceName);
        if (len > 0 && newSch->performanceName[len-1] == '\n')
            newSch->performanceName[len-1] = '\0';
    } else if (cat == 2) {
        printf("Choose Overseas Official Stage:\n");
        for (int i = 0; i < MAX_OFFICIAL; i++) {
            printf("%d. %s\n", i+1, overseasStages[i]);
        }
        int choice;
        do {
            printf("Choice (1-%d): ", MAX_OFFICIAL);
            scanf("%d", &choice);
            getchar();
        } while(choice < 1 || choice > MAX_OFFICIAL);
        strcpy(newSch->performanceName, overseasStages[choice-1]);
    } else if (cat == 3) {
        printf("Choose Domestic Official Stage:\n");
        for (int i = 0; i < MAX_OFFICIAL; i++) {
            printf("%d. %s\n", i+1, domesticStages[i]);
        }
        int choice;
        do {
            printf("Choice (1-%d): ", MAX_OFFICIAL);
            scanf("%d", &choice);
            getchar();
        } while(choice < 1 || choice > MAX_OFFICIAL);
        strcpy(newSch->performanceName, domesticStages[choice-1]);
    } else {
        printf("Invalid category. Aborting add.\n");
        free(newSch);
        return;
    }

    printf("Enter number of songs (max %d): ", MAX_SONGS);
    scanf("%d", &newSch->songCount);
    if (newSch->songCount < 0) newSch->songCount = 0;
    if (newSch->songCount > MAX_SONGS) newSch->songCount = MAX_SONGS;
    getchar(); // consume newline

    for (int i = 0; i < newSch->songCount; i++) {
        printf("Enter song %d: ", i+1);
        fgets(newSch->songs[i], MAX_STR, stdin);
        size_t len = strlen(newSch->songs[i]);
        if (len > 0 && newSch->songs[i][len-1] == '\n') {
            newSch->songs[i][len-1] = '\0';
        }
    }

    insertSchedule(newSch);
    printf("Schedule added.\n");
}

// Search schedules by date or performance name
static void searchSchedule(void) {
    char input[MAX_STR];
    printf("Search by date (YYYY-MM-DD) or performance name: ");
    scanf(" %99[^\n]", input);

    int found = 0;
    Schedule *curr = head;
    while (curr) {
        if (strcmp(curr->date, input) == 0 || strstr(curr->performanceName, input) != NULL) {
            printSchedule(curr);
            found = 1;
        }
        curr = curr->next;
    }
    if (!found) {
        printf("No schedules found.\n");
    }
}

// Print schedules within a year and month range
static void printSchedulesByMonth(void) {
    int year, startMonth, endMonth;
    printf("Enter year (YYYY): ");
    scanf("%d", &year);
    printf("Enter start month (1-12): ");
    scanf("%d", &startMonth);
    printf("Enter end month (1-12): ");
    scanf("%d", &endMonth);
    if (startMonth < 1) startMonth = 1;
    if (endMonth > 12) endMonth = 12;
    if (startMonth > endMonth) {
        printf("Invalid month range.\n");
        return;
    }

    Schedule *curr = head;
    int printedMonth = 0;
    int printedAny = 0;
    while (curr) {
        int y, m;
        if (sscanf(curr->date, "%4d-%2d", &y, &m) == 2) {
            if (y == year && m >= startMonth && m <= endMonth) {
                if (m != printedMonth) {
                    printf("== Month %02d ==\n", m);
                    printedMonth = m;
                }
                printSchedule(curr);
                printedAny = 1;
            }
        }
        curr = curr->next;
    }
    if (!printedAny) {
        printf("No schedules found in the specified range.\n");
    }
}

// Save schedules to file
static void saveSchedulesToFile(void) {
    FILE *fp = fopen("schedules.txt", "w");
    if (!fp) {
        printf("Failed to open file for writing.\n");
        return;
    }
    Schedule *curr = head;
    while (curr) {
        fprintf(fp, "%s|%s|%s|%s|%s|%d\n", curr->country, curr->region,
                curr->date, curr->time, curr->performanceName, curr->songCount);
        for (int i = 0; i < curr->songCount; i++) {
            fprintf(fp, "%s\n", curr->songs[i]);
        }
        curr = curr->next;
    }
    fclose(fp);
    printf("Schedules saved to schedules.txt\n");
}

// Free all allocated schedules
static void freeSchedules(void) {
    Schedule *curr = head;
    while (curr) {
        Schedule *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    head = NULL;
}

void planSchedule(void) {
    int choice;
    while (1) {
        printf("\n=== Schedule Management Menu ===\n");
        printf("1. Add Schedule\n2. Search Schedule\n3. Print Schedules by Month\n4. Save & Exit\n");
        printf("Select option: ");
        scanf("%d", &choice);
        getchar();
