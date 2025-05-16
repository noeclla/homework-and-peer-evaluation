#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARTISTS 100
#define MAX_STR_LEN 100

typedef struct {
    char name[MAX_STR_LEN];
    char nickname[MAX_STR_LEN];
    char dob[MAX_STR_LEN];
    char gender[MAX_STR_LEN];
    char education[MAX_STR_LEN];
    char phone[MAX_STR_LEN];
    char email[MAX_STR_LEN];
    char nationality[MAX_STR_LEN];
    char height[MAX_STR_LEN];
    char weight[MAX_STR_LEN];
    char blood_type[MAX_STR_LEN];
    char allergies[MAX_STR_LEN];
    char sns[MAX_STR_LEN];
} Artist;

Artist artists[MAX_ARTISTS];
int artist_count = 0;

typedef enum {XOR=1, OR, AND, BIT_SHIFT} EncryptAlgo;

void encrypt(char *data, const char *key, EncryptAlgo algo) {
    size_t key_len = strlen(key);
    for (size_t i = 0; i < strlen(data); i++) {
        switch (algo) {
            case XOR: data[i] ^= key[i % key_len]; break;
            case OR:  data[i] |= key[i % key_len]; break;
            case AND: data[i] &= key[i % key_len]; break;
            case BIT_SHIFT:
                data[i] = (data[i] << 1) | (data[i] >> 7);
                data[i] ^= key[i % key_len];
                break;
        }
    }
}

void decrypt(char *data, const char *key, EncryptAlgo algo) {
    size_t key_len = strlen(key);
    for (size_t i = 0; i < strlen(data); i++) {
        switch (algo) {
            case XOR: data[i] ^= key[i % key_len]; break;
            case OR:  data[i] &= ~(key[i % key_len]); break; // approximate
            case AND: data[i] |= ~(key[i % key_len]); break; // approximate
            case BIT_SHIFT:
                data[i] ^= key[i % key_len];
                data[i] = (data[i] >> 1) | (data[i] << 7);
                break;
        }
    }
}

void saveToFile(const char *filename, const char *key, EncryptAlgo algo) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) { perror("File open failed"); return; }
    for (int i = 0; i < artist_count; i++) {
        Artist temp = artists[i];
        encrypt(temp.email, key, algo);
        encrypt(temp.phone, key, algo);
        encrypt(temp.allergies, key, algo);
        fwrite(&temp, sizeof(Artist), 1, fp);
    }
    fclose(fp);
}

void loadFromFile(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return;
    artist_count = 0;
    while (fread(&artists[artist_count], sizeof(Artist), 1, fp) == 1 && artist_count < MAX_ARTISTS) {
        artist_count++;
    }
    fclose(fp);
}

void protectMyData(void) {
    char key[50];
    int algo_choice = 1;
    const char filename[] = "artists.dat";

    printf("Select encryption algorithm:\n1. XOR (default)\n2. OR\n3. AND\n4. Bit Shift\nChoice: ");
    if (scanf("%d", &algo_choice) != 1 || algo_choice < 1 || algo_choice > 4) algo_choice = 1;
    while (getchar() != '\n'); // clear stdin

    EncryptAlgo algo = (EncryptAlgo)algo_choice;

    printf("Enter encryption key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;

    printf("Enter number of artists: ");
    if (scanf("%d", &artist_count) != 1 || artist_count <= 0 || artist_count > MAX_ARTISTS) {
        printf("Invalid number.\n");
        artist_count = 0;
        return;
    }
    while (getchar() != '\n');

    for (int i = 0; i < artist_count; i++) {
        Artist *a = &artists[i];
        printf("Artist %d:\n", i+1);
#define INPUT(field) do { printf(#field ": "); fgets(a->field, MAX_STR_LEN, stdin); a->field[strcspn(a->field, "\n")] = 0; } while(0)
        INPUT(name);
        INPUT(nickname);
        INPUT(dob);
        INPUT(gender);
        INPUT(education);
        INPUT(phone);
        INPUT(email);
        INPUT(nationality);
        INPUT(height);
        INPUT(weight);
        INPUT(blood_type);
        INPUT(allergies);
        INPUT(sns);
#undef INPUT
    }

    saveToFile(filename, key, algo);
    loadFromFile(filename);

    printf("\nArtists loaded.\nList of Nicknames:\n");
    for (int i = 0; i < artist_count; i++) {
        printf("%d. %s\n", i+1, artists[i].nickname);
    }

    int sel;
    printf("Select an artist by number to view details: ");
    if (scanf("%d", &sel) != 1 || sel < 1 || sel > artist_count) {
        printf("Invalid selection.\n");
        return;
    }
    while (getchar() != '\n');

    Artist *s = &artists[sel-1];
    printf("\nName: %s\nNickname: %s\nDOB: %s\nGender: %s\nEducation: %s\n", s->name, s->nickname, s->dob, s->gender, s->education);
    printf("Phone (encrypted): %s\nEmail (encrypted): %s\nNationality: %s\nHeight: %s\nWeight: %s\nBlood Type: %s\nAllergies (encrypted): %s\nSNS: %s\n",
           s->phone, s->email, s->nationality, s->height, s->weight, s->blood_type, s->allergies, s->sns);

    printf("Do you want to decrypt sensitive info? (y/n): ");
    char dec;
    if (scanf(" %c", &dec) == 1 && (dec == 'y' || dec == 'Y')) {
        char key2[50];
        int algo2;
        printf("Re-enter encryption algorithm (1:XOR,2:OR,3:AND,4:Bit Shift): ");
        if (scanf("%d", &algo2) != 1 || algo2 < 1 || algo2 > 4) algo2 = 1;
        while (getchar() != '\n');

        EncryptAlgo algo_d = (EncryptAlgo)algo2;

        printf("Re-enter key: ");
        fgets(key2, sizeof(key2), stdin);
        key2[strcspn(key2, "\n")] = 0;

        char phone_d[MAX_STR_LEN], email_d[MAX_STR_LEN], allergies_d[MAX_STR_LEN];
        strcpy(phone_d, s->phone);
        strcpy(email_d, s->email);
        strcpy(allergies_d, s->allergies);

        decrypt(phone_d, key2, algo_d);
        decrypt(email_d, key2, algo_d);
        decrypt(allergies_d, key2, algo_d);

        printf("\nDecrypted Info:\nPhone: %s\nEmail: %s\nAllergies: %s\n", phone_d, email_d, allergies_d);
    }
}

int main(void) {
    protectMyData();
    return 0;
}
