#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MEDIA 10
#define MAX_TRAINEES 6

const char *media[MAX_MEDIA] = {
    "BBC", "JTBC", "KBS", "MBC", "MTN",
    "SBS", "YTN", "OhMyNews", "SamProTV", "Hankyoreh"
};

void swap(char *a, char *b) {
    char t = *a; *a = *b; *b = t;
}

void interactionMedia() {
    int n, h;
    char trainees[MAX_TRAINEES];  // 'A', 'B', ...
    const char *chosenMedia[MAX_TRAINEES];

    srand((unsigned)time(NULL));

    do {
        printf("Enter number of trainees (4-6): ");
        scanf("%d", &n);
    } while(n < 4 || n > MAX_TRAINEES);

    // Assign trainee IDs
    for(int i=0; i<n; i++) trainees[i] = 'A' + i;

    // Random media assignment for bottom
    for(int i=0; i<n; i++) {
        chosenMedia[i] = media[rand() % MAX_MEDIA];
    }

    printf("Enter number of horizontal lines: ");
    scanf("%d", &h);

    printf("Enter horizontal swaps as pairs of letters (e.g. AB):\n");
    for(int i=0; i<h; i++) {
        char start, end;
        scanf(" %c%c", &start, &end);
        // Find indices
        int si = start - 'A';
        int ei = end - 'A';
        if(si >=0 && si < n-1 && ei == si+1) {
            swap(&trainees[si], &trainees[ei]);
        } else {
            printf("Invalid swap, ignoring.\n");
        }
    }

    // Print final matching
    printf("Final trainee to media matching:\n");
    for(int i=0; i<n; i++) {
        int pos = trainees[i] - 'A';
        printf("%c -> %s\n", trainees[i], chosenMedia[pos]);
    }
}

int main() {
    interactionMedia();
    return 0;
}
