#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_QUESTIONS 10
#define NUM_QUESTIONS 5
#define PASS_SCORE 80

typedef struct {
    int id;
    char question[100];
    char answer[100];
} Question;

typedef struct {
    char name[100];
    char nationality[50];
    int score;
    int passed;
} Trainee;

int checkAnswer(Question q, char *input) {
    return strcmp(q.answer, input) == 0;
}

void testKoreanLang(Trainee *trainees, int tCount, Question *questions) {
    srand(time(NULL));
    int tidx;
    do {
        tidx = rand() % tCount;
    } while (strcmp(trainees[tidx].nationality, "Korean") == 0);

    printf("Welcome, %s, to the Korean quiz session!\n", trainees[tidx].name);
    printf("Quiz starts in 30 seconds...\n");

    int score = 0, used[MAX_QUESTIONS] = {0};
    for (int i = 0, qid; i < NUM_QUESTIONS; ) {
        qid = rand() % MAX_QUESTIONS;
        if (used[qid]) continue;
        used[qid] = 1;

        printf("Q%d: %s\n> ", questions[qid].id, questions[qid].question);
        char input[100];
        scanf(" %[^\n]", input);

        if (checkAnswer(questions[qid], input))
            score += 20;
        i++;
    }

    trainees[tidx].score = score;
    trainees[tidx].passed = (score >= PASS_SCORE);

    printf("\nScore: %d/100\nStatus: %s\n", score, trainees[tidx].passed ? "PASS" : "FAIL");
}

int main() {
    Question questions[MAX_QUESTIONS] = {
        {1, "Say 'hello' in Korean.", "안녕하세요"},
        {2, "Say 'thank you' in Korean.", "감사합니다"},
        {3, "Say 'food' in Korean.", "음식"},
        {4, "Say 'good morning' in Korean.", "좋은 아침"},
        {5, "Say 'friend' in Korean.", "친구"},
        {6, "Say 'goodbye' in Korean.", "안녕히 가세요"},
        {7, "Say 'love' in Korean.", "사랑"},
        {8, "Say 'sorry' in Korean.", "미안합니다"},
        {9, "Say 'school' in Korean.", "학교"},
        {10, "Say 'good night' in Korean.", "좋은 밤"}
    };

    Trainee trainees[] = {
        {"Park Ji-yeon", "Korean", 0, 0},
        {"John Smith", "American", 0, 0},
        {"Léa Dubois", "French", 0, 0},
        {"Carlos Ruiz", "Mexican", 0, 0}
    };

    testKoreanLang(trainees, 4, questions);
    return 0;
}
