#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char keyword[8];  
    char message[512]; 
} EasterEgg;

void charToBinary(char c, char *binary) {
    for (int i = 7; i >= 0; i--) {
        binary[7 - i] = (c & (1 << i)) ? '1' : '0';
    }
    binary[8] = '\0';
}

int isEasterEgg(char *input) {
    return strcmp(input, "specter") == 0;
}

int isRightChar(char expectedBinary[], char userInput) {
    char userBinary[9];
    charToBinary(userInput, userBinary);
    return strcmp(expectedBinary, userBinary) == 0;
}

void find_easter_egg(EasterEgg *egg) {
    printf("<<Arthur's Easter Egg>>\n");

    char keyword[8] = "specter";
    char binary[8][9];
    for (int i = 0; i < 8; i++) {
        charToBinary(keyword[i], binary[i]);
    }

    for (int i = 7; i >= 0; i--) {
        printf("%s ", binary[i]);
    }
    printf("\n");

    char input[9];
    int correctCount = 0;
    for (int i = 7; i >= 0; i--) {
        printf("Enter the character for the binary: %s\n", binary[i]);
        do {
            printf("> ");
            scanf("%s", input);
        } while (!isRightChar(binary[i], input[0])); 
        correctCount++;
    }

    printf("Enter the hidden word: ");
    scanf("%s", input);
    if (isEasterEgg(input)) {
        printf("##Easter Egg Discovered!$$\n");
        printf("%s\n", egg->message); 
    } else {
        printf("Incorrect word. Returning to menu...\n");
    }
}

void shuffleKeywordAndConvertToBinary(char *keyword) {
    char shuffled[8];
    int oddIndex = 0, evenIndex = 0;
    for (int i = 0; i < 8; i++) {
        if (i % 2 == 0) {
            shuffled[evenIndex] = keyword[i];
            evenIndex++;
        } else {
            shuffled[oddIndex] = keyword[i];
            oddIndex++;
        }
    }

    char binary[8][9];
    for (int i = 0; i < 8; i++) {
        charToBinary(shuffled[i], binary[i]);
    }

    printf("Shuffled and reversed binary:\n");
    for (int i = 7; i >= 0; i--) {
        printf("%s ", binary[i]);
    }
    printf("\n");
}

void menu() {
    EasterEgg egg = {"specter", "I confess. After graduating from university, I was blinded by the arrogance of starting a startup and recklessly blocked my friends' paths. I painfully learned that when I am the only one convinced by my idea, it leads to disastrous results. The past Arthur was a ghost of dogmatism and stubbornness."};

    char choice;
    while (1) {
        printf("\n=== Main Menu ===\n");
        printf("[1] Enter 'Arthur' to find Easter Egg\n[0] Exit\n> ");
        scanf(" %c", &choice);

        if (choice == '1') {
            char input[10];
            printf("Enter 'Arthur' to trigger the Easter egg: ");
            scanf("%s", input);

            if (strcmp(input, "Arthur") == 0) {
                find_easter_egg(&egg);
            } else {
                printf("Incorrect input. Returning to menu...\n");
            }
        } else if (choice == '0') {
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }
}

int main() {
    menu();
    return 0;
}
