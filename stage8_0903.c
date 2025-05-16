#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 64
#define HASH_SIZE 101

typedef struct Member {
    char name[MAX_NAME_LEN];
    int age;
    char gender; // M/F/O
    struct Member *next;
} Member;

Member *hashMap[HASH_SIZE];

int member_count = 0;

unsigned int hash(const char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % HASH_SIZE;
}

void initHashMap(void) {
    for (int i = 0; i < HASH_SIZE; i++)
        hashMap[i] = NULL;
    member_count = 0;
}

Member *createMember(const char *name, int age, char gender) {
    Member *m = (Member*)malloc(sizeof(Member));
    if (!m) return NULL;
    strncpy(m->name, name, MAX_NAME_LEN - 1);
    m->name[MAX_NAME_LEN-1] = 0;
    m->age = age;
    m->gender = gender;
    m->next = NULL;
    return m;
}

Member *findMember(const char *name) {
    unsigned int idx = hash(name);
    Member *cur = hashMap[idx];
    while (cur) {
        if (strcmp(cur->name, name) == 0)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

int addMember(const char *name, int age, char gender) {
    if (findMember(name)) return 0; // Already exists
    Member *m = createMember(name, age, gender);
    if (!m) return 0;
    unsigned int idx = hash(name);
    m->next = hashMap[idx];
    hashMap[idx] = m;
    member_count++;
    return 1;
}

int deleteMember(const char *name) {
    unsigned int idx = hash(name);
    Member *cur = hashMap[idx];
    Member *prev = NULL;
    while (cur) {
        if (strcmp(cur->name, name) == 0) {
            if (prev) prev->next = cur->next;
            else hashMap[idx] = cur->next;
            free(cur);
            member_count--;
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

int modifyMember(const char *name, int new_age, char new_gender) {
    Member *m = findMember(name);
    if (!m) return 0;
    m->age = new_age;
    m->gender = new_gender;
    return 1;
}

void printMember(const Member *m) {
    if (!m) return;
    printf("Name: %s, Age: %d, Gender: %c\n", m->name, m->age, m->gender);
}

void printAllMembers(void) {
    if (member_count == 0) {
        printf("No members found.\n");
        return;
    }
    for (int i = 0; i < HASH_SIZE; i++) {
        Member *cur = hashMap[i];
        while (cur) {
            printMember(cur);
            cur = cur->next;
        }
    }
}

int loadMembers(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    initHashMap();
    char name[MAX_NAME_LEN];
    int age;
    char gender;
    while (fscanf(fp, "%63s %d %c", name, &age, &gender) == 3) {
        addMember(name, age, gender);
    }
    fclose(fp);
    return 1;
}

int saveMembers(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        Member *cur = hashMap[i];
        while (cur) {
            fprintf(fp, "%s %d %c\n", cur->name, cur->age, cur->gender);
            cur = cur->next;
        }
    }
    fclose(fp);
    return 1;
}

void adminFanClub(void) {
    int choice;
    char name[MAX_NAME_LEN];
    int age;
    char gender;
    char filename[128];
    printf("Enter filename to load/save members: ");
    if (!fgets(filename, sizeof(filename), stdin)) return;
    filename[strcspn(filename, "\n")] = 0;

    if (!loadMembers(filename)) {
        printf("Failed to load members, starting fresh.\n");
        initHashMap();
    } else {
        printf("Members loaded.\n");
    }

    while (1) {
        printf("\nFan Club Admin Menu:\n");
        printf("1. Add Member\n2. Delete Member\n3. Modify Member\n4. Search Member\n5. Print All Members\n6. Save and Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) break;
        while(getchar() != '\n');

        switch (choice) {
            case 1:
                printf("Name: "); fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                printf("Age: "); scanf("%d", &age); while(getchar()!='\n');
                printf("Gender (M/F/O): "); scanf("%c", &gender); while(getchar()!='\n');
                if (addMember(name, age, gender))
                    printf("Member added.\n");
                else
                    printf("Member already exists or error.\n");
                break;

            case 2:
                printf("Name to delete: "); fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                if (deleteMember(name))
                    printf("Member deleted.\n");
                else
                    printf("Member not found.\n");
                break;

            case 3:
                printf("Name to modify: "); fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                printf("New Age: "); scanf("%d", &age); while(getchar()!='\n');
                printf("New Gender (M/F/O): "); scanf("%c", &gender); while(getchar()!='\n');
                if (modifyMember(name, age, gender))
                    printf("Member modified.\n");
                else
                    printf("Member not found.\n");
                break;

            case 4:
                printf("Name to search: "); fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                {
                    Member *m = findMember(name);
                    if (m) printMember(m);
                    else printf("Member not found.\n");
                }
                break;

            case 5:
                printAllMembers();
                break;

            case 6:
                if (saveMembers(filename))
                    printf("Members saved. Exiting.\n");
                else
                    printf("Failed to save members.\n");
                return;

            default:
                printf("Invalid choice.\n");
                break;
        }
    }
}

int main(void) {
    adminFanClub();
    return 0;
}
