#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5
#define MIN_LEN 30
#define MAX_LEN 70

const char *wowText[] = {"Calm", "Exciting", "Meaningful"};

struct Member {
    char name[20];
    char nickname[20];
};

struct Journey {
    char nickname[20];
    char content[MAX_LEN + 1];
    int date; // YYYYMMDD
    int wow;  // 0-10
};

struct Member members[MAX] = {
    {"Arthur", "art"}, {"Go-eun", "goeun"}, {"Min", "min"}, {"YJ", "yj"}, {"So", "so"}
};

struct Journey memories[50];
int memoryCount = 0;
int cheatTime = 0;

int getHour(int dateTime) {
    return dateTime % 100;
}

void printWow(int wow) {
    printf("%s", wowText[wow <= 3 ? 0 : wow <= 6 ? 1 : 2]);
}

void createMemory() {
    char answer;
    char nick[20];
    int found = 0;
    printf("Enter your nickname: ");
    scanf("%s", nick);
    for (int i = 0; i < MAX; i++) {
        if (!strcmp(nick, members[i].nickname)) {
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Invalid nickname.\n");
        return;
    }

    printf("Do you want to record your journey? (y/n): ");
    scanf(" %c", &answer);

    if (answer == 'y' || answer == 'Y') {
        struct Journey j;
        strcpy(j.nickname, nick);
        printf("Enter your journey (30~70 chars): ");
        getchar();
        fgets(j.content, MAX_LEN + 1, stdin);

        int len = 0;
        for (; j.content[len]; len++);
        if (len < MIN_LEN || len > MAX_LEN) {
            printf("Length must be between 30 and 70.\n");
            return;
        }

        printf("Enter date (YYYYMMDD): ");
        scanf("%d", &j.date);
        printf("Enter wow effect (0~10): ");
        scanf("%d", &j.wow);
        memories[memoryCount++] = j;
        printf("Journey saved!\n");
    } else if (answer == 'n' || answer == 'N') {
        printf("1. View memories\n2. Find moment\n3. Back\nChoose: ");
        int choice;
        scanf("%d", &choice);
        if (choice == 1) displayMemory();
        else if (choice == 2) findImpressiveMoment();
        else return;
    } else {
        printf("Invalid input. Returning to menu.\n");
    }
}

void displayMemory() {
    char nick[20];
    printf("Enter nickname to view: ");
    scanf("%s", nick);

    for (int i = 0; i < memoryCount; i++) {
        if (!strcmp(memories[i].nickname, nick)) {
            printf("[%d] %s\n", i + 1, memories[i].content);
            int y = memories[i].date / 10000;
            int m = (memories[i].date / 100) % 100;
            int d = memories[i].date % 100;
            printf("Date: %d-%02d-%02d, Wow: ", y, m, d);
            printWow(memories[i].wow);
            printf("\n");
        }
    }
}

void findImpressiveMoment() {
    char nick[20], wowType[12];
    printf("Enter nickname: ");
    scanf("%s", nick);
    printf("Enter wow type (Calm/Exciting/Meaningful): ");
    scanf("%s", wowType);

    int index = (!strcmp(wowType, "Calm")) ? 0 : (!strcmp(wowType, "Exciting")) ? 1 : 2;

    for (int i = 0; i < memoryCount; i++) {
        int type = memories[i].wow <= 3 ? 0 : memories[i].wow <= 6 ? 1 : 2;
        if (!strcmp(memories[i].nickname, nick) && type == index) {
            int y = memories[i].date / 10000;
            int m = (memories[i].date / 100) % 100;
            int d = memories[i].date % 100;
            printf("%d-%02d-%02d: %s\n", y, m, d, memories[i].content);
        }
    }
}

void buildMemorialPark() {
    time_t t = time(NULL);
    struct tm *now = localtime(&t);
    int hour = now->tm_hour;

    if (cheatTime) hour = cheatTime;

    if (hour < 18 || hour > 22) {
        printf("You can use this between 18:00~22:00 only.\n");
        return;
    }

    printf("[Journey Recording]\nT/t: Test time | Any other key: Continue\n");
    char c;
    scanf(" %c", &c);
    if (c == 'T' || c == 't') {
        printf("Enter hour (0~23): ");
        scanf("%d", &cheatTime);
    }
    createMemory();
}
