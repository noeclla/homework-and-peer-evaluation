#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_TRAINEES 8
#define MAX_MENTORS 8

const char *trainee_names[MAX_TRAINEES] = {"Hyekyung", "Goeun", "Woncheol", "Youngjin", "Minji", "Sujin", "Doyoung", "Jisoo"};
const char *trainee_nicks[MAX_TRAINEES] = {"HK", "GE", "WC", "YJ", "MJ", "SJ", "DY", "JS"};

typedef struct {
    int id;
    char name[32];
    int mentee_idx; 
} Mentor;

int trainee_abilities[MAX_TRAINEES];
Mentor mentors[MAX_MENTORS];
int mentor_count = 0;

int parseIntMember(const char *nick) {
    int sum = 0;
    for (int i = 0; nick[i]; i++) sum += (int)nick[i];
    return sum;
}

int getRandomAbility() {
    return rand() % 901 + 100; 
}

void initTrainees() {
    for (int i = 0; i < MAX_TRAINEES; i++)
        trainee_abilities[i] = getRandomAbility();
    printf("Trainees initialized.\n");
}

void inputMentors() {
    char name[32];
    printf("Enter up to 8 mentor names (Enter to stop):\n");
    for (int i = 0; i < MAX_MENTORS; i++) {
        printf("Mentor %d name: ", i + 1);
        fgets(name, sizeof(name), stdin);
        if (name[0] == '\n') break;
        name[strcspn(name, "\n")] = 0;
        mentors[mentor_count].id = i + 1;
        strcpy(mentors[mentor_count].name, name);
        mentors[mentor_count].mentee_idx = -1;
        mentor_count++;
    }
}

void matchMentoring() {
    if (mentor_count < MAX_TRAINEES) {
        printf("Need at least 8 mentors for 1:1 matching.\n");
        return;
    }
    for (int i = 0; i < MAX_TRAINEES; i++) {
        int m_id = i % MAX_MENTORS;
        mentors[m_id].mentee_idx = i;
    }
    printf("Mentoring matched successfully.\n");
}

void viewMentoring() {
    printf("\n--- Mentoring Assignments ---\n");
    for (int i = 0; i < mentor_count; i++) {
        if (mentors[i].mentee_idx != -1) {
            int t = mentors[i].mentee_idx;
            printf("Trainee %d - %s (%s), Ability: %d -> Mentor ID %d: %s\n",
                   t + 1, trainee_names[t], trainee_nicks[t],
                   trainee_abilities[t], mentors[i].id, mentors[i].name);
        }
    }
}

void mentoringMenu() {
    char input[4];
    while (1) {
        printf("\n[1] Init Trainees\n[2] Input Mentors\n[3] Match Mentors\n[4] View Mentoring\n[0] Back\n> ");
        fgets(input, sizeof(input), stdin);
        switch (input[0]) {
            case '1': initTrainees(); break;
            case '2': inputMentors(); break;
            case '3': matchMentoring(); break;
            case '4': viewMentoring(); break;
            case '0': return;
            default: printf("Invalid.\n");
        }
    }
}

int main() {
    srand((unsigned)time(NULL));
    char input[4];
    while (1) {
        printf("\n=== Magrathea Training System ===\n");
        printf("[1] Mentoring (Self-Management & Teamwork)\n[0] Exit\n> ");
        fgets(input, sizeof(input), stdin);
        if (input[0] == '0') break;
        else if (input[0] == '1') mentoringMenu();
        else printf("Try again.\n");
    }
    return 0;
}
