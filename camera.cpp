#include "Camera.h" 

//���캯��
Camera::Camera()
{
    Vector3 zero = Vector3(0.0, 0.0, 0.0);      
    Vector3 view = Vector3(0.0, 1.0, 0.5);      
    Vector3 up   = Vector3(0.0, 0.0, 1.0);      
    //��ʼ�������
    m_Position  = zero;                 
    m_View      = view;             
    m_UpVector  = up;   
    m_Speed     = 1.1f;
}

Camera::~Camera()
{
}

//�����������λ��,�������������
void Camera::setCamera( float positionX, float positionY, float positionZ,
                        float viewX,     float viewY,     float viewZ,
                        float upVectorX, float upVectorY, float upVectorZ)
{
    //��������
    Vector3 Position    = Vector3(positionX, positionY, positionZ);
    Vector3 View        = Vector3(viewX, viewY, viewZ);
    Vector3 UpVector    = Vector3(upVectorX, upVectorY, upVectorZ);
    //���������
    m_Position = Position;  
    m_View     = View;          
    m_UpVector = UpVector;  
}

//��ת���������
void Camera::rotateView(float angle, float x, float y, float z)
{
    Vector3 newView;
    //���㷽������
    Vector3 view = m_View - m_Position;
    //����sin��cos
    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);
    //������ת������xֵ
    newView.x  = (cosTheta + (1 - cosTheta) * x * x)        * view.x;
    newView.x += ((1 - cosTheta) * x * y - z * sinTheta)    * view.y;
    newView.x += ((1 - cosTheta) * x * z + y * sinTheta)    * view.z;
    //������ת������yֵ
    newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)    * view.x;
    newView.y += (cosTheta + (1 - cosTheta) * y * y)        * view.y;
    newView.y += ((1 - cosTheta) * y * z - x * sinTheta)    * view.z;
    //������ת������zֵ
    newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)    * view.x;
    newView.z += ((1 - cosTheta) * y * z + x * sinTheta)    * view.y;
    newView.z += (cosTheta + (1 - cosTheta) * z * z)        * view.z;
    //����������ķ���
    m_View = m_Position + newView;
}

//�������ת�����
void Camera::setViewByMouse()
{
    POINT mousePos;                                   //���浱ǰ���λ��
    int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; //�õ���Ļ��ȵ�һ��
    int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1; //�õ���Ļ�߶ȵ�һ��
    float angleY = 0.0f;                              //�����������ת�Ƕ�
    float angleZ = 0.0f;                              //�����������ת�Ƕ�               
    static float currentRotX = 0.0f;
    //�õ���ǰ���λ��
    GetCursorPos(&mousePos);                        
    ShowCursor(TRUE);
    //������û���ƶ�,���ø���
    if( (mousePos.x == middleX) && (mousePos.y == middleY) )
        glutPostRedisplay();
    //�������λ������Ļ����
    SetCursorPos(middleX, middleY); 
    //�õ�����ƶ�����
    angleY = (float)( (middleX - mousePos.x) ) / 500.0f;       
    angleZ = (float)( (middleY - mousePos.y) ) / 500.0f;       
    static float lastRotX = 0.0f;      //���ڱ�����ת�Ƕ�
    lastRotX = currentRotX; 
    //���������������ת�Ƕ�
    currentRotX += angleZ;
    //���������ת���ȴ���1.0,���ǽ�ȡ��1.0����ת
    if(currentRotX > 1.0f)     
    {
        currentRotX = 1.0f;
        //���ݱ���ĽǶ���ת����
        if(lastRotX != 1.0f) 
        {
            //ͨ������ҵ�����ת����ֱ������
            Vector3 vAxis = m_View - m_Position;
            vAxis = vAxis.crossProduct(m_UpVector);
            vAxis = vAxis.normalize();
            ///��ת
            rotateView( 1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
        }
    }
    //�����ת����С��-1.0,��Ҳ��ȡ��-1.0����ת
    else if(currentRotX < -1.0f)
    {
        currentRotX = -1.0f;
        if(lastRotX != -1.0f)
        {
            //ͨ������ҵ�����ת����ֱ������
            Vector3 vAxis = m_View - m_Position;
            vAxis = vAxis.crossProduct(m_UpVector);
            vAxis = vAxis.normalize();
            ///��ת
            rotateView( -1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
        }
    }
    //�������תangleZ��
    else 
    {   
        //�ҵ�����ת����ֱ����
        Vector3 vAxis = m_View - m_Position;
        vAxis = vAxis.crossProduct(m_UpVector);
        vAxis = vAxis.normalize();
        ///��ת
        rotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
    }
    //����������ת�����
    rotateView(angleY, 0, 1, 0);
}

//�����ƶ������
void Camera::yawCamera(float speed)
{
    Vector3 yaw;
    Vector3 cross = m_View - m_Position;
    cross = cross.crossProduct(m_UpVector);
    //��λ��
    yaw = cross.normalize();
    m_Position.x += yaw.x * speed;
    m_Position.z += yaw.z * speed;

    m_View.x += yaw.x * speed;
    m_View.z += yaw.z * speed;
}

//ǰ���ƶ������
void Camera::moveCamera(float speed)
{
    //���㷽������
    Vector3 vector = m_View - m_Position;
    vector = vector.normalize();         //��λ��
    //���������
    m_Position.x += vector.x * speed;    //�����ٶȸ���λ��
    m_Position.z += vector.z * speed;
    m_View.x += vector.x * speed;        //�����ٶȸ��·���
    m_View.z += vector.z * speed; 
}


//�����ӵ�
void Camera::setLook()
{
    gluLookAt(m_Position.x, m_Position.y, m_Position.z, 
              m_View.x,  m_View.y,     m_View.z,    
              m_UpVector.x, m_UpVector.y, m_UpVector.z);
}