
// cameraDlg.h: 头文件
//
#pragma once
#include<opencv2/highgui/highgui.hpp> 
using namespace std;
using namespace cv;
// CcameraDlg 对话框
class CcameraDlg : public CDialogEx
{
// 构造
public:
	CcameraDlg(CWnd* pParent = nullptr);	// 标准构造函数
	Mat frame  ,frame_R, frame_L;               //定义采集帧
	IplImage img  ,img_R, img_L;            //定义转换到IplImage图片的类
	VideoCapture capture;    //定义视频显示变量
	CStatic m_pic;           //定义图片控件变量
	int pic_save = 0;               //定义采集图像变量
	bool cameraopen;           //定义相机是否开启
	void CcameraDlg::drawToDC(IplImage* image, UINT ID);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
