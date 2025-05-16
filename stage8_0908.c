#include <stdio.h>
#include <string.h>
#include <unistd.h>   
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "stage8.h"

#define MAX_MSG_LEN 43
#define MIN_MSG_LEN 24
#define NUM_SETS 5
#define DISPLAY_WIDTH 18
#define DISPLAY_TIME_SEC 24
#define TOP_SPEED_US 300000  
#define BOT_SPEED_US (TOP_SPEED_US / 2)

static char topMsgs[NUM_SETS][MAX_MSG_LEN];
static char botMsgs[NUM_SETS][MAX_MSG_LEN];

static void clearScreen() {
    printf("\033[2J\033[H");  
}

static void printBillboard(const char *top, const char *bot, int topPos, int botPos) {
    char topDisp[DISPLAY_WIDTH + 1];
    char botDisp[DISPLAY_WIDTH + 1];
    int topLen = strlen(top);
    int botLen = strlen(bot);

    for (int i = 0; i < DISPLAY_WIDTH; i++) {
        topDisp[i] = top[(topPos + i) % topLen];
        botDisp[i] = bot[(botPos + i) % botLen];
    }
    topDisp[DISPLAY_WIDTH] = '\0';
    botDisp[DISPLAY_WIDTH] = '\0';

    printf("%s\n%s\n", topDisp, botDisp);
}

void showMyAdMsg(void) {
    for (int i = 0; i < NUM_SETS; i++) {
      
        do {
            printf("Enter top line message %d (24-42 chars): ", i+1);
            if (!fgets(topMsgs[i], MAX_MSG_LEN, stdin)) exit(1);
            topMsgs[i][strcspn(topMsgs[i], "\n")] = 0;
        } while (strlen(topMsgs[i]) < MIN_MSG_LEN || strlen(topMsgs[i]) > 42);

        
        do {
            printf("Enter bottom line message %d (24-42 chars): ", i+1);
            if (!fgets(botMsgs[i], MAX_MSG_LEN, stdin)) exit(1);
            botMsgs[i][strcspn(botMsgs[i], "\n")] = 0;
        } while (strlen(botMsgs[i]) < MIN_MSG_LEN || strlen(botMsgs[i]) > 42);
    }

    for (int set = 0; set < NUM_SETS; set++) {
        int topLen = strlen(topMsgs[set]);
        int botLen = strlen(botMsgs[set]);
        int topPos = 0, botPos = 0;
        time_t start = time(NULL);

        while (difftime(time(NULL), start) < DISPLAY_TIME_SEC) {
            clearScreen();
            printBillboard(topMsgs[set], botMsgs[set], topPos, botPos);
            topPos = (topPos + 1) % topLen;
            botPos = (botPos + 2) % botLen;  
            usleep(TOP_SPEED_US);
        }
    }
}
