#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // Include for random seed
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>  // For Sleep() on Windows
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

#define HEIGHT 20
#define WIDTH 20

int gameover, score;
int x, y, fruitx, fruity, flag;
int tailX[100], tailY[100];
int nTail;

// Function to set up the game
void setup() {
    gameover = 0;
    x = HEIGHT / 2;
    y = WIDTH / 2;
    fruitx = rand() % HEIGHT;
    fruity = rand() % WIDTH;
    score = 0;
}

// Function to draw the game screen
void draw() {
    system("cls || clear");

    // Draw boundaries and snake
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
                printf("#");
            else if (i == x && j == y)
                printf("0");
            else if (i == fruitx && j == fruity)
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }

    // Display score
    printf("Score: %d\n", score);
    printf("Press X to quit the game\n");
}

// Function to handle user input
void input() {
#ifdef _WIN32
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                flag = 1;
                break;
            case 'd':
                flag = 2;
                break;
            case 'w':
                flag = 3;
                break;
            case 's':
                flag = 4;
                break;
            case 'x':
                gameover = 1;
                break;
        }
    }
#else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    switch (ch) {
        case 'a':
            flag = 1;
            break;
        case 'd':
            flag = 2;
            break;
        case 'w':
            flag = 3;
            break;
        case 's':
            flag = 4;
            break;
        case 'x':
            gameover = 1;
            break;
    }
#endif
}

// Function for game logic
void logic() {
    switch (flag) {
        case 1: // Left
            y--;
            break;
        case 2: // Right
            y++;
            break;
        case 3: // Up
            x--;
            break;
        case 4: // Down
            x++;
            break;
    }

    // Check for collision with boundaries
    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH)
        gameover = 1;

    // Check if snake eats the fruit
    if (x == fruitx && y == fruity) {
        score += 10;
        fruitx = rand() % HEIGHT;
        fruity = rand() % WIDTH;
    }
}

// Main function
int main() {
    setup();

    // Game loop
    while (!gameover) {
        draw();
        input();
        logic();
#ifdef _WIN32
        Sleep(100); // Sleep for 100 milliseconds (Windows)
#else
        usleep(100000); // Sleep for 100 milliseconds (Unix/Linux)
#endif
    }

    printf("Game Over! Final Score: %d\n", score);

    return 0;
}

