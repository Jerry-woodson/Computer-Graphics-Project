#include<windows.h>
#include<gl/glut.h>
#include<stdio.h>

#define BITMAP_ID 0x4D42		// the universal bitmap ID
int width=0,height=0;//窗口的高度和宽度
BITMAPINFOHEADER bitmapInfoHeader;	// 位图文件信息头bitmap info header
unsigned char* bitmapData;		// 指向位图像素数据的指针

// LoadBitmapFile读取指定名称的bmp文件，并返回文件像素数据及位图头信息。支持24位位图文件
//位图文件像素数据是将像素数据按行存放，当一行像素数据占用字节数不是4的倍数时，要补齐空字节，把一行数据凑成4字节的倍数
//每个像素的3个颜色分量按照BGR顺序存放，为了方便使用，一般将B和R分量交换
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;								// 文件指针
	BITMAPFILEHEADER	bitmapFileHeader;		// 位图文件头
	unsigned char		*bitmapImage;			// 位图图像数据
	int					imageIdx = 0;			// 图像索引计数器（循环遍历像素用）
	unsigned char		tempRGB;				// 交换用临时变量

	// 以二进制读方式打开文件
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// 读取位图文件头
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// 读取位图信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// 将文件指针移动到位图数据开始位置
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// 为位图图像数据分配足够的内存
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);//bitmapInfoHeader->biSizeImage为文件中存储的位图数据字节数

	// 验证内存分配verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	//读入位图图像数据
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// 确保位图数据读入
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// 由于BMP文件中实际存储格式为BGR，所以交换R和B分量
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



void display()//需补充完成
{
	glClear(GL_COLOR_BUFFER_BIT);//擦除背景
                 int n=bitmapInfoHeader.biWidth;//读取位图的一行像素数
	int width=...;//根据n计算图像一行像素占用字节数（由于windows位图特殊的规定，一行字节数必须是4的倍数，不足则补齐）
	glBegin(GL_POINTS);//绘制线
	    for(int i=0;i<bitmapInfoHeader.biHeight&&i<height;i++)//按像素行进行循环
			for(int j=0;j<n&&j<width;j++)//按像素列循环
			{
				//已知每行像素字节数为width，计算一维数组bitmapData
				//中对应第i行j列的像素数据位置并取出RGB分量。
				int r,g,b;
				r=... ;//取出R分量
				g=... ;//取出G分量
				b=... ;//取出B分量，windows位图文件中RGB各分量范围为0-255

				glColor3f(... ,... ,... );//转换为opengl的0-1的颜色分量范围
				glVertex2f(j,i); //设置对应像素的颜色
			}		
	glEnd();
	glFlush();
}

//重绘回调函数：w--当前窗体的宽度；h--当前窗体的高度
//GLsizei--OpenGL自定义数据类型，表示整数，专用于表示尺寸相关变量
void reshape(GLsizei w, GLsizei h) 
{
	width=w;//将当前窗口的尺寸记录在全局变量width和height中
	height=h;
	//定义视口大小
	glViewport(0, 0, w, h); 
	//指定矩阵模式：GL_PROJECTION / GL_MODELVIEW
	glMatrixMode(GL_PROJECTION); 
	//将矩阵设置为单位矩阵
	glLoadIdentity(); 
	//指定裁剪窗口大小
	gluOrtho2D (0,w-1,0,h-1); //为了精度，需修改该数值
	glMatrixMode(GL_MODELVIEW); 
}

int main(int argc,char **argv) 
{
	glutInit(&argc,argv);
    //初始化glut工具包的显示模式：单缓冲|RGB颜色模式
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
     //指定窗体大小，单位像素
	glutInitWindowSize(500,500);
 	//指定窗体左上角初始位置，单位像素
	glutInitWindowPosition(0,0);	
    //创建名称为"simple"的显示窗体
	glutCreateWindow("simple"); 
	glutDisplayFunc(display); 	//注册重绘回调函数
	glutReshapeFunc(reshape); 
	bitmapData = LoadBitmapFile("test.bmp", &bitmapInfoHeader);//加载test.bmp文件并读入其图像数据，bitmapData指向位图像素数据
	glutMainLoop();
    return 0;
}