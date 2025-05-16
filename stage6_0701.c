#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DATE_LEN 11
#define TITLE_MAX 50
#define FIELD_MAX 30
#define QUAL_MAX 100

typedef struct {
    char *qualifications; // dynamically allocated string
} JobDetail;

typedef struct JobPost {
    int id;
    char title[TITLE_MAX];
    char postingDate[DATE_LEN]; // "YYYY-MM-DD"
    char deadline[DATE_LEN];
    int hires;
    char jobField[FIELD_MAX];
    JobDetail detail;
    struct JobPost *next;
} JobPost;

typedef struct {
    char name[20];
    char apiKey[50];
    char apiURL[100];
} SocialAPI;

SocialAPI socialAPIs[] = {
    {"Facebook", "FB_API_KEY", "https://api.facebook.com/post"},
    {"Instagram", "IG_API_KEY", "https://api.instagram.com/post"},
    {"Thread", "TH_API_KEY", "https://api.thread.com/post"},
    {"LinkedIn", "LI_API_KEY", "https://api.linkedin.com/post"},
    {"X", "X_API_KEY", "https://api.x.com/post"}
};
const int socialCount = sizeof(socialAPIs)/sizeof(socialAPIs[0]);

JobPost *jobList = NULL;
JobPost *expiredList = NULL;
int nextID = 1;

void clearInput() {
    int c; while((c=getchar()) != '\n' && c != EOF);
}

int dateCompare(const char *date1, const char *date2) {
    // returns <0 if date1 < date2, 0 if equal, >0 if date1 > date2
    return strcmp(date1, date2);
}

void addJobPost() {
    JobPost *job = malloc(sizeof(JobPost));
    if (!job) { printf("Memory error\n"); return; }
    job->id = nextID++;
    job->next = NULL;

    printf("Enter Job Title (max %d chars): ", TITLE_MAX-1);
    fgets(job->title, TITLE_MAX, stdin);
    job->title[strcspn(job->title, "\n")] = 0;

    printf("Enter Posting Date (YYYY-MM-DD): ");
    fgets(job->postingDate, DATE_LEN, stdin);
    job->postingDate[strcspn(job->postingDate, "\n")] = 0;

    printf("Enter Deadline (YYYY-MM-DD): ");
    fgets(job->deadline, DATE_LEN, stdin);
    job->deadline[strcspn(job->deadline, "\n")] = 0;

    printf("Enter Number of Hires: ");
    scanf("%d", &job->hires);
    clearInput();

    printf("Enter Job Field (max %d chars): ", FIELD_MAX-1);
    fgets(job->jobField, FIELD_MAX, stdin);
    job->jobField[strcspn(job->jobField, "\n")] = 0;

    printf("Enter Qualifications (no length limit, end with a blank line):\n");
    size_t qualSize = 128, qualLen = 0;
    job->detail.qualifications = malloc(qualSize);
    if (!job->detail.qualifications) { printf("Memory error\n"); free(job); return; }
    job->detail.qualifications[0] = 0;

    char line[QUAL_MAX];
    while (fgets(line, QUAL_MAX, stdin)) {
        if (strcmp(line, "\n") == 0) break; // empty line to end
        size_t lineLen = strlen(line);
        if (qualLen + lineLen + 1 > qualSize) {
            qualSize *= 2;
            char *temp = realloc(job->detail.qualifications, qualSize);
            if (!temp) { printf("Memory error\n"); free(job->detail.qualifications); free(job); return; }
            job->detail.qualifications = temp;
        }
        strcat(job->detail.qualifications, line);
        qualLen += lineLen;
    }

    job->next = jobList;
    jobList = job;

    printf("Job post created with ID %d\n", job->id);
}

void viewJobDetails(JobPost *job) {
    printf("\nID: %d\nTitle: %s\nPosting Date: %s\nDeadline: %s\nHires: %d\nField: %s\nQualifications:\n%s\n",
        job->id, job->title, job->postingDate, job->deadline, job->hires, job->jobField, job->detail.qualifications);
}

void moveExpiredJobs() {
    time_t t = time(NULL);
    struct tm *tm_now = localtime(&t);
    char today[DATE_LEN];
    snprintf(today, DATE_LEN, "%04d-%02d-%02d", tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday);

    JobPost **ptr = &jobList;
    while (*ptr) {
        if (dateCompare((*ptr)->deadline, today) < 0) {
            // expired: remove from jobList and add to expiredList
            JobPost *expired = *ptr;
            *ptr = expired->next;
            expired->next = expiredList;
            expiredList = expired;
        } else {
            ptr = &(*ptr)->next;
        }
    }
}

void viewJobPostings() {
    moveExpiredJobs();
    if (!jobList) {
        printf("No active job postings. Please create one.\n");
        addJobPost();
        return;
    }

    printf("Active Job Postings:\n");
    for (JobPost *p=jobList; p; p=p->next) {
        printf("ID: %d | Title: %s | Deadline: %s\n", p->id, p->title, p->deadline);
    }
    printf("Enter ID to view details or 0 to return: ");
    int id; scanf("%d", &id); clearInput();
    if (id == 0) return;

    for (JobPost *p=jobList; p; p=p->next) {
        if (p->id == id) {
            viewJobDetails(p);
            return;
        }
    }
    printf("Job post with ID %d not found.\n", id);
}

void viewExpiredJobs() {
    if (!expiredList) {
        printf("No expired job postings.\n");
        return;
    }
    printf("Expired Job Postings:\n");
    for (JobPost *p=expiredList; p; p=p->next) {
        printf("ID: %d | Title: %s | Deadline: %s\n", p->id, p->title, p->deadline);
    }
    printf("Enter ID to view details or 0 to return: ");
    int id; scanf("%d", &id); clearInput();
    if (id == 0) return;

    for (JobPost *p=expiredList; p; p=p->next) {
        if (p->id == id) {
            viewJobDetails(p);
            return;
        }
    }
    printf("Job post with ID %d not found.\n", id);
}

void postOnSocialNetwork() {
    if (!jobList && !expiredList) {
        printf("No job postings available to post.\n");
        return;
    }

    printf("Select Social Network:\n");
    for (int i=0; i<socialCount; i++) {
        printf("%d. %s\n", i+1, socialAPIs[i].name);
    }
    printf("Enter choice (0 to cancel): ");
    int choice; scanf("%d", &choice); clearInput();
    if (choice == 0) return;
    if (choice < 1 || choice > socialCount) {
        printf("Invalid choice.\n");
        return;
    }

    int id;
    printf("Enter Job Post ID to post: ");
    scanf("%d", &id); clearInput();

    JobPost *job = NULL;
    for (JobPost *p=jobList; p; p=p->next) if(p->id == id) job = p;
    if (!job) {
        for (JobPost *p=expiredList; p; p=p->next) if(p->id == id) job = p;
    }
    if (!job) {
        printf("Job post with ID %d not found.\n", id);
        return;
    }

    // Stub: "posting"
    printf("Posting '%s' on %s...\n", job->title, socialAPIs[choice-1].name);
    printf("Using API Key: %s\n", socialAPIs[choice-1].apiKey);
    printf("API URL: %s\n", socialAPIs[choice-1].apiURL);
    printf("Post successful!\n");
}

void freeJobs(JobPost *head) {
    while (head) {
        JobPost *tmp = head;
        head = head->next;
        free(tmp->detail.qualifications);
        free(tmp);
    }
}

void findSpecialist() {
    int running = 1;

    while (running) {
        printf("\n[II. Training > 6. Visual & Image Training > A. Finding People]\n");
        printf("1. Create Job Posting\n2. View Job Postings\n3. Post on Social Networks\n4. View Expired Job Postings\n5. Exit\n");
        printf("Select: ");
        int sel; scanf("%d", &sel); clearInput();

        switch(sel) {
            case 1: addJobPost(); break;
            case 2: viewJobPostings(); break;
            case 3: postOnSocialNetwork(); break;
            case 4: viewExpiredJobs(); break;
            case 5: running = 0; break;
            default: printf("Invalid option.\n"); break;
        }
    }

    freeJobs(jobList);
    freeJobs(expiredList);
}

int main() {
    findSpecialist();
    return 0;
}
