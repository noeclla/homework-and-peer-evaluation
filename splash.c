#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep function

#define WIDTH 101 // Width of the splash screen

void print_splash_screen(const char *name, const char *date) {
    system("clear"); // Clear the screen (Linux/macOS), use "cls" for Windows
    
    // Print upper border
    for (int i = 0; i < WIDTH; i++) printf("+");
    printf("\n");
    
    // Print left-angled triangle and splash screen content
    for (int i = 1; i <= 5; i++) {
        for (int j = 0; j < i; j++) printf("*");
        
        if (i == 2) printf("%*s[Magrathea ver 0.1]%*s", (WIDTH - 30) / 2, "", (WIDTH - 30) / 2, "");
        else if (i == 3) printf("%*sMagrathea, where a shining planet is created in a wasteland with no grass,%*s", (WIDTH - 89) / 2, "", (WIDTH - 89) / 2, "");
        else if (i == 4) printf("%*sa place where unseen potential is discovered and gems are polished by the hands of experts,%*s", (WIDTH - 99) / 2, "", (WIDTH - 99) / 2, "");
        else if (i == 5) printf("%*sWelcome to Magrathea.%*s", (WIDTH - 28) / 2, "", (WIDTH - 28) / 2, "");
        
        for (int j = 0; j < 6 - i; j++) printf("*");
        printf("\n");
    }
    
    // Print lower border
    for (int i = 0; i < WIDTH; i++) printf("+");
    printf("\n");
    
    // Print user and date info
    printf("[User]: %s%*s[Execution Time]: %s\n", name, WIDTH - 35 - (int)strlen(name), "", date);
    for (int i = 0; i < WIDTH; i++) printf("=");
    printf("\n");
}

int main() {
    char name[100], date[11];
    
    // Input section
    printf("[Please enter the current date in the \"yyyy-mm-dd\" format]: ");
    scanf("%10s", date);
    printf("[Please enter your name]: ");
    scanf("%99s", name);
    
    // Process confirmation
    printf("**The input has been processed successfully.**\n");
    sleep(3); // Wait for 3 seconds
    
    // Display the splash screen
    print_splash_screen(name, date);
    
    return 0;
}
