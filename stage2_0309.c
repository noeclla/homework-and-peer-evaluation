#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MEMBERS 4
#define MAX_NAME_LEN 20
#define MAX_NICK_LEN 20
#define MAX_GIFT_LEN 50
#define ENCODED_LEN 100

typedef struct {
    char name[MAX_NAME_LEN];
    char nickname[MAX_NICK_LEN];
} Member;

typedef struct {
    char giverNick[MAX_NICK_LEN];
    char gift[MAX_GIFT_LEN];
} GiftInfo;

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64_encode(const unsigned char *input, int len, char *output) {
    int i = 0, j = 0;
    unsigned char a3[3];
    unsigned char a4[4];

    while (len--) {
        a3[i++] = *(input++);
        if (i == 3) {
            a4[0] = (a3[0] & 0xfc) >> 2;
            a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
            a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
            a4[3] = a3[2] & 0x3f;
            for (i = 0; i < 4; i++) output[j++] = base64_table[a4[i]];
            i = 0;
        }
    }
    if (i) {
        int k;
        for (k = i; k < 3; k++) a3[k] = '\0';
        a4[0] = (a3[0] & 0xfc) >> 2;
        a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
        a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
        a4[3] = a3[2] & 0x3f;
        for (k = 0; k < i + 1; k++) output[j++] = base64_table[a4[k]];
        while (i++ < 3) output[j++] = '=';
    }
    output[j] = '\0';
}

int base64_char_val(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1; // Invalid char
}

void base64_decode(const char *input, unsigned char *output, int *out_len) {
    int i = 0, j = 0;
    unsigned char a3[3];
    unsigned char a4[4];
    int len = 0;

    while (input[i]) {
        int k;
        for (k = 0; k < 4 && input[i]; k++) {
            if (input[i] == '=') {
                a4[k] = 0;
            } else {
                a4[k] = base64_char_val(input[i]);
                if (a4[k] == -1) {
                    output[len] = '\0';
                    *out_len = len;
                    return;
                }
            }
            i++;
        }

        a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
        a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
        a3[2] = ((a4[2] & 0x3) << 6) + a4[3];

        for (k = 0; k < 3; k++) {
            if (input[i - 1 - (3 - k)] == '=') break;
            output[len++] = a3[k];
        }
    }
    *out_len = len;
}

int strcmp_no_lib(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++;
        b++;
    }
    return *a - *b;
}

int find_member_by_nick(Member members[], int size, const char *nick) {
    int i;
    for (i = 0; i < size; i++) {
        if (strcmp_no_lib(members[i].nickname, nick) == 0)
            return i;
    }
    return -1;
}

void playManito() {
    Member members[MAX_MEMBERS] = {
        {"Arthur", "Art"},
        {"Sophia", "Sophie"},
        {"Liam", "Lee"},
        {"Emma", "Em"}
    };

    char giverNick[MAX_NICK_LEN];
    char gift[MAX_GIFT_LEN];
    char encoded[ENCODED_LEN];
    unsigned char decoded[ENCODED_LEN];
    int decoded_len;

    printf("Enter your nickname (Manito): ");
    scanf("%19s", giverNick);

    int giverIndex = find_member_by_nick(members, MAX_MEMBERS, giverNick);
    if (giverIndex == -1) {
        printf("Nickname not found.\n");
        return;
    }

    printf("Enter your gift: ");
    scanf(" %49[^\n]", gift);

    char combined[MAX_NICK_LEN + MAX_GIFT_LEN + 2];
    int ci = 0, gi = 0;
    // Copy giverNick to combined
    while (giverNick[gi] != '\0') combined[ci++] = giverNick[gi++];
    combined[ci++] = '|'; // separator
    gi = 0;
    while (gift[gi] != '\0') combined[ci++] = gift[gi++];
    combined[ci] = '\0';

    base64_encode((unsigned char *)combined, ci, encoded);

    srand((unsigned int)time(NULL));
    int receiverIndex;
    do {
        receiverIndex = rand() % MAX_MEMBERS;
    } while (receiverIndex == giverIndex);

    char guess[MAX_NICK_LEN];
    while (1) {
        printf("Guess the receiver's nickname: ");
        scanf("%19s", guess);
        if (strcmp_no_lib(guess, members[receiverIndex].nickname) == 0) {
            printf("Correct! Here is the encoded gift info:\n%s\n", encoded);
            break;
        } else {
            printf("Wrong guess. Try again.\n");
        }
    }

    printf("Enter the encoded info you received: ");
    scanf("%99s", encoded);

    base64_decode(encoded, decoded, &decoded_len);
    decoded[decoded_len] = '\0';  // Null terminate

    char decodedGiver[MAX_NICK_LEN];
    char decodedGift[MAX_GIFT_LEN];
    int i = 0, j = 0;

    while (decoded[i] != '|' && decoded[i] != '\0' && i < MAX_NICK_LEN - 1) {
        decodedGiver[i] = decoded[i];
        i++;
    }
    decodedGiver[i] = '\0';
    if (decoded[i] == '|') i++; // skip separator

    j = 0;
    while (decoded[i] != '\0' && j < MAX_GIFT_LEN - 1) {
        decodedGift[j++] = decoded[i++];
    }
    decodedGift[j] = '\0';

    printf("Decoded info:\nManito: %s\nGift: %s\n", decodedGiver, decodedGift);
}

int main() {
    printf("=== Manito Game ===\n");
    playManito();
    return 0;
}
