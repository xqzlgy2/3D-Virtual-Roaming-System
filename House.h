#ifndef __HOUSE_H__
#define __HOUSE_H__

#include "skybox.h"

//绘制房屋
class House
{
public:
    //构造函数
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
    GLuint	EnvTexture;//环境纹理
	GLUquadricObj *quadratic;//二次几何体
	GLint winWidth, winHeight;
	GLfloat xEye, yEye, zEye;
	GLuint  texture[8];   //纹理
	GLfloat rotate; //圆环旋转角
public:
	GLboolean m_flag; //材质标识
};

#endif