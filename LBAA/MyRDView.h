#pragma once
#include "MyAxisView.h"

// MyRDView ��ͼ

class MyRDView : public MyAxisView
{
	DECLARE_DYNCREATE(MyRDView)

protected:
	MyRDView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~MyRDView();

public:
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

protected:
	// �������
	bool ispushed;
	// ��������
	int pushedline;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


