#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define WIDTH 40
#define HEIGHT 20

typedef struct {
    int x, y;
} Point;

Point snake[100];
int snakeLength;
Point food;
int direction;
int gameOver;

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

void initializeGame();
void drawGame();
void generateFood();
void moveSnake();
int checkCollision();
void update();

int main() {
    initscr();              // Initialize ncurses
    noecho();               // Disable echoing of characters
    curs_set(FALSE);        // Hide the cursor
    keypad(stdscr, TRUE);   // Enable arrow keys
    nodelay(stdscr, TRUE);  // Make getch() non-blocking

    initializeGame();

    while (!gameOver) {
        int ch = getch();
        if (ch == KEY_UP && direction != DOWN) direction = UP;
        if (ch == KEY_DOWN && direction != UP) direction = DOWN;
        if (ch == KEY_LEFT && direction != RIGHT) direction = LEFT;
        if (ch == KEY_RIGHT && direction != LEFT) direction = RIGHT;

        update();
        usleep(100000); // Sleep for 100ms
    }

    endwin(); // End ncurses mode
    printf("Game Over! Your final score is: %d\n", snakeLength - 1);
    return 0;
}

void initializeGame() {
    snakeLength = 1;
    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;
    direction = RIGHT;
    gameOver = 0;
    generateFood();
}

void drawGame() {
    clear();
    // Draw borders
    for (int i = 0; i < WIDTH + 2; i++) mvprintw(0, i, "#");
    for (int i = 0; i < HEIGHT; i++) {
        mvprintw(i + 1, 0, "#");
        for (int j = 0; j < WIDTH; j++) {
            if (i == food.y && j == food.x) {
                mvprintw(i + 1, j + 1, "F");
            } else {
                int isSnake = 0;
                for (int k = 0; k < snakeLength; k++) {
                    if (snake[k].x == j && snake[k].y == i) {
                        mvprintw(i + 1, j + 1, "O");
                        isSnake = 1;
                        break;
                    }
                }
                if (!isSnake) mvprintw(i + 1, j + 1, " ");
            }
        }
        mvprintw(i + 1, WIDTH + 1, "#");
    }
    for (int i = 0; i < WIDTH + 2; i++) mvprintw(HEIGHT + 1, i, "#");

    mvprintw(HEIGHT + 3, 0, "Score: %d", snakeLength - 1);
    refresh();
}

void generateFood() {
    food.x = rand() % WIDTH;
    food.y = rand() % HEIGHT;
}

void moveSnake() {
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    if (direction == UP) snake[0].y--;
    if (direction == DOWN) snake[0].y++;
    if (direction == LEFT) snake[0].x--;
    if (direction == RIGHT) snake[0].x++;

    if (snake[0].x == food.x && snake[0].y == food.y) {
        snakeLength++;
        generateFood();
    }
}

int checkCollision() {
    if (snake[0].x < 0 || snake[0].x >= WIDTH || snake[0].y < 0 || snake[0].y >= HEIGHT) return 1;

    for (int i = 1; i < snakeLength; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) return 1;
    }

    return 0;
}

void update() {
    moveSnake();
    if (checkCollision()) {
        gameOver = 1;
    } else {
        drawGame();
    }
}