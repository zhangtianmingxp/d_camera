
// cameraDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "camera.h"
#include "cameraDlg.h"
#include "afxdialogex.h"
#include "catch_image.h"
#include "edge_detection.h"
#include "SGBM.h"
#include "CvvImage.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "CvvImage.h"
#include "afxwin.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define Image_Width  320    //【可能需要修改的程序参数2】：定义单目输出图像的宽度，相机支持的双目分辨率见产品参数表（双目分辨率的宽度除以2即为单目宽度）
#define Image_height 480     //【可能需要修改的程序参数3】：定义单目输出图像的高度



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcameraDlg 对话框
void CcameraDlg::drawToDC(IplImage* image, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC pHdc = pDC->GetSafeHdc();

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);

	CvvImage cimg;
	cimg.CopyOf(image);
	cimg.DrawToHDC(pHdc, rect);

	ReleaseDC(pDC);
}






CcameraDlg::CcameraDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAMERA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcameraDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CcameraDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CcameraDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CcameraDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &CcameraDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CcameraDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CcameraDlg::OnBnClickedButton7)
	ON_WM_TIMER()
	
END_MESSAGE_MAP()


// CcameraDlg 消息处理程序

BOOL CcameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcameraDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcameraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CcameraDlg::OnBnClickedButton2()
{
	catch_image *p1 = new catch_image;
	const char *argv[100] = { "你好","我好","大家好","good","better","very good","反正这里的东西用不上" };
	p1->yunxing(1, &argv[0]);
	delete p1;
}

	


void CcameraDlg::OnBnClickedButton3()
{
	edge_detection *p2 = new edge_detection;
	const char *argv[100] = { "你好","我好","大家好","good","better","very good","反正这里的东西用不上" };
	p2->byjc(1, &argv[0]);
	delete p2;
}




void CcameraDlg::OnBnClickedButton5()
{
	SGBM *p4 = new SGBM;

	p4->main3();
	delete p4;
}


void CcameraDlg::OnBnClickedButton4()
{
	cameraopen = true;
	capture = 0;  //打开摄像头
	capture.set(CV_CAP_PROP_FRAME_WIDTH, Image_Width * 2); //设置捕获图像的宽度，为双目图像的宽度
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, Image_height);  //设置捕获图像的高度
	if (capture.isOpened())
	{
		
		capture >> frame;    //输出采集的帧图像
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
		IplImage img_L = frame_L;
		IplImage img_R = frame_R;
		drawToDC(&img_R, IDC_STATIC_RIGHT);
		drawToDC(&img_L, IDC_STATIC_LEFT);
	}
	SetTimer(1, 10, NULL);   //设置定时器
}


void CcameraDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	cameraopen = false;
	capture.release();    //关闭视频采集
	destroyAllWindows();
	Invalidate();    //刷新窗口
}

void CcameraDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (cameraopen)
	{
		pic_save++;
		char file_name[100];
		sprintf_s(file_name, "Left%u.bmp", pic_save);
		imwrite(file_name, frame_L);
		sprintf_s(file_name, "Right%u.bmp", pic_save);
		imwrite(file_name, frame_R);
		MessageBox(_T("抓取图片成功！"));
	}
	else
		MessageBox(_T("相机未打开！"));
	
}


void CcameraDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	capture >> frame;
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
	IplImage img_L = frame_L;
	IplImage img_R = frame_R;
	drawToDC(&img_L, IDC_STATIC_LEFT);
	drawToDC(&img_R, IDC_STATIC_RIGHT);
	CDialogEx::OnTimer(nIDEvent);
}

