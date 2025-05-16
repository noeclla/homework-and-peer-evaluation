#include <stdio.h>

#define MAX_MEMBERS 5
#define MAX_SUBMENUS 6
#define MAX_NAME_LEN 50
#define MAX_NICKNAME_LEN 50

enum SubMenuID { A, B, D, E, F };

const char* submenuNames[MAX_SUBMENUS] = {
    "Mentoring",
    "Trauma Management",
    "Self-Reflection",
    "Journey Log",
    "Emotion Diary"
};

const char* memberInfo[MAX_MEMBERS][2] = {
    {"Alice", "Ali"},
    {"Bob", "Bobby"},
    {"Charlie", "Chuck"},
    {"Diana", "Di"},
    {"Ethan", "E"}
};

typedef struct {
    char memberName[MAX_NAME_LEN];
    char nickname[MAX_NICKNAME_LEN];
    int selectedSubMenus[MAX_SUBMENUS];
    int subMenuCount;
} Dashboard;

void showDashboard();
void selectSubMenu(Dashboard* db);
void displayDashboard(const Dashboard* db);
void getDataHub(const Dashboard* db);

int main() {
    showDashboard();
    return 0;
}

void showDashboard() {
    Dashboard db = {0};
    int memberIndex = -1;

    printf("Select a member by nickname:\n");
    for (int i = 0; i < MAX_MEMBERS; i++) {
        printf("%d. %s (%s)\n", i + 1, memberInfo[i][0], memberInfo[i][1]);
    }

    scanf("%d", &memberIndex);
    if (memberIndex < 1 || memberIndex > MAX_MEMBERS) {
        printf("Invalid selection.\n");
        return;
    }

    memberIndex -= 1;
    snprintf(db.memberName, MAX_NAME_LEN, "%s", memberInfo[memberIndex][0]);
    snprintf(db.nickname, MAX_NICKNAME_LEN, "%s", memberInfo[memberIndex][1]);

    int choice = 0;
    printf("1. Edit Dashboard\n2. View Dashboard\n");
    scanf("%d", &choice);

    if (choice == 1) {
        selectSubMenu(&db);
    } else if (choice == 2) {
        if (db.subMenuCount == 0) {
            printf("No submenus selected.\n");
            return;
        }
        displayDashboard(&db);
    } else {
        printf("Invalid choice.\n");
    }
}

void selectSubMenu(Dashboard* db) {
    printf("Select submenus to add (enter numbers separated by spaces, end with 0):\n");
    for (int i = 0; i < MAX_SUBMENUS; i++) {
        printf("%d. %s\n", i + 1, submenuNames[i]);
    }

    int id = 0;
    db->subMenuCount = 0;
    while (1) {
        scanf("%d", &id);
        if (id == 0) break;
        if (id < 1 || id > MAX_SUBMENUS) {
            printf("Invalid submenu ID.\n");
            continue;
        }
        db->selectedSubMenus[db->subMenuCount++] = id - 1;
    }
}

void displayDashboard(const Dashboard* db) {
    printf("Dashboard for %s (%s):\n", db->memberName, db->nickname);
    for (int i = 0; i < db->subMenuCount; i++) {
        int id = db->selectedSubMenus[i];
        printf("Submenu: %s\n", submenuNames[id]);
        getDataHub(db);
    }
}

void getDataHub(const Dashboard* db) {
    printf("Fetching data for selected submenus...\n");
}
