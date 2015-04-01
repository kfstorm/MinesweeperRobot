
// MinesweeperRobotDlg.h : 头文件
//

#pragma once

#include "Options.h"

// CMinesweeperRobotDlg 对话框
class CMinesweeperRobotDlg : public CDialogEx
{
// 构造
public:
	CMinesweeperRobotDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MINESWEEPERROBOT_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedManual();
	afx_msg void OnBnClickedPrimary();
	afx_msg void OnBnClickedMiddle();
	afx_msg void OnBnClickedExpert();
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedSolve();
	Options LoadOptions(void);
};
