#include <stdio.h>
#include <string.h>

#define MAX 7

typedef struct {
    char food[20];
    int cal;
} Food;

typedef struct {
    Food data[MAX];
    int top;
} Stack;

void init(Stack *s) {
    s->top = -1;
}

int isEmpty(Stack *s) {
    return s->top == -1;
}

int pop(Stack *s, Food *f) {
    if (isEmpty(s)) return 0;
    *f = s->data[s->top--];
    return 1;
}

void push(Stack *s, Food f) {
    if (s->top < MAX - 1) {
        s->data[++(s->top)] = f;
    }
}

void printStack(Stack *s) {
    int total = 0;
    printf("Remaining food records:\n");
    for(int i = 0; i <= s->top; i++) {
        printf("- %s (%d calories)\n", s->data[i].food, s->data[i].cal);
        total += s->data[i].cal;
    }
    printf("Total calories: %d\n", total);
}

int getExerciseCal(int choice) {
    int cal[] = {0, 200, 250, 300, 150, 250, 300, 100};
    if (choice < 1 || choice > 7) return 0;
    return cal[choice];
}

void manageMyDailyCalories(Stack *s) {
    if (isEmpty(s)) {
        printf("No food records found.\n");
        return;
    }
    printf("Choose exercise type (1-7): ");
    int ex; 
    scanf("%d", &ex);
    int exCal = getExerciseCal(ex);
    if (exCal == 0) {
        printf("Invalid exercise number.\n");
        return;
    }
    Food topFood = s->data[s->top];
    if (exCal >= topFood.cal) {
        Food removed;
        pop(s, &removed);
        printf("Removed %s (%d calories) from food records after exercise.\n", removed.food, removed.cal);
    } else {
        printf("Exercise insufficient: You need %d more calories burned.\n", topFood.cal - exCal);
    }
    printStack(s);
}

int main() {
    Stack myStack;
    init(&myStack);
    push(&myStack, (Food){"Rice", 300});
    push(&myStack, (Food){"Kimchi", 150});
    push(&myStack, (Food){"Chicken breast", 250});
    manageMyDailyCalories(&myStack);
    return 0;
}
