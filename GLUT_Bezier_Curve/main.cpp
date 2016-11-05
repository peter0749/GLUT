#if 1
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <GL\glut.h>

size_t width=640, height=480;
GLfloat minX=0.0f, maxX=1024.0f, minY=0.0f, maxY=1024.0f;
GLfloat black[3] = {0.0f, 0.0f, 0.0f}; // black color
double IteStep=0.0001f;
double p[4][2];
int mouseX=0, mouseY=0;
int windowID;

void Blending(double &x, double &y, double t)
{
     double yt = p[0][1]*(1-t)*(1-t)*(1-t) + 3*p[1][1]*(1-t)*(1-t)*t + 3*p[2][1]*(1-t)*t*t + p[3][1]*t*t*t;
     double xt = p[0][0]*(1-t)*(1-t)*(1-t) + 3*p[1][0]*(1-t)*(1-t)*t + 3*p[2][0]*(1-t)*t*t + p[3][0]*t*t*t;
     x=xt; y=yt;
} 

void PaintPix(void)
{
     /*Only on first Quadrant*/
     glClearColor(1.0f,1.0f,1.0f,1.0f);//specify clear values for the color buffers
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen buffer
     glBegin(GL_POINTS); // start drawing in single pixel mode
     double fX, fY, i;
     for(i=0; i<1.0f; i+=IteStep){
         Blending(fX,fY,i);
         glColor3fv(black);
         glVertex3f((GLfloat)fX, (GLfloat)fY, 0.0f);
     }
     glEnd(); // end drawing
	 glutSwapBuffers(); // swap the buffers - [ 2 ]
}

void window_shape(int w, int h)
{
     glViewport (0, 0, (GLsizei)w, (GLsizei)h); // set new dimension of viewable screen
     width = w; height = h;
     glutPostRedisplay(); // repaint the window
}

void Mouse(int bottom, int state, int x, int y)
{
     static int counter=0;
     y = height-y;
     x *= ((maxX-minX)/width);
     y *= ((maxY-minY)/height);
     if(state){
         if(counter==4){
             glutPostRedisplay();
             counter=0;
             return;
         }
         p[counter][0] = x;
         p[counter][1] = y;
         counter++;
     }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);//Initalize GLUT library;
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);//Set the initial display mode
    /*
    * GLUT_RGBA:   Bitmask, select an RGBA mode window.
    * GLUT_DOUBLE: Bit mask to select a double buffered window.
    * GLUT_DEPTH:  Bit mask to select a window with a depth buffer.
    * note: Reference from https://www.opengl.org/resources/libraries/glut/spec3/node12.html
    */
    glClearColor(1.0f,1.0f,1.0f,1.0f);//specify clear values for the color buffers
    GLsizei windowX = ( glutGet(GLUT_SCREEN_WIDTH)-width )/2;
    GLsizei windowY = ( glutGet(GLUT_SCREEN_HEIGHT)-height )/2;
    glutInitWindowPosition(windowX, windowY);
    glutInitWindowSize(width, height);
    windowID = glutCreateWindow("Bezier Curve");
    
    glShadeModel(GL_SMOOTH);//GL smooth shading
    glEnable(GL_DEPTH_TEST);//Prevent pixels behind the first pixel cover the first
    glEnable(GL_POINT_SMOOTH);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode (GL_PROJECTION);//Use GL_PROJECTION glMatrixMode
    glLoadIdentity(); // Load matrix "I"
    glOrtho(minX, maxX, minY, maxY, ((GLfloat)-1), (GLfloat)1);//multiply the current matrix with an orthographic matrix
    
    /*Event handling methods*/
    glutDisplayFunc(PaintPix);
    glutReshapeFunc(window_shape);
    glutMouseFunc(Mouse);
    glutMainLoop();
    return 0;
}
#endif
