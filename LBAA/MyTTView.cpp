// MyTTView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LBAA.h"
#include "MyTTView.h"


// MyTTView

IMPLEMENT_DYNCREATE(MyTTView, MyAxisView/*CView*/)

MyTTView::MyTTView()
{

}

MyTTView::~MyTTView()
{
}

BEGIN_MESSAGE_MAP(MyTTView, MyAxisView/*CView*/)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// MyTTView ��ͼ

//void MyTTView::OnDraw(CDC* pDC)
//{
//	CDocument* pDoc = GetDocument();
//	// TODO: �ڴ���ӻ��ƴ���
//}


// MyTTView ���

#ifdef _DEBUG
void MyTTView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void MyTTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// MyTTView ��Ϣ�������


int MyTTView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	adtmodel::GetInstance()->m_TView = this->m_hWnd;
	return 0;
}
