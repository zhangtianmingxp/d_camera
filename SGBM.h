#pragma once

#include "camera_head.h"
// SGBM 对话框

class SGBM : public CDialogEx
{
	DECLARE_DYNAMIC(SGBM)

public:
	SGBM(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SGBM();
	int main3();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
