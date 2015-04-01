
// MinesweeperRobotDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMinesweeperRobotDlg �Ի���




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


// CMinesweeperRobotDlg ��Ϣ�������

BOOL CMinesweeperRobotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CheckDlgButton(IDC_EXPERT, BST_CHECKED);
	CheckDlgButton(IDC_RANDOMWHENNOWAY, BST_CHECKED);
	SetDlgItemInt(IDC_HEIGHT, 9);
	SetDlgItemInt(IDC_WIDTH, 9);
	SetDlgItemInt(IDC_MINES, 10);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMinesweeperRobotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMinesweeperRobotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMinesweeperRobotDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!CWnd::FindWindow(_T("Minesweeper"), NULL)) {
		AfxMessageBox(_T("û���ҵ�ɨ��"));
		return;
	}

	options = LoadOptions();
	if (options.m_rows == 999) return;
	options.m_mission = Options::WORK;
	CreateThread(NULL, 0, CRobot::BeginSweep, &options, NULL, NULL);
}

void CMinesweeperRobotDlg::OnBnClickedManual()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_STATICHEIGHT)->EnableWindow();
	GetDlgItem(IDC_STATICWIDTH)->EnableWindow();
	GetDlgItem(IDC_STATICMINES)->EnableWindow();
	GetDlgItem(IDC_HEIGHT)->EnableWindow();
	GetDlgItem(IDC_WIDTH)->EnableWindow();
	GetDlgItem(IDC_MINES)->EnableWindow();
}


void CMinesweeperRobotDlg::OnBnClickedPrimary()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_STATICHEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICWIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICMINES)->EnableWindow(FALSE);
	GetDlgItem(IDC_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_MINES)->EnableWindow(FALSE);
}


void CMinesweeperRobotDlg::OnBnClickedMiddle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_STATICHEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICWIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICMINES)->EnableWindow(FALSE);
	GetDlgItem(IDC_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_MINES)->EnableWindow(FALSE);
}


void CMinesweeperRobotDlg::OnBnClickedExpert()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_STATICHEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICWIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATICMINES)->EnableWindow(FALSE);
	GetDlgItem(IDC_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_MINES)->EnableWindow(FALSE);
}

void CMinesweeperRobotDlg::OnBnClickedTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!CWnd::FindWindow(_T("Minesweeper"), NULL)) {
		AfxMessageBox(_T("û���ҵ�ɨ��"));
		return;
	}

	options = LoadOptions();
	if (options.m_rows == 999) return;
	options.m_mission = Options::SCAN;
	CreateThread(NULL, 0, CRobot::BeginSweep, &options, NULL, NULL);
}


void CMinesweeperRobotDlg::OnBnClickedSolve()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!CWnd::FindWindow(_T("Minesweeper"), NULL)) {
		AfxMessageBox(_T("û���ҵ�ɨ��"));
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
			AfxMessageBox(_T("����������ַ�Χ����"));
			toptions.m_rows = 999;
		}
	}

	toptions.m_animation = ((CButton*) GetDlgItem(IDC_ANIMATION))->GetCheck();
	toptions.m_RandomWhenNoWay = ((CButton*) GetDlgItem(IDC_RANDOMWHENNOWAY))->GetCheck();
	return toptions;
}
