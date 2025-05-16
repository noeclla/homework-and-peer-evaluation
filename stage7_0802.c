#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_AUTHOR 50

typedef struct BookList {
    int order;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    struct BookList *next;
} BookList;

BookList *readingHead = NULL; // Reading list head
BookList *readHead = NULL;    // Read books list head

/* Create new book node */
BookList *createBook(int order, const char *title, const char *author) {
    BookList *b = malloc(sizeof(BookList));
    if (!b) { perror("malloc"); exit(1); }
    b->order = order;
    strncpy(b->title, title, MAX_TITLE-1);
    b->title[MAX_TITLE-1] = '\0';
    strncpy(b->author, author, MAX_AUTHOR-1);
    b->author[MAX_AUTHOR-1] = '\0';
    b->next = NULL;
    return b;
}

/* Insert book into linked list by order */
void insertBook(BookList **head, BookList *newBook) {
    if (!*head || (*head)->order > newBook->order) {
        newBook->next = *head;
        *head = newBook;
        return;
    }
    BookList *cur = *head;
    while (cur->next && cur->next->order < newBook->order)
        cur = cur->next;
    newBook->next = cur->next;
    cur->next = newBook;
}

/* Reorder all nodes to have consecutive orders starting at 1 */
void reorderList(BookList *head) {
    int o = 1;
    while (head) {
        head->order = o++;
        head = head->next;
    }
}

/* Load reading list from file */
void loadReadingList(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Failed to open %s\n", filename);
        return;
    }
    char line[200], title[MAX_TITLE], author[MAX_AUTHOR];
    int order;
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d|%[^|]|%[^\n]", &order, title, author) == 3) {
            BookList *b = createBook(order, title, author);
            insertBook(&readingHead, b);
        }
    }
    fclose(f);
    reorderList(readingHead);
}

/* Print book list */
void printList(const char *label, BookList *head) {
    printf("%s:\n", label);
    if (!head) {
        printf("  (empty)\n");
        return;
    }
    while (head) {
        printf(" %d. %s by %s\n", head->order, head->title, head->author);
        head = head->next;
    }
}

/* Find book node by order */
BookList *findBook(BookList *head, int order) {
    while (head) {
        if (head->order == order) return head;
        head = head->next;
    }
    return NULL;
}

/* Remove book by order */
void removeBook(BookList **head, int order) {
    BookList *cur = *head, *prev = NULL;
    while (cur && cur->order != order) {
        prev = cur;
        cur = cur->next;
    }
    if (!cur) {
        printf("Order %d not found.\n", order);
        return;
    }
    if (prev) prev->next = cur->next;
    else *head = cur->next;
    free(cur);
    reorderList(*head);
}

/* Change order of a book */
void changeOrder(BookList **head, int oldOrder, int newOrder) {
    if (oldOrder == newOrder) return;
    BookList *cur = *head, *prev = NULL;
    while (cur && cur->order != oldOrder) {
        prev = cur;
        cur = cur->next;
    }
    if (!cur) {
        printf("Order %d not found.\n", oldOrder);
        return;
    }
    // Remove node
    if (prev) prev->next = cur->next;
    else *head = cur->next;

    // Update order and re-insert
    cur->order = newOrder;
    insertBook(head, cur);
    reorderList(*head);
}

/* Add to read list maintaining order */
void addToReadList(BookList *book) {
    if (!book) return;
    // Check duplicates
    BookList *check = readHead;
    while (check) {
        if (strcmp(check->title, book->title) == 0 && strcmp(check->author, book->author) == 0)
            return; // Already in read list
        check = check->next;
    }
    BookList *copy = createBook(book->order, book->title, book->author);
    insertBook(&readHead, copy);
    reorderList(readHead);
}

/* The main menu function */
void doReadingList() {
    int choice, order, newOrder;
    char title[MAX_TITLE], author[MAX_AUTHOR];
    loadReadingList("book_list.txt");

    while (1) {
        printf("\n[Reading List Menu]\n");
        printf("1. Print Reading List\n");
        printf("2. Add New Book\n");
        printf("3. Change Book Order\n");
        printf("4. Remove Book\n");
        printf("5. Mark Book as Read\n");
        printf("6. Print Read Books\n");
        printf("0. Exit\n");
        printf("Select: ");
        if (scanf("%d", &choice) != 1) break;
        getchar(); // remove newline

        switch(choice) {
            case 1:
                printList("Reading List", readingHead);
                break;
            case 2:
                printf("Enter order to add: ");
                scanf("%d", &order); getchar();
                printf("Enter book title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\r\n")] = 0;
                printf("Enter author: ");
                fgets(author, sizeof(author), stdin);
                author[strcspn(author, "\r\n")] = 0;
                insertBook(&readingHead, createBook(order, title, author));
                reorderList(readingHead);
                printf("Book added.\n");
                break;
            case 3:
                printf("Enter current order: ");
                scanf("%d", &order);
                printf("Enter new order: ");
                scanf("%d", &newOrder);
                changeOrder(&readingHead, order, newOrder);
                printf("Order changed.\n");
                break;
            case 4:
                printf("Enter order to remove: ");
                scanf("%d", &order);
                removeBook(&readingHead, order);
                printf("Book removed if existed.\n");
                break;
            case 5:
                printf("Enter order to mark as read: ");
                scanf("%d", &order);
                {
                    BookList *b = findBook(readingHead, order);
                    if (b) {
                        addToReadList(b);
                        printf("Book marked as read.\n");
                    } else {
                        printf("Book not found.\n");
                    }
                }
                break;
            case 6:
                printList("Read Books", readHead);
                break;
            case 0:
                // Free lists and exit
                {
                    BookList *tmp;
                    while (readingHead) {
                        tmp = readingHead;
                        readingHead = readingHead->next;
                        free(tmp);
                    }
                    while (readHead) {
                        tmp = readHead;
                        readHead = readHead->next;
                        free(tmp);
                    }
                }
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

