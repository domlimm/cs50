#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int check_key(string key);
void substitute_to_cipher(string plaintext, string key);
int get_position(char alphabet);

int main(int argc, string argv[]) {
    if (argc != 2) {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (argc == 2) {
        string key = argv[1];
        int check = check_key(key);

        if (check == 1) {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        else if (check == 2) {
            printf("Key must contain only alphabets.\n");
            return 1;
        }
        else if (check == 3) {
            printf("Key must not contain duplicated characters.\n");
            return 1;
        }
        else {
            string plaintext = get_string("plaintext: ");
            substitute_to_cipher(plaintext, key);
        }
    }

    printf("\n");
    return 0;
}

int check_key(string key) {
    int keyLength = strlen(key);

    if (keyLength != 26) {
        return 1;
    }

    // Check for all alphabets
    for (int i = 0; i < keyLength; i++) {
        if (isalpha(key[i]) == 0) {
            return 2;
        }
    }

    char temp_key[keyLength], temp;

    strncpy(temp_key, key, keyLength);

    for (int i = 0; i < keyLength - 1; i++) { // Check current in str array, lth - 1 as last pos value will be shifted to last
      for (int j = i+1; j < keyLength; j++) { // Check next in str array
         if (temp_key[i] > temp_key[j]) {
            temp = temp_key[i];
            temp_key[i] = temp_key[j];
            temp_key[j] = temp;
         }
      }
    }

    for (int i = 0; i < keyLength; i++) {
        if (temp_key[i] == temp_key[i+1]) {
            return 3;
        }
    }

    return 0;
}

void substitute_to_cipher(string plaintext, string key) {
    int plaintext_length = strlen(plaintext), pos = 0;

    printf("ciphertext: ");

    for (int i = 0; i < plaintext_length; i++) {
        char character = plaintext[i];
        pos = get_position(character);

        if (pos != -1) {
            if (isalpha(character) > 0) {
                if (isupper(character) > 0) {
                    // printf("%c", (char) toupper(key[pos]));
                    char c = (char) toupper(key[pos]);
                    printf("%c", c);
                    continue;
                }
                else if (isupper(character) == 0) {
                    // printf("%c", (char) toupper(key[pos]));
                    char c = (char) tolower(key[pos]);
                    printf("%c", c);
                    continue;
                }
            }
        }
        else {
            printf("%c", character);
            continue;
        }
    }
}

int get_position(char alphabet) {
    string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", lower = "abcdefghijklmnopqrstuvwxyz";

    if (isupper(alphabet) > 0) {
        for (int j = 0, n = strlen(upper); j < n; j++) {
            if (alphabet == upper[j]) {
                return j;
            }
        }
    }
    else {
        for (int k = 0, o = strlen(lower); k < o; k++) {
            if (alphabet == lower[k]) {
                return k;
            }
        }
    }
    return -1;
}
