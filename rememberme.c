#include <stdio.h>
#include <string.h>

#define MAX_MESSAGES 10
#define MAX_LENGTH 200
#define NUM_FAILED 2

// Immutable memorial info: name, id, audition, email
const char memorial01_arr[NUM_FAILED][MAX_LENGTH] = {
    "Carlos Gomez,100006,milliways,carlos@example.com",
    "Soojin Lee,100005,milliways,soojin@example.com"
};

// Rolling paper messages array
char rollingpp01_arr[MAX_MESSAGES][MAX_LENGTH];
int message_count = 0;

void print_failed_candidates() {
    printf("=== Unsuccessful Candidates ===\n");
    for (int i = 0; i < NUM_FAILED; i++) {
        char name[50];
        sscanf(memorial01_arr[i], "%[^,]", name);
        printf("%d. %s\n", i + 1, name);
    }
}

void write_message() {
    int choice;
    char sender[50], message[100];

    print_failed_candidates();
    printf("Select candidate number to write message to: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > NUM_FAILED) {
        printf("Invalid selection.\n");
        return;
    }

    getchar(); // clear newline
    printf("Your Name: ");
    fgets(sender, sizeof(sender), stdin);
    sender[strcspn(sender, "\n")] = '\0';

    printf("Write message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    char json_message[MAX_LENGTH];
    snprintf(json_message, sizeof(json_message), "\"%s\": \"%s\"", sender, message);
    strcpy(rollingpp01_arr[message_count++], json_message);

    printf("Message saved!\n");
}

void show_messages() {
    int choice;
    print_failed_candidates();
    printf("Select candidate number to view messages: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > NUM_FAILED) {
        printf("Invalid selection.\n");
        return;
    }

    printf("\nMessages for %s:\n", memorial01_arr[choice - 1]);
    for (int i = 0; i < message_count; i++) {
        printf("- %s\n", rollingpp01_arr[i]);
    }

    char send;
    printf("Send all messages to this candidate? (Y/N): ");
    scanf(" %c", &send);

    if (send == 'Y' || send == 'y') {
        char email[50];
        sscanf(memorial01_arr[choice - 1], "%*[^,],%*[^,],%*[^,],%s", email);
        printf("Sending to: %s...\n", email);
        printf("Email has been sent successfully!\n");
    } else {
        printf("Canceled.\n");
    }
}

int main() {
    int option;

    while (1) {
        printf("\n==== Rolling Paper System ====\n");
        printf("1. Write a Message\n");
        printf("2. View & Send Messages\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1: write_message(); break;
            case 2: show_messages(); break;
            case 3: return 0;
            default: printf("Invalid option.\n");
        }
    }
}
