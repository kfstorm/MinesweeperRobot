#pragma once

#include "Operation.h"

class Algorithm
{
public:
	static enum Difficulty { PRIMARY, MIDDLE, EXPERT, MANUAL };
	static enum SquareType { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, ORIGINAL, MINE, QUESTIONMARK, ALREADYOPEN, SEARCH_NOTMINE, SEARCH_MINE };
	
	int m_rows, m_columns;
	int m_RemainderMines;
	SquareType m_MineMatrix[100][100];
	CList<Operation, Operation> m_OperationList;
	CList<int, int> m_WaitForIdentifyList_Row;
	CList<int, int> m_WaitForIdentifyList_Column;
	CList<int, int> m_WaitForCheckList_Row;
	CList<int, int> m_WaitForCheckList_Column;
	bool m_NeverCheck[100][100];

	bool m_started;
	bool m_finished;

	bool m_Search_NotMine[100][100];
	bool m_Search_Mine[100][100];
	bool m_Search_Searched[100][100];

	Algorithm(Difficulty difficulty = PRIMARY, int rows = 0, int columns = 0, int mines = 0);
	~Algorithm(void);
	CList<Operation, Operation> * GetOperations(void);
	bool IsFinished(void);
	void ComputeNextStep(void);
	void SetSquareType(int row, int column, SquareType type);
	void NewWaitForIdentify(int row, int column);
	void NewWaitForCheck(int row, int column);
	void AddOperation(Operation NewOperation);
	
	void Search_Clear();
	bool Search(int step, int row, int column);
	bool Search_Check();
	void GetCertainOperations(void);
	void GetNotCertainOperations(void);
};

extern bool RandomWhenNoWay;