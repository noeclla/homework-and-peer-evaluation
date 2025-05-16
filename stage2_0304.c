#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MEMBER_NUM 5
#define SONG_NUM 7
#define NAME_LEN 20
#define TITLE_LEN 50
#define CONS_LEN 20
#define MAX_TRY 2

typedef struct {
    char title[TITLE_LEN];
    char consonants[CONS_LEN];
    char lyric[100];
} KpopSong;

char members[MEMBER_NUM][NAME_LEN] = {
    "Jinho", "Eunseo", "Mira", "Hwan", "Yubin"
};
char nicknames[MEMBER_NUM][NAME_LEN] = {
    "jin", "seo", "mi", "hwan", "yu"
};

KpopSong songs[SONG_NUM];
int songCount = 0;

void extractConsonant(const char* title, char* cons) {
    int i = 0, j = 0;
    while(title[i]) {
        unsigned char c = title[i];
        if((c >= 0xB0 && c <= 0xC8) || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            cons[j++] = title[i];
        }
        i++;
    }
    cons[j] = 0;
}

void inputKpopSongInitials() {
    songCount = 0;
    for(int i=0; i<SONG_NUM; i++) {
        printf("Enter song title %d: ", i+1);
        fgets(songs[i].title, TITLE_LEN, stdin);
        songs[i].title[strcspn(songs[i].title, "\n")] = 0;

        extractConsonant(songs[i].title, songs[i].consonants);

        printf("Enter lyric snippet for bonus mission: ");
        fgets(songs[i].lyric, sizeof(songs[i].lyric), stdin);
        songs[i].lyric[strcspn(songs[i].lyric, "\n")] = 0;

        songCount++;
    }
}

int checkKpopSongTitle(const char* guess, const char* consonants) {
    char consGuess[CONS_LEN];
    extractConsonant(guess, consGuess);
    return strcmp(consGuess, consonants) == 0;
}

int getBlackKnight(int current) {
    int next;
    do {
        next = rand() % MEMBER_NUM;
    } while(next == current);
    return next;
}

int inputSongTitle(int memberIdx, const char* cons) {
    char guess[TITLE_LEN];
    for(int try=0; try < MAX_TRY; try++) {
        printf("%s (%s), enter song title guess (%d/2): ", members[memberIdx], nicknames[memberIdx], try+1);
        fgets(guess, TITLE_LEN, stdin);
        guess[strcspn(guess, "\n")] = 0;

        if(checkKpopSongTitle(guess, cons)) {
            printf("%s님, 미션을 성공적으로 완료했습니다!\n", members[memberIdx]);
            return 1;
        } else {
            printf("틀렸습니다.\n");
        }
    }
    return 0;
}

void completeMission() {
    if(songCount == 0) {
        printf("먼저 노래 정보를 입력하세요.\n");
        inputKpopSongInitials();
    }

    srand(time(NULL));
    int currentMember = 0;
    int completed = 0;

    for(int i=0; i<songCount; i++) {
        printf("\n초성: %s\n", songs[i].consonants);
        int success = inputSongTitle(currentMember, songs[i].consonants);
        if(!success) {
            currentMember = getBlackKnight(currentMember);
            printf("미션 대행자: %s\n", members[currentMember]);
            success = inputSongTitle(currentMember, songs[i].consonants);
            if(!success) {
                printf("밀리웨이즈는 팀워크를 더 발전시켜서 다시 도전하세요!\n");
                return;
            }
        }
        currentMember = (currentMember + 1) % MEMBER_NUM;
        completed++;
    }

    if(completed == songCount) {
        printf("밀리웨이즈의 팀워크는 훌륭합니다!\n");
    }
}

void bonusMission() {
    printf("\n=== 보너스 미션: 가사 일부로 노래 맞추기 ===\n");
    srand(time(NULL));
    int idx = rand() % songCount;
    printf("가사 일부: %s\n", songs[idx].lyric);

    char guess[TITLE_LEN];
    for(int try=0; try < MAX_TRY; try++) {
        printf("노래 제목을 입력하세요 (%d/2): ", try+1);
        fgets(guess, TITLE_LEN, stdin);
        guess[strcspn(guess, "\n")] = 0;
        if(strcmp(guess, songs[idx].title) == 0) {
            printf("미션 성공!\n");
            return;
        } else {
            printf("틀렸습니다.\n");
        }
    }
    printf("미션 실패...\n");
}
