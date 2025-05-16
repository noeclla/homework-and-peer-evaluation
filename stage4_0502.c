#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 30
#define MAX_MEMBERS 20

typedef enum { MALE, FEMALE, UNKNOWN } Gender;

typedef struct {
    char name[MAX_NAME];
    Gender gender;
} MilliwaysMember;
typedef struct {
    char name[MAX_NAME];
    Gender gender;
    double frequency;   
    double amplitude;  
    double jitter;      
    int G, R, B, A, S;  
} MyVoiceData;

typedef struct {
    double minFreq, maxFreq;
    double minAmp, maxAmp;
    double minJit, maxJit;
} GenderRange;

const GenderRange ranges[2] = {
    {85.0, 180.0, 2.5, 3.6, 0.2, 0.6},  
    {165.0, 255.0, 3.5, 4.7, 0.3, 1.0}   
};

MilliwaysMember members[MAX_MEMBERS] = {
    {"Arthur", MALE},
    {"Young-jin", MALE},
    {"Hye-kyung", FEMALE},
    {"Soo-min", FEMALE}
};
int memberCount = 4;

Gender parseGender(char *g) {
    if (strcmp(g, "M") == 0 || strcmp(g, "m") == 0) return MALE;
    if (strcmp(g, "F") == 0 || strcmp(g, "f") == 0) return FEMALE;
    return UNKNOWN;
}

MilliwaysMember* findMember(const char *name) {
    for (int i = 0; i < memberCount; i++) {
        if (strcmp(members[i].name, name) == 0)
            return &members[i];
    }
    return NULL;
}

double randDouble(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

void getMyVoiceFromHeaven(MyVoiceData *voice) {
    GenderRange r = ranges[voice->gender];
    voice->frequency = randDouble(r.minFreq, r.maxFreq);
    voice->amplitude = randDouble(r.minAmp, r.maxAmp);
    voice->jitter = randDouble(r.minJit, r.maxJit);

    voice->G = randInt(0, 3);
    voice->R = randInt(0, 3);
    voice->B = randInt(0, 3);
    voice->A = randInt(0, 3);
    voice->S = randInt(0, 3);
}

int guessDouble(const char *label, double target, double tolerance) {
    int tries = 3;
    double guess;

    printf("Guess your %s (3 tries):\n", label);
    while (tries--) {
        printf("Your guess: ");
        if (scanf("%lf", &guess) != 1) {
            while(getchar() != '\n');
            printf("Invalid input.\n");
            tries++;
            continue;
        }
        if (guess > target + tolerance)
            printf("Too high.\n");
        else if (guess < target - tolerance)
            printf("Too low.\n");
        else
            return 1; 
    }
    return 0;
}

int guessIntGRBAS(const char *label, int target) {
    int tries = 3;
    int guess;

    printf("Guess your %s (0-3, 3 tries):\n", label);
    while (tries--) {
        printf("Your guess: ");
        if (scanf("%d", &guess) != 1 || guess < 0 || guess > 3) {
            while(getchar() != '\n');
            printf("Invalid input.\n");
            tries++;
            continue;
        }
        if (guess > target)
            printf("Too high.\n");
        else if (guess < target)
            printf("Too low.\n");
        else
            return 1; 
    }
    return 0;
}

void findMyVoice() {
    char nickname[MAX_NAME];
    MilliwaysMember *member = NULL;
    MyVoiceData voice;

    printf("Enter your Milliways nickname: ");
    fgets(nickname, sizeof(nickname), stdin);
    nickname[strcspn(nickname, "\n")] = 0;

    member = findMember(nickname);
    if (!member) {
        printf("Member not found.\n");
        return;
    }

    voice.gender = member->gender;
    strcpy(voice.name, member->name);

    getMyVoiceFromHeaven(&voice);

    printf("\nLet's find your voice, %s!\n", voice.name);

    double freqTol = 2.0;       
    double ampTol = 0.1;        
    double jitTol = 0.05;       

    int success = 1;
    if (!guessDouble("frequency (Hz)", voice.frequency, freqTol)) {
        printf("Frequency not found. Try again later.\n");
        success = 0;
    }
    if (!guessDouble("amplitude (dB)", voice.amplitude, ampTol)) {
        printf("Amplitude not found. Try again later.\n");
        success = 0;
    }
    if (!guessDouble("voice pulse (jitter %)", voice.jitter, jitTol)) {
        printf("Voice pulse not found. Try again later.\n");
        success = 0;
    }
    printf("\nNow, let's guess your GRBAS voice tone.\n");
    success &= guessIntGRBAS("G (Grade)", voice.G);
    success &= guessIntGRBAS("R (Roughness)", voice.R);
    success &= guessIntGRBAS("B (Breathiness)", voice.B);
    success &= guessIntGRBAS("A (Asthenia)", voice.A);
    success &= guessIntGRBAS("S (Strain)", voice.S);

    if (success)
        printf("\nCongratulations, %s! You found your voice!\n", voice.name);
    else
        printf("\nKeep trying, %s! Your voice is unique and worth discovering.\n", voice.name);
}

int main() {
    srand((unsigned int)time(NULL));

    int choice;
    while (1) {
        printf("\nMenu:\n1. Find My Voice\n2. Exit\nChoose: ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        getchar(); 

        if (choice == 1)
            findMyVoice();
        else
            break;
    }
    return 0;
}
