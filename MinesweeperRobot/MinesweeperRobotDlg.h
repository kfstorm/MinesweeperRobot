
// MinesweeperRobotDlg.h : ͷ�ļ�
//

#pragma once

#include "Options.h"

// CMinesweeperRobotDlg �Ի���
class CMinesweeperRobotDlg : public CDialogEx
{
// ����
public:
	CMinesweeperRobotDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MINESWEEPERROBOT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
