#ifndef __HOUSE_H__
#define __HOUSE_H__

#include "skybox.h"

//���Ʒ���
class House
{
public:
    //���캯��
    House(GLint, GLint, GLfloat, GLfloat, GLfloat);
    ~House();
    void Init();
	AUX_RGBImageRec* LoadBMP(char*);
	int LoadTextures();
	GLuint EmptyTexture();
	void DrawScene();
	void DrawCrystal();
	void DrawTeapot();
	void DrawDesk();
	void DrawYard();
	void DrawGrid();
	void DrawHouse();
	void DrawFurniture();

private:
    GLuint	EnvTexture;//��������
	GLUquadricObj *quadratic;//���μ�����
	GLint winWidth, winHeight;
	GLfloat xEye, yEye, zEye;
	GLuint  texture[8];   //����
	GLfloat rotate; //Բ����ת��
public:
	GLboolean m_flag; //���ʱ�ʶ
};

#endif