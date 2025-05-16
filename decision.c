#include <stdio.h>
#include <math.h>

#define CATEGORIES 9
#define MAX_MEMBERS 5

enum Category {
    Physical, Knowledge, SelfMgmt, Teamwork,
    Korean, Vocal, Dance, Visual, Acting
};

const char *category_names[CATEGORIES] = {
    "Physical Strength", "Knowledge", "Self-Management", "Teamwork",
    "Korean Language", "Vocal", "Dance", "Visual", "Acting"
};

int main() {
    int scores[MAX_MEMBERS][CATEGORIES];
    int min_score, team_total = 0;
    int members, pass_score;

    printf("Number of members: ");
    scanf("%d", &members);

    printf("Minimum individual score: ");
    scanf("%d", &min_score);

    printf("Team average passing score: ");
    scanf("%d", &pass_score);

    for (int m = 0; m < members; m++) {
        printf("\nMember %d:\n", m + 1);
        for (int c = 0; c < CATEGORIES; c++) {
            int s;
            do {
                printf("%s: ", category_names[c]);
                scanf("%d", &s);
            } while (s < 0 || s > 100);
            scores[m][c] = s;
            if (s < min_score) {
                printf("\nMinimum score requirement: %d\n", min_score);
                printf("Team must retake %s.\n", category_names[c]);
                return 0;
            }
        }
    }

    for (int c = 0; c < CATEGORIES; c++) {
        int sum = 0;
        for (int m = 0; m < members; m++) {
            sum += scores[m][c];
        }
        team_total += sum / members; // truncate decimals
    }

    int avg = team_total / CATEGORIES;

    printf("\nTeam average score: %d\n", avg);

    switch (avg / 5) {
        case 19:
        case 20:
            printf("Congratulations! Your stage debut is confirmed!\n");
            break;
        case 18:
            printf("The Debut Certification Team will hold a final evaluation meeting to make a decision.\n");
            break;
        case 16:
        case 17: {
            int cat_avg[CATEGORIES];
            for (int i = 0; i < CATEGORIES; i++) {
                int sum = 0;
                for (int j = 0; j < members; j++)
                    sum += scores[j][i];
                cat_avg[i] = sum / members;
            }

            // Find 3 lowest scores
            for (int i = 0; i < CATEGORIES - 1; i++) {
                for (int j = i + 1; j < CATEGORIES; j++) {
                    if (cat_avg[i] > cat_avg[j]) {
                        int tmp = cat_avg[i];
                        cat_avg[i] = cat_avg[j];
                        cat_avg[j] = tmp;

                        const char *tmpn = category_names[i];
                        category_names[i] = category_names[j];
                        category_names[j] = tmpn;
                    }
                }
            }

            printf("Retake the three lowest-scoring categories (%s, %s, %s) after 30 days.\n",
                   category_names[0], category_names[1], category_names[2]);
            break;
        }
        default:
            printf("Insufficient performance. Full retest required.\n");
    }

    return 0;
}
