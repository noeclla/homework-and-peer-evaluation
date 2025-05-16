#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPR_NAME 50
#define MAX_DESC 200

typedef struct ExprNode {
    char name[MAX_EXPR_NAME];
    char desc[MAX_DESC];
    struct ExprNode *prev, *next;
} ExprNode;

ExprNode *head = NULL;

/* Create new expression node */
ExprNode *createExpr(const char *name, const char *desc) {
    ExprNode *node = malloc(sizeof(ExprNode));
    if (!node) { perror("malloc"); exit(1); }
    strncpy(node->name, name, MAX_EXPR_NAME-1);
    node->name[MAX_EXPR_NAME-1] = '\0';
    strncpy(node->desc, desc, MAX_DESC-1);
    node->desc[MAX_DESC-1] = '\0';
    node->prev = node->next = NULL;
    return node;
}

/* Append node to doubly linked list */
void appendExpr(ExprNode **head, ExprNode *node) {
    if (!*head) {
        *head = node;
        return;
    }
    ExprNode *cur = *head;
    while (cur->next) cur = cur->next;
    cur->next = node;
    node->prev = cur;
}

/* Load expressions from file */
void loadExpressions(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Failed to open %s\n", filename);
        return;
    }
    char line[256], name[MAX_EXPR_NAME], desc[MAX_DESC];
    int lineNum = 0;

    while (fgets(line, sizeof(line), f)) {
        lineNum++;
        if (lineNum % 2 == 1) { // odd lines: name
            strncpy(name, line, MAX_EXPR_NAME-1);
            name[strcspn(name, "\r\n")] = 0;
        } else { // even lines: desc
            strncpy(desc, line, MAX_DESC-1);
            desc[strcspn(desc, "\r\n")] = 0;
            ExprNode *node = createExpr(name, desc);
            appendExpr(&head, node);
        }
    }
    fclose(f);
}

/* Print all expressions */
void printExpressions() {
    ExprNode *cur = head;
    if (!cur) {
        printf("(No expressions)\n");
        return;
    }
    while (cur) {
        printf("Expression: %s\n", cur->name);
        printf("Description: %s\n\n", cur->desc);
        cur = cur->next;
    }
}

/* Search by keyword in description */
void searchByKeyword(const char *keyword) {
    ExprNode *cur = head;
    int found = 0;
    while (cur) {
        if (strstr(cur->desc, keyword)) {
            printf("Found expression:\n%s\n%s\n\n", cur->name, cur->desc);
            found = 1;
        }
        cur = cur->next;
    }
    if (!found) printf("No expression found containing \"%s\".\n", keyword);
}

/* Swap adjacent nodes at position pos (1-based) */
void swapAdjacent(int pos) {
    if (pos < 1) {
        printf("Invalid position.\n");
        return;
    }
    ExprNode *cur = head;
    int i = 1;
    while (cur && i < pos) {
        cur = cur->next;
        i++;
    }
    if (!cur || !cur->next) {
        printf("Position out of range or no adjacent node to swap.\n");
        return;
    }
    ExprNode *A = cur;
    ExprNode *B = cur->next;
    ExprNode *prev = A->prev;
    ExprNode *next = B->next;

    // Swap A and B
    if (prev) prev->next = B; else head = B;
    B->prev = prev;
    B->next = A;
    A->prev = B;
    A->next = next;
    if (next) next->prev = A;

    printf("Swapped expressions at positions %d and %d.\n", pos, pos+1);
}

/* Free the list */
void freeList() {
    ExprNode *cur = head;
    while (cur) {
        ExprNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    head = NULL;
}

/* Menu function */
void haveALook() {
    loadExpressions("look.txt");

    int choice;
    char keyword[100];
    int pos;

    while (1) {
        printf("\n[Expression Menu]\n");
        printf("1. Print all expressions\n");
        printf("2. Search expression by keyword\n");
        printf("3. Swap adjacent expressions\n");
        printf("0. Exit\n");
        printf("Select: ");
        if (scanf("%d", &choice) != 1) break;
        getchar();

        switch (choice) {
            case 1:
                printExpressions();
                break;
            case 2:
                printf("Enter keyword to search in description: ");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\r\n")] = 0;
                searchByKeyword(keyword);
                break;
            case 3:
                printf("Enter position to swap with next: ");
                scanf("%d", &pos);
                getchar();
                swapAdjacent(pos);
                break;
            case 0:
                freeList();
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}
