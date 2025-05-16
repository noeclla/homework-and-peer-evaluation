#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100

// Installment payment info per month
typedef struct MonthlyPayment {
    int monthNumber;        // 납부 월 수
    double balance;         // 할부 잔액
    double principalPart;   // 할부 원금 (한 달에 갚는 원금)
    double interestPart;    // 할부 수수료 (이자)
    double monthlyPayment;  // 월 납부액 = principalPart + interestPart
    struct MonthlyPayment *next;
} MonthlyPayment;

// Queue for monthly payments using linked list
typedef struct {
    MonthlyPayment *front;
    MonthlyPayment *rear;
    int size;
} PaymentQueue;

// Enqueue a monthly payment
void enqueue(PaymentQueue *q, MonthlyPayment *mp) {
    mp->next = NULL;
    if (!q->rear) {
        q->front = q->rear = mp;
    } else {
        q->rear->next = mp;
        q->rear = mp;
    }
    q->size++;
}

// Dequeue a monthly payment
MonthlyPayment* dequeue(PaymentQueue *q) {
    if (!q->front) return NULL;
    MonthlyPayment *tmp = q->front;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    q->size--;
    return tmp;
}

// Print one monthly payment info line
void printMonthlyPayment(const MonthlyPayment *mp) {
    printf("Month %d: Balance=%.2f, Principal=%.2f, Interest=%.2f, Monthly Payment=%.2f\n",
        mp->monthNumber, mp->balance, mp->principalPart, mp->interestPart, mp->monthlyPayment);
}

// Free entire queue
void freeQueue(PaymentQueue *q) {
    while (dequeue(q)) {}
}

// studyMoney main function
void studyMoney(void) {
    char productName[MAX_NAME_LEN];
    double principal;
    int months;
    double interestRate;

    printf("=== Installment Payment Simulation ===\n");

    printf("Enter product name: ");
    fgets(productName, MAX_NAME_LEN, stdin);
    size_t len = strlen(productName);
    if (len > 0 && productName[len-1] == '\n')
        productName[len-1] = '\0';

    printf("Enter principal amount (원금): ");
    if (scanf("%lf", &principal) != 1 || principal <= 0) {
        printf("Invalid principal amount.\n");
        while(getchar() != '\n'); // flush
        return;
    }

    printf("Enter installment months (할부 개월): ");
    if (scanf("%d", &months) != 1 || months <= 0) {
        printf("Invalid months.\n");
        while(getchar() != '\n'); // flush
        return;
    }

    printf("Enter interest rate (할부 수수료율 %% per month, e.g. 1.5): ");
    if (scanf("%lf", &interestRate) != 1 || interestRate < 0) {
        printf("Invalid interest rate.\n");
        while(getchar() != '\n'); // flush
        return;
    }
    while(getchar() != '\n'); // flush

    printf("\nProduct: %s\nPrincipal: %.2f\nMonths: %d\nInterest Rate: %.2f%% per month\n\n",
        productName, principal, months, interestRate);

    double totalInterest = principal * (interestRate / 100.0) * months;
    double totalPayable = principal + totalInterest;
    double monthlyPrincipal = principal / months;

    PaymentQueue paymentQueue = {NULL, NULL, 0};

    double remainingBalance = totalPayable;

    for (int i = 1; i <= months; i++) {
        MonthlyPayment *mp = (MonthlyPayment *)malloc(sizeof(MonthlyPayment));
        if (!mp) {
            printf("Memory allocation failed.\n");
            freeQueue(&paymentQueue);
            return;
        }
        mp->monthNumber = i;

        // Interest for this month calculated on remaining principal (not entire totalPayable)
        double interestForMonth = (principal - monthlyPrincipal * (i - 1)) * (interestRate / 100.0);

        // monthly payment: fixed principal + interest for this month
        mp->principalPart = monthlyPrincipal;
        mp->interestPart = interestForMonth;
        mp->monthlyPayment = mp->principalPart + mp->interestPart;

        remainingBalance -= mp->monthlyPayment;
        if (remainingBalance < 0) remainingBalance = 0;

        mp->balance = remainingBalance;

        enqueue(&paymentQueue, mp);
    }

    printf("Monthly Payment Schedule:\n");
    MonthlyPayment *tmp = paymentQueue.front;
    while (tmp) {
        printMonthlyPayment(tmp);
        tmp = tmp->next;
    }
    printf("\n");

    // Payment simulation - dequeue one monthly payment at a time
    char command[10];
    while (paymentQueue.size > 0) {
        printf("Enter 'pay' to make monthly payment, 'quit' to exit: ");
        if (fgets(command, sizeof(command), stdin) == NULL)
            break;
        // Remove trailing newline
        len = strlen(command);
        if (len > 0 && command[len-1] == '\n')
            command[len-1] = '\0';

        if (strcmp(command, "pay") == 0) {
            MonthlyPayment *paid = dequeue(&paymentQueue);
            printf("Paid month %d payment: %.2f\n", paid->monthNumber, paid->monthlyPayment);
            free(paid);
            printf("Remaining payment schedule:\n");
            tmp = paymentQueue.front;
            if (!tmp) {
                printf("All payments completed.\n");
            } else {
                while (tmp) {
                    printMonthlyPayment(tmp);
                    tmp = tmp->next;
                }
            }
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Invalid command.\n");
        }
    }

    freeQueue(&paymentQueue);
    printf("Simulation ended.\n");
}

int main(void) {
    studyMoney();
    return 0;
}
