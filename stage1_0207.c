#include <stdio.h>
#include <string.h>

#define MAX 50

typedef struct {
    char date[11];
    char weather[10];
    char timeOfDay[15]; // PreMeal, PostMeal, BeforeShow
    char exercise[20];
    int duration; // minutes
    int calories;
} Workout;

Workout workouts[MAX];
int count = 0;

void setMyWorkoutRoutine() {
    printf("Enter workout data (max %d). Enter 'end' for date to stop.\n", MAX);
    while(count < MAX) {
        printf("Date (YYYY-MM-DD): ");
        scanf("%10s", workouts[count].date);
        if(strcmp(workouts[count].date, "end") == 0) break;
        printf("Weather: ");
        scanf("%9s", workouts[count].weather);
        printf("Time of day (PreMeal/PostMeal/BeforeShow): ");
        scanf("%14s", workouts[count].timeOfDay);
        printf("Exercise type: ");
        scanf("%19s", workouts[count].exercise);
        printf("Duration (minutes): ");
        scanf("%d", &workouts[count].duration);
        printf("Calories burned: ");
        scanf("%d", &workouts[count].calories);
        count++;
    }
}

void printAllWorkouts() {
    printf("\nAll workouts:\n");
    for(int i = 0; i < count; i++) {
        printf("%s %s %s %s %dmin %d cal\n",
               workouts[i].date, workouts[i].weather, workouts[i].timeOfDay,
               workouts[i].exercise, workouts[i].duration, workouts[i].calories);
    }
}

void statsMyWorkoutRoutine() {
    char queryDate[11], queryTime[15];
    printf("Enter date for exercise info (YYYY-MM-DD): ");
    scanf("%10s", queryDate);
    printf("Enter time of day (PreMeal/PostMeal/BeforeShow): ");
    scanf("%14s", queryTime);

    printf("\nExercises on %s during %s:\n", queryDate, queryTime);
    for(int i = 0; i < count; i++) {
        if(strcmp(workouts[i].date, queryDate) == 0 && strcmp(workouts[i].timeOfDay, queryTime) == 0) {
            printf("- %s, %d minutes\n", workouts[i].exercise, workouts[i].duration);
        }
    }

    // Total calories per date
    int calByDateCount = 0;
    char dates[MAX][11];
    int caloriesByDate[MAX];
    for(int i=0; i<count; i++) {
        int found = 0;
        for(int j=0; j<calByDateCount; j++) {
            if(strcmp(dates[j], workouts[i].date) == 0) {
                caloriesByDate[j] += workouts[i].calories;
                found = 1;
                break;
            }
        }
        if(!found) {
            strcpy(dates[calByDateCount], workouts[i].date);
            caloriesByDate[calByDateCount] = workouts[i].calories;
            calByDateCount++;
        }
    }
    printf("\nTotal calories burned per day:\n");
    for(int i=0; i<calByDateCount; i++) {
        printf("%s: %d cal\n", dates[i], caloriesByDate[i]);
    }

    // Most performed exercise
    char exercises[MAX][20];
    int exCounts[MAX] = {0};
    int exCountTotal = 0;
    for(int i=0; i<count; i++) {
        int found = 0;
        for(int j=0; j<exCountTotal; j++) {
            if(strcmp(exercises[j], workouts[i].exercise) == 0) {
                exCounts[j]++;
                found = 1;
                break;
            }
        }
        if(!found) {
            strcpy(exercises[exCountTotal], workouts[i].exercise);
            exCounts[exCountTotal] = 1;
            exCountTotal++;
        }
    }
    int maxExIdx = 0;
    for(int i=1; i<exCountTotal; i++) {
        if(exCounts[i] > exCounts[maxExIdx]) maxExIdx = i;
    }
    printf("\nMost performed exercise: %s (%d times)\n", exercises[maxExIdx], exCounts[maxExIdx]);

    // Day with longest total workout time
    int timeByDate[MAX] = {0};
    for(int i=0; i<count; i++) {
        for(int j=0; j<calByDateCount; j++) {
            if(strcmp(workouts[i].date, dates[j]) == 0) {
                timeByDate[j] += workouts[i].duration;
                break;
            }
        }
    }
    int maxTimeIdx = 0;
    for(int i=1; i<calByDateCount; i++) {
        if(timeByDate[i] > timeByDate[maxTimeIdx]) maxTimeIdx = i;
    }
    printf("Day with longest workout time: %s (%d minutes)\n", dates[maxTimeIdx], timeByDate[maxTimeIdx]);

    // Day most performed a specific exercise
    char queryExercise[20];
    printf("Enter exercise to find most active day: ");
    scanf("%19s", queryExercise);

    int exDayCounts[MAX] = {0};
    for(int i=0; i<count; i++) {
        if(strcmp(workouts[i].exercise, queryExercise) == 0) {
            for(int j=0; j<calByDateCount; j++) {
                if(strcmp(workouts[i].date, dates[j]) == 0) {
                    exDayCounts[j]++;
                    break;
                }
            }
        }
    }
    int maxExDayIdx = 0;
    for(int i=1; i<calByDateCount; i++) {
        if(exDayCounts[i] > exDayCounts[maxExDayIdx]) maxExDayIdx = i;
    }
    printf("Day with most %s workouts: %s (%d times)\n", queryExercise, dates[maxExDayIdx], exDayCounts[maxExDayIdx]);

    // Bonus: find timeOfDay with highest average calorie burn
    int calByTime[3] = {0}; // PreMeal, PostMeal, BeforeShow
    int countByTime[3] = {0};
    for(int i=0; i<count; i++) {
        if(strcmp(workouts[i].timeOfDay, "PreMeal") == 0) {
            calByTime[0] += workouts[i].calories; countByTime[0]++;
        } else if(strcmp(workouts[i].timeOfDay, "PostMeal") == 0) {
            calByTime[1] += workouts[i].calories; countByTime[1]++;
        } else if(strcmp(workouts[i].timeOfDay, "BeforeShow") == 0) {
            calByTime[2] += workouts[i].calories; countByTime[2]++;
        }
    }
    double avgCal[3] = {0};
    for(int i=0; i<3; i++) {
        if(countByTime[i] > 0) avgCal[i] = (double)calByTime[i] / countByTime[i];
    }
    int bestTime = 0;
    for(int i=1; i<3; i++) {
        if(avgCal[i] > avgCal[bestTime]) bestTime = i;
    }
    const char* times[3] = {"PreMeal", "PostMeal", "BeforeShow"};
    printf("Highest average calorie burn time: %s (%.2f cal)\n", times[bestTime], avgCal[bestTime]);
}

int main() {
    setMyWorkoutRoutine();
    printAllWorkouts();
    statsMyWorkoutRoutine();
    return 0;
}
