#include<stdlib.h>
#include<GL\glut.h>
#include<time.h>
#include "skybox.h"
 
class Particle
{
private:
	GLfloat position[3];//���ӵ�λ��
	GLfloat v0;//���ӵĳ��ٶ�
	GLfloat g;//�������ٶ�
	GLfloat size;//��С
	GLfloat sizeSet[4];
	GLfloat gSet[4];
	GLuint display_list;
public:
	Particle()
	{
		sizeSet[0] = 0.40;
		sizeSet[1] = 0.45;
		sizeSet[2] = 0.50;
		sizeSet[3] = 0.55;
		gSet[0] = 0.01;
		gSet[1] = 0.02;
		gSet[2] = 0.04;
		gSet[3] = 0.06;
	}
	GLuint createDL()
	{
		GLuint DL;
		DL = glGenLists(1);
		glNewList(DL,GL_COMPILE); 
		GLUquadricObj *qobj = gluNewQuadric();
		glDisable(GL_TEXTURE_2D);
		gluSphere(qobj, size, 20, 20);//��С��
		glEnable(GL_TEXTURE_2D);
		glEndList();
		return DL;
	}
	void init()//���ӳ�ʼ��
	{
		display_list = createDL();
		position[0] = rand() % MAP_WIDTH;
		position[1] = 100;
		position[2] = rand() % MAP_WIDTH;
		//��֤����û������
		if ((position[0] > 512 - CELL_WIDTH*6) && (position[0] < 512 + CELL_WIDTH*6)
			&&(position[2] > 512 + CELL_WIDTH*8) && (position[2] < 512 + CELL_WIDTH*14))
		{
			position[0] += CELL_WIDTH*12;
			position[2] += CELL_WIDTH*6;
		}
		int sizeIndex = rand() % 4;
		size = sizeSet[sizeIndex];
		g = gSet[sizeIndex];//������ٶ�
		v0 = (float)(rand() % 6) / (float)10;//�����ʼ�����ٶ�
	}
	void draw()
	{
		GLfloat diffuse[3] = {1.0, 1.0, 1.0};
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse);
		glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		glCallList(display_list);
		glPopMatrix();
	}
	void move()
	{
		position[1] -= v0;
		v0 += g/10;
		if(position[1] <= 0)
			die();
	}
	void die()
	{
		init();
	}
};