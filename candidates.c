#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CANDIDATES 6
#define MAX_LEN 256

char candidate01[MAX_LEN], candidate02[MAX_LEN], candidate03[MAX_LEN];
char candidate04[MAX_LEN], candidate05[MAX_LEN], candidate06[MAX_LEN];

char* member_info[MAX_CANDIDATES] = {
    candidate01, candidate02, candidate03,
    candidate04, candidate05, candidate06
};

int calculate_age(int year, int month, int day) {
    time_t now;
    struct tm birthdate = {0};
    int age;

    time(&now);
    struct tm *current = localtime(&now);

    birthdate.tm_year = year - 1900;
    birthdate.tm_mon = month - 1;
    birthdate.tm_mday = day;

    age = current->tm_year - birthdate.tm_year;
    if (current->tm_mon < birthdate.tm_mon ||
        (current->tm_mon == birthdate.tm_mon && current->tm_mday < birthdate.tm_mday)) {
        age--;
    }

    return age;
}

int main() {
    char group_name[100];
    printf("Enter Audition Group Name: ");
    
    int idx = 0;
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF && idx < sizeof(group_name) - 1) {
        group_name[idx++] = ch;
    }
    group_name[idx] = '\0';

    printf("\n####################################\n");
    printf("     [%s] Audition Candidate Data Entry\n", group_name);
    printf("####################################\n");

    int i = 0;
    while (i < MAX_CANDIDATES) {
        char name[MAX_LEN], dob[11], gender, email[MAX_LEN], nationality[MAX_LEN];
        float bmi;
        char primary[MAX_LEN], secondary[MAX_LEN], mbti[10], intro[MAX_LEN];
        int topik;

        printf("Entering information for candidate #%d.\n", i + 1);
        printf("---------------------------------\n");

        printf("1. Name: ");
        fgets(name, MAX_LEN, stdin);
        name[strcspn(name, "\n")] = '\0';

        printf("2. Date of Birth (YYYY/MM/DD format): ");
        fgets(dob, sizeof(dob), stdin);
        dob[strcspn(dob, "\n")] = '\0';

        printf("3. Gender (F for Female, M for Male): ");
        scanf(" %c", &gender); getchar();

        printf("4. Email: ");
        fgets(email, MAX_LEN, stdin);
        email[strcspn(email, "\n")] = '\0';

        printf("5. Nationality: ");
        fgets(nationality, MAX_LEN, stdin);
        nationality[strcspn(nationality, "\n")] = '\0';

        printf("6. BMI: ");
        scanf("%f", &bmi); getchar();

        printf("7. Primary Skill: ");
        fgets(primary, MAX_LEN, stdin);
        primary[strcspn(primary, "\n")] = '\0';

        printf("8. Secondary Skill: ");
        fgets(secondary, MAX_LEN, stdin);
        secondary[strcspn(secondary, "\n")] = '\0';

        printf("9. Korean Proficiency Level (TOPIK): ");
        scanf("%d", &topik); getchar();

        printf("10. MBTI: ");
        fgets(mbti, sizeof(mbti), stdin);
        mbti[strcspn(mbti, "\n")] = '\0';

        printf("11. Introduction: ");
        fgets(intro, MAX_LEN, stdin);
        intro[strcspn(intro, "\n")] = '\0';

        int year, month, day;
        sscanf(dob, "%d/%d/%d", &year, &month, &day);
        int age = calculate_age(year, month, day);

        char *topikStr = (topik == 0) ? "Native" : (char[2]){topik + '0', '\0'};

        sprintf(member_info[i],
            "%s (%d)| %04d%02d%02d | %c | %s | %s | %.1f | %s | %s | %s | %s\n%s",
            name, age, year, month, day, gender, email, nationality, bmi,
            primary, secondary, topikStr, mbti, intro);

        i++;
        printf("=================================\n");
    }

    printf("\n####################################\n");
    printf("     [%s] Audition Candidate Data Review\n", group_name);
    printf("####################################\n");
    printf("=============================================================================================\n");
    printf("Name (Age)        | DOB       | Gender | Email                 | Nationality | BMI  | Primary Skill | Secondary Skill | TOPIK | MBTI |\n");
    printf("=============================================================================================\n");

    for (i = 0; i < MAX_CANDIDATES; i++) {
        char *line = strtok(member_info[i], "\n");
        char *intro = strtok(NULL, "\n");
        printf("%-90s |\n", line);
        printf("-------------------------------------------------------------------------------------\n");
        printf("%s\n", intro);
        printf("---------------------------------------------------------------------------------------------\n");
    }

    return 0;
}

