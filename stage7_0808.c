#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR 100
#define ALBUMS 5

typedef struct Hint {
    char w1[MAX_STR], w2[MAX_STR];
} Hint;

typedef struct Node {
    char album[MAX_STR], content[300];
    Hint hint;
    struct Node *prev, *next;
} Node;

void trim(char *s) {
    int n = (int)strlen(s);
    if (n > 0 && (s[n-1]=='\n' || s[n-1]=='\r')) s[n-1] = 0;
}

int containsIgnoreCase(const char *hay, const char *need) {
    char h[300], n[MAX_STR];
    int i;
    for(i=0; hay[i] && i<299; i++) h[i]=tolower(hay[i]);
    h[i]=0;
    for(i=0; need[i] && i<MAX_STR-1; i++) n[i]=tolower(need[i]);
    n[i]=0;
    return strstr(h,n)!=NULL;
}

int load(Node **head) {
    FILE *f = fopen("world_view.txt","r");
    if(!f) return 0;
    Node *prev = NULL;
    for(int i=0; i<ALBUMS; i++) {
        Node *cur = malloc(sizeof(Node));
        if(!cur) { fclose(f); return 0; }
        if(!fgets(cur->album, MAX_STR, f)) { free(cur); break; }
        trim(cur->album);
        if(!fgets(cur->content, 300, f)) { free(cur); break; }
        trim(cur->content);
        cur->hint.w1[0]=cur->hint.w2[0]=0;
        cur->prev=prev; cur->next=NULL;
        if(prev) prev->next=cur; else *head=cur;
        prev=cur;
        fgets(cur->content, 300, f); // skip blank line if any
    }
    fclose(f);
    return 1;
}

int inputHints(Node *head) {
    Node *cur = head;
    while(cur && cur->next) {
        cur=cur->next;
        for(int tries=0; tries<3; tries++) {
            printf("\nEnter 2 hint words for album '%s' (from previous album content):\n", cur->album);
            printf("Hint 1: "); if(!fgets(cur->hint.w1, MAX_STR, stdin)) return 0;
            printf("Hint 2: "); if(!fgets(cur->hint.w2, MAX_STR, stdin)) return 0;
            trim(cur->hint.w1); trim(cur->hint.w2);
            Node *prev = cur->prev;
            if(!containsIgnoreCase(prev->content, cur->hint.w1) ||
               !containsIgnoreCase(prev->content, cur->hint.w2)) {
                printf("Hints must appear in previous album content. Retry.\n");
                continue;
            }
            if((!containsIgnoreCase(cur->content, cur->hint.w1) ||
                !containsIgnoreCase(cur->content, cur->hint.w2)) && tries<2) {
                printf("Hints not fully in current album content. Retry.\n");
                continue;
            }
            break;
        }
    }
    return 1;
}

void printAll(Node *head) {
    printf("\n--- WorldView ---\n");
    int i=1;
    for(Node *cur=head; cur; cur=cur->next, i++) {
        printf("Album %d: %s\n", i, cur->album);
        printf("Content: %s\n", cur->content);
        if(cur->hint.w1[0])
            printf("Hints: %s, %s\n", cur->hint.w1, cur->hint.w2);
        printf("---------------\n");
    }
}

void freeList(Node *head) {
    while(head) {
        Node *t = head;
        head=head->next;
        free(t);
    }
}

int main() {
    Node *head=NULL;
    printf("=== K-POP WorldView ===\n");
    if(!load(&head)) { printf("Failed to load file.\n"); return 1; }
    if(!inputHints(head)) { printf("Input hints failed.\n"); freeList(head); return 1; }
    printAll(head);
    freeList(head);
    return 0;
}
