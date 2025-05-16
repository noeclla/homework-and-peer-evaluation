#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LEN 256
typedef struct Node {
    int date;
    char place[50], show[50], reflect[LEN], gratitude[LEN];
    struct Node *l, *r;
} Node;

Node* newNode(int d, char *p, char *s, char *r, char *g) {
    Node *n = malloc(sizeof(Node));
    n->date=d; strncpy(n->place,p,49); strncpy(n->show,s,49);
    strncpy(n->reflect,r,LEN-1); strncpy(n->gratitude,g,LEN-1);
    n->place[49]=n->show[49]=n->reflect[LEN-1]=n->gratitude[LEN-1]=0;
    n->l=n->r=NULL; return n;
}

Node* insert(Node *t, Node *n) {
    if(!t) return n;
    if(n->date<t->date) t->l=insert(t->l,n);
    else if(n->date>t->date) t->r=insert(t->r,n);
    else { // overwrite existing
        strncpy(t->place,n->place,49); strncpy(t->show,n->show,49);
        strncpy(t->reflect,n->reflect,LEN-1); strncpy(t->gratitude,n->gratitude,LEN-1);
        free(n);
    }
    return t;
}

Node* search(Node *t, int d) {
    if(!t||t->date==d) return t;
    return d<t->date ? search(t->l,d) : search(t->r,d);
}

Node* minNode(Node *t) { while(t&&t->l) t=t->l; return t; }

Node* delete(Node *t, int d) {
    if(!t) return NULL;
    if(d<t->date) t->l=delete(t->l,d);
    else if(d>t->date) t->r=delete(t->r,d);
    else {
        if(!t->l){ Node *r=t->r; free(t); return r; }
        if(!t->r){ Node *l=t->l; free(t); return l; }
        Node *m=minNode(t->r);
        t->date=m->date; strncpy(t->place,m->place,49); strncpy(t->show,m->show,49);
        strncpy(t->reflect,m->reflect,LEN-1); strncpy(t->gratitude,m->gratitude,LEN-1);
        t->r=delete(t->r,m->date);
    }
    return t;
}

void inorderDates(Node *t) {
    if(!t) return;
    inorderDates(t->l);
    printf("%08d\n", t->date);
    inorderDates(t->r);
}

void printNode(Node *n) {
    if(!n){ printf("Not found.\n"); return; }
    printf("Date: %08d\nPlace: %s\nShow: %s\nReflection:\n%s\nGratitude:\n%s\n",
        n->date, n->place, n->show, n->reflect, n->gratitude);
}

int getMMDD(int d) { return ((d/100)%100)*100 + (d%100); }

int currentMMDD() {
    time_t t=time(NULL); struct tm *lt=localtime(&t);
    return (lt->tm_mon+1)*100 + lt->tm_mday;
}

void printMatching(Node *t, int mmdd) {
    if(!t) return;
    printMatching(t->l, mmdd);
    if(getMMDD(t->date)==mmdd) printNode(t);
    printMatching(t->r, mmdd);
}

void readln(char *b,int s) {
    if(fgets(b,s,stdin)) {
        size_t l=strlen(b);
        if(l>0 && b[l-1]=='\n') b[l-1]=0;
    }
}

int parseDate(char *s) {
    if(strlen(s)!=8) return 0;
    for(int i=0;i<8;i++) if(s[i]<'0'||s[i]>'9') return 0;
    return atoi(s);
}

int main() {
    Node *root=NULL; char cmd[10], dateS[10], place[50], show[50], refl[LEN], grat[LEN];
    printf("=== Self-Reflection Manager ===\n");
    int mmdd = currentMMDD();
    printMatching(root, mmdd);

    while(1) {
        printf("\nI)nput O)utput S)earch D)elete Q)uit > ");
        readln(cmd,10);
        if(cmd[0]=='q'||cmd[0]=='Q') break;

        if(cmd[0]=='I'||cmd[0]=='i') {
            printf("Date YYYYMMDD: "); readln(dateS,10);
            int d=parseDate(dateS);
            if(!d){ printf("Bad date.\n"); continue; }
            printf("Place: "); readln(place,50);
            printf("Show name: "); readln(show,50);
            printf("Reflection: "); readln(refl,LEN);
            printf("Gratitude: "); readln(grat,LEN);
            root=insert(root,newNode(d,place,show,refl,grat));
            printf("Saved.\n");
        }
        else if(cmd[0]=='O'||cmd[0]=='o') {
            if(!root) printf("No data.\n");
            else { printf("Dates:\n"); inorderDates(root); }
        }
        else if(cmd[0]=='S'||cmd[0]=='s') {
            printf("Search date YYYYMMDD: "); readln(dateS,10);
            int d=parseDate(dateS);
            if(!d){ printf("Bad date.\n"); continue; }
            printNode(search(root,d));
        }
        else if(cmd[0]=='D'||cmd[0]=='d') {
            printf("Delete date YYYYMMDD: "); readln(dateS,10);
            int d=parseDate(dateS);
            if(!d){ printf("Bad date.\n"); continue; }
            if(!search(root,d)) { printf("No such date.\n"); }
            else { root=delete(root,d); printf("Deleted.\n"); }
        }
        else printf("Invalid.\n");
    }

    while(root) root=delete(root,root->date);
    return 0;
}
