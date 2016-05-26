#pragma once
#include "MyAxisView.h"

// MyRDView 视图

class MyRDView : public MyAxisView
{
	DECLARE_DYNCREATE(MyRDView)

protected:
	MyRDView();           // 动态创建所使用的受保护的构造函数
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
	// 按下左键
	bool ispushed;
	// 锁定的线
	int pushedline;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


