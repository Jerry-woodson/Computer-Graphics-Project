#include<windows.h>
#include<gl/glut.h>
#include<stdio.h>

#define BITMAP_ID 0x4D42		// the universal bitmap ID
int width=0,height=0;//���ڵĸ߶ȺͿ��
BITMAPINFOHEADER bitmapInfoHeader;	// λͼ�ļ���Ϣͷbitmap info header
unsigned char* bitmapData;		// ָ��λͼ�������ݵ�ָ��

// LoadBitmapFile��ȡָ�����Ƶ�bmp�ļ����������ļ��������ݼ�λͼͷ��Ϣ��֧��24λλͼ�ļ�
//λͼ�ļ����������ǽ��������ݰ��д�ţ���һ����������ռ���ֽ�������4�ı���ʱ��Ҫ������ֽڣ���һ�����ݴճ�4�ֽڵı���
//ÿ�����ص�3����ɫ��������BGR˳���ţ�Ϊ�˷���ʹ�ã�һ�㽫B��R��������
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;								// �ļ�ָ��
	BITMAPFILEHEADER	bitmapFileHeader;		// λͼ�ļ�ͷ
	unsigned char		*bitmapImage;			// λͼͼ������
	int					imageIdx = 0;			// ͼ��������������ѭ�����������ã�
	unsigned char		tempRGB;				// ��������ʱ����

	// �Զ����ƶ���ʽ���ļ�
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// ��ȡλͼ�ļ�ͷ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// ��ȡλͼ��Ϣͷ
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// ���ļ�ָ���ƶ���λͼ���ݿ�ʼλ��
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Ϊλͼͼ�����ݷ����㹻���ڴ�
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);//bitmapInfoHeader->biSizeImageΪ�ļ��д洢��λͼ�����ֽ���

	// ��֤�ڴ����verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	//����λͼͼ������
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// ȷ��λͼ���ݶ���
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// ����BMP�ļ���ʵ�ʴ洢��ʽΪBGR�����Խ���R��B����
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)//
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// close the file and return the bitmap image data
	fclose(filePtr);
	return bitmapImage;
}



void display()//�貹�����
{
	glClear(GL_COLOR_BUFFER_BIT);//��������
                 int n=bitmapInfoHeader.biWidth;//��ȡλͼ��һ��������
	int width=...;//����n����ͼ��һ������ռ���ֽ���������windowsλͼ����Ĺ涨��һ���ֽ���������4�ı������������룩
	glBegin(GL_POINTS);//������
	    for(int i=0;i<bitmapInfoHeader.biHeight&&i<height;i++)//�������н���ѭ��
			for(int j=0;j<n&&j<width;j++)//��������ѭ��
			{
				//��֪ÿ�������ֽ���Ϊwidth������һά����bitmapData
				//�ж�Ӧ��i��j�е���������λ�ò�ȡ��RGB������
				int r,g,b;
				r=... ;//ȡ��R����
				g=... ;//ȡ��G����
				b=... ;//ȡ��B������windowsλͼ�ļ���RGB��������ΧΪ0-255

				glColor3f(... ,... ,... );//ת��Ϊopengl��0-1����ɫ������Χ
				glVertex2f(j,i); //���ö�Ӧ���ص���ɫ
			}		
	glEnd();
	glFlush();
}

//�ػ�ص�������w--��ǰ����Ŀ�ȣ�h--��ǰ����ĸ߶�
//GLsizei--OpenGL�Զ����������ͣ���ʾ������ר���ڱ�ʾ�ߴ���ر���
void reshape(GLsizei w, GLsizei h) 
{
	width=w;//����ǰ���ڵĳߴ��¼��ȫ�ֱ���width��height��
	height=h;
	//�����ӿڴ�С
	glViewport(0, 0, w, h); 
	//ָ������ģʽ��GL_PROJECTION / GL_MODELVIEW
	glMatrixMode(GL_PROJECTION); 
	//����������Ϊ��λ����
	glLoadIdentity(); 
	//ָ���ü����ڴ�С
	gluOrtho2D (0,w-1,0,h-1); //Ϊ�˾��ȣ����޸ĸ���ֵ
	glMatrixMode(GL_MODELVIEW); 
}

int main(int argc,char **argv) 
{
	glutInit(&argc,argv);
    //��ʼ��glut���߰�����ʾģʽ��������|RGB��ɫģʽ
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
     //ָ�������С����λ����
	glutInitWindowSize(500,500);
 	//ָ���������Ͻǳ�ʼλ�ã���λ����
	glutInitWindowPosition(0,0);	
    //��������Ϊ"simple"����ʾ����
	glutCreateWindow("simple"); 
	glutDisplayFunc(display); 	//ע���ػ�ص�����
	glutReshapeFunc(reshape); 
	bitmapData = LoadBitmapFile("test.bmp", &bitmapInfoHeader);//����test.bmp�ļ���������ͼ�����ݣ�bitmapDataָ��λͼ��������
	glutMainLoop();
    return 0;
}