#include <gl/glut.h>
#include <gl/glu.h>
#include <gl/gl.h>
#include <gl/GLAUX.h>

#include "camera.h"
#include "SkyBox.h"
#include "House.h"
#include "Particle.h"

GLint winWidth = 800, winHeight = 600;
GLfloat xEye = 512, yEye = 35, zEye = 512;
GLboolean gridOn = false;

Camera m_Camera;
CSkyBox m_SkyBox;
House m_House(winWidth, winHeight, xEye, yEye, zEye);
Particle particles[500];
GLint particlenum = 500;

void init(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //启用纹理
    glEnable(GL_TEXTURE_2D);
    //初始化天空
    if (!m_SkyBox.Init())
    {
        printf("初始化天空失败!");
        exit(0);
    }
    //设置摄像机
    m_Camera.setCamera(xEye, yEye, zEye, xEye, yEye, zEye+1, 0, 1, 0);
	m_House.Init();
	//初始化粒子
	for (int i = 0; i < particlenum; i++)
		particles[i].init();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //放置摄像机
    m_Camera.setLook();
    //绘制天空
    m_SkyBox.CreateSkyBox(0, 0, 0, 1.0, 0.5, 1.0);
	if (gridOn)
		m_House.DrawGrid();
	m_House.DrawScene();
	//改变粒子并绘制
	for (int i = 0; i < particlenum; i++)
	{
		particles[i].draw();
		particles[i].move();
	}
    glutSwapBuffers();
}

void ChangeSize(int width, int height)
{
	winWidth = width; winHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 4000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void motion(int x, int y)
{
    m_Camera.setViewByMouse();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
	{
        case 'w':
            m_Camera.moveCamera(m_Camera.getSpeed());
            break;
        case 's':
            m_Camera.moveCamera(-m_Camera.getSpeed());
            break;
        case 'a':
            m_Camera.yawCamera(-m_Camera.getSpeed());
            break;
        case 'd':
            m_Camera.yawCamera(m_Camera.getSpeed());
            break;
		case 'g':
            gridOn = !gridOn;
            break;
		case 'm':
			m_House.m_flag = !m_House.m_flag;
			break;
		case 27:
            exit(0);
            break;
    }
    glutPostRedisplay();
}

 void specialkey(int key, int x, int y)
 {
	switch(key)
	{
		case GLUT_KEY_F1:
			m_SkyBox.flag = 1;
			m_SkyBox.Init();
            break;
		case GLUT_KEY_F2:
			m_SkyBox.flag = 2;
			m_SkyBox.Init();
			break;
		case GLUT_KEY_F3:
			m_SkyBox.flag = 3;
			m_SkyBox.Init();
			break;
		case GLUT_KEY_F4:
			m_SkyBox.flag = 4;
			m_SkyBox.Init();
			break;
	}
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
	glutCreateWindow("OPENGL 3D");
    init();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkey);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}