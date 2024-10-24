#include <stdio.h>
#include <stdlib.h>
#include <time.h>   
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
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

void setup() {
    gameover = 0;
    x = HEIGHT / 2;
    y = WIDTH / 2;
    fruitx = rand() % HEIGHT;
    fruity = rand() % WIDTH;
    score = 0;
    nTail = 0;
}

void draw() {
    system("cls || clear");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
                printf("#");
            else if (i == x && j == y)
                printf("0"); // Head of the snake
            else {
                int isTail = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == i && tailY[k] == j) {
                        printf("o"); // Body of the snake
                        isTail = 1;
                        break;
                    }
                }
                if (!isTail) {
                    if (i == fruitx && j == fruity)
                        printf("*"); // Fruit
                    else
                        printf(" ");
                }
            }
        }
        printf("\n");
    }

    printf("Score: %d\n", score);
    printf("Press X to quit the game\n");
}

void input() {
#ifdef _WIN32
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                flag = 1; // Left
                break;
            case 'd':
                flag = 2; // Right
                break;
            case 'w':
                flag = 3; // Up
                break;
            case 's':
                flag = 4; // Down
                break;
            case 'x':
                gameover = 1; // Quit
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
            flag = 1; // Left
            break;
        case 'd':
            flag = 2; // Right
            break;
        case 'w':
            flag = 3; // Up
            break;
        case 's':
            flag = 4; // Down
            break;
        case 'x':
            gameover = 1; // Quit
            break;
    }
#endif
}

// Function for game logic
void logic() {
    // Save current position to the tail
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

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
    if (x < 1 || x >= HEIGHT - 1 || y < 1 || y >= WIDTH - 1)
        gameover = 1;

    // Check if snake eats the fruit
    if (x == fruitx && y == fruity) {
        score += 10;
        fruitx = rand() % (HEIGHT - 2) + 1; // Generate fruit within bounds
        fruity = rand() % (WIDTH - 2) + 1; // Generate fruit within bounds
        nTail++; // Increase the length of the snake
    }

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameover = 1;
        }
    }
}

// Main function
int main() {
    srand(time(0)); // Seed for random number generation
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
