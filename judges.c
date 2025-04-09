#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_JUDGES 100
#define MAX_INPUT 512
#define FIELD_COUNT 7

char judges_array[MAX_JUDGES][MAX_INPUT];

int count_fields(const char *input) {
    int count = 0;
    for (int i = 0; input[i]; i++) {
        if (input[i] == ',') {
            count++;
        }
    }
    return count + 1; // Number of commas + 1 = number of fields
}

void trim(char *str) {
    int i = 0, j = 0;
    char temp[MAX_INPUT];
    while (str[i]) {
        if (str[i] != '\n' && str[i] != '\r') {
            temp[j++] = str[i];
        }
        i++;
    }
    temp[j] = '\0';
    strcpy(str, temp);
}

void display_judges(const char *project, int judge_count) {
    printf("\n####################################\n");
    printf("#        Display Judge Data        #\n");
    printf("####################################\n");

    for (int i = 0; i < judge_count; i++) {
        printf("[Judge %d]\n", i + 1);
        char temp[MAX_INPUT];
        strcpy(temp, judges_array[i]);

        char *token = strtok(temp, ",");
        const char *labels[FIELD_COUNT] = {
            "Name", "Gender", "Affiliation", "Title", "Expertise", "Email", "Phone"
        };

        int index = 0;
        while (token != NULL && index < FIELD_COUNT) {
            while (*token == ' ') token++; // trim leading space
            printf("%s: %s\n", labels[index++], token);
            token = strtok(NULL, ",");
        }
        printf("-----------------------------------\n");
    }
}

int main() {
    char project[MAX_INPUT];
    int total_judges = 0, selected_members = 0;

    printf("####################################\n");
    printf("#      Judge List Data Entry      #\n");
    printf("####################################\n");

    printf("Participating Project: ");
    fgets(project, sizeof(project), stdin);
    trim(project);

    printf("Total Number of Judges: ");
    scanf("%d", &total_judges);
    getchar(); // Clear newline

    printf("Number of Selected Members: ");
    scanf("%d", &selected_members);
    getchar(); // Clear newline

    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("Starting to input information for %d judges.\n", total_judges);
    printf("++++++++++++++++++++++++++++++++++++\n");

    int i = 0;
    while (i < total_judges) {
        printf("Judge %d: ", i + 1);
        fgets(judges_array[i], MAX_INPUT, stdin);
        trim(judges_array[i]);

        if (count_fields(judges_array[i]) != FIELD_COUNT) {
            printf("The input items are incorrect. Please enter them again.\n");
            continue;
        }
        i++;
    }

    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("Judge information entry is complete.\n");
    printf("++++++++++++++++++++++++++++++++++++\n");

    printf("\"%s\" Should we check the judge information? ", project);
    char confirm = getchar();

    if (confirm == 'Y') {
        display_judges(project, total_judges);
    } else {
        printf("Exiting without displaying judge information.\n");
    }

    return 0;
}
