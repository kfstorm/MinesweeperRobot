#include "StdAfx.h"
#include "Algorithm.h"

const int Search_MaxStep = 8;

bool RandomWhenNoWay = false;

const int RowOffset[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int ColumnOffset[] = {-1, 0, 1, -1, 1, -1, 0, 1};

CList<int, int> trow, tcolumn;


Algorithm::Algorithm(Difficulty difficulty, int rows, int columns, int mines):
m_started(false), m_finished(false)
{
	srand(time(0));

	switch (difficulty)
	{
	case PRIMARY:
		m_rows = m_columns = 9;
		m_RemainderMines = 10;
		break;
	case MIDDLE:
		m_rows = m_columns = 16;
		m_RemainderMines = 40;
		break;
	case EXPERT:
		m_rows = 16; m_columns = 30;
		m_RemainderMines = 99;
		break;
	case MANUAL:
		m_rows = rows; m_columns = columns;
		m_RemainderMines = mines;
		break;
	default:
		m_rows = m_columns = m_RemainderMines = 0;
		break;
	}
	for (int i = 0; i < m_rows; ++i)
		for (int j = 0; j < m_columns; ++j) {
			m_MineMatrix[i][j] = SquareType::ORIGINAL;
			m_NeverCheck[i][j] = false;
		}
}


Algorithm::~Algorithm(void)
{
}


CList<Operation, Operation> * Algorithm::GetOperations(void)
{
	if (m_OperationList.IsEmpty())
		ComputeNextStep();
	return &m_OperationList;
}


bool Algorithm::IsFinished(void)
{
	if (m_finished) return true;
	
	while(!m_WaitForCheckList_Row.IsEmpty())
	{
		int row = m_WaitForCheckList_Row.RemoveHead();
		int column = m_WaitForCheckList_Column.RemoveHead();
		if (!m_NeverCheck[row][column])
		{
			int originalcount = 0;
			for (int i = 0; i < 8; ++i)
			{
				int newrow = row + RowOffset[i];
				int newcolumn = column + ColumnOffset[i];
				if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
					switch (m_MineMatrix[newrow][newcolumn])
					{
					case ORIGINAL:
						++originalcount; break;
					}
			}
			if (originalcount)
			{
				trow.AddTail(row); tcolumn.AddTail(column);
			} else
				m_NeverCheck[row][column] = true;
		}
	}

	while (!trow.IsEmpty())
	{
		int row = trow.RemoveHead();
		int column = tcolumn.RemoveHead();
		if (!m_NeverCheck[row][column]) {
			m_WaitForCheckList_Row.AddTail(row);
			m_WaitForCheckList_Column.AddTail(column);
		}
	}
	
	if (m_started && m_WaitForCheckList_Row.IsEmpty())
	{
		int originalcount2 = 0, minecount2 = 0;
		for (int i = 0; i < m_rows; ++i)
			for (int j = 0; j < m_columns; ++j)
				switch (m_MineMatrix[i][j])
				{
				case ORIGINAL:
					++originalcount2; break;
				case MINE:
					++minecount2; break;
				}
		return originalcount2 == 0 && minecount2 == m_RemainderMines;
	} else return false;
}


void Algorithm::ComputeNextStep(void)
{
	TRACE(_T("ComputeNextStep\n"));

	if (!m_started)
	{
		m_started = true;
		//int row = rand() % m_rows;
		//int column = rand() % m_columns;
		int row = m_rows / 2;
		int column = m_columns / 2;
		m_OperationList.AddTail(Operation(Operation::OPEN, row, column));
		NewWaitForIdentify(row, column);
		return;
	}

	GetCertainOperations();

	if (m_OperationList.IsEmpty() && RandomWhenNoWay)
		GetNotCertainOperations();

	if (m_OperationList.IsEmpty())
		m_finished = true;
}

void Algorithm::SetSquareType(int row, int column, SquareType type)
{
	if (type == ORIGINAL)
		return;
	m_MineMatrix[row][column] = type;
	if (type == ZERO) {
		m_NeverCheck[row][column] = true;
		for (int i = 0; i < 8; ++i) {
			int newrow = row + RowOffset[i];
			int newcolumn = column + ColumnOffset[i];
			if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
				if (m_MineMatrix[newrow][newcolumn] == ORIGINAL) {
					//TRACE(_T("(%2d,%2d) NewWaitForIdentify (%2d, %2d)\n"), row, column, newrow, newcolumn);
					NewWaitForIdentify(newrow, newcolumn);
				}
		}
	} else if (type < 9)
		NewWaitForCheck(row, column);
}


void Algorithm::NewWaitForIdentify(int row, int column)
{
	m_WaitForIdentifyList_Row.AddTail(row);
	m_WaitForIdentifyList_Column.AddTail(column);
}

void Algorithm::NewWaitForCheck(int row, int column)
{
	m_WaitForCheckList_Row.AddTail(row);
	m_WaitForCheckList_Column.AddTail(column);
}

void Algorithm::AddOperation(Operation NewOperation)
{
	m_OperationList.AddTail(NewOperation);
}

void Algorithm::Search_Clear()
{
	for (int i = 0; i < m_rows; ++i)
		for (int j = 0; j < m_columns; ++j)
		{
			m_Search_Searched[i][j] = false;
			m_Search_NotMine[i][j] = false;
			m_Search_Mine[i][j] = false;
		}
	return;

	//if (step > Search_MaxStep) return;
	//m_Search_Searched[row][column] = false;
	//int originalcount = 0;
	//for (int i = 0; i < 8; ++i)
	//{
	//	int newrow = row + RowOffset[i];
	//	int newcolumn = column + ColumnOffset[i];
	//	if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
	//		switch (m_MineMatrix[newrow][newcolumn])
	//		{
	//		case ORIGINAL:
	//			m_Search_NotMine[newrow][newcolumn] = m_Search_Mine[newrow][newcolumn] = false;
	//			++originalcount;
	//			break;
	//		}
	//}
	//
	//if (originalcount == 0) return;

	//for (int i = 0; i < 8; ++i)
	//{
	//	int newrow = row + RowOffset[i];
	//	int newcolumn = column + ColumnOffset[i];
	//	if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
	//		switch (m_MineMatrix[newrow][newcolumn])
	//		{
	//		case ONE: case TWO: case THREE: case FOUR: case FIVE: case SIX: case SEVEN: case EIGHT:
	//			Search_Clear(step+originalcount, newrow, newcolumn);
	//		}
	//}
}

bool Algorithm::Search(int step, int row, int column)
{
	//static bool justin = true;
	if (step > Search_MaxStep) return true;
	if (m_Search_Searched[row][column]) return true;

	int originalcount = 0, minecount = 0;
	for (int i = 0; i < 8; ++i)
	{
		int newrow = row + RowOffset[i];
		int newcolumn = column + ColumnOffset[i];
		if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
			switch (m_MineMatrix[newrow][newcolumn])
			{
			case ORIGINAL:
				++originalcount; break;
			case SEARCH_MINE: case MINE:
				++minecount; break;
			}
	}
	if (minecount + originalcount < m_MineMatrix[row][column]
	|| minecount > m_MineMatrix[row][column])
		return false;

	int originalcount2 = 0, minecount2 = 0;
	for (int i = 0; i < m_rows; ++i)
		for (int j = 0; j < m_columns; ++j)
			switch (m_MineMatrix[i][j])
			{
			case ORIGINAL:
				++originalcount2; break;
			case SEARCH_MINE: case MINE:
				++minecount2; break;
			}
	if (originalcount2 + minecount2 < m_RemainderMines)
		return false;
	
	if (originalcount == 0)
	{
		m_Search_Searched[row][column] = true;
		for (int i = 0; i < 8; ++i)
		{
			int newrow = row + RowOffset[i];
			int newcolumn = column + ColumnOffset[i];
			if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
				if (m_MineMatrix[newrow][newcolumn] < 9 && m_MineMatrix[newrow][newcolumn] > 0
					&& !m_NeverCheck[newrow][newcolumn])
				{
					//justin = true;
					if (!Search(step, newrow, newcolumn))
					{
						m_Search_Searched[row][column] = false;
						return false;
					}
				}
		}
		m_Search_Searched[row][column] = false;
		return true;
	}

	for (int i = 0; i < 8; ++i)
	{
		int newrow = row + RowOffset[i];
		int newcolumn = column + ColumnOffset[i];
		if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
			switch (m_MineMatrix[newrow][newcolumn])
			{
			case ORIGINAL:
				//m_Search_Searched[row][column] = true;
				m_MineMatrix[newrow][newcolumn] = SEARCH_NOTMINE;
				//justin = false;
				bool a = Search(step+1, row, column);
				m_Search_NotMine[newrow][newcolumn] |= a;
				m_MineMatrix[newrow][newcolumn] = SEARCH_MINE;
				//justin = false;
				bool b = minecount2 < m_RemainderMines && Search(step+1, row, column);
				m_Search_Mine[newrow][newcolumn] |= b;

				m_MineMatrix[newrow][newcolumn] = ORIGINAL;
				return a || b;
				break;
			}
	}
	return true;
}

bool Algorithm::Search_Check()
{
	bool ret = false;
	for (int i = 0; i < m_rows; ++i)
		for (int j = 0; j < m_columns; ++j)
			switch (m_MineMatrix[i][j])
			{
			case ORIGINAL:
				if (m_Search_NotMine[i][j] ^ m_Search_Mine[i][j])
				{
					if (m_Search_NotMine[i][j])
					{
						AddOperation(Operation(Operation::OPEN, i, j));
						NewWaitForIdentify(i, j);
					}
					if (m_Search_Mine[i][j])
					{
						m_MineMatrix[i][j] = MINE;
						AddOperation(Operation(Operation::MARKMINE, i, j));
					}
					ret = true;
				}
				break;
			}
	
	return ret;
}

void Algorithm::GetCertainOperations(void)
{
	while(!m_WaitForCheckList_Row.IsEmpty())
	{
		int row = m_WaitForCheckList_Row.RemoveHead();
		int column = m_WaitForCheckList_Column.RemoveHead();
		if (!m_NeverCheck[row][column])
		{
			int minecount = 0, originalcount = 0;
			for (int i = 0; i < 8; ++i)
			{
				int newrow = row + RowOffset[i];
				int newcolumn = column + ColumnOffset[i];
				if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
					switch (m_MineMatrix[newrow][newcolumn])
					{
					case MINE:
						++minecount; break;
					case ORIGINAL:
						++originalcount; break;
					}
			}
				
			if (minecount == m_MineMatrix[row][column])
			{
				m_NeverCheck[row][column] = true;
				if (originalcount)
				{
					AddOperation(Operation(Operation::QUICKOPEN, row, column));
					for (int i = 0; i < 8; ++i)
					{
						int newrow = row + RowOffset[i];
						int newcolumn = column + ColumnOffset[i];
						if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
							if (m_MineMatrix[newrow][newcolumn] == ORIGINAL)
							{
								m_MineMatrix[newrow][newcolumn] = ALREADYOPEN;
								NewWaitForIdentify(newrow, newcolumn);
							}
					}
				}
			}
			else if (minecount + originalcount == m_MineMatrix[row][column])
			{
				m_NeverCheck[row][column] = true;
				for (int i = 0; i < 8; ++i)
					{
						int newrow = row + RowOffset[i];
						int newcolumn = column + ColumnOffset[i];
						if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
							if (m_MineMatrix[newrow][newcolumn] == ORIGINAL) {
								m_MineMatrix[newrow][newcolumn] = MINE;
								AddOperation(Operation(Operation::MARKMINE, newrow, newcolumn));
							}
					}
			} else {
				trow.AddTail(row); tcolumn.AddTail(column);
			}

		}
	}

	while (!trow.IsEmpty())
	{
		int row = trow.RemoveHead();
		int column = tcolumn.RemoveHead();
		if (!m_NeverCheck[row][column]) {
			m_WaitForCheckList_Row.AddTail(row);
			m_WaitForCheckList_Column.AddTail(column);
		}
	}

	if (m_OperationList.IsEmpty())
	{
		TRACE(_T("Searching...\n"));
		while(!m_WaitForCheckList_Row.IsEmpty())
		{
			int row = m_WaitForCheckList_Row.RemoveHead();
			int column = m_WaitForCheckList_Column.RemoveHead();
			trow.AddTail(row); tcolumn.AddTail(column);
			if (!m_NeverCheck[row][column])
			{
				Search_Clear();
				Search(1, row, column);
				if (Search_Check()) break;
				//Search_Check();
			}
		}

		while (!trow.IsEmpty())
		{
			int row = trow.RemoveHead();
			int column = tcolumn.RemoveHead();
			if (!m_NeverCheck[row][column]) {
				m_WaitForCheckList_Row.AddTail(row);
				m_WaitForCheckList_Column.AddTail(column);
			}
		}
	}

	if (m_OperationList.IsEmpty())
	{
		int originalcount2 = 0, minecount2 = 0;
		for (int i = 0; i < m_rows; ++i)
			for (int j = 0; j < m_columns; ++j)
				switch (m_MineMatrix[i][j])
				{
				case ORIGINAL:
					++originalcount2; break;
				case MINE:
					++minecount2; break;
				}
		if (originalcount2 && originalcount2 + minecount2 == m_RemainderMines)
			for (int i = 0; i < m_rows; ++i)
				for (int j = 0; j < m_columns; ++j)
					switch (m_MineMatrix[i][j])
					{
					case ORIGINAL:
						AddOperation(Operation(Operation::OPEN, i, j));
						NewWaitForIdentify(i, j);
						break;
					}
	}

}


void Algorithm::GetNotCertainOperations(void)
{
	if (!m_WaitForCheckList_Row.IsEmpty())
	{
		int SelectRow = -1, SelectColumn = -1;
		double ExplodeProbility = 1;
		while (!m_WaitForCheckList_Row.IsEmpty())
		{
			int row = m_WaitForCheckList_Row.RemoveHead();
			int column = m_WaitForCheckList_Column.RemoveHead();
			trow.AddTail(row), tcolumn.AddTail(column);
			if (!m_NeverCheck[row][column])
			{
				int minecount = 0, originalcount = 0;
				for (int i = 0; i < 8; ++i)
				{
					int newrow = row + RowOffset[i];
					int newcolumn = column + ColumnOffset[i];
					if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
						switch (m_MineMatrix[newrow][newcolumn])
						{
						case MINE:
							++minecount; break;
						case ORIGINAL:
							++originalcount; break;
						}
				}
				if ((double) (m_MineMatrix[row][column] - minecount) / originalcount < ExplodeProbility)
				{
					SelectRow = row; SelectColumn = column;
					ExplodeProbility = (double) (m_MineMatrix[row][column] - minecount) / originalcount;
				}
			}
		}

		while (!trow.IsEmpty())
		{
			int row = trow.RemoveHead();
			int column = tcolumn.RemoveHead();
			if (!m_NeverCheck[row][column]) {
				m_WaitForCheckList_Row.AddTail(row);
				m_WaitForCheckList_Column.AddTail(column);
			}
		}

	if (SelectRow != -1)
		for (int i = 0; i < 8; ++i)
		{
			int newrow = SelectRow + RowOffset[i];
			int newcolumn = SelectColumn + ColumnOffset[i];
			if (newrow >= 0 && newrow < m_rows && newcolumn >=0 && newcolumn < m_columns)
				if (m_MineMatrix[newrow][newcolumn] == ORIGINAL)
				{
					AddOperation(Operation(Operation::OPEN, newrow, newcolumn));
					NewWaitForIdentify(newrow, newcolumn);
					break;
				}
		}

	} else {
		bool flag = false;
		for (int i = 0; i < m_rows; ++i)
		{
			for (int j = 0; j < m_columns; ++j)
			{
				switch (m_MineMatrix[i][j])
				{
				case ORIGINAL:
					AddOperation(Operation(Operation::OPEN, i, j));
					NewWaitForIdentify(i, j);
					flag = true;
					break;
				}
				if (flag) break;
			}
			if (flag) break;
		}
	}

}
