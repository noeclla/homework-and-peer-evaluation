#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHAR 8
#define PICK_CHAR 4
#define MAX_NAME 50

typedef struct Character {
    char name[MAX_NAME];
    int isNegative; 
} Character;

typedef struct Node {
    Character character;
    int ratio;
    struct Node *prev, *next;
} Node;

Node* createNode(Character c, int ratio) {
    Node *n = malloc(sizeof(Node));
    n->character = c;
    n->ratio = ratio;
    n->prev = n->next = NULL;
    return n;
}

void insertSorted(Node **head, Node *newNode) {
    if (!*head) {
        *head = newNode;
        return;
    }
    Node *cur = *head;
    while (cur && cur->ratio > newNode->ratio) cur = cur->next;
    if (!cur) { // insert at end
        Node *tail = *head;
        while (tail->next) tail = tail->next;
        tail->next = newNode;
        newNode->prev = tail;
    } else if (!cur->prev) { // insert at head
        newNode->next = cur;
        cur->prev = newNode;
        *head = newNode;
    } else { // insert middle
        newNode->next = cur;
        newNode->prev = cur->prev;
        cur->prev->next = newNode;
        cur->prev = newNode;
    }
}

void printList(Node *head) {
    printf("Character Analysis:\n");
    for (Node *cur = head; cur; cur = cur->next) {
        printf(" - %s: %d%% %s\n", cur->character.name, cur->ratio,
               cur->character.isNegative ? "(Negative)" : "");
    }
    printf("\n");
}

Node* removeNegative(Node **head) {
    Node *cur = *head;
    Node *removedHead = NULL;
    while (cur) {
        Node *next = cur->next;
        if (cur->character.isNegative) {
            // Remove from list
            if (cur->prev) cur->prev->next = cur->next;
            else *head = cur->next;
            if (cur->next) cur->next->prev = cur->prev;
            // Insert into removed list
            cur->prev = cur->next = NULL;
            cur->next = removedHead;
            if (removedHead) removedHead->prev = cur;
            removedHead = cur;
        }
        cur = next;
    }
    return removedHead;
}

int containsCharacter(Node *head, Character c) {
    for (Node *cur = head; cur; cur = cur->next)
        if (strcmp(cur->character.name, c.name) == 0) return 1;
    return 0;
}

void replaceNegatives(Node **head, Node *removed, Character chars[], int n) {
    Node *curRemoved = removed;
    while (curRemoved) {
        Character *replacement = NULL;
        for (int i = 0; i < n; i++) {
            if (!chars[i].isNegative && !containsCharacter(*head, chars[i])) {
                replacement = &chars[i];
                break;
            }
        }
        if (!replacement) break;
        strcpy(curRemoved->character.name, replacement->name);
        curRemoved->character.isNegative = 0;
        // Insert back sorted
        Node *nextRemoved = curRemoved->next;
        curRemoved->prev = curRemoved->next = NULL;
        insertSorted(head, curRemoved);
        curRemoved = nextRemoved;
    }
    while (curRemoved) {
        Node *tmp = curRemoved;
        curRemoved = curRemoved->next;
        free(tmp);
    }
}

Node* buildRandomAnalysis(Character chars[], int n) {
    srand((unsigned)time(NULL));
    Node *head = NULL;

    int picked[PICK_CHAR] = {-1, -1, -1, -1};
    int negIndex = -1;

    // Pick one negative first
    for (int i = 0; i < n; i++) {
        if (chars[i].isNegative) { negIndex = i; break; }
    }
    picked[0] = negIndex;

    // Pick 3 other unique chars (non-negative)
    int count = 1;
    while (count < PICK_CHAR) {
        int r = rand() % n;
        if (picked[0] == r) continue;
        int duplicate = 0;
        for (int j = 0; j < count; j++)
            if (picked[j] == r) { duplicate = 1; break; }
        if (duplicate) continue;
        picked[count++] = r;
    }

    // Generate random ratios summing 100
    int ratios[PICK_CHAR];
    int sum = 100;
    for (int i = 0; i < PICK_CHAR - 1; i++) {
        ratios[i] = rand() % (sum + 1);
        sum -= ratios[i];
    }
    ratios[PICK_CHAR-1] = sum;

    // Create nodes & insert sorted by ratio
    for (int i = 0; i < PICK_CHAR; i++) {
        Node *nNode = createNode(chars[picked[i]], ratios[i]);
        insertSorted(&head, nNode);
    }
    return head;
}

int loadCharacters(const char *file, Character arr[], int max) {
    FILE *f = fopen(file, "r");
    if (!f) {
        printf("File open error\n");
        return 0;
    }
    int i = 0;
    char line[128];
    while (i < max && fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        strcpy(arr[i].name, line);
        // next line is 0 or 1 (negative flag)
        if (!fgets(line, sizeof(line), f)) break;
        arr[i].isNegative = atoi(line);
        i++;
    }
    fclose(f);
    return i;
}

void studyMyCharacter() {
    Character chars[MAX_CHAR];
    int count = loadCharacters("character.txt", chars, MAX_CHAR);
    if (count < PICK_CHAR) {
        printf("Not enough characters\n");
        return;
    }

    Node *analysis = buildRandomAnalysis(chars, count);

    printf("Before removing negative characters:\n");
    printList(analysis);

    Node *removed = removeNegative(&analysis);

    printf("After removing negative characters:\n");
    printList(analysis);

    printf("Replacing removed negatives with other characters...\n");
    replaceNegatives(&analysis, removed, chars, count);

    printList(analysis);

    // free all
    Node *cur = analysis;
    while(cur) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}
