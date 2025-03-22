#include <GL/glut.h>     //Guarantees that gl.h and glu.h are properly included
#include <iostream>
#include <math.h>

using namespace std;
int ch = 1;
int matrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
int radius = 90;
int gmend = 0;
void changeturn(int &   ch)
{
    if(ch==1)
        ch=2;
    else if(ch==2)
        ch=1;
}

void cross(int x, int y)
{
    glColor3f(1,0,0);
    glLineWidth(8);
    glBegin(GL_LINES);
    glVertex2i(x-100,y+100);
    glVertex2i(x+100,y-100);
    glVertex2i(x-100,y-100);
    glVertex2i(x+100,y+100);
    glEnd();
}

void circle(int xc, int yc)
{
    int x = 0, y = radius;
    int p = 1 - radius;
    glColor3f(0, 1, 0);
    glPointSize(8);
    glBegin(GL_POINTS);

    while (x <= y) {
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);

        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }

    glEnd();
}

void winline(int x1, int y1, int x2, int y2)
{
    glColor3f(0,0,0.5);
    glLineWidth(8);
    glBegin(GL_LINES);
    glVertex2i(x1,y1);
    glVertex2i(x2,y2);
    glEnd();
}

bool boardmatrix(int x, int y)
{
    if(gmend==1)
        return false;
    if(matrix[x][y]==0)
    {
        matrix[x][y] = ch;
        if (ch == 1)
            cross(-200 + y*200, 200-x*200);
        else
            circle(-200 + y*200, 200-x*200);
        changeturn(ch);

        return true;
    }
    else
    {
        return false;
    }
}

bool checkwin()
{
    for(int i = 0; i<3; i++)
    {
        if(matrix[i][0]==matrix[i][1] && matrix[i][1]==matrix[i][2] && matrix[i][1]!=0)
        {
            int ln = -(i-1)*200;
            winline(-300,ln,300,ln);
            return true;
        }
        if(matrix[0][i]==matrix[1][i] && matrix[1][i]==matrix[2][i] && matrix[1][i]!=0)
        {
            int ln = (i-1)*200;
            winline(ln,-300,ln,300);
            return true;
        }
    }
    if(matrix[0][0]==matrix[1][1] && matrix[1][1]==matrix[2][2] && matrix[1][1]!=0)
    {
        winline(-300,300,300,-300);
        return true;
    }
    else if(matrix[0][2]==matrix[1][1] && matrix[1][1]==matrix[2][0] && matrix[1][1]!=0)
    {
        winline(-300,-300,300,300);
        return true;
    }

    return false;
}

void decidesquare(int x, int y)
{
    if(x<-100 && y< -100)
    {
        boardmatrix(2,0);
    }
    else if(x>-100 && x<100 && y<-100)
    {
        boardmatrix(2,1);
    }
    else if(x>100 && y<-100)
    {
        boardmatrix(2,2);
    }
    else if(x<-100 && y>-100 && y<100)
    {
        boardmatrix(1,0);
    }
    else if(x>-100 && x<100 && y>-100 && y<100)
    {
        boardmatrix(1,1);
    }
    else if(x>100 && y>-100 && y<100)
    {
        boardmatrix(1,2);
    }
    else if(x<-100 && y>100)
    {
        boardmatrix(0,0);
    }
    else if(x>-100 && x<100 && y>100)
    {
        boardmatrix(0,1);
    }
    else if(x>100 && y>100)
    {
        boardmatrix(0,2);
    }
    if(checkwin())
    {
        if(gmend==0)
        {
            changeturn(ch);
            cout<<"Player "<<ch<<" wins!"<<endl;
        }
        gmend=1;
    }
}




void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        x = x - 300;
        y = 300 - y;
        decidesquare(x, y);
        glFlush();
    }
}

void initialize()
{
 glClearColor(1.0, 1.0, 1.0, 1.0);           //to this color the window will be cleared
 glClear(GL_COLOR_BUFFER_BIT);              //actually clears the window. Once the clearing color is set, the window is cleared to that color whenever glClear() is called. This clearing color can be changed with another call to glClearColor().
 gluOrtho2D(-300, 300, -300, 300);                // creates a matrix for projecting 2D coordinates onto the screen and multiplies the current projection matrix by it.
}
void initialboard(){
    glColor3f(0,0,0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2i(-100,-300);
    glVertex2i(-100,300);
    glVertex2i(100,-300);
    glVertex2i(100,300);
    glVertex2i(-300,100);
    glVertex2i(300,100);
    glVertex2i(-300,-100);
    glVertex2i(300,-100);
    glEnd();                    // Marks the end of a vertex data list
    glFlush();
    glutMouseFunc(mouse);    // It allows you to link a keyboard key with a routine that's invoked when the key button is pressed or released.
}

int main(int argc, char **argv)
{
 glutInit(&argc, argv);            // Initializes Glut and processes any command line argument. It should be called before any other Glut routines.
 glutInitDisplayMode(GLUT_SINGLE);  // specifies whether to use RGBA/ Color index color model.
 glutInitWindowSize(600, 600);
 glutInitWindowPosition(800, 100);
 glutCreateWindow("Tic-tac-toe");
 initialize();


 glutDisplayFunc(initialboard);          // First and most important event callback function whenever GLUT determines that contents of the window need to be redisplayed the callback function registered by glutDisplayFunc() is executed.
                                      // You should put all the routines you need to redraw the scene in the display callback function.
 glutMainLoop();
 return 0;
}



