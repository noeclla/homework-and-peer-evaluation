#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 50
#define SUBJECTS 5

typedef struct {
    char name[20];
    char nickname[20];
    int age;
    char gender;
    int grade; 
} Student;

typedef struct {
    char nickname[20];
    float weights[SUBJECTS]; 
    float scores[SUBJECTS];
    float avgScore;
} LearningInfo;

Student students[MAX];
LearningInfo learnings[MAX];
int studentCount = 0;

float round1(float num) {
    return roundf(num * 10) / 10;
}

void inputCommonLearningInfo() {
    const char *subjects[SUBJECTS] = {"English", "Math", "Korean", "Social", "Science"};
    printf("Enter student info and grades. Enter 'end' for name to stop.\n");
    while(studentCount < MAX) {
        printf("Name: ");
        scanf("%19s", students[studentCount].name);
        if(strcmp(students[studentCount].name, "end") == 0) break;

        printf("Nickname: ");
        scanf("%19s", students[studentCount].nickname);

        printf("Age: ");
        scanf("%d", &students[studentCount].age);
        students[studentCount].grade = students[studentCount].age - 6; 

        printf("Gender (M/F): ");
        scanf(" %c", &students[studentCount].gender);

        printf("Enter weights for subjects (sum=1.0):\n");
        float totalWeight = 0;
        for(int i=0; i<SUBJECTS; i++) {
            printf("%s weight: ", subjects[i]);
            scanf("%f", &learnings[studentCount].weights[i]);
            totalWeight += learnings[studentCount].weights[i];
        }
        if(fabs(totalWeight - 1.0) > 0.01) {
            printf("Weights must sum to 1.0. Try again.\n");
            continue;
        }

        strcpy(learnings[studentCount].nickname, students[studentCount].nickname);

        float totalScore = 0;
        for(int i=0; i<SUBJECTS; i++) {
            printf("%s score: ", subjects[i]);
            scanf("%f", &learnings[studentCount].scores[i]);
            totalScore += learnings[studentCount].scores[i] * learnings[studentCount].weights[i];
        }

        learnings[studentCount].avgScore = round1(totalScore);

        studentCount++;
    }
}

void printStudentInfo() {
    printf("\nStudents (by nickname desc):\n");
    for(int i=0; i<studentCount-1; i++) {
        for(int j=i+1; j<studentCount; j++) {
            if(strcmp(students[i].nickname, students[j].nickname) < 0) {
                Student tmp = students[i];
                students[i] = students[j];
                students[j] = tmp;
                LearningInfo tmpL = learnings[i];
                learnings[i] = learnings[j];
                learnings[j] = tmpL;
            }
        }
    }
    for(int i=0; i<studentCount; i++) {
        printf("%s %s Age:%d Gender:%c Grade:%d\n", students[i].name, students[i].nickname, students[i].age, students[i].gender, students[i].grade);
    }
}

void printAscendAvgGrade() {
    printf("\nStudents by average score ascending:\n");
    for(int i=0; i<studentCount-1; i++) {
        for(int j=i+1; j<studentCount; j++) {
            if(learnings[i].avgScore > learnings[j].avgScore) {
                LearningInfo tmp = learnings[i];
                learnings[i] = learnings[j];
                learnings[j] = tmp;
                Student tmpS = students[i];
                students[i] = students[j];
                students[j] = tmpS;
            }
        }
    }
    for(int i=0; i<studentCount; i++) {
        printf("%s %s Average: %.1f\n", students[i].name, students[i].nickname, learnings[i].avgScore);
    }
}

void printDescendHighWeightGrade() {
    printf("\nSubject with highest weight per student and scores descending:\n");
    for(int i=0; i<studentCount; i++) {
        int maxIdx = 0;
        for(int j=1; j<SUBJECTS; j++) {
            if(learnings[i].weights[j] > learnings[i].weights[maxIdx]) maxIdx = j;
        }
        printf("%s %s Subject idx %d Score %.1f Weight %.2f\n",
            students[i].name, students[i].nickname, maxIdx, learnings[i].scores[maxIdx], learnings[i].weights[maxIdx]);
    }
}

void printExcludeMaxMinGrade(int subjectIdx) {
    printf("\nScores excluding max and min for subject %d:\n", subjectIdx);
    float arr[MAX];
    for(int i=0; i<studentCount; i++) {
        arr[i] = learnings[i].scores[subjectIdx];
    }
    float max = arr[0], min = arr[0];
    for(int i=1; i<studentCount; i++) {
        if(arr[i] > max) max = arr[i];
        if(arr[i] < min) min = arr[i];
    }
    for(int i=0; i<studentCount; i++) {
        if(arr[i] != max && arr[i] != min) {
            printf("%s %s %.1f\n", students[i].name, students[i].nickname, arr[i]);
        }
    }
}

void obfuscateName(char *name) {
    for(int i=0; name[i] != '\0'; i++) {
        printf("%d ", (unsigned char)name[i]);
    }
    printf("\n");
}

void obfuscateNickname(char *nickname) {
    int len = strlen(nickname);
    for(int i=len-1; i>=0; i--) {
        putchar(nickname[i]);
    }
    printf("\n");
}

int main() {
    inputCommonLearningInfo();
    printStudentInfo();
    printAscendAvgGrade();
    printDescendHighWeightGrade();
    int subjectChoice;
    printf("Enter subject index (0-4) for exclude max/min: ");
    scanf("%d", &subjectChoice);
    printExcludeMaxMinGrade(subjectChoice);

    printf("\nObfuscated names and nicknames:\n");
    for(int i=0; i<studentCount; i++) {
        obfuscateName(students[i].name);
        obfuscateNickname(students[i].nickname);
    }
    return 0;
}
