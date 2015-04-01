#pragma once

class Operation
{
public:
	static enum OperationType { OPEN, MARKMINE, QUICKOPEN };
	int m_row, m_column;
	OperationType m_operation;

	Operation();
	Operation(OperationType operation, int row, int column);
	~Operation(void);
	bool operator==(const Operation & rOperation) const;
};

