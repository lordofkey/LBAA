#pragma once
#include "MyAxisView.h"

// MyTTView ��ͼ

class MyTTView : public MyAxisView
{
	DECLARE_DYNCREATE(MyTTView)

protected:
	MyTTView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~MyTTView();

public:
//	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


