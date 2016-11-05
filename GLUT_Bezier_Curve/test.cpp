//-----------------------------------------------------------------------------
//                                                              2008/6/28
//                            Rotating Objects
//                                                              by還是零分
//-----------------------------------------------------------------------------
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <GL\glut.h>
int old_rot_x=0;   //剛按下滑鼠時的視窗座標
int old_rot_y=0;

int rot_x=0;      //拖曳後的相對座標，用這決定要旋轉幾度
int rot_y=0;

int record_x=0;      //紀錄上一次旋轉的角度
int record_y=0;

void WindowSize(int , int );            //負責視窗及繪圖內容的比例
void Keyboard(unsigned char , int, int );   //獲取鍵盤輸入
void Mouse(int , int , int , int );         //獲取滑鼠按下和放開時的訊息
void MotionMouse(int , int );            //獲取滑鼠按下期間的訊息
void Display(void);                     //描繪

int main()
{
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(400,400);         //視窗長寬
   glutInitWindowPosition(600,80);         //視窗左上角的位置
   glutCreateWindow("這裡是視窗標題");      //建立視窗

   //下面五個是用來指定Callback函數
   glutReshapeFunc(WindowSize);
   glutKeyboardFunc(Keyboard);
   glutMouseFunc(Mouse);
   glutMotionFunc(MotionMouse);
   glutDisplayFunc(Display);

   glutMainLoop();
   return 0;
}

void Display(void)
{
   glClearColor(1.0, 1.0, 1.0, 1.0);   //用白色塗背景
   glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0,0,10.0f,0,0,0,0,1,0);   //視線的座標及方向
   glRotatef((float)rot_y+(float)record_y, 1.0, 0.0, 0.0);//以x軸當旋轉軸
   glRotatef((float)rot_x+(float)record_x, 0.0, 1.0, 0.0);//以y軸當旋轉軸
   glBegin(GL_TRIANGLES);
      glColor3f( 1, 0, 0);glVertex3f( 8.6603, -5, -3);
      glColor3f( 0, 1, 0);glVertex3f(      0, 10, -3);
      glColor3f( 0, 0, 1);glVertex3f(-8.6603, -5, -3);

      glColor3f( 1, 0, 0);glVertex3f( 8.6603, -5, 0);
      glColor3f( 0, 1, 0);glVertex3f(      0, 10, 0);
      glColor3f( 0, 0, 1);glVertex3f(-8.6603, -5, 0);

      glColor3f( 1, 0, 0);glVertex3f( 8.6603, -5, 3);
      glColor3f( 0, 1, 0);glVertex3f(      0, 10, 3);
      glColor3f( 0, 0, 1);glVertex3f(-8.6603, -5, 3);
   glEnd();
   glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y)
{
   printf("你所按按鍵的碼是%x\t此時視窗內的滑鼠座標是(%d,%d)\n", key, x, y);
}

void WindowSize(int w, int h)
{
   printf("目前視窗大小為%dX%d\n",w,h);
   glViewport(0, 0, w, h);            //當視窗長寬改變時，畫面也跟著變
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-10,10,-10,10,-10,30);      //正交投影
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void Mouse(int button, int state, int x, int y)
{
   if(state)
   {
      record_x += x - old_rot_x;
      record_y += y - old_rot_y;
      
      rot_x = 0;   //沒有歸零會有不理想的結果
      rot_y = 0;
   }
   else
   {
      old_rot_x = x;
      old_rot_y = y;
   }
}

void MotionMouse(int x, int y)
{
   rot_x = x - old_rot_x;
   rot_y = y - old_rot_y;
   glutPostRedisplay();
}
#endif
