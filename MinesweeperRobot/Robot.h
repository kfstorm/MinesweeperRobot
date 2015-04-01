
#pragma once

#include "Algorithm.h"

// CRobot ÃüÁîÄ¿±ê

class CRobot
{
public:
	
	CRobot(Algorithm::Difficulty difficulty = Algorithm::PRIMARY, int rows = 0, int columns = 0, int mines = 0);
	virtual ~CRobot();

	CWnd *m_pWndMinesweeper;
	CRect m_RectMineMatrix;
	Algorithm::Difficulty m_difficulty;
	int m_rows, m_columns, m_mines;

	Algorithm* m_pAlgo;

	//CRect m_SquareRect[100][100];

	bool GetMineMatrix();
	CRect GetSquareRect(int row, int column);
	CPoint GetCenterCoordinate(int row, int column);
	void Open(int row, int column);
	void QuickOpen(int row, int column);
	void MarkMine(int row, int column);

	static DWORD WINAPI BeginSweep(LPVOID lpvoid);
	Algorithm::SquareType GetSquareType(int row, int column);
	static bool SameColor(COLORREF color1, COLORREF color2, int max = 500);
	static CPoint GetPointInSquare(const CRect &square, int xpercent, int ypercent);
	static Algorithm::SquareType AnalysisSquare(const CRect &rect);
	static bool AnalysisSquare2(const CRect &rect, int *xpercent, int *ypercent, int cpercent, const COLORREF *constants, int cconstants);
	
	static bool IsOpen(const CRect &rect);
	//static bool IsZero(const CRect &rect);
	static bool IsOne(const CRect &rect);
	static bool IsTwo(const CRect &rect);
	static bool IsThree(const CRect &rect);
	static bool IsFour(const CRect &rect);
	static bool IsFive(const CRect &rect);
	static bool IsSix(const CRect &rect);
	static bool IsSeven(const CRect &rect);
	static bool IsEight(const CRect &rect);
	static bool IsMine(const CRect &rect);
	static bool IsQuestionMark(const CRect &rect);
	static bool IsVisibleMine(const CRect &rect);
	void ExcuteOperation(Operation operation);
	static void Work(CRobot *pRobot);
	static void ScanMatrix(CRobot *pRobot);
	CPoint GetSquareLeftTopPoint(int row, int column);
	static void DrawRectangles(CRobot *pRobot);
	static COLORREF GetColor(int x, int y);
	static void CaptureDesktop(void);
	static void Solve(CRobot *pRobot);
	//void GetMatrixLines(void);
};