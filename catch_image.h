#pragma once

#include "camera_head.h"


// catch_image 对话框

class catch_image : public CDialogEx
{
	DECLARE_DYNAMIC(catch_image)

public:
	catch_image(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~catch_image();
    
	virtual int yunxing(int argc, const char** argv);
	catch_image *p1;
	void show_catch();


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
