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

int isFull(Stack *s) {
    return s->top == MAX - 1;
}

int isEmpty(Stack *s) {
    return s->top == -1;
}

int push(Stack *s, Food f) {
    if (isFull(s)) return 0;
    s->data[++(s->top)] = f;
    return 1;
}

int validCal(int c) {
    int allowed[] = {100,150,200,250,300};
    for(int i=0; i<5; i++)
        if(c == allowed[i]) return 1;
    return 0;
}

int setMyDailyFood() {
    Stack s;
    init(&s);
    int total = 0;
    while(s.top < MAX -1) {
        Food f;
        printf("음식명 입력: ");
        scanf("%19s", f.food);
        printf("칼로리 입력(100,150,200,250,300): ");
        scanf("%d", &f.cal);
        if (!validCal(f.cal)) {
            printf("잘못된 칼로리입니다.\n");
            continue;
        }
        if (total + f.cal > 1500) {
            printf("1500칼로리 초과, 종료합니다.\n");
            break;
        }
        if(!push(&s, f)) {
            printf("스택이 가득 찼습니다.\n");
            break;
        }
        total += f.cal;
    }
    printf("총 섭취 칼로리: %d\n", total);
    return total;
}

int main() {
    setMyDailyFood();
    return 0;
}
