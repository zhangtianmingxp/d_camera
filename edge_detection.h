#pragma once

#include "camera_head.h"
// edge_detection 对话框

class edge_detection : public CDialogEx
{
	DECLARE_DYNAMIC(edge_detection)

public:
	edge_detection(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~edge_detection();
	virtual int byjc(int argc, const char** argv);
	edge_detection *p2;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
