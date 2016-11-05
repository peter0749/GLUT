#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#ifdef __linux__
#include <GL/glut.h>
#elif __WIN32
#include <GL\glut.h>
#endif
#include <algorithm>
#include <complex>
#include <stack>
#define PALETTE_SIZE 256
#define EPS 0.997f
#define MP std::make_pair
#define DEF_W 800
#define DEF_H 450
#define STRING_LEN 1000
typedef std::pair<GLfloat,GLfloat> PII;
typedef std::pair< PII, PII > PIV;

int TH_HOLD=150;
size_t width=DEF_W, height=DEF_H;
GLfloat minX = -4.0f, maxX = 4.0f, minY = -2.25f, maxY = 2.25f;
GLfloat dX = ((maxX-minX)/(GLfloat)width)*EPS, dY = ((maxY-minY)/(GLfloat)height)*EPS;
GLfloat palette[PALETTE_SIZE][3];//R,G,B;
GLfloat black[3] = {0.0f,0.0f,0.0f};
const GLfloat r_HOLD = 4.1f;
bool FullScreen=false;
char title[STRING_LEN];
int consoleID;
int old_dx=0, old_dy=0, new_dx=0, new_dy=0;
std::stack< PIV > HistoryWindow;

const GLfloat YIQ[3][3]= {
        {0.299,0.587,0.114},
        {0.596,-0.274,-0.322},
        {0.211,-0.523,0.312}
        };
const GLfloat rYIQ[3][3]= {
        {1.0f, 0.956, 0.621},
        {1.0f, -0.272, -0.647},
        {1.0f, -1.106, 1.703}
        };
inline void slow_YIQ2RGB( GLfloat mtx[3] ){
       GLfloat temp[3];
       memset(temp,0x00,sizeof(temp));
       for(int k=0; k<3; ++k){
               for(int i=0; i<3; ++i){
                      temp[i] += rYIQ[i][k] *  mtx[k];
               }
       }
       memcpy(mtx, temp, std::min(sizeof(mtx),sizeof(temp)));
}

inline void refresh_diff(void){
       dX = ((maxX-minX)/(GLfloat)width)*EPS, dY = ((maxY-minY)/(GLfloat)height)*EPS;
}

inline double mu_trans(double x, double mu){
       return (x>0?1.0f:(-1.0f)) * (log(1+mu*std::abs(x))/log(1+mu));
}

void InitPalette(void){
       GLfloat oriI=0.0f, oriQ=0.5226f, oriY=0.0f;
       const double stepQ = 0.5226f*2.0f/(double)PALETTE_SIZE;
       const double stepI = 0.5957f*2.0f/(double)PALETTE_SIZE;
       const double stepY = 1.0f/(double)PALETTE_SIZE;
       
       /*Use YIQ, Q Channel.*/
       for(int i=0; i<PALETTE_SIZE; ++i){
               palette[i][0] = pow(i*stepY,0.3f);//Y
               //palette[i][0] = (GLfloat)mu_trans((double)(i/PALETTE_SIZE), 255.0f);
               palette[i][1] = (GLfloat)(stepI*(double)i);//I
               palette[i][2] = oriQ-(GLfloat)(stepQ*(double)i);//Q
       }
       /*Convert YIQ to RGB*/
       for(int i=0; i<PALETTE_SIZE; ++i){
               slow_YIQ2RGB(palette[i]);
       }
}

GLfloat *in_set(double re, double img){
       int counter = 0;
       std::complex<double> Z(re,img), nZ(0.0f,0.0f);
       while( counter<TH_HOLD ){
              nZ = nZ*nZ + Z;
              ++counter;
              if(std::abs(nZ) > 4.0f){
                  counter = (int)floor( (float)counter / (float)TH_HOLD * (float)PALETTE_SIZE);
                  return palette[ counter  ];
              }
       }
       return black;
}

void Paint(void){
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
     glBegin(GL_POINTS);
     for(GLfloat i=maxY; i>minY; i-=dY){
                 for(GLfloat j=maxX; j>minX; j-=dX){
                             glColor3fv( in_set(j,i) );
                             glVertex3f( j,i,0.0f );
                 }
     }
     glEnd();
     glutSwapBuffers();
     sprintf(title,"MandelBrot   X:%.2lf,%.2lf Y:%.2lf,%.2lf", (double)minX, (double)maxX,\
             (double)minY,(double)maxY
     );
     glutSetWindowTitle(title);
}

//old_dx=0, old_dy=0, new_dx=0, new_dy=0, rec_dx=0, rec_dy=0;
void Mouse_event(int button, int state, int x, int y){
     if(state==GLUT_DOWN && button==GLUT_LEFT_BUTTON){
               old_dx = x;
               old_dy = height-y; 
     }else if( state==GLUT_UP && button==GLUT_LEFT_BUTTON ){
           new_dx = x;
           if(new_dx<old_dx)std::swap(new_dx,old_dx);
           y=height-y;
           if(y>old_dy)std::swap(y,old_dy);
           new_dy = (double)old_dy - ((double)height/(double)width)*((double)new_dx-(double)old_dx);
           HistoryWindow.push( MP( MP(minX,maxX),MP(minY,maxY) ) );
           minX = minX + dX*(double)(old_dx);
           maxX = maxX - dX*(double)(width-new_dx);
           minY = minY + dY*(double)(new_dy);
           maxY = maxY - dY*(double)(height-old_dy);
           refresh_diff();
           glLoadIdentity();
           glOrtho(minX,maxX,minY,maxY, ((GLfloat)-1.0f), (GLfloat)1.0f);
           glutPostRedisplay();
     }
}

void window_shape(int x, int y){
     glViewport(0, 0, (GLsizei)x, (GLsizei)y);
     width = (size_t)x; height=(size_t)y;
     refresh_diff();
     glutPostRedisplay();
}

void keyEvent(unsigned char key, int x, int y){ // function to handle key pressing
	switch(key){
		case 'F': // pressing F is turning on/off fullscreen mode
		case 'f':
			if(FullScreen){
                width = DEF_W; height=DEF_H;
				glutReshapeWindow(width,height); // sets default window size
				GLsizei windowX = (glutGet(GLUT_SCREEN_WIDTH)-width)/2;
				GLsizei windowY = (glutGet(GLUT_SCREEN_HEIGHT)-height)/2;
				glutPositionWindow(windowX, windowY); // centers window on the screen
				FullScreen = false;
			}
			else{
				FullScreen = true;
				glutFullScreen(); // go to fullscreen mode
			}
			glutPostRedisplay();
			break;
		case '-':
             if(TH_HOLD>0){
                 TH_HOLD-=15;
                 glutPostRedisplay();
             }
             break;
        case '+':
             TH_HOLD+=15;
             glutPostRedisplay();
             break;
        case '8':
             minY+=(dY*10);maxY+=(dY*10);
             refresh_diff();
             glLoadIdentity();
             glOrtho(minX,maxX,minY,maxY, ((GLfloat)-1.0f), (GLfloat)1.0f);
             glutPostRedisplay();
             break;
        case '2':
             minY-=(dY*10);maxY-=(dY*10);
             refresh_diff();
             glLoadIdentity();
             glOrtho(minX,maxX,minY,maxY, ((GLfloat)-1.0f), (GLfloat)1.0f);
             glutPostRedisplay();
             break;
        case '4':
             minX-=(dX*10);maxX-=(dX*10);
             refresh_diff();
             glLoadIdentity();
             glOrtho(minX,maxX,minY,maxY, ((GLfloat)-1.0f), (GLfloat)1.0f);
             glutPostRedisplay();
             break;
        case '6':
             minX+=(dX*10);maxX+=(dX*10);
             refresh_diff();
             glLoadIdentity();
             glOrtho(minX,maxX,minY,maxY, ((GLfloat)-1.0f), (GLfloat)1.0f);
             glutPostRedisplay();
             break;
        case '5':
             if(!HistoryWindow.empty()){
                 minX = HistoryWindow.top().first.first;
                 maxX = HistoryWindow.top().first.second;
                 minY = HistoryWindow.top().second.first;
                 maxY = HistoryWindow.top().second.second;
                 HistoryWindow.pop();
                 refresh_diff();
                 glLoadIdentity();
                 glOrtho(minX,maxX,minY,maxY, ((GLfloat)-1.0f), (GLfloat)1.0f);
                 glutPostRedisplay();
             }
             break;
		case 27 : // escape key - close the program
			glutDestroyWindow(consoleID);
			exit(0);
			break;
	}
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    InitPalette();
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    GLsizei windowX = ( glutGet(GLUT_SCREEN_WIDTH)-width )/2;
    GLsizei windowY = ( glutGet(GLUT_SCREEN_HEIGHT)-height )/2;
    glutInitWindowPosition(windowX, windowY);
    glutInitWindowSize(width, height);
    consoleID = glutCreateWindow("Mandelbrot");
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);//SMOOTH POINTS BEFORE PUSH INTO BUFFER
    //glEnable(GL_DITHER);
    glViewport(0,0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(minX,maxX,minY,maxY, ((GLfloat)-1.0f), (GLfloat)1.0f);
    
    glutDisplayFunc(Paint);
    glutReshapeFunc(window_shape);
    glutMouseFunc(Mouse_event);
    glutKeyboardFunc(keyEvent);
    
    glutMainLoop();
    return 0;
}
