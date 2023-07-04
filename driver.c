#include <stdlib.h>  // for abs function

// Define the size of the display area
#define AREA_WIDTH 10
#define AREA_HEIGHT 10

// Define the size of the character bitmap
#define CHAR_WIDTH 5
#define CHAR_HEIGHT 7

// Define the character bitmap
// For simplicity, let's define two characters: space (ASCII 32) and 'A' (ASCII 65)
int ASCII_TABLE[128][CHAR_HEIGHT][CHAR_WIDTH] = {
    [32] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    },
    [65] = {
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1}
    }
    // Other character definitions go here...
};

// Delay function
void delay(unsigned int milliseconds) {
    unsigned int i, j;
    for (i = 0; i < milliseconds; i++) {
        for (j = 0; j < 5000; j++) {
            // Adjust loop iterations for desired delay length
            asm("nop"); // Insert no-operation instruction for fine-tuning delay
        }
    }
}

// Function to move the laser to a specific position
void sweepLaser(int x, int y) {
    writePortB(x);
    writePortC(y);
}

// Function to turn the laser on or off
void setLaserState(int state) {
    writePortA(state);
}

// Function to display a dot at the specified position
void displayDot(int x, int y) {
    sweepLaser(x, y);
    setLaserState(1);
    delay(10);  // Adjust the delay as needed
    setLaserState(0);
}

// Function to display a character
void displayCharacter(char character) {
    int bitmap[CHAR_HEIGHT][CHAR_WIDTH];
    for (int i = 0; i < CHAR_HEIGHT; i++) {
        for (int j = 0; j < CHAR_WIDTH; j++) {
            bitmap[i][j] = ASCII_TABLE[(int)character][i][j];
        }
    }
    for (int y = 0; y < CHAR_HEIGHT; y++) {
        for (int x = 0; x < CHAR_WIDTH; x++) {
            if (bitmap[y][x] == 1) {
                displayDot(x, y);
            }
        }
    }
}

// Function to display a string
void displayString(const char *str) {
    while (*str) {
        displayCharacter(*str++);
        delay(1000);  // Adjust the delay as needed
    }
}

int main() {
    // Display the string
    displayString("A A");

    return 0;
}
