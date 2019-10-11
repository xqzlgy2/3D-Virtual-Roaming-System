#include "House.h"
#include "material.h"

House::House(GLint winWidth, GLint winHeight, GLfloat xEye, GLfloat yEye, GLfloat zEye)
{
	this->winWidth = winWidth;
	this->winHeight = winHeight;
	this->xEye = xEye;
	this->yEye = yEye;
	this->zEye = zEye;
	m_flag = true;
}

House::~House()
{
}

//����λͼ
AUX_RGBImageRec* House::LoadBMP(char *Filename)
{
	FILE *File=NULL;
	if (!Filename)
		return NULL;
	File=fopen(Filename,"r");
	if (File)
	{
		fclose(File);
		return auxDIBImageLoad(Filename);
	}
	return NULL;
}

//��������
int House::LoadTextures()
{
	int status=FALSE;
	AUX_RGBImageRec *TextureImage[8];	
	memset(TextureImage,0,sizeof(void *)*8); 
	if ((TextureImage[0]=LoadBMP("data/wood.bmp"))&&(TextureImage[1]=LoadBMP("data/yard.bmp"))&&(TextureImage[2]=LoadBMP("data/wall.bmp"))
		&&(TextureImage[3]=LoadBMP("data/roof.bmp"))&&(TextureImage[4]=LoadBMP("data/flower.bmp"))&&(TextureImage[5]=LoadBMP("data/bookshelf.bmp"))
		&&(TextureImage[6]=LoadBMP("data/window.bmp"))&&(TextureImage[7]=LoadBMP("data/painting.bmp")))
	{	
		status=TRUE;
		glGenTextures(8, &texture[0]);
		for(int i=0;i<8;i++)
		{
			glBindTexture(GL_TEXTURE_2D,texture[i]);//������
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//�����˲�ģʽ
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i]->sizeX, TextureImage[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data);//��������
		}
		for (int i=0; i<8; i++)
			if (TextureImage[i]!=NULL)
			{
				if (TextureImage[i]->data!=NULL)
					free(TextureImage[i]->data);
				free(TextureImage[i]);
			}
	}
	return status;
}

void House::Init()
{
	//���μ�����
	quadratic=gluNewQuadric();
	gluQuadricNormals(quadratic,GL_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);//���μ������Զ���������
	EnvTexture = EmptyTexture();//Ϊ���������ٿռ�
	//���ù���
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHT0);	
	glEnable(GL_LIGHTING);
	//��������
	LoadTextures();
	//��ʼ����ת��
	rotate = 0;
}

//Ϊ�����ٿռ�
GLuint House::EmptyTexture()
{
	GLuint txtnumber;
	unsigned int* data;
	data = (unsigned int*)new GLuint[((winWidth* winHeight)* 4 * sizeof(unsigned int))];//���￪�ٵĴ�С��DrawGlassBall()ʹ�õ�һ��
	ZeroMemory(data,((winWidth* winHeight)* 4 * sizeof(unsigned int)));//����

	glGenTextures(1, &txtnumber);
	glBindTexture(GL_TEXTURE_2D, txtnumber);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, winWidth/2, winHeight/2, 0,GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	delete [] data;	
	return txtnumber;
}

//ˮ����
void House::DrawCrystal()
{
	GLboolean lp, tp;
    glGetBooleanv(GL_LIGHTING, &lp);
    glGetBooleanv(GL_TEXTURE_2D, &tp);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPushAttrib(GL_CURRENT_BIT);   //���浱ǰ����

	glBindTexture(GL_TEXTURE_2D,EnvTexture);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0,winWidth, winHeight, 0);//����Ļ���ݿ�����������

	glPushMatrix();
	//����
	glMaterialfv(GL_FRONT, GL_AMBIENT, glass_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, glass_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, glass_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, glass_shininess);
	glTranslatef(xEye-4,yEye-5,zEye+CELL_WIDTH*4);
	glRotatef(-90,1,0,0);
	glRotatef(180,0,0,1);
	gluSphere(quadratic,3,30,30);

	glPopMatrix();
    glPopAttrib();
    //�ָ�����״̬
    if (!tp)
        glDisable(GL_TEXTURE_2D);
    if (!lp)
        glDisable(GL_LIGHTING);
}

//���
void House::DrawTeapot()
{
	GLboolean lp, tp;
    glGetBooleanv(GL_LIGHTING, &lp);
    glGetBooleanv(GL_TEXTURE_2D, &tp);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPushAttrib(GL_CURRENT_BIT);   //���浱ǰ����

	glPushMatrix();
	//����
	glMaterialfv(GL_FRONT, GL_AMBIENT, brass_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brass_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, brass_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, brass_shininess);
	glTranslatef(xEye+4,yEye-5,zEye+CELL_WIDTH*4);
	glutSolidTeapot(3);

	glPopMatrix();
    glPopAttrib();
    //�ָ�����״̬
    if (tp)
        glEnable(GL_TEXTURE_2D);
    if (!lp)
        glDisable(GL_LIGHTING);
}

void House::DrawDesk()
{
	GLboolean lp, tp;
    glGetBooleanv(GL_LIGHTING, &lp);
    glGetBooleanv(GL_TEXTURE_2D, &tp);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPushAttrib(GL_CURRENT_BIT);   //���浱ǰ����
	//����
	glMaterialfv(GL_FRONT, GL_AMBIENT, wood_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wood_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wood_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, wood_shininess);
	//����(Բ)
	glPushMatrix();
	GLint n = 100;
	GLfloat pi = 3.14159, R = 10.0;
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_POLYGON);
	for (GLint i = 0; i < n; i++)
		glVertex3f(xEye+R*cos(2*pi/n*i), yEye-8, zEye+CELL_WIDTH*4+R*sin(2*pi/n*i));
	glEnd();
	glPopMatrix();
	//����
	glPushMatrix();
	glTranslatef(xEye,yEye-10,zEye+CELL_WIDTH*4);
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	glRotatef(180.0f,0.0f,0.0f,1.0f);
	gluCylinder(quadratic,R,R,2.0,30.0,30.0);
	glPopMatrix();
	//����
	glPushMatrix();
	glTranslatef(xEye,yEye-30,zEye+CELL_WIDTH*4);
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	glRotatef(180.0f,0.0f,0.0f,1.0f);
	gluCylinder(quadratic,3.0,3.0,20.0,30.0,30.0);
	glPopMatrix();

	glPopAttrib();
    //�ָ�����״̬
    if (!tp)
        glDisable(GL_TEXTURE_2D);
    if (!lp)
        glDisable(GL_LIGHTING);
}

void House::DrawYard()
{
	GLboolean lp, tp;
    glGetBooleanv(GL_LIGHTING, &lp);
    glGetBooleanv(GL_TEXTURE_2D, &tp);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPushAttrib(GL_CURRENT_BIT);   //���浱ǰ����

	//����
	glMaterialfv(GL_FRONT, GL_AMBIENT, normal_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, normal_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, normal_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, normal_shininess);

	//������
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*7,1,zEye);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(xEye + CELL_WIDTH*7,1,zEye);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(xEye + CELL_WIDTH*7,1,zEye + CELL_WIDTH*14);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*7,1,zEye + CELL_WIDTH*14);
	glEnd();
	glPopMatrix();

	//����̺
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*2,2,zEye + CELL_WIDTH*2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye - CELL_WIDTH*3,2,zEye + CELL_WIDTH*2);
	glTexCoord2f(6.0f, 1.0f); glVertex3f(xEye - CELL_WIDTH*3,2,zEye + CELL_WIDTH*8);
	glTexCoord2f(6.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*2,2,zEye + CELL_WIDTH*8);
	glEnd();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*2,2,zEye + CELL_WIDTH*2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*3,2,zEye + CELL_WIDTH*2);
	glTexCoord2f(6.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*3,2,zEye + CELL_WIDTH*8);
	glTexCoord2f(6.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*2,2,zEye + CELL_WIDTH*8);
	glEnd();
	glPopMatrix();

	glPopAttrib();
    //�ָ�����״̬
    if (!tp)
        glDisable(GL_TEXTURE_2D);
    if (!lp)
        glDisable(GL_LIGHTING);
}

void House::DrawHouse()
{
	GLboolean lp, tp;
    glGetBooleanv(GL_LIGHTING, &lp);
    glGetBooleanv(GL_TEXTURE_2D, &tp);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPushAttrib(GL_CURRENT_BIT);   //���浱ǰ����

	//����
	glMaterialfv(GL_FRONT, GL_AMBIENT, wood_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wood_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wood_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, wood_shininess);

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	//����ǽ
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,1,zEye + CELL_WIDTH*8);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,1,zEye + CELL_WIDTH*14);
	glTexCoord2f(2.0f, 2.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*14);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glEnd();
	//����ǽ
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,1,zEye + CELL_WIDTH*14);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*14);
	glTexCoord2f(5.0f, 2.0f); glVertex3f(xEye - CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*14);
	glTexCoord2f(5.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*6,1,zEye + CELL_WIDTH*14);
	glEnd();
	//����ǽ
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*6,1,zEye + CELL_WIDTH*8);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*6,1,zEye + CELL_WIDTH*14);
	glTexCoord2f(2.0f, 2.0f); glVertex3f(xEye - CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*14);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(xEye - CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glEnd();
	//��ǰǽ
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,1,zEye + CELL_WIDTH*8);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glTexCoord2f(2.0f, 2.0f); glVertex3f(xEye + CELL_WIDTH*2,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*2,1,zEye + CELL_WIDTH*8);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*6,1,zEye + CELL_WIDTH*8);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(xEye - CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glTexCoord2f(2.0f, 2.0f); glVertex3f(xEye - CELL_WIDTH*2,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*2,1,zEye + CELL_WIDTH*8);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*14);
	glTexCoord2f(5.0f, 2.0f); glVertex3f(xEye - CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*14);
	glTexCoord2f(5.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glEnd();

	//���ݶ�
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*14);
	glTexCoord2f(1.5f, 1.0f); glVertex3f(xEye, CELL_WIDTH*7,zEye + CELL_WIDTH*11);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*14);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*14);
	glTexCoord2f(1.5f, 1.0f); glVertex3f(xEye, CELL_WIDTH*7,zEye + CELL_WIDTH*11);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*14);
	glTexCoord2f(1.5f, 1.0f); glVertex3f(xEye, CELL_WIDTH*7,zEye + CELL_WIDTH*11);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*6,CELL_WIDTH*4,zEye + CELL_WIDTH*8);
	glTexCoord2f(1.5f, 1.0f); glVertex3f(xEye, CELL_WIDTH*7,zEye + CELL_WIDTH*11);
	glEnd();

	//����
	glMaterialfv(GL_FRONT, GL_AMBIENT, normal_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, normal_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, normal_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, normal_shininess);

	//����̺
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,2,zEye + CELL_WIDTH*8);
	glTexCoord2f(0.0f, 6.0f); glVertex3f(xEye + CELL_WIDTH*6,2,zEye + CELL_WIDTH*14);
	glTexCoord2f(12.0f, 6.0f); glVertex3f(xEye - CELL_WIDTH*6,2,zEye + CELL_WIDTH*14);
	glTexCoord2f(12.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*6,2,zEye + CELL_WIDTH*8);
	glEnd();
	glPopMatrix();

	glPopMatrix();
	glPopAttrib();
    //�ָ�����״̬
    if (!tp)
        glDisable(GL_TEXTURE_2D);
    if (!lp)
        glDisable(GL_LIGHTING);
}

void House::DrawFurniture()
{
	GLboolean lp, tp;
    glGetBooleanv(GL_LIGHTING, &lp);
    glGetBooleanv(GL_TEXTURE_2D, &tp);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPushAttrib(GL_CURRENT_BIT);   //���浱ǰ����

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	//����
	glMaterialfv(GL_FRONT, GL_AMBIENT, wood_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wood_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wood_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, wood_shininess);
	//������
	glPushMatrix();
	glTranslatef(xEye - CELL_WIDTH*5,yEye-30,zEye+CELL_WIDTH*13);
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	gluCylinder(quadratic,3.0,3.0,10.0,30.0,30.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(xEye - CELL_WIDTH*3.5,yEye-30,zEye+CELL_WIDTH*13);
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	gluCylinder(quadratic,3.0,3.0,10.0,30.0,30.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(xEye - CELL_WIDTH*3.5,yEye-30,zEye+CELL_WIDTH*10.2);
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	gluCylinder(quadratic,3.0,3.0,10.0,30.0,30.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(xEye - CELL_WIDTH*5,yEye-30,zEye+CELL_WIDTH*10.2);
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	gluCylinder(quadratic,3.0,3.0,10.0,30.0,30.0);
	glPopMatrix();
	//����
	glMaterialfv(GL_FRONT, GL_AMBIENT, ruby_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ruby_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ruby_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, ruby_shininess);
	//������
	glPushMatrix();
	glTranslatef(xEye - CELL_WIDTH*4.3,yEye-20,zEye+CELL_WIDTH*11.5);
	glScalef(3.4f, 0.3f, 5.0f);
	glutSolidCube(10.0f);
	glPopMatrix();
	if (m_flag) //��һ�ײ���
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, jade_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, jade_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, jade_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, jade_shininess);
	}
	else //�ڶ��ײ���
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, silver_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, silver_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, silver_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, silver_shininess);
	}
	//Բ��1
	glPushMatrix();
	glTranslatef(xEye - CELL_WIDTH*4.3,yEye-13,zEye+CELL_WIDTH*11.5);
	glRotatef(-90.0f + rotate,1.0f,0.0f,0.0f);
	glutSolidTorus(1, 4, 30, 30); 
	glPopMatrix();

	if (m_flag) //��һ�ײ���
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, chrome_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, chrome_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, chrome_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, chrome_shininess);
	}
	else //�ڶ��ײ���
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, gold_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, gold_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, gold_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, gold_shininess);
	}
	//Բ��2
	glPushMatrix();
	glTranslatef(xEye - CELL_WIDTH*4.3,yEye-13,zEye+CELL_WIDTH*12.5);
	glRotatef(-90.0f + rotate,1.0f,0.0f,0.0f);
	glutSolidTorus(1, 4, 30, 30); 
	glPopMatrix();
	
	if (m_flag) //��һ�ײ���
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, violet_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, violet_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, violet_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, violet_shininess);
	}
	else //�ڶ��ײ���
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, bronze_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, bronze_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, bronze_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, bronze_shininess);
	}
	//Բ��3
	glPushMatrix();
	glTranslatef(xEye - CELL_WIDTH*4.3,yEye-13,zEye+CELL_WIDTH*10.5);
	glRotatef(-90.0f + rotate,1.0f,0.0f,0.0f);
	glutSolidTorus(1, 4, 30, 30); 
	glPopMatrix();

	//�����1
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	//����
	glMaterialfv(GL_FRONT, GL_AMBIENT, wood_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wood_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wood_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, wood_shininess);
	//����
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*5,2,zEye + CELL_WIDTH*12);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*5,CELL_WIDTH*2,zEye + CELL_WIDTH*12);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*2,zEye + CELL_WIDTH*12);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*6,2,zEye + CELL_WIDTH*12);
	glEnd();
	glPopMatrix();

	//����
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*5,CELL_WIDTH*2,zEye + CELL_WIDTH*12);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*5,CELL_WIDTH*2,zEye + CELL_WIDTH*14);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*2,zEye + CELL_WIDTH*14);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*2,zEye + CELL_WIDTH*12);
	glEnd();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	//����
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*5,2,zEye + CELL_WIDTH*12);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*5,CELL_WIDTH*2,zEye + CELL_WIDTH*12);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*5,CELL_WIDTH*2,zEye + CELL_WIDTH*14);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*5,2,zEye + CELL_WIDTH*14);
	glEnd();
	glPopMatrix();

	//�����2
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	//����
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*5,2,zEye + CELL_WIDTH*10);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*5,CELL_WIDTH*2,zEye + CELL_WIDTH*10);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*2,zEye + CELL_WIDTH*10);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*6,2,zEye + CELL_WIDTH*10);
	glEnd();
	glPopMatrix();

	//����
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*5,CELL_WIDTH*2,zEye + CELL_WIDTH*8);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*5,CELL_WIDTH*2,zEye + CELL_WIDTH*10);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*2,zEye + CELL_WIDTH*10);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*6,CELL_WIDTH*2,zEye + CELL_WIDTH*8);
	glEnd();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	//����
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*5,2,zEye + CELL_WIDTH*8);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*5,CELL_WIDTH*2,zEye + CELL_WIDTH*8);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*5,CELL_WIDTH*2,zEye + CELL_WIDTH*10);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*5,2,zEye + CELL_WIDTH*10);
	glEnd();
	glPopMatrix();

	//����
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*3,CELL_WIDTH*2,zEye + CELL_WIDTH*8-0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye - CELL_WIDTH*3,CELL_WIDTH*3,zEye + CELL_WIDTH*8-0.1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xEye - CELL_WIDTH*4,CELL_WIDTH*3,zEye + CELL_WIDTH*8-0.1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*4,CELL_WIDTH*2,zEye + CELL_WIDTH*8-0.1);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*3,CELL_WIDTH*2,zEye + CELL_WIDTH*8-0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*3,CELL_WIDTH*3,zEye + CELL_WIDTH*8-0.1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*4,CELL_WIDTH*3,zEye + CELL_WIDTH*8-0.1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*4,CELL_WIDTH*2,zEye + CELL_WIDTH*8-0.1);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*2,CELL_WIDTH*2,zEye + CELL_WIDTH*14-0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye - CELL_WIDTH*2,CELL_WIDTH*3,zEye + CELL_WIDTH*14-0.1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xEye - CELL_WIDTH*3,CELL_WIDTH*3,zEye + CELL_WIDTH*14-0.1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*3,CELL_WIDTH*2,zEye + CELL_WIDTH*14-0.1);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*2,CELL_WIDTH*2,zEye + CELL_WIDTH*14-0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*2,CELL_WIDTH*3,zEye + CELL_WIDTH*14-0.1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*3,CELL_WIDTH*3,zEye + CELL_WIDTH*14-0.1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*3,CELL_WIDTH*2,zEye + CELL_WIDTH*14-0.1);
	glEnd();
	glPopMatrix();

	//���ͻ�
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xEye - CELL_WIDTH*0.625,CELL_WIDTH*2,zEye + CELL_WIDTH*14-0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xEye - CELL_WIDTH*0.625,CELL_WIDTH*3.25,zEye + CELL_WIDTH*14-0.1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xEye + CELL_WIDTH*0.625,CELL_WIDTH*3.25,zEye + CELL_WIDTH*14-0.1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xEye + CELL_WIDTH*0.625,CELL_WIDTH*2,zEye + CELL_WIDTH*14-0.1);
	glEnd();
	glPopMatrix();

	glPopAttrib();
    //�ָ�����״̬
    if (!tp)
        glDisable(GL_TEXTURE_2D);
    if (!lp)
        glDisable(GL_LIGHTING);
}

//�����������
void House::DrawGrid()
{
    //��ó�����״̬
    GLboolean  lp, tp;
    glGetBooleanv(GL_LIGHTING, &lp);
    glGetBooleanv(GL_TEXTURE_2D, &tp);
    //�ر�����͹���
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glPushAttrib(GL_CURRENT_BIT);   //���浱ǰ����
    glPushMatrix();                 //ѹ���ջ
    glTranslatef(0.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);    //������ɫ
    //��X,Zƽ���ϻ�������
    for (float i = 0; i <= MAP_WIDTH; i += CELL_WIDTH)
    {
        //������
        glBegin(GL_LINES);
        //X�᷽��
        glVertex3f(0, 1, i);
        glVertex3f(MAP_WIDTH, 1, i);
        //Z�᷽��
        glVertex3f(i, 1, 0);
        glVertex3f(i, 1, MAP_WIDTH);
        glEnd();
    }
    glPopMatrix();
    glPopAttrib();
    //�ָ�����״̬
    if (tp)
        glEnable(GL_TEXTURE_2D);
    if (lp)
        glEnable(GL_LIGHTING);
}

void House::DrawScene()
{
	DrawCrystal();
	DrawTeapot();
	DrawDesk();
	DrawYard();
	DrawHouse();
	DrawFurniture();
	rotate += 1.0;
}