#include <iostream>
#include <cmath>
#include <ctime>
#include <GL/glut.h>

using namespace std;

const int BOARD_HEIGHT = 8;
const int BOARD_WIDTH = 8;
int BOARD_MATRIX[BOARD_HEIGHT][BOARD_WIDTH] = {0};
int MINES_NO = sqrt(BOARD_HEIGHT*BOARD_WIDTH);
bool REVEALED[BOARD_HEIGHT][BOARD_WIDTH] = {false};
bool FLAGGED[BOARD_HEIGHT][BOARD_WIDTH] = {false};
bool gameOver = false;
bool gameWin = false;



void displayboard()
{
    for(int i = 0; i<BOARD_HEIGHT; i++)
    {
        for(int j = 0; j<BOARD_WIDTH; j++)
        cout<<BOARD_MATRIX[i][j]<<" ";
        cout<<endl;
    }
}

int getRandom(int mini, int maxi) {
    return mini + rand() % (maxi - mini + 1);
}

void addmines()
{
    int noofmines = 0;
    while(noofmines!=MINES_NO)
    {
        int x = getRandom(0,7);
        int y = getRandom(0,7);
        if(BOARD_MATRIX[x][y]!=9)
        {
            BOARD_MATRIX[x][y]=9;
            noofmines++;
        }
    }
}

int checknumbers(int x, int y) {
    int no = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newX = x + i;
            int newY = y + j;

            if (i == 0 && j == 0)
                continue;

            if (newX >= 0 && newX < BOARD_HEIGHT && newY >= 0 && newY < BOARD_WIDTH) {
                if (BOARD_MATRIX[newX][newY] == 9) {
                    no++;
                }
            }
        }
    }
    return no;
}

void addnumbers()
{
    for(int i = 0; i<BOARD_HEIGHT; i++)
    {
        for(int j = 0; j<BOARD_WIDTH; j++)
        {
            if(BOARD_MATRIX[i][j]!=9)
            {
                BOARD_MATRIX[i][j]=checknumbers(i,j);
            }
        }
    }
}
void drawnumber(int row, int col) {
    if (!REVEALED[row][col] || BOARD_MATRIX[row][col] == 0 || BOARD_MATRIX[row][col] == 9)
        return;
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(50 + col * 50 - 7, 50 + (7 - row) * 50-10, 0);
    glScalef(0.2, 0.2, 1);

    char num = BOARD_MATRIX[row][col] + '0';
    glutStrokeCharacter(GLUT_STROKE_ROMAN, num);

    glPopMatrix();
    glFlush();
}

void drawText(const char *text, int x, int y)
{
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(0.2, 0.2, 1);

    while (*text)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *text);
        text++;
    }

    glPopMatrix();
}

void drawboardmatrix()
{
    glColor3f(1, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2i(25,25);
    glVertex2i(425,25);
    glVertex2i(425,25);
    glVertex2i(425,425);
    glVertex2i(425,425);
    glVertex2i(25,425);
    glVertex2i(25,425);
    glVertex2i(25,25);
    glEnd();
    glColor3f(0,0,0);
    glLineWidth(1);
    glBegin(GL_LINES);
    for(int i = 1; i<8; i++)
    {
        glVertex2i(25+(i*50),25);
        glVertex2i(25+(i*50),425);
        glVertex2i(25,25+(i*50));
        glVertex2i(425,25+(i*50));
    }
    glEnd();
}

void checkWinCondition()
{
    int flaggedMines = 0;
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (BOARD_MATRIX[i][j] == 9 && FLAGGED[i][j])
                flaggedMines++;
        }
    }
    if (flaggedMines == MINES_NO)
    {
        gameWin = true;
    }
}

void drawExclamation(int row, int col) {
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(50 + col * 50 - 5, 50 + (7 - row) * 50 - 10, 0);
    glScalef(0.2, 0.2, 1);

    glutStrokeCharacter(GLUT_STROKE_ROMAN, '!');

    glPopMatrix();
}


void revealzeros(int row, int col) {
    // Check boundaries
    if (row < 0 || row >= BOARD_HEIGHT || col < 0 || col >= BOARD_WIDTH)
        return;

    // Stop if already revealed or a mine
    if (REVEALED[row][col] || BOARD_MATRIX[row][col] == 9)
        return;

    // Reveal current cell
    REVEALED[row][col] = true;

    if (BOARD_MATRIX[row][col] == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i != 0 || j != 0) {
                    revealzeros(row + i, col + j);
                }
            }
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawboardmatrix();
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (FLAGGED[i][j])
                glColor3f(1, 0, 0);
            else if (REVEALED[i][j])
                glColor3f(0, 0, 0);
            else
                continue;
            glBegin(GL_QUADS);
            glVertex2i(25 + j * 50 + 2, 25 + (7 - i) * 50 + 2);
            glVertex2i(25 + (j + 1) * 50 - 2, 25 + (7 - i) * 50 + 2);
            glVertex2i(25 + (j + 1) * 50 - 2, 25 + (8 - i) * 50 - 2);
            glVertex2i(25 + j * 50 + 2, 25 + (8 - i) * 50 - 2);
            glEnd();
            if (gameOver && BOARD_MATRIX[i][j] == 9 && REVEALED[i][j])
                drawExclamation(i, j);

            drawnumber(i, j);

        }
        if (gameOver)
            drawText("Game Over! You Lose!", 50, 225);
        if (gameWin)
            drawText("Game Over! You Win!", 100, 225);
    }
    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (gameOver || gameWin)
        return;

    if (state == GLUT_DOWN) {
        int col = (x - 25) / 50;
        int row = (y - 25) / 50;

        if (col >= 0 && col < BOARD_WIDTH && row >= 0 && row < BOARD_HEIGHT) {
            if (button == GLUT_LEFT_BUTTON)
            {
                if (FLAGGED[row][col])
                return;

                if (BOARD_MATRIX[row][col] == 9)
                {
                    gameOver = true;
                    REVEALED[row][col] = true;

                }

                else if (BOARD_MATRIX[row][col] == 0)
                revealzeros(row, col);

                else
                REVEALED[row][col] = true;

            }
            else if (button == GLUT_RIGHT_BUTTON)
            {
                FLAGGED[row][col] = !FLAGGED[row][col];
                checkWinCondition();
            }


            glutPostRedisplay();
        }
    }
}

void initialize() {
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 450, 0, 450);
}
int main(int argc, char** argv)
{
    srand(time(0));
    addmines();
    addnumbers();
    //displayboard();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(450, 450);
    glutCreateWindow("Minesweeper");

    initialize();
    glutDisplayFunc(display);  // Set display callback function
    glutMouseFunc(mouse);
    glutMainLoop();  // Enter main loop

    return 0;
}
