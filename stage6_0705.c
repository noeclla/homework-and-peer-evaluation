#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 256

typedef enum { MALE, FEMALE } Gender;

typedef struct {
    Gender gender;
    char *name;
    char *feature;
} HairStyle;

typedef struct {
    HairStyle *style;
    int externalScore, internalScore;
    double suitability;
} HairScore;

// Load hairstyles from file (format: Gender:Name:Feature)
int loadHairStyles(const char *filename, HairStyle **styles) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("Open file"); return 0; }

    int cap = 8, count = 0;
    HairStyle *arr = malloc(cap * sizeof(HairStyle));
    if (!arr) { fclose(f); return 0; }

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (!line[0]) continue;

        char *g = strtok(line, ":");
        char *n = strtok(NULL, ":");
        char *feat = strtok(NULL, ":");
        if (!g || !n || !feat) continue;

        if (count == cap) {
            cap *= 2;
            HairStyle *tmp = realloc(arr, cap * sizeof(HairStyle));
            if (!tmp) break;
            arr = tmp;
        }

        Gender gen = (strcmp(g, "Male") == 0) ? MALE : FEMALE;
        arr[count].gender = gen;
        arr[count].name = strdup(n);
        arr[count].feature = strdup(feat);
        count++;
    }
    fclose(f);
    *styles = arr;
    return count;
}

// Get number between 5 and 10 from user
int getCount() {
    int n;
    do {
        printf("Enter number of hairstyles to test (5-10): ");
        if (scanf("%d", &n) != 1) { while(getchar()!='\n'); continue; }
    } while (n < 5 || n > 10);
    while(getchar()!='\n');
    return n;
}

// Calculate suitability score
double calcSuitability(int ext, int intl) {
    return ext * 0.7 + intl * 0.3;
}

// Quick sort by suitability ascending
void quickSort(HairScore *arr, int left, int right) {
    if (left >= right) return;
    double pivot = arr[right].suitability;
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (arr[j].suitability <= pivot) {
            i++;
            HairScore tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        }
    }
    HairScore tmp = arr[i+1]; arr[i+1] = arr[right]; arr[right] = tmp;
    quickSort(arr, left, i);
    quickSort(arr, i+2, right);
}

void testHairStyles() {
    HairStyle *all = NULL;
    int total = loadHairStyles("hair_style.txt", &all);
    if (total == 0) { printf("No hairstyles loaded.\n"); return; }

    int n = getCount();

    // Ask user gender
    char c;
    Gender userGender;
    do {
        printf("Enter gender (M/F): ");
        c = getchar();
        while(getchar()!='\n');
        if (c == 'M' || c == 'm') userGender = MALE;
        else if (c == 'F' || c == 'f') userGender = FEMALE;
        else continue;
        break;
    } while(1);

    // Filter by gender
    HairStyle *filtered = malloc(total * sizeof(HairStyle));
    int fcount = 0;
    for (int i = 0; i < total; i++)
        if (all[i].gender == userGender)
            filtered[fcount++] = all[i];

    if (fcount < n) {
        printf("Not enough hairstyles for your gender (%d available).\n", fcount);
        free(filtered);
        goto cleanup;
    }

    srand(time(NULL));

    // Pick n unique random hairstyles
    int *chosen = malloc(n * sizeof(int));
    for (int i = 0; i < n; ) {
        int r = rand() % fcount;
        int unique = 1;
        for (int j = 0; j < i; j++) if (chosen[j] == r) unique = 0;
        if (unique) chosen[i++] = r;
    }

    HairScore *scores = malloc(n * sizeof(HairScore));
    printf("\nHair Style Tests:\n");
    for (int i = 0; i < n; i++) {
        HairStyle *hs = &filtered[chosen[i]];
        int ext = rand() % 51 + 50;   // 50-100
        int intl = rand() % 51 + 50;  // 50-100
        double suit = calcSuitability(ext, intl);
        scores[i].style = hs;
        scores[i].externalScore = ext;
        scores[i].internalScore = intl;
        scores[i].suitability = suit;

        printf("%d. %s (Ext: %d, Int: %d) Suitability: %.2f\n",
            i+1, hs->name, ext, intl, suit);
    }

    quickSort(scores, 0, n-1);

    printf("\nSorted by suitability (low to high):\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s Suitability: %.2f\n", i+1, scores[i].style->name, scores[i].suitability);
    }

    free(scores);
    free(chosen);
    free(filtered);

cleanup:
    for (int i = 0; i < total; i++) {
        free(all[i].name);
        free(all[i].feature);
    }
    free(all);
}
