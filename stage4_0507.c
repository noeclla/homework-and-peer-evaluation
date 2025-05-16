#include <stdio.h>

#define ITEMS 10
#define MAX_DAYS 31

typedef struct {
    int id;
    const char *activity;
    const char *period; // "daily" or "weekly"
} ChecklistItem;

int records[ITEMS][MAX_DAYS] = {0};
int year = 2025, month = 5;

ChecklistItem checklist[ITEMS] = {
    {1, "Morning abdominal breathing (5-10 min)", "daily"},
    {2, "Evening oral breathing (5-10 min)", "daily"},
    {3, "Light & soft vocalization (10-15 min)", "daily"},
    {4, "Pitch and tone practice (10 min)", "daily"},
    {5, "Pronunciation & intonation (10 min)", "daily"},
    {6, "Avoid vocal strain activities (any yes is done)", "daily"},
    {7, "Drink enough water (8 cups)", "daily"},
    {8, "Maintain humidity (40%-60%)", "daily"},
    {9, "Rest voice after use (30+ min)", "daily"},
    {10,"Lesson or counseling with vocal tutor (once)", "weekly"}
};

int daysInMonth(int y, int m) {
    int md[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && ((y%4==0 && y%100!=0) || y%400==0)) return 29;
    return md[m-1];
}

int yesNo(const char *msg) {
    char c;
    while (1) {
        printf("%s (y/n): ", msg);
        c = getchar();
        while(getchar()!='\n'); // flush
        if (c=='y' || c=='Y') return 1;
        if (c=='n' || c=='N') return 0;
        printf("Please enter y or n.\n");
    }
}

void inputDay(int day) {
    printf("Input for %d/%d/%d\n", year, month, day+1);
    for (int i=0; i<ITEMS; i++) {
        if (checklist[i].period[0]=='w' && (day+1)%7 != 0) {
            // For weekly items, only input on weekly days
            records[i][day] = records[i][day>0 ? day-1 : 0];
            continue;
        }
        if (i==5) { // Item 6 special: multiple conditions
            const char *conds[] = {
                "Long calls or talks",
                "Singing high or low notes",
                "Noisy environment",
                "Long singing practice",
                "Stressful voice use"
            };
            int done = 0;
            for (int c=0; c<5; c++) {
                if (yesNo(conds[c])) done = 1;
            }
            records[i][day] = done ? 1 : 0;
        } else {
            records[i][day] = yesNo(checklist[i].activity);
        }
    }
}

void printStats(int startDay, int length) {
    printf("Stats from day %d to %d:\n", startDay+1, startDay+length);
    for (int i=0; i<ITEMS; i++) {
        int sum=0, min=-1, max=-1;
        for (int d=0; d<length; d++) {
            int val = records[i][startDay+d];
            sum += val;
            if (val) {
                if (min==-1) min=d;
                max=d;
            }
        }
        double rate = 100.0*sum/length;
        printf("%2d. %-35s: %.1f%% complete", i+1, checklist[i].activity, rate);
        if (min==-1) printf(", no completion");
        else printf(", earliest day %d, latest day %d", min+1+startDay, max+1+startDay);
        printf("\n");
    }
    double total=0;
    for (int d=0; d<length; d++) {
        int daySum=0;
        for (int i=0; i<ITEMS; i++) daySum += records[i][startDay+d];
        total += daySum;
    }
    printf("Average completed items per day: %.2f\n", total/length);
}

int main() {
    int days = daysInMonth(year, month);

    while (1) {
        printf("\n1) Input checklist\n2) Weekly stats\n3) Monthly stats\n4) Exit\nChoose: ");
        int sel;
        if (scanf("%d%*c", &sel) != 1) break;

        if (sel == 1) {
            int d;
            printf("Enter day (1-%d): ", days);
            if (scanf("%d%*c", &d) != 1 || d<1 || d>days) {
                printf("Invalid day.\n");
                continue;
            }
            inputDay(d-1);
        } else if (sel == 2) {
            if (days < 7) {
                printf("Not enough data for weekly stats.\n");
                continue;
            }
            printStats(0, 7);
        } else if (sel == 3) {
            printStats(0, days);
        } else if (sel == 4) break;
        else printf("Invalid choice.\n");
    }
    return 0;
}
