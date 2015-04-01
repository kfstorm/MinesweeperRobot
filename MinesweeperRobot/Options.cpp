#include "StdAfx.h"
#include "Options.h"


Options::Options(void):
m_difficulty(Algorithm::EXPERT), m_rows(0), m_columns(0), m_mines(0), m_animation(false), m_RandomWhenNoWay(true), m_mission(WORK)
{
}


Options::~Options(void)
{
}
