#include <stdio.h>
#include <cs50.h>

void print_left(int height, int i);
void print_right(int height, int i);

int main(void)
{
    printf("Build your own Pyramid!\n");

    int height = 0;

    do {
        height = get_int("How tall do you want your pyramid to be? (1-8): \n");
    }
    while (height < 1 || height > 8);

    for (int i = height; i > 0; i--) { // Levels
        if (height == 1) {
            printf("#  #");
            printf("\n");
            return 0;
        }
        print_left(height, i);
        printf("  ");
        print_right(height, i);
        printf("\n");
    }
}

void print_left(int height, int i) {
    if (i != 1) {
        printf("%*c", i - 1, ' '); // Left spaces
    }

    for (int k = 0; k < height - i + 1; k++) { // Left bricks
        printf("#");
    }
}

void print_right(int height, int i) {
    for (int k = 0; k < height - i + 1; k++) { // Right bricks
        printf("#");
    }
}
