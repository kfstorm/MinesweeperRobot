// Robot.cpp : 实现文件
//

#include "stdafx.h"
#include "MinesweeperRobot.h"
#include "Robot.h"
#include "Options.h"

int HorzRes = CWnd::GetDesktopWindow()->GetDC()->GetDeviceCaps(HORZRES);
int VertRes = CWnd::GetDesktopWindow()->GetDC()->GetDeviceCaps(VERTRES);
COLORREF *pCapture = (COLORREF*) malloc(HorzRes * VertRes * 4);
CDC mDC;
CBitmap mBitmap;

// CRobot

const int OpenWait_WithAnimation = 200, OpenWait_WithoutAnimation = 100;
const int MarkMineWait = 30;
const int QuickOpenWait_WithAnimation = 200, QuickOpenWait_WithoutAnimation = 100;
const int AfterOperationsWait_WithAnimation = 400, AfterOperationsWait_WithoutAnimation = 50;

int OpenWait = OpenWait_WithoutAnimation,
	QuickOpenWait = QuickOpenWait_WithoutAnimation,
	AfterOperationsWait = AfterOperationsWait_WithoutAnimation;

//CDC *pDCGlobal;

CRobot::CRobot(Algorithm::Difficulty difficulty, int rows, int columns, int mines):
m_pWndMinesweeper(NULL), m_difficulty(difficulty), m_rows(0), m_columns(0), m_pAlgo(NULL)
{
	m_pWndMinesweeper = CWnd::FindWindow(_T("Minesweeper"), NULL);
	if (m_pWndMinesweeper) {
		switch (difficulty)
		{
		case Algorithm::PRIMARY:
			m_rows = m_columns = 9;
			m_mines = 10;
			break;
		case Algorithm::MIDDLE:
			m_rows = m_columns = 16;
			m_mines = 40;
			break;
		case Algorithm::EXPERT:
			m_rows = 16; m_columns = 30;
			m_mines = 99;
			break;
		case Algorithm::MANUAL:
			m_rows = rows; m_columns = columns;
			m_mines = mines;
			break;
		default:
			m_rows = m_columns = m_mines = 0;
			break;
		}
		GetMineMatrix();
		m_pAlgo = new Algorithm(difficulty, m_rows, m_columns, m_mines);
	}
	static bool init = false;
	if (!init)
	{
		CDC *pDesktopDC = CWnd::GetDesktopWindow()->GetDC();
		mDC.CreateCompatibleDC(pDesktopDC);
		mBitmap.CreateCompatibleBitmap(pDesktopDC, HorzRes, VertRes);
		mDC.SelectObject(&mBitmap);
		init = true;
	}

	//CWnd* pMinesweeper = CWnd::FindWindow(_T("Minesweeper"), NULL);
	//pMinesweeper->ShowWindow(SW_RESTORE);
	//theApp.m_pMainWnd->ShowWindow(SW_SHOWMINIMIZED);
	//Sleep(1000);
	//pMinesweeper->SetForegroundWindow();
	//Sleep(1000);
	//GetMatrixLines();
}

CRobot::~CRobot()
{
	free(m_pAlgo);
}

bool CRobot::GetMineMatrix()
{
//	static const int left = 30, right = 28, top = 31, bottom = 31;
//	static const int SquareEdge = 18;
	static const int left = 61, right = 59, top = 62, bottom = 64;
	static const int SquareEdge = 36;

	if (!m_pWndMinesweeper) return false;
	CRect rect;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);
	m_pWndMinesweeper->GetWindowRect(&m_RectMineMatrix);
	m_RectMineMatrix.top -= rect.top; m_RectMineMatrix.bottom -= rect.bottom;
	m_RectMineMatrix.left -= rect.left; m_RectMineMatrix.right -= rect.right;
	CRect rect2;
	m_pWndMinesweeper->GetClientRect(&rect2);

	CRect StandardClientRect(0, 0, left + SquareEdge * m_columns + 2 + right, top + SquareEdge * m_rows + 2 + bottom);
	
	TRACE(_T("StandardClientRect:   left: %d right: %d top: %d bottom: %d\n"), StandardClientRect.left, StandardClientRect.right, StandardClientRect.top, StandardClientRect.bottom);
	m_RectMineMatrix.left += (rect2.right - rect2.left) * left / (StandardClientRect.right - StandardClientRect.left);
	m_RectMineMatrix.right -= (rect2.right - rect2.left) * right / (StandardClientRect.right - StandardClientRect.left);
	m_RectMineMatrix.top += (rect2.bottom - rect2.top) * top / (StandardClientRect.bottom - StandardClientRect.top);
	m_RectMineMatrix.bottom -= (rect2.bottom - rect2.top) * bottom / (StandardClientRect.bottom - StandardClientRect.top);
	
	//m_RectMineMatrix.top += rect2.bottom * 0.0875; m_RectMineMatrix.left += rect2.right * 0.05;
	//m_RectMineMatrix.bottom -= rect2.bottom * 0.09; m_RectMineMatrix.right -= rect2.right * 0.05;

	return m_RectMineMatrix.bottom && m_RectMineMatrix.right;
}

CPoint CRobot::GetSquareLeftTopPoint(int row, int column)
{
	return CPoint(m_RectMineMatrix.left + (m_RectMineMatrix.right - m_RectMineMatrix.left) * column / m_columns,// + (column > (m_columns + 1) / 2 ? 1 : 0),
		m_RectMineMatrix.top + (m_RectMineMatrix.bottom - m_RectMineMatrix.top) * row / m_rows);// + (row > (m_rows + 1) / 2 ? 1 : 0));
}


CRect CRobot::GetSquareRect(int row, int column)
{
	//return CRect(	CPoint(m_RectMineMatrix.left + (m_RectMineMatrix.right - m_RectMineMatrix.left) * column / m_columns,
	//	m_RectMineMatrix.top + (m_RectMineMatrix.bottom - m_RectMineMatrix.top) * row / m_rows),
	//	CPoint(m_RectMineMatrix.left + (m_RectMineMatrix.right - m_RectMineMatrix.left) * (column + 1) / m_columns + 1,
	//	m_RectMineMatrix.top + (m_RectMineMatrix.bottom - m_RectMineMatrix.top) * (row + 1) / m_rows + 1));
	//return m_SquareRect[row][column];

	//static COLORREF BLACK = 0, BLUE = RGB(50, 84, 126);
	//CPoint point = GetCenterCoordinate(row, column);
	//CRect ret;
	//int x, y;
	//
	//x = point.x;
	//while (!SameColor(GetColor(x, point.y), BLACK, 2000) && !SameColor(GetColor(x, point.y), BLUE, 2000))
	//	--x;
	//ret.left = x;
	//x = point.x;
	//while (!SameColor(GetColor(x, point.y), BLACK, 2000) && !SameColor(GetColor(x, point.y), BLUE, 2000))
	//	++x;
	//ret.right = x;
	//y = point.y;
	//while (!SameColor(GetColor(point.x, y), BLACK, 2000) && !SameColor(GetColor(point.x, y), BLUE, 2000))
	//	--y;
	//ret.top = y;
	//y = point.y;
	//while (!SameColor(GetColor(point.x, y), BLACK, 2000) && !SameColor(GetColor(point.x, y), BLUE, 2000))
	//	++y;
	//ret.bottom = y;
	//
	//CRect ret2 = CRect(GetSquareLeftTopPoint(row, column), GetSquareLeftTopPoint(row + 1, column + 1));
	//return ret;
	return CRect(GetSquareLeftTopPoint(row, column), GetSquareLeftTopPoint(row + 1, column + 1));
}

CPoint CRobot::GetCenterCoordinate(int row, int column)
{
	return CPoint(m_RectMineMatrix.left + (m_RectMineMatrix.right - m_RectMineMatrix.left) * (2 * column + 1) / 2 / m_columns, m_RectMineMatrix.top + (m_RectMineMatrix.bottom - m_RectMineMatrix.top) * ( 2 * row + 1 ) / 2 / m_rows);
}

void CRobot::Open(int row, int column)
{
	CPoint coord = GetCenterCoordinate(row, column);
	SetCursorPos(coord.x, coord.y);
	m_pWndMinesweeper->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON);
	m_pWndMinesweeper->SendMessage(WM_LBUTTONUP);
}

void CRobot::QuickOpen(int row, int column)
{
	CPoint coord = GetCenterCoordinate(row, column);
	SetCursorPos(coord.x, coord.y);
	m_pWndMinesweeper->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON);
	m_pWndMinesweeper->SendMessage(WM_RBUTTONDOWN, MK_LBUTTON | MK_RBUTTON);
	m_pWndMinesweeper->SendMessage(WM_LBUTTONUP);
	m_pWndMinesweeper->SendMessage(WM_RBUTTONUP);
}

void CRobot::MarkMine(int row, int column)
{
	CPoint coord = GetCenterCoordinate(row, column);
	SetCursorPos(coord.x, coord.y);
	m_pWndMinesweeper->SendMessage(WM_MOUSEMOVE);
	m_pWndMinesweeper->SendMessage(WM_RBUTTONDOWN, MK_RBUTTON);
	m_pWndMinesweeper->SendMessage(WM_RBUTTONUP);
}

DWORD WINAPI CRobot::BeginSweep(LPVOID lpvoid)
{
	Options* pOptions = (Options*) lpvoid;
	
	switch (pOptions->m_animation)
	{
	case true:
		OpenWait = OpenWait_WithAnimation;
		QuickOpenWait = QuickOpenWait_WithAnimation;
		AfterOperationsWait = AfterOperationsWait_WithAnimation;
		break;
	case false:
		OpenWait = OpenWait_WithoutAnimation;
		QuickOpenWait = QuickOpenWait_WithoutAnimation;
		AfterOperationsWait = AfterOperationsWait_WithoutAnimation;
		break;
	}

	RandomWhenNoWay = pOptions->m_RandomWhenNoWay;

	CWnd::FindWindow(_T("Minesweeper"), NULL)->ShowWindow(SW_RESTORE);
	CRobot* pRobot = NULL;
	pRobot = new CRobot(pOptions->m_difficulty, pOptions->m_rows, pOptions->m_columns, pOptions->m_mines);
	theApp.m_pMainWnd->ShowWindow(SW_SHOWMINIMIZED);
	Sleep(1000);
	pRobot->m_pWndMinesweeper->SetForegroundWindow();

	switch (pOptions->m_mission)
	{
	case Options::WORK:
		Work(pRobot); break;
	case Options::SOLVE:
		Solve(pRobot); break;
	case Options::SCAN:
		ScanMatrix(pRobot); break;
	case Options::DRAW:
		DrawRectangles(pRobot); break;
	}
	delete pRobot;

	theApp.m_pMainWnd->ShowWindow(SW_RESTORE);
	theApp.m_pMainWnd->SetForegroundWindow();

	return 0;
}

Algorithm::SquareType CRobot::GetSquareType(int row, int column)
{
	clock_t c1, c2;
	c1 = clock();
	CRect rect = GetSquareRect(row, column);
//	pDCGlobal = pDC;
	Algorithm::SquareType ret = AnalysisSquare(rect);
	c2 = clock();
	TRACE(_T("GetSquareType Time: %d ms\n"), c2 - c1);
	return ret;
}

bool CRobot::SameColor(COLORREF color1, COLORREF color2, int max)
{
	int r = GetRValue(color1) - GetRValue(color2);
	int g = GetGValue(color1) - GetGValue(color2);
	int b = GetBValue(color1) - GetBValue(color2);
	return r * r + g * g + b * b < max;
}


CPoint CRobot::GetPointInSquare(const CRect &square, int xpercent, int ypercent)
{
	return CPoint(square.left + (square.right - square.left) * xpercent / 100, square.top + (square.bottom - square.top) * ypercent / 100);
}

Algorithm::SquareType CRobot::AnalysisSquare(const CRect &rect)
{
	//if (IsVisibleMine(rect))
	//	return Algorithm::QUESTIONMARK;
	if (IsOpen(rect))
	{
		if (IsOne(rect))
			return Algorithm::ONE;
		if (IsTwo(rect))
			return Algorithm::TWO;
		//if (IsZero(rect))
		//	return Algorithm::ZERO;
		if (IsThree(rect))
			return Algorithm::THREE;
		if (IsFour(rect))
			return Algorithm::FOUR;
		if (IsFive(rect))
			return Algorithm::FIVE;
		if (IsSix(rect))
			return Algorithm::SIX;
		if (IsSeven(rect))
			return Algorithm::SEVEN;
		if (IsEight(rect))
			return Algorithm::EIGHT;
		return Algorithm::ZERO;
	} else {
		if (IsMine(rect))
			return Algorithm::MINE;
		if (IsQuestionMark(rect))
			return Algorithm::QUESTIONMARK;
		return Algorithm::ORIGINAL;
	}
}

bool CRobot::AnalysisSquare2(const CRect &rect, int *xpercent, int *ypercent, int cpercent, const COLORREF *constants, int cconstants)
{
	//static int xoffset[] = {0, 2, 4, 6, 8, 10}, yoffset[] = {0, 2, 4, 6, 8, 10};
	static int xoffset[] = {0, 9, 3, 6}, yoffset[] = {0, 9, 3, 6};
	//static int xoffset[] = {0, -3, -6, -9, 3, 6, 9}, yoffset[] = {0, -3, -6, -9, 3, 6, 9};
	//static int xoffset[] = {0, 9, 5}, yoffset[] = {0, 9, 5};
	//static int xoffset[] = {0, 5}, yoffset[] = {0, 5};
	//static int xoffset[] = {0}, yoffset[] = {0};
	for (int i = 0; i < cpercent; ++i)
		for (int j = 0; j < sizeof(xoffset) / sizeof(xoffset[0]); ++j)
		{
			CPoint point = GetPointInSquare(rect, xpercent[i] + xoffset[j], ypercent[i] + yoffset[j]);
			COLORREF colors = GetColor(point.x, point.y);
			//CPoint point = GetPointInSquare(rect, xpercent[i] + xoffset[j], ypercent[i] + yoffset[j]);
			//COLORREF colors = pDC->GetPixel(point);
			//pDCGlobal->SetPixel(point, 0);
			for (int k = 0; k < cconstants; ++k)
				if (SameColor(colors, constants[k]))
					return true;
		}
	return false;
}

bool CRobot::IsOpen(const CRect &rect)
{
	static const COLORREF COLORS[] = { RGB(222, 234, 246), RGB(206, 217, 235), RGB(181, 191, 218), RGB(166, 174, 213) };
	//static const COLORREF COLORS[] = { RGB(201, 215, 234), RGB(169, 173, 208) };
	static int xpercent[] = {85}, ypercent[] = {50};

	return AnalysisSquare2(rect, xpercent, ypercent, sizeof(xpercent) / sizeof(xpercent[0]), COLORS, sizeof(COLORS) / sizeof(COLORS[0]));
}

//bool CRobot::IsZero(const CRect &rect)
//{
//	static const COLORREF COLORS[] = { RGB(222, 234, 246), RGB(206, 217, 235), RGB(181, 191, 218), RGB(166, 174, 213) };
//	static int xpercent[] = {30, 40, 50, 60, 70}, ypercent[] = {30, 40, 50, 60, 70};
//
//	static int xoffset[] = {0}, yoffset[] = {0};
//	for (int i = 0; i < sizeof(xpercent) / sizeof(xpercent[0]); ++i)
//		for (int j = 0; j < sizeof(xoffset) / sizeof(xoffset[0]); ++j)
//		{
//			COLORREF colors = pDC->GetPixel(GetPointInSquare(rect, xpercent[i] + xoffset[j], ypercent[i] + yoffset[j]));
//			int k;
//			for (k = 0; k < sizeof(COLORS) / sizeof(COLORS[0]); ++k)
//				if (SameColor(colors, COLORS[k]))
//					break;
//			if (k == sizeof(COLORS) / sizeof(COLORS[0])) return false;
//		}
//	return true;
//}

bool CRobot::IsOne(const CRect &rect)
{
	static const COLORREF COLORS[] = { RGB(65, 79, 193) };
	static int xpercent[] = {50}, ypercent[] = {75};

	return AnalysisSquare2(rect, xpercent, ypercent, sizeof(xpercent) / sizeof(xpercent[0]), COLORS, sizeof(COLORS) / sizeof(COLORS[0]));
}

bool CRobot::IsTwo(const CRect &rect)
{
	static const COLORREF COLORS[] = { RGB(28, 105, 8) };
	static int xpercent[] = {39}, ypercent[] = {72};

	return AnalysisSquare2(rect, xpercent, ypercent, sizeof(xpercent) / sizeof(xpercent[0]), COLORS, sizeof(COLORS) / sizeof(COLORS[0]));
}

bool CRobot::IsThree(const CRect &rect)
{
	static const COLORREF COLORS[] = { RGB(174, 5, 0) };
	static int xpercent[] = {64}, ypercent[] = {33};

	return AnalysisSquare2(rect, xpercent, ypercent, sizeof(xpercent) / sizeof(xpercent[0]), COLORS, sizeof(COLORS) / sizeof(COLORS[0]));
}

bool CRobot::IsFour(const CRect &rect)
{
	static const COLORREF COLORS[] = { RGB(2, 0, 130) };
	static int xpercent[] = {64}, ypercent[] = {64};

	return AnalysisSquare2(rect, xpercent, ypercent, sizeof(xpercent) / sizeof(xpercent[0]), COLORS, sizeof(COLORS) / sizeof(COLORS[0]));
}

bool CRobot::IsFive(const CRect &rect)
{
	static const COLORREF COLORS[] = { RGB(125, 0, 0) };
	static int xpercent[] = {39}, ypercent[] = {33};

	return AnalysisSquare2(rect, xpercent, ypercent, sizeof(xpercent) / sizeof(xpercent[0]), COLORS, sizeof(COLORS) / sizeof(COLORS[0]));
}

bool CRobot::IsSix(const CRect &rect)
{
	static const COLORREF COLORS[] = { RGB(0, 125, 131) };
	static int xpercent[] = {64}, ypercent[] = {17};

	return AnalysisSquare2(rect, xpercent, ypercent, sizeof(xpercent) / sizeof(xpercent[0]), COLORS, sizeof(COLORS) / sizeof(COLORS[0]));
}

bool CRobot::IsSeven(const CRect &rect)
{
	//static const COLORREF COLORS[] = { RGB(164, 5, 14) };
	//static int xpercent[] = {60}, ypercent[] = {13};

	//return AnalysisSquare2(rect, xpercent, ypercent, sizeof(xpercent) / sizeof(xpercent[0]), COLORS, sizeof(COLORS) / sizeof(COLORS[0]));
	return false;
}

bool CRobot::IsEight(const CRect &rect)
{
	return false;
}

bool CRobot::IsMine(const CRect &rect)
{
	static const COLORREF COLORS[] = { RGB(255, 0, 0) };
	static int xpercent[] = {49}, ypercent[] = {31};

	return AnalysisSquare2(rect, xpercent, ypercent, sizeof(xpercent) / sizeof(xpercent[0]), COLORS, sizeof(COLORS) / sizeof(COLORS[0]));
}

bool CRobot::IsQuestionMark(const CRect &rect)
{
	static const COLORREF COLORS[] = { RGB(236, 238, 250) };
	static int xpercent[] = {53}, ypercent[] = {21};

	return AnalysisSquare2(rect, xpercent, ypercent, sizeof(xpercent) / sizeof(xpercent[0]), COLORS, sizeof(COLORS) / sizeof(COLORS[0]));
}

bool CRobot::IsVisibleMine(const CRect &rect)
{
	static const COLORREF COLORS[] = { RGB(249, 166, 175) };
	static int xpercent[] = {39}, ypercent[] = {21};

	return AnalysisSquare2(rect, xpercent, ypercent, sizeof(xpercent) / sizeof(xpercent[0]), COLORS, sizeof(COLORS) / sizeof(COLORS[0]));
}

void CRobot::ExcuteOperation(Operation operation)
{
	switch (operation.m_operation)
	{
	case Operation::OperationType::OPEN:
		Open(operation.m_row, operation.m_column);
		Sleep(OpenWait);
		break;
	case Operation::OperationType::MARKMINE:
		MarkMine(operation.m_row, operation.m_column);
		Sleep(MarkMineWait);
		break;
	case Operation::OperationType::QUICKOPEN:
		QuickOpen(operation.m_row, operation.m_column);
		Sleep(QuickOpenWait);
		break;
	default:
		break;
	}
}

void CRobot::Work(CRobot *pRobot)
{
	Operation LastOperation;
	while(!pRobot->m_pAlgo->IsFinished()) {
		clock_t c1, c2;
		c1 = clock();
		CList<Operation, Operation> * pOperation = pRobot->m_pAlgo->GetOperations();
		c2 = clock();
		TRACE(_T("ComputeTime: %d ms\n"), c2 - c1);
		while (!pOperation->IsEmpty())
		{
			if (LastOperation.m_row == -1)
				pRobot->ExcuteOperation(pOperation->GetHead());
			if (pOperation->GetHead() == LastOperation)
				pRobot->m_pAlgo->m_finished = true;
			pRobot->ExcuteOperation(LastOperation = pOperation->RemoveHead());
		}
		Sleep(AfterOperationsWait);

		CaptureDesktop();

		bool identified[100][100] = {0};
		int count = 0;
		while (!pRobot->m_pAlgo->m_WaitForIdentifyList_Row.IsEmpty()) {
			int row = pRobot->m_pAlgo->m_WaitForIdentifyList_Row.RemoveHead();
			int column = pRobot->m_pAlgo->m_WaitForIdentifyList_Column.RemoveHead();
			if (row >= 0 && row < pRobot->m_rows && column >= 0 && column < pRobot->m_columns)
			{
				if (identified[row][column]) continue;
				identified[row][column] = true;
				++count;
				pRobot->m_pAlgo->SetSquareType(row, column, pRobot->GetSquareType(row, column));
				TRACE(_T("Identify: (%2d, %2d) -> %d\n"), row, column, pRobot->m_pAlgo->m_MineMatrix[row][column]);
				if (pRobot->m_pAlgo->m_MineMatrix[row][column] == Algorithm::QUESTIONMARK)
					pRobot->m_pAlgo->m_finished = true;
			}
		}
		TRACE(_T("Identified: %d\n"), count);
	}
}


void CRobot::ScanMatrix(CRobot *pRobot)
{
	FILE *fout = fopen("d:\\test.txt", "w");

	CaptureDesktop();

	for (int i = 0; i < pRobot->m_rows; ++i)
	{
		for (int j = 0; j < pRobot->m_columns; ++j)
		{
			Algorithm::SquareType xxx = pRobot->GetSquareType(i, j);
			switch (xxx)
			{
			case Algorithm::MINE:
				fprintf(fout, " * ");
				break;
			case Algorithm::QUESTIONMARK:
				fprintf(fout, " ? ");
				break;
			case Algorithm::ZERO:
				fprintf(fout, "   ");
				break;
			case Algorithm::ORIGINAL:
				fprintf(fout, " 0 ");
				break;
			default:
				fprintf(fout, "%2d ", xxx);
				break;
			}
		}
		fprintf(fout, "\n");
	}

	fclose(fout);
}


void CRobot::DrawRectangles(CRobot *pRobot)
{
	CaptureDesktop();
	CDC* pDC = CWnd::GetDesktopWindow()->GetDC();
	pDC->SelectObject(::GetStockObject(NULL_BRUSH));
	while (true)
	for (int i = 0; i < pRobot->m_rows; ++i)
		for (int j = 0; j < pRobot->m_columns; ++j)
		{
			CRect rect = pRobot->GetSquareRect(i,j);
			pDC->Rectangle(rect);
		}
	CWnd::GetDesktopWindow()->ReleaseDC(pDC);
}


COLORREF CRobot::GetColor(int x, int y)
{
	DWORD c = pCapture[y * HorzRes + x];
	return RGB(GetBValue(c), GetGValue(c), GetRValue(c));
}


void CRobot::CaptureDesktop(void)
{
	Sleep(100);
	mDC.BitBlt(0, 0, HorzRes, VertRes, CWnd::GetDesktopWindow()->GetDC(), 0, 0, SRCCOPY);
	mBitmap.GetBitmapBits(HorzRes * VertRes * 4, pCapture);
}


void CRobot::Solve(CRobot *pRobot)
{
	CaptureDesktop();
	for (int i = 0; i < pRobot->m_rows; ++i)
		for (int j = 0; j < pRobot->m_columns; ++j)
			pRobot->m_pAlgo->m_MineMatrix[i][j] = pRobot->GetSquareType(i, j);
	
	pRobot->m_pAlgo->m_started = true;
	for (int i = 0; i < pRobot->m_rows; ++i)
		for (int j = 0; j < pRobot->m_columns; ++j)
			if (pRobot->m_pAlgo->m_MineMatrix[i][j] > 0 && pRobot->m_pAlgo->m_MineMatrix[i][j] < 9)
				pRobot->m_pAlgo->NewWaitForCheck(i, j);
	Work(pRobot);
}


//void CRobot::GetMatrixLines(void)
//{
//	static COLORREF BLACK = 0, BLUE = RGB(50, 84, 126);
//	CaptureDesktop();
//	int x, y;
//	for (int row = 0; row < m_rows; ++row)
//		for (int column = 0; column < m_columns; ++column)
//		{
//			CPoint point = GetCenterCoordinate(row, column);
//			
//			x = point.x;
//			while (!SameColor(GetColor(x, point.y), BLACK) && SameColor(GetColor(x, point.y), BLUE))
//				--x;
//			m_SquareRect[row][column].left = x + 1;
//			x = point.x;
//			while (!SameColor(GetColor(x, point.y), BLACK) && SameColor(GetColor(x, point.y), BLUE))
//				++x;
//			m_SquareRect[row][column].right = x;
//			y = point.y;
//			while (!SameColor(GetColor(point.x, y), BLACK) && SameColor(GetColor(point.x, y), BLUE))
//				--y;
//			m_SquareRect[row][column].top = y + 1;
//			y = point.y;
//			while (!SameColor(GetColor(point.x, y), BLACK) && SameColor(GetColor(point.x, y), BLUE))
//				++y;
//			m_SquareRect[row][column].bottom = y;
//		}
//}
