#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 30
#define MAX_FEEDBACK 51
#define MAX_MIDI_NOTES 12
#define MIN_MIDI_NOTES 5
#define MAX_TRAINEES 20

typedef struct {
    char name[MAX_NAME];
    int vocalRange;  
    int midiNotes[MAX_MIDI_NOTES];
    int numNotes;
    double vocalStability;
    char feedback[MAX_FEEDBACK];
} Trainee;

Trainee trainees[MAX_TRAINEES];
int traineeCount = 0;

int existsInArray(int *arr, int size, int val) {
    for (int i = 0; i < size; i++)
        if (arr[i] == val) return 1;
    return 0;
}

void generateUniqueMIDINotes(int *arr, int count, int maxRange) {
    int i = 0;
    while (i < count) {
        int val = rand() % (maxRange + 1);  
        if (!existsInArray(arr, i, val)) {
            arr[i++] = val;
        }
    }
}

double calcStability(int *arr, int count) {
    if (count <= 1) return 0.0;
    double sumDiff = 0;
    for (int i = 1; i < count; i++) {
        int diff = arr[i] - arr[i-1];
        if (diff < 0) diff = -diff;
        sumDiff += diff;
    }
    return sumDiff / (count - 1);
}

void provideFeedback(char *feedback) {
    printf("Enter feedback (max 50 chars): ");
    fgets(feedback, MAX_FEEDBACK, stdin);
    feedback[strcspn(feedback, "\n")] = 0;
}

void measure(Trainee *t) {
    printf("Enter vocal range (0-127): ");
    scanf("%d", &t->vocalRange);
    while (t->vocalRange < 0 || t->vocalRange > 127) {
        printf("Invalid. Enter vocal range (0-127): ");
        scanf("%d", &t->vocalRange);
    }

    printf("Enter number of MIDI notes (%d-%d): ", MIN_MIDI_NOTES, MAX_MIDI_NOTES);
    scanf("%d", &t->numNotes);
    while (t->numNotes < MIN_MIDI_NOTES || t->numNotes > MAX_MIDI_NOTES || t->numNotes > t->vocalRange + 1) {
        printf("Invalid. Enter number of MIDI notes (%d-%d and <= vocal range + 1): ", MIN_MIDI_NOTES, MAX_MIDI_NOTES);
        scanf("%d", &t->numNotes);
    }
    getchar(); 

    generateUniqueMIDINotes(t->midiNotes, t->numNotes, t->vocalRange);

    for (int i = 0; i < t->numNotes - 1; i++) {
        for (int j = 0; j < t->numNotes - 1 - i; j++) {
            if (t->midiNotes[j] > t->midiNotes[j+1]) {
                int tmp = t->midiNotes[j];
                t->midiNotes[j] = t->midiNotes[j+1];
                t->midiNotes[j+1] = tmp;
            }
        }
    }

    t->vocalStability = calcStability(t->midiNotes, t->numNotes);
}

int compareStability(const void *a, const void *b) {
    double diff = ((Trainee*)a)->vocalStability - ((Trainee*)b)->vocalStability;
    if (diff < 0) return -1;
    else if (diff > 0) return 1;
    else return 0;
}

void printVocalInfo() {
    if (traineeCount == 0) {
        printf("No trainees evaluated yet.\n");
        return;
    }
    qsort(trainees, traineeCount, sizeof(Trainee), compareStability);
    printf("\n--- Vocal Evaluation Results ---\n");
    for (int i = 0; i < traineeCount; i++) {
        printf("Name: %s\n", trainees[i].name);
        printf("Stability: %.2f\n", trainees[i].vocalStability);
        printf("Feedback: %s\n\n", trainees[i].feedback);
    }
}

void evalVocal() {
    if (traineeCount >= MAX_TRAINEES) {
        printf("Max trainees reached.\n");
        return;
    }

    printf("Enter trainee name: ");
    fgets(trainees[traineeCount].name, MAX_NAME, stdin);
    trainees[traineeCount].name[strcspn(trainees[traineeCount].name, "\n")] = 0;

    measure(&trainees[traineeCount]);

    provideFeedback(trainees[traineeCount].feedback);

    traineeCount++;
}

int main() {
    srand((unsigned int)time(NULL));
    int choice;

    while (1) {
        printf("\nMenu:\n1. Vocal Evaluation\n2. Show Results\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        getchar(); 

        if (choice == 1) evalVocal();
        else if (choice == 2) printVocalInfo();
        else break;
    }
    return 0;
}
