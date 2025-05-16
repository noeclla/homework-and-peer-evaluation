#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_WORDS 100
#define GAME_WORDS 42
#define MAX_NAME 20
#define MAX_MEMBERS 10

// 밀리웨이즈 멤버 닉네임 (예시)
char *members[MAX_MEMBERS] = {"Alex", "Bob", "Cindy", "Diana", "Ethan", "Fiona", "George", "Helen", "Ian", "Jane"};

typedef struct {
    char challenger[MAX_NAME];
    int challenger_count;
    int challenger_time;
    char opponent[MAX_NAME];
    int opponent_count;
    int opponent_time;
} Score;

char words[TOTAL_WORDS][5] = {
    "가방","나라","사과","나무","학교","연필","의자","바다","우산","전화",
    "친구","시계","책상","커피","노트","컴퓨터","자동","생각","음악","꽃",
    "달력","의사","시장","카메라","신발","도로","손가","자전","기차","버스",
    "공원","의류","음식","사전","공부","소리","영화","천사","생일","전화",
    "나비","산책","달빛","도서","초코","화분","자동","기름","병원","시장",
    "하늘","바람","우유","초등","화장","상자","여행","의자","비행","신호",
    "종이","노래","천둥","전기","창문","하루","가족","책","달걀","요리",
    "고양","나무","도로","별","식사","바다","의자","친구","집","자동",
    "바람","산","물","꽃","시간","공부","시계","전화","의사","노트"
};

Score last_score = { "",0,0,"",0,0 };

void flush() { while(getchar()!='\n'); }

int isMember(char *name) {
    int i;
    for(i=0;i<MAX_MEMBERS;i++)
        if(strcmp(members[i],name)==0) return 1;
    return 0;
}

void shuffleWords(char arr[][5], int n) {
    int i,j;
    char temp[5];
    for(i=n-1;i>0;i--) {
        j=rand()%(i+1);
        strcpy(temp, arr[i]);
        strcpy(arr[i], arr[j]);
        strcpy(arr[j], temp);
    }
}

int canChain(char *prev, char *next) {
    return prev[strlen(prev)-1]==next[0];
}

int playGame(char challenger[], char opponent[], Score *score) {
    char gameWords[GAME_WORDS][5];
    int i, used[GAME_WORDS] = {0};
    int count_chal=0, count_opp=0;
    int turn=0; // 0 challenger, 1 opponent
    char input[5];
    time_t start, now;
    int wordCount = 0;

    // 랜덤 42개 단어 선택
    int idxs[GAME_WORDS];
    for(i=0;i<GAME_WORDS;i++) idxs[i] = i;
    shuffleWords(words, TOTAL_WORDS);
    for(i=0;i<GAME_WORDS;i++) strcpy(gameWords[i], words[i]);

    printf("게임 시작! 엔터 누르면 시작합니다.\n");
    getchar();
    start = time(NULL);

    char lastWord[5] = "";
    while(1) {
        now = time(NULL);
        if(now - start >= 60) break;

        printf("%s 차례입니다. 단어 입력(종료는 0): ", (turn==0)?challenger:opponent);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input,"\n")] = 0;
        if(strcmp(input,"0")==0) break;

        // 유효 단어 검사: 게임 단어에 존재 & 아직 사용 안함 & 끝말잇기 규칙
        int valid = 0, wi;
        for(wi=0; wi<GAME_WORDS; wi++) {
            if(strcmp(gameWords[wi],input)==0 && !used[wi]) {
                if(wordCount==0 || canChain(lastWord, input)) {
                    valid = 1; break;
                }
            }
        }
        if(!valid) {
            printf("잘못된 단어입니다. 다시 입력하세요.\n");
            continue;
        }

        used[wi] = 1;
        strcpy(lastWord, input);
        wordCount++;

        if(turn==0) count_chal++;
        else count_opp++;

        turn = 1 - turn;
    }

    score->challenger_count = count_chal;
    score->challenger_time = (int)(time(NULL) - start);
    score->opponent_count = count_opp;
    score->opponent_time = score->challenger_time; // 시간은 동일

    return 0;
}

void displayWinner(Score *score) {
    printf("\n게임 결과\n");
    printf("%s: 단어 %d개, 시간 %d초\n", score->challenger, score->challenger_count, score->challenger_time);
    printf("%s: 단어 %d개, 시간 %d초\n", score->opponent, score->opponent_count, score->opponent_time);

    if(score->challenger_count > score->opponent_count)
        printf("승자: %s\n", score->challenger);
    else if(score->challenger_count < score->opponent_count)
        printf("승자: %s\n", score->opponent);
    else {
        if(score->challenger_time < score->opponent_time)
            printf("승자: %s\n", score->challenger);
        else if(score->challenger_time > score->opponent_time)
            printf("승자: %s\n", score->opponent);
        else
            printf("무승부입니다.\n");
    }
}

void doWordRelay() {
    char challenger[MAX_NAME], opponent[MAX_NAME];
    Score score;

    printf("도전자 닉네임 입력: ");
    fgets(challenger, MAX_NAME, stdin);
    challenger[strcspn(challenger,"\n")] = 0;
    if(!isMember(challenger)) {
        printf("멤버가 아닙니다.\n");
        return;
    }

    if(strlen(last_score.challenger)>0) {
        strcpy(opponent, last_score.challenger);
        printf("자동으로 이전 우승자 %s 와 게임 진행합니다.\n", opponent);
    } else {
        printf("대상자 닉네임 입력: ");
        fgets(opponent, MAX_NAME, stdin);
        opponent[strcspn(opponent,"\n")] = 0;
        if(!isMember(opponent)) {
            printf("멤버가 아닙니다.\n");
            return;
        }
    }

    strcpy(score.challenger, challenger);
    strcpy(score.opponent, opponent);

    playGame(challenger, opponent, &score);
    displayWinner(&score);

    last_score = score;
}

int main() {
    srand((unsigned)time(NULL));
    doWordRelay();
    return 0;
}
