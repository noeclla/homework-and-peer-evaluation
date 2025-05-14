#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_TRAINING 8

const char *main_menu[] = {
    "I. Audition Management", "II. Training", "III. Debut"
};

const char *train_menu[] = {
    "1. Physical Strength & Knowledge", "2. Self-Management & Teamwork",
    "3. Language & Pronunciation", "4. Vocal", "5. Dance",
    "6. Visual & Image", "7. Acting & Stage Performance", "8. Fan Communication"
};

char results[MAX_TRAINING] = {0};

void show_menu(const char **menu, int count, int is_training) {
    for (int i = 0; i < count; i++) {
        printf("%d. %s", i + 1, menu[i]);
        if (is_training && results[i]) printf(" [%c]", results[i]);
        printf("\n");
    }
    printf("Enter choice (0/Q/q to exit): ");
}

void eval(int idx) {
    if (results[idx] == 'P') return;
    if ((idx == 1 && results[0] != 'P') || (idx > 1 && (results[0] != 'P' || results[1] != 'P'))) {
        printf("Complete prior stages first.\n");
        return;
    }
    char input[4];
    printf("Enter result? (Y/N): "); fgets(input, 4, stdin);
    if (toupper(input[0]) != 'Y') return;
    printf("Pass certification? (Y/N): "); fgets(input, 4, stdin);
    results[idx] = (toupper(input[0]) == 'Y') ? 'P' : 'F';
}

void training_menu_loop() {
    char input[4];
    while (1) {
        printf("\n=== Training Menu ===\n");
        show_menu(train_menu, MAX_TRAINING, 1);
        fgets(input, 4, stdin);
        if (input[0] == '\n' || input[0] == '0' || toupper(input[0]) == 'Q') break;
        int ch = atoi(input);
        if (ch >= 1 && ch <= MAX_TRAINING) eval(ch - 1);
    }
}

int main() {
    char input[4];
    while (1) {
        printf("\n=== Main Menu ===\n");
        show_menu(main_menu, 3, 0);
        fgets(input, 4, stdin);
        if (input[0] == '\n' || input[0] == '0' || toupper(input[0]) == 'Q') break;
        switch (atoi(input)) {
            case 1: printf("Audition Management selected.\n"); break;
            case 2: training_menu_loop(); break;
            case 3: printf("Debut selected.\n"); break;
            default: printf("Invalid choice.\n"); break;
        }
    }
    return 0;
}

