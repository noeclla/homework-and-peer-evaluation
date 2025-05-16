#include <stdio.h>
#include <string.h>
#include <time.h>

#define MEMBER_NUM 5
#define NAME_LEN 20
#define ANSWER_MIN 20
#define ANSWER_MAX 50
#define DATE_LEN 11
#define Q_NUM 3

typedef struct {
    char name[NAME_LEN];
    char nickname[NAME_LEN];
} Member;

typedef struct {
    char nickname[NAME_LEN];
    char date[DATE_LEN];
    char answers[Q_NUM][ANSWER_MAX + 1];
} DailyReflection;

const Member members[MEMBER_NUM] = {
    {"Jinho", "jin"},
    {"Eunseo", "seo"},
    {"Mira", "mi"},
    {"Hwan", "hwan"},
    {"Yubin", "yu"}
};

const char questions[Q_NUM][100] = {
    "오늘 어떤 성취를 이뤘나?",
    "어떤 어려움을 극복했나?",
    "내일은 어떤 계획을 세울까?"
};

void getTodayDate(char* buf) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buf, DATE_LEN, "%Y-%m-%d", tm_info);
}

void inputReflection(DailyReflection* dr) {
    int idx;
    printf("멤버 선택:\n");
    for(int i=0; i<MEMBER_NUM; i++)
        printf("%d. %s (%s)\n", i+1, members[i].name, members[i].nickname);
    do {
        printf("번호 입력: ");
        scanf("%d", &idx);
        while(getchar() != '\n'); 
    } while(idx < 1 || idx > MEMBER_NUM);

    strcpy(dr->nickname, members[idx-1].nickname);
    getTodayDate(dr->date);

    for(int i=0; i<Q_NUM; i++) {
        do {
            printf("%s (20~50자): ", questions[i]);
            fgets(dr->answers[i], ANSWER_MAX+1, stdin);
            dr->answers[i][strcspn(dr->answers[i], "\n")] = 0;
        } while(strlen(dr->answers[i]) < ANSWER_MIN || strlen(dr->answers[i]) > ANSWER_MAX);
    }
}

int answerLengthWithoutSpace(const char* str) {
    int len = 0;
    for(; *str; str++) if(*str != ' ') len++;
    return len;
}

void displayReflection(const DailyReflection* dr) {
    printf("\n[%s] %s님의 자기 성찰\n", dr->date, dr->nickname);

    int order[Q_NUM] = {0,1,2};
    for(int i=0; i<Q_NUM-1; i++) {
        for(int j=i+1; j<Q_NUM; j++) {
            if(answerLengthWithoutSpace(dr->answers[order[i]]) < answerLengthWithoutSpace(dr->answers[order[j]])) {
                int tmp = order[i]; order[i] = order[j]; order[j] = tmp;
            }
        }
    }

    for(int i=0; i<Q_NUM; i++) {
        int q = order[i];
        printf("%s\n%s\n\n", questions[q], dr->answers[q]);
    }
}

void doSelfReflection() {
    DailyReflection dr;
    inputReflection(&dr);
    displayReflection(&dr);
    printf("[2. 자기관리와 팀워크] 메뉴로 돌아갑니다.\n");
}
