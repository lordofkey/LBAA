// LBAA.h : LBAA Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CLBAAApp:
// �йش����ʵ�֣������ LBAA.cpp
//

class CLBAAApp : public CWinApp
{
public:
	CLBAAApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLBAAApp theApp;