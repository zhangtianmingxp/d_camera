// edge_detection.cpp: 实现文件
//

#include "pch.h"
#include "camera.h"
#include "edge_detection.h"
#include "afxdialogex.h"
#define Image_Width   320     //【可能需要修改的程序参数2】：定义单目输出图像的宽度，相机支持的双目分辨率见产品参数表（双目分辨率的宽度除以2即为单目宽度）
#define Image_height  480      //【可能需要修改的程序参数3】：定义单目输出图像的高度

// edge_detection 对话框

IMPLEMENT_DYNAMIC(edge_detection, CDialogEx)

edge_detection::edge_detection(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAMERA_DIALOG, pParent)
{

}

int edge_detection::byjc(int argc, const char** argv)            //程序主函数
{
	CommandLineParser parser(argc, argv, keys);
	parser.about("Video Capture");

	if (parser.has("help"))                      //帮助信息
	{
		parser.printMessage();
		return 0;
	}
	String videoFile = parser.get<String>(0);

	if (!parser.check())
	{
		parser.printErrors();
		return 0;
	}

	VideoCapture cap;
	if (videoFile != "")
	{
		cap.open(videoFile);
	}
	else
	{
		cap.open(0);    //【可能需要修改的程序参数4】：打开端口1对应的设备，电脑自带摄像头一般编号为0，外接摄像头编号为1，也可能是反过来的

		cap.set(CV_CAP_PROP_FRAME_WIDTH, Image_Width * 2); //设置捕获图像的宽度，为双目图像的宽度
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, Image_height);  //设置捕获图像的高度
	}
	if (!cap.isOpened())                                  //判断是否成功打开相机
	{
		return -1;
	}

	Mat frame, frame_L, frame_R;

	cap >> frame;                                         //从相机捕获一帧图像

	double fScale = 0.5;                                  //定义缩放系数，可对图像进行缩放显示  
	Size dsize = Size(Image_Width / 2, Image_height / 2);
	Mat image_L_resize = Mat(dsize, CV_32S);              //定义矩阵用于存放缩放后的图像
	Mat image_R_resize = Mat(dsize, CV_32S);
	char file_name[100];                                  //定义文件名称，用于自动保存图像时命名，100表示抓拍的图像不超过100张
	uchar num = 1;

	uchar Resize_Flag = 0;
	int key = 0;                                          //键盘按下按键后获取的键值

	Mat Gray_L, Gray_R;                                   //用于存放灰度数据
	Mat Gray_Resize_L, Gray_Resize_R;                     //用于存放灰度数据

	while (1)
	{

		key = waitKey(1);                                 //获取键盘按下后的键值

		cap >> frame;                                     //从相机捕获一帧图像

		if (HNY_CV_002 == 0)  //假设使用的设备型号不是：HNY-CV-002
		{
			frame_L = frame(Rect(0, 0, Image_Width, Image_height));             //获取左Camera的图像
			frame_R = frame(Rect(Image_Width, 0, Image_Width, Image_height));   //获取右Camera的图像
		}
		else                  //假设使用的设备型号是：HNY-CV-002
		{
			frame_L = frame(Rect(Image_Width, 0, Image_Width, Image_height));   //获取左Camera的图像
			frame_R = frame(Rect(0, 0, Image_Width, Image_height));             //获取右Camera的图像
		}

		cvtColor(frame_L, Gray_L, CV_BGR2GRAY);              //将左目源图像（彩色图）转为灰度数据
		Canny(Gray_L, Gray_L, 10, 100, 3);                   //对图像进行Canny算子的图像边缘检测
		cvtColor(frame_R, Gray_R, CV_BGR2GRAY);              //将右目源图像（彩色图）转为灰度数据
		Canny(Gray_R, Gray_R, 10, 100, 3);                   //对图像进行Canny算子的图像边缘检测


		if (key == 'R')       //当左目与右目图像及其对应的canny效果图均显示在屏幕最前端时，按下大写字母R，则图像将被缩放后显示，再次按下则原比例显示。
		{
			if (Resize_Flag == 0) Resize_Flag = 1;           //标志位
			else Resize_Flag = 0;
			key = 0;
		}
		if (Resize_Flag == 1)
		{
			resize(frame_L, image_L_resize, dsize);          //对捕捉的左目图像进行缩放操作
			namedWindow("Video_L", 1);
			imshow("Video_L", image_L_resize);
			resize(frame_R, image_R_resize, dsize);          //对捕捉的右目图像进行缩放操作
			namedWindow("Video_R", 1);
			imshow("Video_R", image_R_resize);

			resize(Gray_L, Gray_Resize_L, dsize);            //缩放图像
			namedWindow("Canny_L", 1);                       //实时显示左目Canny边缘检测的效果图
			imshow("Canny_L", Gray_Resize_L);
			resize(Gray_R, Gray_Resize_R, dsize);            //缩放图像
			namedWindow("Canny_R", 1);                       //实时显示右目Canny边缘检测的效果图
			imshow("Canny_R", Gray_Resize_R);
		}
		else
		{
			namedWindow("Video_L", 1);                       //对图像进行原比例显示
			imshow("Video_L", frame_L);
			namedWindow("Video_R", 1);
			imshow("Video_R", frame_R);

			namedWindow("Canny_L", 1);                       //实时显示左目Canny边缘检测的效果图
			imshow("Canny_L", Gray_L);
			namedWindow("Canny_R", 1);                       //实时显示右目Canny边缘检测的效果图
			imshow("Canny_R", Gray_R);
		}

		if (key == 'C')    //当左目与右目图像及其对应的canny效果图均显示在屏幕最前端时，按下大写字母C，将左目与右目原始图像（非缩放图）抓拍并存储在Mian.cpp同级目录的文件夹。
		{
			sprintf_s(file_name, "Left%u.bmp", num);
			imwrite(file_name, frame_L);
			sprintf_s(file_name, "Right%u.bmp", num);
			imwrite(file_name, frame_R);

			sprintf_s(file_name, "Left_Canny_%u.bmp", num);
			imwrite(file_name, Gray_L);
			sprintf_s(file_name, "Right_Canny_%u.bmp", num);
			imwrite(file_name, Gray_R);

			num++;
			key = 0;
		}
	}

	cap.release();         //释放对相机的控制
	return 0;
}


edge_detection::~edge_detection()
{
}

void edge_detection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(edge_detection, CDialogEx)
END_MESSAGE_MAP()


// edge_detection 消息处理程序
