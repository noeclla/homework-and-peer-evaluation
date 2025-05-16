#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_QUEUE 4
#define MAX_LINE 256

typedef struct {
    char *group, *concept, *desc;
} KpopConcept;

typedef struct {
    char *items[MAX_QUEUE];
    int front, rear, count;
} Queue;

typedef struct {
    char *items[MAX_QUEUE];
    int top;
} Stack;

void clearInput() {
    while(getchar()!='\n');
}

void initQueue(Queue *q) { q->front=0; q->rear=-1; q->count=0; }
int enqueue(Queue *q, char *s) {
    if (q->count==MAX_QUEUE) return 0;
    q->rear=(q->rear+1)%MAX_QUEUE; q->items[q->rear]=s; q->count++; return 1;
}
char* dequeue(Queue *q) {
    if (q->count==0) return NULL;
    char *res=q->items[q->front];
    q->front=(q->front+1)%MAX_QUEUE; q->count--; return res;
}
int isQueueEmpty(Queue *q) { return q->count==0; }

void initStack(Stack *s) { s->top=-1; }
int push(Stack *s, char *str) {
    if (s->top==MAX_QUEUE-1) return 0;
    s->items[++(s->top)] = str; return 1;
}
char* pop(Stack *s) {
    if (s->top==-1) return NULL;
    return s->items[(s->top)--];
}
int isStackEmpty(Stack *s) { return s->top==-1; }

void shuffle(char **arr, int n) {
    for (int i=n-1; i>0; i--) {
        int j=rand()%(i+1);
        char *tmp=arr[i]; arr[i]=arr[j]; arr[j]=tmp;
    }
}

int readConceptFile(const char *fname, KpopConcept **out) {
    FILE *f=fopen(fname,"r");
    if (!f) { printf("Can't open %s\n", fname); return 0; }
    int cap=4, cnt=0; KpopConcept *arr=malloc(sizeof(KpopConcept)*cap);
    char line[MAX_LINE], desc[1024];
    while (fgets(line,sizeof(line),f)) {
        if (strlen(line)<=1) continue;
        line[strcspn(line,"\r\n")]=0;
        char *grp=strdup(line);
        if (!fgets(line,sizeof(line),f)) break;
        line[strcspn(line,"\r\n")]=0;
        char *con=strdup(line);
        desc[0]=0;
        while (fgets(line,sizeof(line),f)) {
            line[strcspn(line,"\r\n")]=0;
            if (strcmp(line,"---")==0) break;
            strcat(desc,line); strcat(desc,"\n");
        }
        char *dsc=strdup(desc);
        if (cnt==cap) {
            cap*=2;
            KpopConcept *tmp=realloc(arr,sizeof(KpopConcept)*cap);
            if (!tmp) break; arr=tmp;
        }
        arr[cnt].group=grp; arr[cnt].concept=con; arr[cnt].desc=dsc;
        cnt++;
    }
    fclose(f); *out=arr; return cnt;
}

void freeConcepts(KpopConcept *arr,int n) {
    for(int i=0;i<n;i++) {
        free(arr[i].group);
        free(arr[i].concept);
        free(arr[i].desc);
    }
    free(arr);
}

void displayMismatch(char **g, char **c, int n) {
    printf("\n## Mismatched Group and Concept ##\n");
    for(int i=0;i<n;i++)
        printf("%d. %s : %s\n", i+1, g[i], c[i]);
    printf("\n");
}

void matchQueue(char **g, char **c, int n) {
    Queue qg,qc; initQueue(&qg); initQueue(&qc);
    for(int i=0;i<n;i++) { enqueue(&qg,g[i]); enqueue(&qc,c[i]); }
    int corr=0, incorr=0;
    clock_t start=clock();
    while(!isQueueEmpty(&qg)) {
        char *gr=dequeue(&qg);
        char *co=dequeue(&qc);
        printf("Match '%s' with '%s'? (y/n): ", gr, co);
        int ch=getchar(); clearInput();
        if(ch=='y'||ch=='Y') corr++; else incorr++;
    }
    clock_t end=clock();
    printf("Completed in %.2fs. Correct: %d, Incorrect: %d\n\n",
           (double)(end-start)/CLOCKS_PER_SEC, corr, incorr);
}

void matchStack(char **g, char **c, int n) {
    Stack sg, sc; initStack(&sg); initStack(&sc);
    for(int i=0;i<n;i++) { push(&sg,g[i]); push(&sc,c[i]); }
    int corr=0, incorr=0;
    clock_t start=clock();
    while(!isStackEmpty(&sg)) {
        char *gr=pop(&sg);
        char *co=pop(&sc);
        printf("Match '%s' with '%s'? (y/n): ", gr, co);
        int ch=getchar(); clearInput();
        if(ch=='y'||ch=='Y') corr++; else incorr++;
    }
    clock_t end=clock();
    printf("Completed in %.2fs. Correct: %d, Incorrect: %d\n\n",
           (double)(end-start)/CLOCKS_PER_SEC, corr, incorr);
}

void defineConcept() {
    srand((unsigned)time(NULL));
    printf("[Concept Research]\nLoading data...\n");
    KpopConcept *data=NULL;
    int total=readConceptFile("concept.txt",&data);
    if(total==0) { printf("No data found.\n"); return; }
    printf("Loaded %d concepts.\n", total);

    int n=0;
    do {
        printf("How many sets to extract (1-%d)? ", (total<MAX_QUEUE)?total:MAX_QUEUE);
        scanf("%d",&n);
        clearInput();
    } while(n<1 || n>total || n>MAX_QUEUE);

    int idxs[total];
    for(int i=0;i<total;i++) idxs[i]=i;
    for(int i=total-1;i>0;i--) {
        int j=rand()%(i+1);
        int t=idxs[i]; idxs[i]=idxs[j]; idxs[j]=t;
    }

    char *groups[MAX_QUEUE], *concepts[MAX_QUEUE];
    for(int i=0;i<n;i++) {
        groups[i] = data[idxs[i]].group;
        concepts[i] = data[idxs[i]].concept;
    }

    shuffle(groups, n);
    shuffle(concepts, n);

    displayMismatch(groups, concepts, n);

    int choice=0;
    do {
        printf("Match using: 1.Queue 2.Stack ? ");
        scanf("%d",&choice);
        clearInput();
    } while(choice!=1 && choice!=2);

    if(choice==1) matchQueue(groups, concepts, n);
    else matchStack(groups, concepts, n);

    freeConcepts(data, total);
}
