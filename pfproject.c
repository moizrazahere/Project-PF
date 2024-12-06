include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 20
#define MAX_LENGTH 100

int gameOver, score;
int x, y, fruitX, fruitY, tailX[MAX_LENGTH], tailY[MAX_LENGTH];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

// Function to initialize game state
void Setup()
{
    gameOver = 0;  // Game is not over at start
    dir = STOP;    // Snake is not moving initially
    x = WIDTH / 2; // Snake starts at the middle of the board
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;  // Random fruit position
    fruitY = rand() % HEIGHT;
    score = 0;  // Score starts at 0
    nTail = 0;  // No tail segments at the beginning
}

// Function to load previous score from file
int LoadScore()
{
    FILE *file = fopen("score.txt", "r");
    int prevScore = 0;
    if (file != NULL) {
        fscanf(file, "Score: %d", &prevScore);  // Read previous score from file
        fclose(file);
    }
    return prevScore;
}

// Function to save the current score to a file
void SaveScore()
{
    FILE *file = fopen("score.txt", "w");
    if (file != NULL) {
        fprintf(file, "Score: %d\n", score);  // Write the current score to file
        fclose(file);
    }
}

// Function to draw the game board
void Draw()
{
    int i, j;
    system("cls");  // Clears the console screen
    for (i = 0; i < WIDTH + 2; i++)  // Top border
        printf("#");
    printf("\n");

    for (i = 0; i < HEIGHT; i++)  // Loop through each row
    {
        for (j = 0; j < WIDTH; j++)  // Loop through each column
        {
            if (j == 0)
                printf("#");

            if (i == y && j == x)
                printf("O");  // Draw snake's head
            else if (i == fruitY && j == fruitX)
                printf("F");  // Draw fruit
            else
            {
            int k;
                int isprint = 0;
                for (k = 0; k < nTail; k++)  // Draw tail
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        printf("o");
                        isprint = 1;
                    }
                }
                if (!isprint)
                    printf(" ");  // Empty space
            }

            if (j == WIDTH - 1)
                printf("#");  // Right border
        }
        printf("\n");
    }

    for (i = 0; i < WIDTH + 2; i++)  // Bottom border
        printf("#");
    printf("\n");

    printf("Score: %d\n", score);  // Display score
}

// Function to handle player input
void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':  // Left
            dir = LEFT;
            break;
        case 'd':  // Right
            dir = RIGHT;
            break;
        case 'w':  // Up
            dir = UP;
            break;
        case 's':  // Down
            dir = DOWN;
            break;
        case 'x':  // Exit the game
            gameOver = 1;
            break;
        }
    }
}

// Function to update game logic
void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    int i;
    for (i = 1; i < nTail; i++)  // Move the tail
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Move snake's head
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    // Check for collisions with walls
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0) {
        gameOver = 1;
    }

    // Check for collisions with tail
    for ( i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;

    // If snake eats fruit
    if (x == fruitX && y == fruitY)
    {
        score += 10;  // Increase score
        fruitX = rand() % WIDTH;  // Random new fruit position
        fruitY = rand() % HEIGHT;
        nTail++;  // Increase the length of the tail
    }
}

int main()
{
    int prevScore = LoadScore();  // Load previous score from file
    printf("Previous Score: %d\n", prevScore);

    Setup();  // Initialize game state
    while (!gameOver)
    {
        Draw();  // Display the game board
        Input();  // Handle user input
        Logic();  // Update the game logic
        Sleep(10);  // Pause to slow down the game loop (Windows)
    }

    SaveScore();  // Save the current score to file
    printf("Game Over! Your score: %d\n", score);
    return 0;
}
