#include "StdAfx.h"
#include "Operation.h"

Operation::Operation()
{
	m_operation = OPEN;
	m_row = m_column = -1;
}

Operation::Operation(OperationType operation, int row, int column):
m_operation(operation), m_row(row), m_column(column)
{
}


Operation::~Operation(void)
{
}


bool Operation::operator==(const Operation & rOperation) const
{
	return m_operation == rOperation.m_operation && m_row == rOperation.m_row && m_column == rOperation.m_column;
}
