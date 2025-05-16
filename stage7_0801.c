#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DETAIL 1024
#define MAX_LINE 256

typedef struct Subject {
    char name[64];
    char details[MAX_DETAIL];
    struct Subject *next;
} Subject;

Subject *createSubject(const char *name) {
    Subject *s = malloc(sizeof(Subject));
    if (!s) { perror("malloc"); exit(1); }
    strncpy(s->name, name, 63);
    s->name[63] = '\0';
    s->details[0] = '\0';
    s->next = NULL;
    return s;
}

void appendSubject(Subject **head, Subject *newSubj) {
    if (!*head) {
        *head = newSubj;
        return;
    }
    Subject *cur = *head;
    while (cur->next) cur = cur->next;
    cur->next = newSubj;
}

void updateSubjectInfo(Subject *head, const char *line) {
    while (head) {
        if (strstr(line, head->name)) {
            const char *p = strstr(line, head->name) + strlen(head->name);
            while (*p == ':' || *p == ' ') p++;
            if (strlen(head->details) + strlen(p) + 2 < MAX_DETAIL) {
                if (head->details[0]) strcat(head->details, " ");
                strcat(head->details, p);
            }
            break;
        }
        head = head->next;
    }
}

/* Load content from file */
void loadContentFile(Subject *head, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Failed to open %s\n", filename);
        return;
    }
    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, f)) {
        line[strcspn(line, "\r\n")] = 0;  
        updateSubjectInfo(head, line);
    }
    fclose(f);
}

void printSubjectList(Subject *head) {
    int i = 1;
    while (head) {
        printf("%d. %s\n", i++, head->name);
        for (const char *p = head->details; *p; p++) {
            putchar(*p);
            if (*p == '.') putchar('\n');
        }
        printf("\n");
        head = head->next;
    }
}

void learnPerformanceStageTheory() {
    const char *subjects[] = {
        "Acting Theory",
        "Stage Theory",
        "Script Analysis",
        "Dialogue Interpretation and Emotional Expression",
        "Character Development",
        "Storytelling Theory",
        "Stage Movement and Poses"
    };

    Subject *head = NULL;
    for (int i = 0; i < 7; i++)
        appendSubject(&head, createSubject(subjects[i]));

    loadContentFile(head, "perform_stage.txt");
    printSubjectList(head);

    while (head) {
        Subject *tmp = head;
        head = head->next;
        free(tmp);
    }
}

