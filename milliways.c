#include <stdio.h>#include <stdio.h>
#include <stdlib.h>

#define TOTAL 6
#define FINAL 4

const char *candidate01_arr[TOTAL] = {
    "{\"name\":\"Jiyeon Park\",\"id\":100001,\"pass\":true}",
    "{\"name\":\"Ethan Smith\",\"id\":100002,\"pass\":true}",
    "{\"name\":\"Helena Silva\",\"id\":100003,\"pass\":true}",
    "{\"name\":\"Liam Wilson\",\"id\":100004,\"pass\":true}",
    "{\"name\":\"Soojin Lee\",\"id\":100005,\"pass\":false}",
    "{\"name\":\"Carlos Gomez\",\"id\":100006,\"pass\":false}"
};

char milliways01_arr[FINAL][400]; // Final member records

float heights[FINAL] = {1.68f, 1.78f, 1.63f, 1.75f}; // in meters
char blood[FINAL][3] = {"A", "O", "B", "AB"};
char allergy[FINAL][30] = {"dairy", "peanut", "fish", "shellfish"};
char hobby[FINAL][50] = {
    "dancing, composing", 
    "singing, gym training", 
    "singing, painting", 
    "dancing, producing"
};
char sns[FINAL][50] = {
    "Instagram - @Ariel_Jiyeon", 
    "Twitter - @Simba_Ethan", 
    "Instagram - @Belle_Helena", 
    "Instagram - @Aladdin_Liam"
};
char nickname[FINAL][20] = {"Ariel", "Simba", "Belle", "Aladdin"};
char education[FINAL][20] = {"dropout_HS1", "dropout_MS3", "MS_grad", "dropout_MS2"};

void classify_bmi(float bmi) {
    int category = 0;
    if (bmi < 18.5f) category = 0;
    else if (bmi < 23.0f) category = 1;
    else if (bmi < 25.0f) category = 2;
    else category = 3;

    switch (category) {
        case 0: printf("BMI: %.1f (Underweight)\n", bmi); break;
        case 1: printf("BMI: %.1f (Normal)\n", bmi); break;
        case 2: printf("BMI: %.1f (Overweight)\n", bmi); break;
        case 3: printf("BMI: %.1f (Obese)\n", bmi); break;
    }
}

int main() {
    float bmi_list[FINAL], weight;
    char name[FINAL][30] = {"Jiyeon Park", "Ethan Smith", "Helena Silva", "Liam Wilson"};

    for (int i = 0; i < FINAL; i++) {
        printf("\nInterviewing %s:\n", name[i]);
        printf("Enter BMI value: ");
        scanf("%f", &bmi_list[i]);
        weight = heights[i] * heights[i] * bmi_list[i];

        // Format data into one long string
        sprintf(milliways01_arr[i],
            "name: %s, nickname: %s, education: %s, height: %.2fm, weight: %.1fkg, blood: %s, allergy: %s, hobby: %s, sns: %s",
            name[i], nickname[i], education[i], heights[i], weight,
            blood[i], allergy[i], hobby[i], sns[i]
        );
    }

    printf("\n=== Milliways Final Member Profile ===\n");
    for (int i = 0; i < FINAL; i++) {
        printf("\n%s\n", milliways01_arr[i]);
        classify_bmi(bmi_list[i]);
    }

    return 0;
}
