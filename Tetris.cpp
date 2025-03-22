#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 24;
const float BLOCK_SIZE = 25.0f;
const float BOARD_LEFT = 225;
const float BOARD_BOTTOM = 50;
int score = 0;
int currentX = 5, currentY = 0;
int currentPiece, nextPiece;
int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};
bool pieceActive = true;

int shapes[7][5][5] =
{
    {
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,1,0},
        {0,0,0,0,0}
    }, // L
    {
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,1,1,0,0},
        {0,0,0,0,0}
    }, // Reverse L
    {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,1,1,0,0},
        {0,0,1,1,0},
        {0,0,0,0,0}
    }, // Z
    {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,1,0},
        {0,1,1,0,0},
        {0,0,0,0,0}
    }, // Reverse Z
    {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,1,1,1,0},
        {0,0,0,0,0}
    }, // T
    {
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0}
    }, // I
    {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,1,0},
        {0,0,1,1,0},
        {0,0,0,0,0}
    }  // Square
};

// Draw the game board
void drawBoard() {
    glColor3f(1, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2i(225, 50); glVertex2i(475, 50);
    glVertex2i(475, 50); glVertex2i(475, 650);
    glVertex2i(475, 650); glVertex2i(225, 650);
    glVertex2i(225, 650); glVertex2i(225, 50);
    glEnd();

    // Draw grid lines
    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(1);
    glBegin(GL_LINES);
    for (int i = 1; i < 24; i++) {
        int gap = i * 25;
        glVertex2i(225, 50 + gap);
        glVertex2i(475, 50 + gap);
    }
    for (int i = 1; i < 10; i++) {
        int gap = i * 25;
        glVertex2i(225 + gap, 50);
        glVertex2i(225 + gap, 650);
    }
    glEnd();
    glFlush();
}

// Draw a single square (pink)
void square(float x, float y) {
    glColor3f(1.0f, 0.1f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + BLOCK_SIZE, y);
    glVertex2f(x + BLOCK_SIZE, y + BLOCK_SIZE);
    glVertex2f(x, y + BLOCK_SIZE);
    glEnd();
}

// Draw the current piece
void drawPiece(int pieceType, int boardX, int boardY) {
    if (pieceType < 1 || pieceType > 7) return;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if (shapes[pieceType - 1][row][col] == 1) {
                float x = BOARD_LEFT + (boardX + col - 2) * BLOCK_SIZE;
                float y = BOARD_BOTTOM + (BOARD_HEIGHT - (boardY + row - 2) - 1) * BLOCK_SIZE;
                square(x, y);
            }
        }
    }
}


// Check for collision
bool checkCollision(int pieceType, int newX, int newY) {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if (shapes[pieceType - 1][row][col] == 1) {
                int boardRow = newY + row - 2;
                int boardCol = newX + col - 2;

                // if colliding
                if (boardRow >= BOARD_HEIGHT || boardCol < 0 || boardCol >= BOARD_WIDTH || (boardRow >= 0 && board[boardRow][boardCol] == 1)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void clearLine() {
    for (int row = 0; row < BOARD_HEIGHT; row++) {
        bool fullRow = true;

        // Check if row is completely filled
        for (int col = 0; col < BOARD_WIDTH; col++) {
            if (board[row][col] == 0) {
                fullRow = false;
                break;
            }
        }
        if (fullRow) {
            score++;
            // Move all rows above down
            for (int r = row; r > 0; r--) {
                for (int c = 0; c < BOARD_WIDTH; c++) {
                    board[r][c] = board[r - 1][c];
                }
            }
            for (int c = 0; c < BOARD_WIDTH; c++) {
                board[0][c] = 0;
            }
        }
    }
}

void drawScore() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(40, 600);

    string scoreText = "Score: " + to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2i(40, 570);
    string controlsText = "Press A S D to move";
    for (char c : controlsText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2i(40, 550);
    string rotateText = "Press E to rotate";
    for (char c : rotateText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}


// Store piece into board
void storePiece(int pieceType, int boardX, int boardY) {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if (shapes[pieceType - 1][row][col] == 1) {
                int boardRow = boardY + row - 2;
                int boardCol = boardX + col - 2;

                if (boardRow >= 0 && boardRow < BOARD_HEIGHT && boardCol >= 0 && boardCol < BOARD_WIDTH) {
                    board[boardRow][boardCol] = 1;
                }
            }
        }
    }

    clearLine(); // Check for filled rows and clear them
}

void drawStoredPieces() {
    for (int row = 0; row < BOARD_HEIGHT; row++) {
        for (int col = 0; col < BOARD_WIDTH; col++) {
            if (board[row][col] == 1) {
                float x = BOARD_LEFT + col * BLOCK_SIZE;
                float y = BOARD_BOTTOM + (BOARD_HEIGHT - row - 1) * BLOCK_SIZE;
                square(x, y);
            }
        }
    }
}

// Rotate the piece
void rotatePiece() {
    int temp[5][5];
    for (int row = 0; row < 5; row++)
        for (int col = 0; col < 5; col++)
            temp[col][4 - row] = shapes[currentPiece - 1][row][col];

    for (int row = 0; row < 5; row++)
        for (int col = 0; col < 5; col++)
            shapes[currentPiece - 1][row][col] = temp[row][col];
}

// inputs
void handleInput(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': if (!checkCollision(currentPiece, currentX - 1, currentY)) currentX--; break;
        case 'd': if (!checkCollision(currentPiece, currentX + 1, currentY)) currentX++; break;
        case 's': if (!checkCollision(currentPiece, currentX, currentY + 1)) currentY++; break;
        case 'e': rotatePiece(); break;
    }
    glutPostRedisplay();
}

void endGame() {
    cout << "Game Over!" << endl;
    exit(0);
}


// Main Game loop
void gameLoop(int value) {
    if (pieceActive) {
        if (!checkCollision(currentPiece, currentX, currentY + 1)) {
            currentY++;
        } else {
            storePiece(currentPiece, currentX, currentY);

            // check for game over.
            for (int col = 0; col < BOARD_WIDTH; col++) {
                if (board[0][col] != 0) {
                    endGame();
                    return;
                }
            }

            // Spawn the next piece
            currentPiece = nextPiece;
            nextPiece = (rand() % 7) + 1;
            currentX = 5;
            currentY = 0;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(500, gameLoop, 0);
}


void drawNextPiece() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(500, 480);

    string nextText = "Next Piece";
    for (char c : nextText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    float previewX = 550;
    float previewY = 550;

    glColor3f(0, 0, 1);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2i(500, 500);
    glVertex2i(650, 500);
    glVertex2i(650, 650);
    glVertex2i(500, 650);
    glEnd();

    // Center for the piece
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if (shapes[nextPiece - 1][row][col] == 1) {
                float x = previewX + (col - 2) * BLOCK_SIZE;
                float y = previewY - (row - 2) * BLOCK_SIZE;
                square(x, y);
            }
        }
    }
}


// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBoard();
    drawStoredPieces();
    drawPiece(currentPiece, currentX, currentY);
    drawNextPiece();
    drawScore();
    glFlush();
}


void initialize() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 700, 0, 700);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Tetris");

    initialize();
    srand(time(0));
    currentPiece = (rand() % 7) + 1;
    nextPiece = (rand() % 7) + 1;

    glutDisplayFunc(display);
    glutKeyboardFunc(handleInput);
    glutTimerFunc(500, gameLoop, 0);
    glutMainLoop();
    return 0;
}
