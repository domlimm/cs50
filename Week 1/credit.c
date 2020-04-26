#include <stdio.h>
#include <cs50.h>

// From Problem Set 1:
// American Express uses 15-digit numbers,
// MasterCard uses 16-digit numbers,
// and Visa uses 13- and 16-digit numbers

// All American Express numbers start with 34 or 37;
// most MasterCard numbers start with 51, 52, 53, 54, or 55
// and all Visa numbers start with 4.

int find_length(long long number);
bool check_amex(int digits[], int length);
bool check_master(int digits[], int length);
bool check_visa(int digits[], int length);
bool checksum(int digits[], int length);

int main(void) {
    long long cc = get_long_long("Please enter your Credit Card number:\n");

    int length = find_length(cc);

    int digitsArray[length];

    for (int i = 0; i < length; i++) {
        digitsArray[i] = cc % 10;
        cc /= 10;
    }

    if (length == 15) {
        // Amex
        if (check_amex(digitsArray, length) && checksum(digitsArray, length)) {
            printf("AMEX\n");
        }
        else {
            printf("INVALID\n");
        }
    }
    else if (length == 16) {
        // Mastercard || Visa
        if (check_master(digitsArray, length) && checksum(digitsArray, length)) {
            printf("MASTERCARD\n");
        }
        else if (digitsArray[length - 1] == 4 && checksum(digitsArray, length)) {
            printf("VISA\n");
        }
        else {
            printf("INVALID\n");
        }
    }
    else if (length == 13) {
        // Visa
        if (digitsArray[length - 1] == 4 && checksum(digitsArray, length)) {
            printf("VISA\n");
        }
        else {
            printf("INVALID\n");
        }
    }
    else {
        printf("INVALID\n");
    }
}

int find_length(long long number) {
    int nDigits = 1;

    if (number > 0) {
        while (number > 9) {
            nDigits++;
            number /= 10;
        }
    }

    return nDigits;
}

bool checksum(int digits[], int length) {
    int first_sum = 0, first_product = 0, second_sum = 0;

    for (int i = 1; i < length; i += 2) {
        first_product = digits[i] * 2;
        first_sum += (first_product % 10) + (first_product / 10);
    }

    for (int i = 0; i < length; i += 2) {
        second_sum += digits[i];
    }

    return (first_sum + second_sum) % 10 == 0;
}

bool check_amex(int digits[], int length) {
    if (digits[length - 1] == 3 && (digits[length - 2] == 4 || digits[length - 2] == 7)) {
        return true;
    }
    return false;
}

bool check_master(int digits[], int length) {
    int check_second[5] = {1, 2, 3, 4, 5};
    bool second = false;

    for (int i = 0; i < 5; i++) {
        if (check_second[i] == digits[length - 2]) {
            second = true;
            break;
        }
    }

    if (digits[length - 1] == 5) {
        if (second) {
            return true;
        }
    }

    return false;
}
