#pragma once

#include "Algorithm.h"

class Options
{
public:
	Algorithm::Difficulty m_difficulty;
	int m_rows, m_columns, m_mines;
	bool m_animation, m_RandomWhenNoWay;
	enum Mission { WORK, SOLVE, SCAN, DRAW };
	Mission m_mission;
	
	Options(void);
	~Options(void);
};

