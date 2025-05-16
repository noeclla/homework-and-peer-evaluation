#include <stdio.h>
#include <string.h>

#define MAX_MEM 5
#define MAX_NAME 30

typedef struct {
    char teamName[MAX_NAME];
    int passAvgScore;
    int passMinScore;
    int passTimeLimit;

    int avgScore;
    int minScore;
    int totalTime;
    char finalResult[10];
} TeamPerformance;

void inputBaseLine(TeamPerformance *team, int maxAvg, int maxMin, int minTime, int maxTime, int members) {
    printf("Enter team name: ");
    fgets(team->teamName, MAX_NAME, stdin);
    team->teamName[strcspn(team->teamName, "\n")] = 0;

    do {
        printf("Passing average score (max %d): ", maxAvg);
        scanf("%d", &team->passAvgScore);
    } while (team->passAvgScore < 0 || team->passAvgScore > maxAvg);

    do {
        printf("Passing minimum score (max %d): ", maxMin);
        scanf("%d", &team->passMinScore);
    } while (team->passMinScore < 0 || team->passMinScore > maxMin);

    do {
        printf("Passing time limit in minutes (%d to %d total): ", members * minTime, members * maxTime);
        scanf("%d", &team->passTimeLimit);
    } while (team->passTimeLimit < members * minTime || team->passTimeLimit > members * maxTime);

    getchar(); 
}

void evalTeamPerformance(TeamPerformance *team, int scores[], int times[], int members) {
    int sumScore = 0, minScore = scores[0], sumTime = 0;
    for (int i = 0; i < members; i++) {
        if (scores[i] < minScore) minScore = scores[i];
        sumScore += scores[i];
        sumTime += times[i];
    }
    team->avgScore = sumScore / members;
    team->minScore = minScore;
    team->totalTime = sumTime;

    if (team->avgScore >= team->passAvgScore && team->minScore >= team->passMinScore && team->totalTime <= team->passTimeLimit)
        strcpy(team->finalResult, "Pass");
    else
        strcpy(team->finalResult, "Fail");
}

void learnHangul() {
    TeamPerformance team;
    int scores[MAX_MEM], times[MAX_MEM];

    inputBaseLine(&team, 300, 300, 10, 30, MAX_MEM);

    printf("Enter scores and times for %d members:\n", MAX_MEM);
    for (int i = 0; i < MAX_MEM; i++) {
        printf("Member %d score (max 300): ", i+1);
        scanf("%d", &scores[i]);
        printf("Member %d time (minutes): ", i+1);
        scanf("%d", &times[i]);
    }
    getchar();

    evalTeamPerformance(&team, scores, times, MAX_MEM);

    printf("\nTeam: %s\nAvg Score: %d, Min Score: %d, Total Time: %d\nResult: %s\n",
           team.teamName, team.avgScore, team.minScore, team.totalTime, team.finalResult);
}

void learnGrammar() {
    TeamPerformance team;
    int scores[MAX_MEM], times[MAX_MEM];

    inputBaseLine(&team, 100, 100, 10, 20, MAX_MEM);

    printf("Enter scores and times for %d members:\n", MAX_MEM);
    for (int i = 0; i < MAX_MEM; i++) {
        printf("Member %d score (max 100): ", i+1);
        scanf("%d", &scores[i]);
        printf("Member %d time (minutes): ", i+1);
        scanf("%d", &times[i]);
    }
    getchar();

    evalTeamPerformance(&team, scores, times, MAX_MEM);

    printf("\nTeam: %s\nAvg Score: %d, Min Score: %d, Total Time: %d\nResult: %s\n",
           team.teamName, team.avgScore, team.minScore, team.totalTime, team.finalResult);
}

int main() {
    int choice;
    while(1) {
        printf("\nMenu:\n1. Hangul Learning\n2. Grammar Learning\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) learnHangul();
        else if (choice == 2) learnGrammar();
        else break;
    }
    return 0;
}
