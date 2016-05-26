// MyRDView.cpp : 实现文件
//

#include "stdafx.h"
#include "LBAA.h"
#include "MyRDView.h"
#define IXT(TT) (TT-bx)/ax
// MyRDView

IMPLEMENT_DYNCREATE(MyRDView, MyAxisView/*CView*/)

MyRDView::MyRDView()
: ispushed(false)
, pushedline(0)
{

}

MyRDView::~MyRDView()
{
}

BEGIN_MESSAGE_MAP(MyRDView, MyAxisView/*CView*/)
	ON_WM_CREATE()
//	ON_WM_ERASEBKGND()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// MyRDView 诊断

#ifdef _DEBUG
void MyRDView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void MyRDView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// MyRDView 消息处理程序


int MyRDView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (MyAxisView::OnCreate(lpCreateStruct) == -1)
		return -1;
	adtmodel::GetInstance()->m_RDView = this->m_hWnd;
	// TODO:  在此添加您专用的创建代码

	n_lines = 2;
	adtmodel::GetInstance()->getcutp(&lines);
	return 0;
}

void MyRDView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect drect;
	this->GetClientRect(&drect);
	if(abs(point.x-XT(xData[lines[0]]))<3)
	{
		ispushed = true;
		pushedline = 0;
	}
	else if(abs(point.x-XT(xData[lines[0]+lines[1]]))<3)
	{
		ispushed = true;
		pushedline = 1;
	}
	MyAxisView::OnLButtonDown(nFlags, point);
}

void MyRDView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(xData == NULL)
	{
		return;
	}
    int temx1 = XT(xData[lines[0]]);
	int temx2 = XT(xData[lines[0]+lines[1]]);
	HCURSOR m_WaveCursor;
	m_WaveCursor = ::AfxGetApp()->LoadStandardCursor(IDC_ARROW );
	if(ispushed)
	{
		m_WaveCursor = ::AfxGetApp()->LoadStandardCursor(IDC_SIZEWE );
//////////////////////////////////////////////////////////////////设置cut
		if(pushedline == 0)
		{
			if(point.x > XT(xData[p_begin])&&point.x < XT(xData[p_end -1 - lines[1]])) 
			{
				lines[0] = IXT(point.x)*80;
			}
		}
		else if(pushedline ==1)
		{
			if((point.x < XT(xData[p_end-1]))&&(point.x > XT(xData[lines[0]])))
			{
				lines[1] = IXT(point.x)*80 - lines[0];
			}
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Invalidate();
	}
	else
	{
		/////////////////////////////////////标记位置检测
		if(abs(point.x-temx1)<3||abs(point.x-temx2)<3)
		{
			m_WaveCursor = ::AfxGetApp()->LoadStandardCursor(IDC_SIZEWE );
		}
	}
	::SetCursor(m_WaveCursor);
	MyAxisView::OnMouseMove(nFlags, point);
}

void MyRDView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect drect;
	this->GetClientRect(&drect);

//////////////////////////////////////////////////////////////////设置cut
	if(ispushed)
	{
		if(pushedline == 0)
		{
			if(point.x > XT(xData[p_begin])&&point.x < XT(xData[p_end -1 - lines[1]])) 
			{
				lines[0] = IXT(point.x)*80;
			}
		}
		else if(pushedline ==1)
		{
			if((point.x < XT(xData[p_end-1]))&&(point.x > XT(xData[lines[0]])))
			{
				lines[1] = IXT(point.x)*80 - lines[0];
			}
		}
		char temstr[10];
		sprintf_s(temstr,10,"%d",lines[0]);
		::WritePrivateProfileString(_T("LBAA"),_T("cut1"),temstr,_T(".//setting.ini"));
		sprintf_s(temstr,10,"%d",lines[0]+lines[1]);
		::WritePrivateProfileString(_T("LBAA"),_T("cut2"),temstr,_T(".//setting.ini"));
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ispushed = false;
	MyAxisView::OnLButtonUp(nFlags, point);
}
