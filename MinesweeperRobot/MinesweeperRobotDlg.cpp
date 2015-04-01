
// MinesweeperRobotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MinesweeperRobot.h"
#include "MinesweeperRobotDlg.h"
#include "Robot.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Options options;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMinesweeperRobotDlg 对话框




CMinesweeperRobotDlg::CMinesweeperRobotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMinesweeperRobotDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMinesweeperRobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMinesweeperRobotDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMinesweeperRobotDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_MANUAL, &CMinesweeperRobotDlg::OnBnClickedManual)
	ON_BN_CLICKED(IDC_PRIMARY, &CMinesweeperRobotDlg::OnBnClickedPrimary)
	ON_BN_CLICKED(IDC_MIDDLE, &CMinesweeperRobotDlg::OnBnClickedMiddle)
	ON_BN_CLICKED(IDC_EXPERT, &CMinesweeperRobotDlg::OnBnClickedExpert)
	ON_BN_CLICKED(IDC_TEST, &CMinesweeperRobotDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_SOLVE, &CMinesweeperRobotDlg::OnBnClickedSolve)
END_MESSAGE_MAP()


// CMinesweeperRobotDlg 消息处理程序

BOOL CMinesweeperRobotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CheckDlgButton(IDC_EXPERT, BST_CHECKED);
	CheckDlgButton(IDC_RANDOMWHENNOWAY, BST_CHECKED);
	SetDlgItemInt(IDC_HEIGHT, 9);
	SetDlgItemInt(IDC_WIDTH, 9);
	SetDlgItemInt(IDC_MINES, 10);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMinesweeperRobotDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMinesweeperRobotDlg::OnPaint()
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
HCURSOR CMinesweeperRobotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMinesweeperRobotDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!CWnd::FindWindow(_T("Minesweeper"), NULL)) {
		AfxMessageBox(_T("没有找到扫雷"));
		return;
	}

	options = LoadOptions();
	if (options.m_rows == 999) return;
	options.m_mission = Options::WORK;
	CreateThread(NULL, 0, CRobot::BeginSweep, &options, NULL, NULL);
}

void CMinesweeperRobotDlg::OnBnClickedManual()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_STATICHEIGHT)->EnableWindow();
	GetDlgItem(IDC_STATICWIDTH)->EnableWindow();
	GetDlgItem(IDC_STATICMINES)->EnableWindow();
	GetDlgItem(IDC_HEIGHT)->EnableWindow();
	GetDlgItem(IDC_WIDTH)->EnableWindow();
	GetDlgItem(IDC_MINES)->EnableWindow();
}


void CMinesweeperRobotDlg::OnBnClickedPrimary()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_STATICHEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICWIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICMINES)->EnableWindow(FALSE);
	GetDlgItem(IDC_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_MINES)->EnableWindow(FALSE);
}


void CMinesweeperRobotDlg::OnBnClickedMiddle()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_STATICHEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICWIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICMINES)->EnableWindow(FALSE);
	GetDlgItem(IDC_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_MINES)->EnableWindow(FALSE);
}


void CMinesweeperRobotDlg::OnBnClickedExpert()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_STATICHEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICWIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICMINES)->EnableWindow(FALSE);
	GetDlgItem(IDC_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_MINES)->EnableWindow(FALSE);
}

void CMinesweeperRobotDlg::OnBnClickedTest()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!CWnd::FindWindow(_T("Minesweeper"), NULL)) {
		AfxMessageBox(_T("没有找到扫雷"));
		return;
	}

	options = LoadOptions();
	if (options.m_rows == 999) return;
	options.m_mission = Options::SCAN;
	CreateThread(NULL, 0, CRobot::BeginSweep, &options, NULL, NULL);
}


void CMinesweeperRobotDlg::OnBnClickedSolve()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!CWnd::FindWindow(_T("Minesweeper"), NULL)) {
		AfxMessageBox(_T("没有找到扫雷"));
		return;
	}

	options = LoadOptions();
	if (options.m_rows == 999) return;
	options.m_mission = Options::SOLVE;
	CreateThread(NULL, 0, CRobot::BeginSweep, &options, NULL, NULL);

}


Options CMinesweeperRobotDlg::LoadOptions(void)
{
	Options toptions;

	if (((CButton*) GetDlgItem(IDC_PRIMARY))->GetCheck())
		toptions.m_difficulty = Algorithm::PRIMARY;
	if (((CButton*) GetDlgItem(IDC_MIDDLE))->GetCheck())
		toptions.m_difficulty = Algorithm::MIDDLE;
	if (((CButton*) GetDlgItem(IDC_EXPERT))->GetCheck())
		toptions.m_difficulty = Algorithm::EXPERT;
	if (((CButton*) GetDlgItem(IDC_MANUAL))->GetCheck())
	{
		toptions.m_difficulty = Algorithm::MANUAL;
		toptions.m_rows = GetDlgItemInt(IDC_HEIGHT);
		toptions.m_columns = GetDlgItemInt(IDC_WIDTH);
		toptions.m_mines = GetDlgItemInt(IDC_MINES);
			int height = GetDlgItemInt(IDC_HEIGHT);
		if (toptions.m_rows < 9 || toptions.m_rows > 24 ||
			toptions.m_columns < 9 || toptions.m_columns > 30 ||
			toptions.m_mines < 10 || toptions.m_mines > 668)
		{
			AfxMessageBox(_T("您输入的数字范围有误！"));
			toptions.m_rows = 999;
		}
	}

	toptions.m_animation = ((CButton*) GetDlgItem(IDC_ANIMATION))->GetCheck();
	toptions.m_RandomWhenNoWay = ((CButton*) GetDlgItem(IDC_RANDOMWHENNOWAY))->GetCheck();
	return toptions;
}
