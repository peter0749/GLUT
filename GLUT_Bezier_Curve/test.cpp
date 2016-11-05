//-----------------------------------------------------------------------------
//                                                              2008/6/28
//                            Rotating Objects
//                                                              by�٬O�s��
//-----------------------------------------------------------------------------
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <GL\glut.h>
int old_rot_x=0;   //����U�ƹ��ɪ������y��
int old_rot_y=0;

int rot_x=0;      //�즲�᪺�۹�y�СA�γo�M�w�n����X��
int rot_y=0;

int record_x=0;      //�����W�@�����઺����
int record_y=0;

void WindowSize(int , int );            //�t�d������ø�Ϥ��e�����
void Keyboard(unsigned char , int, int );   //�����L��J
void Mouse(int , int , int , int );         //����ƹ����U�M��}�ɪ��T��
void MotionMouse(int , int );            //����ƹ����U�������T��
void Display(void);                     //�yø

int main()
{
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(400,400);         //�������e
   glutInitWindowPosition(600,80);         //�������W������m
   glutCreateWindow("�o�̬O�������D");      //�إߵ���

   //�U�����ӬO�Ψӫ��wCallback���
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
   glClearColor(1.0, 1.0, 1.0, 1.0);   //�Υզ��I��
   glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0,0,10.0f,0,0,0,0,1,0);   //���u���y�ФΤ�V
   glRotatef((float)rot_y+(float)record_y, 1.0, 0.0, 0.0);//�Hx�b�����b
   glRotatef((float)rot_x+(float)record_x, 0.0, 1.0, 0.0);//�Hy�b�����b
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
   printf("�A�ҫ����䪺�X�O%x\t���ɵ��������ƹ��y�ЬO(%d,%d)\n", key, x, y);
}

void WindowSize(int w, int h)
{
   printf("�ثe�����j�p��%dX%d\n",w,h);
   glViewport(0, 0, w, h);            //��������e���ܮɡA�e���]�����
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-10,10,-10,10,-10,30);      //�����v
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void Mouse(int button, int state, int x, int y)
{
   if(state)
   {
      record_x += x - old_rot_x;
      record_y += y - old_rot_y;
      
      rot_x = 0;   //�S���k�s�|�����z�Q�����G
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
