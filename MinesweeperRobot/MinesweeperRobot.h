
// MinesweeperRobot.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMinesweeperRobotApp:
// �йش����ʵ�֣������ MinesweeperRobot.cpp
//

class CMinesweeperRobotApp : public CWinApp
{
public:
	CMinesweeperRobotApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMinesweeperRobotApp theApp;