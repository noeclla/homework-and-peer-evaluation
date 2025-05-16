#include <stdio.h>
#include <math.h>

#define MAX_EXAMS 9
#define ROOM_COUNT 10

char categories[MAX_EXAMS][20] = {
    "Physical Strength", "Knowledge", "Self-Management", "Teamwork",
    "Korean Language", "Vocal", "Dance", "Visual", "Acting"
};

int scheduled_dates[MAX_EXAMS] = {0}; // YYYYMMDD for each category
int completed[MAX_EXAMS] = {0};       // Mark passed certifications

int get_day(int yyyymmdd) {
    int y = yyyymmdd / 10000;
    int m = (yyyymmdd / 100) % 100;
    int d = yyyymmdd % 100;
    return y * 365 + m * 30 + d; // rough date delta
}

int days_between(int d1, int d2) {
    return get_day(d2) - get_day(d1);
}

void print_categories() {
    for (int i = 0; i < MAX_EXAMS; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }
}

int can_schedule(int id, int start, int today) {
    int required = days_between(start, today);
    if (required < 30 || required > 100) return 0;

    if (id >= 2 && (!completed[0] || !completed[1])) return 0;
    if (id >= 4 && (id < 2 || id < 4) && (!completed[2] || !completed[3])) return 0;

    return 1;
}

void check_reminders(int today) {
    for (int i = 0; i < MAX_EXAMS; i++) {
        if (scheduled_dates[i] && days_between(today, scheduled_dates[i]) <= 10)
            printf("Reminder: %s exam in %d days.\n", categories[i], days_between(today, scheduled_dates[i]));
    }
}

void check_exam_room(int today) {
    for (int i = 0; i < MAX_EXAMS; i++) {
        if (scheduled_dates[i] == today) {
            int room = today % ROOM_COUNT + 1; // simple pseudo-random
            printf("Today: %s exam in Room %d.\n", categories[i], room);
        }
    }
}

int main() {
    int start_date, today, exam_id, exam_date;

    printf("Training Start Date (YYYYMMDD): ");
    scanf("%d", &start_date);

    printf("Today (YYYYMMDD): ");
    scanf("%d", &today);

    check_reminders(today);
    check_exam_room(today);

    while (1) {
        printf("\nSchedule Exam:\n");
        print_categories();
        printf("Select (0 to quit): ");
        scanf("%d", &exam_id);
        if (exam_id == 0) break;

        if (exam_id < 1 || exam_id > MAX_EXAMS) continue;

        printf("Enter Exam Date (YYYYMMDD): ");
        scanf("%d", &exam_date);

        if (can_schedule(exam_id - 1, start_date, exam_date)) {
            scheduled_dates[exam_id - 1] = exam_date;
            completed[exam_id - 1] = 1;
            printf("Scheduled: %s on %d\n", categories[exam_id - 1], exam_date);
        } else {
            printf("Invalid schedule. Requirements not met.\n");
        }
    }

    return 0;
}
