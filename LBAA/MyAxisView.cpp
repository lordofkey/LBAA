// MyAxisView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LBAA.h"
#include "MyAxisView.h"
#include "float.h"
// MyAxisView
IMPLEMENT_DYNCREATE(MyAxisView, CView)
MyAxisView::MyAxisView()
: xData(NULL)
, yData(NULL)
, timeData(NULL)
, y_scale(NULL)
, x_scale(NULL)
, n_yscale(0)
, ax(0)
, lines(NULL)
, n_lines(0)
, drawmode(0)
, p_begin(0)
, p_cend(0)
, p_end(0)

{
	xdefultData = new float[8640000];
	for(int i = 0;i<8640000;i++)
	{
		xdefultData[i] = (float)i;
	}
	timeData = new time_t[8640000];
}
MyAxisView::~MyAxisView()
{
}
BEGIN_MESSAGE_MAP(MyAxisView, CView)
	ON_MESSAGE(WM_ONPLOT,&OnPlot)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
void MyAxisView::OnDraw(CDC* pDC)
{
	CDC Cmemdc;
	CBitmap c_bitmap;
	CRect drect;
	this->GetClientRect(&drect);
/////////////////////////////////////////////���廭����ʼ��
	Cmemdc.CreateCompatibleDC(pDC);
	c_bitmap.CreateCompatibleBitmap(pDC,drect.Width(),drect.Height());
	Cmemdc.SelectObject(&c_bitmap);

	Cmemdc.FillSolidRect(drect,RGB(200,200,200));//
	CPen *pp=new CPen(PS_SOLID,1,RGB(30,30,30));
	Cmemdc.SelectObject(pp);
	Cmemdc.SetBkMode(TRANSPARENT);

	_set_output_format(_TWO_DIGIT_EXPONENT);
	Cmemdc.MoveTo(drect.Width()/10, drect.Height()*1/20);
	Cmemdc.LineTo(drect.Width()/10, drect.Height()*9/10);
	Cmemdc.MoveTo(drect.Width()/10, drect.Height()*9/10);
	Cmemdc.LineTo(drect.Width()*19/20, drect.Height()*9/10);
	if(yData != NULL)
	{
/////////////////////////////////////////////��������
		if(drawmode == 1)
		{
			
			Cmemdc.MoveTo(XT(xData[0]),YT(yData[0]));
			for(int i = p_begin;i<p_cend;i++)
			{

				Cmemdc.LineTo(XT(xData[i]),YT(yData[i]));
			}
		}
		if(drawmode == 2)
		{
			for(int i = p_begin;i<p_cend;i++)
			{
				Cmemdc.SetPixelV(XT(xData[i]),  YT(yData[i])  ,RGB(0,0,0));
				Cmemdc.SetPixelV(XT(xData[i])-1,YT(yData[i])  ,RGB(0,0,0));
				Cmemdc.SetPixelV(XT(xData[i]),  YT(yData[i])-1,RGB(0,0,0));
				Cmemdc.SetPixelV(XT(xData[i])-1,YT(yData[i])-1,RGB(0,0,0));
			}
		}
		if(drawmode == 3)
		{
			for(int i = p_begin;i<p_cend;i++)
			{
				Cmemdc.SetPixelV(XT(xData[i]),YT(yData[i]),RGB(0,0,0));
			}
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////����x������
		char s_sc[100];
		if(x_scale!=NULL)
		{
			if(drawmode == 1)
			{
				for(int i = 0;i<n_xscale;i++)
				{
					Cmemdc.MoveTo(XT(x_scale[i]),drect.Height()*9/10);
					Cmemdc.LineTo(XT(x_scale[i]),drect.Height()*9/10-5);
					sprintf_s(s_sc,100,"%0.2f", x_scale[i]);
					Cmemdc.TextOutA(XT(x_scale[i]) - 10,drect.Height()*9/10+10,s_sc);
				}
			}
			if(drawmode ==2)
			{
				for(int i = 0;i<n_xscale;i++)
				{
					Cmemdc.MoveTo(XT(x_scale[i]),drect.Height()*9/10);
					Cmemdc.LineTo(XT(x_scale[i]),drect.Height()*9/10-5);
					sprintf_s(s_sc,100,"%d", (int)x_scale[i]);
					Cmemdc.TextOutA(XT(x_scale[i]) - 10,drect.Height()*9/10+10,s_sc);
				}
			}
			if(drawmode ==3)
			{
				for(int i = 0;i<n_xscale;i++)
				{
					Cmemdc.MoveTo(XT(x_scale[i]),drect.Height()*9/10);
					Cmemdc.LineTo(XT(x_scale[i]),drect.Height()*9/10-5);
					tm tim;
					localtime_s(&tim, &timeData[(unsigned int)x_scale[i]]);
					sprintf_s(s_sc,100, "%02d:%02d:%02d",tim.tm_hour, tim.tm_min, tim.tm_sec);
					Cmemdc.TextOutA(XT(x_scale[i]) - 10,drect.Height()*9/10+10,s_sc);
				}
			}
		}
///////////////////////////////////////////////////////////////////////////////////////////////////////����y������
		if(y_scale != NULL)
		{
			if(drawmode == 1)
			{
				for(int i = 0;i<n_yscale;i++)
				{
					Cmemdc.MoveTo(drect.Width()/10,    YT(y_scale[i]));
					Cmemdc.LineTo(drect.Width()/10 + 5,YT(y_scale[i]));
					sprintf_s(s_sc,100,"%d", (int)y_scale[i]);
					Cmemdc.TextOutA(drect.Width()/10 - 40,YT(y_scale[i])-7,s_sc);
				}
			}
			if(drawmode == 2||drawmode==3)
			{
				for(int i = 0;i<n_yscale;i++)
				{
					Cmemdc.MoveTo(drect.Width()/10,    YT(y_scale[i]));
					Cmemdc.LineTo(drect.Width()/10 + 5,YT(y_scale[i]));
					sprintf_s(s_sc,100,"%0.3f", y_scale[i]);
					Cmemdc.TextOutA(drect.Width()/10 - 40,YT(y_scale[i])-7,s_sc);
				}
			}
		}
/////////////////////////////////////////////////////////////////////////////////////

	}

///////////////////////////////////////////////////////////���Ʊ����
	if((xData != NULL)&&(lines != NULL))
	{
		Cmemdc.MoveTo(XT(xData[lines[0]]),drect.Height()/20);
		Cmemdc.LineTo(XT(xData[lines[0]]),drect.Height()*9/10);
		Cmemdc.MoveTo(XT(xData[lines[0]+lines[1]]),drect.Height()/20);
		Cmemdc.LineTo(XT(xData[lines[0]+lines[1]]),drect.Height()*9/10);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	pDC->BitBlt(0,0,drect.Width(),drect.Height(),&Cmemdc,0,0,SRCCOPY);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨
	Cmemdc.DeleteDC();                                       //ɾ��DC
	c_bitmap.DeleteObject();                                      //ɾ��λͼ
	pp->DeleteObject();
	delete pp;
}
// MyAxisView ���
#ifdef _DEBUG
void MyAxisView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void MyAxisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG
afx_msg LRESULT MyAxisView::OnPlot(WPARAM wParam, LPARAM lParam)
{

	//struct points
	//{
	//	int mode;  ///��ͼģʽ�� 1-ֱ�ӻ��� 2-�����ƣ����ƣ�  3-�����ƣ�ѹ����4-����������
	//	float* px;
	//	float* py;
	//	time_t time;//��������ʱ���
	//	float min;  //
	//	float max;  //mode = 1ʱ����ʾ��Ҫ��ʾ�ļ���ֵ
	//	int n;     // mode = 1ʱ����ʾ����������  mode = 2ʱ����ʾ��������Ҫ��ʾ�ĵ���

	//};
	points *p_param = (points *)wParam;

#pragma region ��������ϵ
////////////////////////////////////////////////////////////////////////////////////��������ϵ
	if(drawmode != p_param->mode||p_param->mode==4)//���ģʽ�л��������³�ʼ������ϵ
	{
		if(xData != NULL&&xData != xdefultData)
		{
			delete xData;
			xData = NULL;
		}
		if(xData == xdefultData)
		{
			xData =NULL;
		}
		if(yData != NULL)
		{
			delete yData;
			yData = NULL;
		}
		p_begin = 0;
		p_cend = 0;
		p_end = 0;
		drawmode = p_param->mode;
	}
	if(p_param->mode==4)
	{
		return 0;
	}

#pragma endregion

#pragma region ���������ݳ�ʼ��x
//////////////////////////////////////////���������ݳ�ʼ��x
	switch(drawmode)
	{
	case 1:
		p_begin = 0;
		p_end = p_param->n;
		p_cend = p_param->n;
		if(xData != NULL)
		{
			delete xData;
			xData = NULL;
		}
		xData = p_param -> px;
		break;
	case 2:
		xData = xdefultData;
		p_cend++;
		p_end = max(p_cend,p_param->n);
		p_begin = p_end - p_param->n;
		break;
	case 3:
		xData = xdefultData;
		timeData[p_cend] = p_param->time;
		p_cend++;
		p_begin = 0;
		p_end = p_cend;
		break;
	}
#pragma endregion

#pragma region ���������ݳ�ʼ��
/////////////////////////////////////////////////���������ݳ�ʼ��
	switch(drawmode)
	{
	case 1:
		if(yData != NULL)
		{
			delete yData;
			yData = NULL;
		}
		yData = p_param -> py;
		break;
	case 2:
	case 3:
		if(yData == NULL)
		{
			yData = new float[8640000];
		}
		if(_isnan(*p_param->py))          //���yֵ�Ƿ�Ϸ�
		{
			return 0;
		}
		yData[p_cend-1] = *p_param->py;   //����֮ǰp_cend �Ѿ�����1���������︳ֵʱ��Ҫ����1
		break;
	}
#pragma endregion

#pragma region �趨��ֵ
//////////////////////////////////////////////�趨��ֵ
	switch(drawmode)
	{
	case 1:
	case 3:
		a_min = p_param ->min;
		a_max = p_param ->max;
		break;
	case 2:
		a_min = 3.3028235E38f; 
		a_max = -3.3028235E38f;
		for(int i = p_begin;i < p_cend;i++)
		{
			if(a_max < yData[i])
			{
				a_max = yData[i];
			}
			if(a_min > yData[i])
			{
				a_min = yData[i];
			}
		}
		break;
	}
#pragma endregion

#pragma region x������̶�����
////////////////////////////////////////////x������̶�����
	float inter; // ���
	int inx;     //ʮ��ָ��
	int ox;      //x��ķָ�����
	float firstscale;  //��һ������̶ȵ�ֵ



	switch(drawmode)
	{
	case 1:////////////////////////////////////////////////////��xData Ϊ��׼
		ox = 10;
		inter = (xData[p_end-1] - xData[0]) / ox;
		inx = -(int)floor(log10(inter));
		inter = floor(inter * pow(10.0f,inx))* pow(10.0f,-inx);    //���
		n_xscale = 0;
		firstscale = floor(xData[0] * pow(10.0f,inx))* pow(10.0f,-inx);
		n_xscale = (int)((xData[p_end-1] - firstscale)/inter)+1;
		if(x_scale != NULL)
		{
			delete x_scale;
			x_scale = NULL;
		}
		x_scale = new float[n_xscale];
		x_scale[0] = firstscale;
		for(int i = 0;i<n_xscale-1;i++)
		{
			x_scale[i+1] = x_scale[i] + inter;
		}
		break;
	case 2:////////////////////////////////////////////////////��p_Ϊ��׼
		if(x_scale != NULL)
		{
			delete x_scale;
			x_scale = NULL;
		}
		n_xscale = 13;
		x_scale = new float[n_xscale];
		inter = floor((float)(p_end-p_begin)/(n_xscale-1));
		for(int i = 0;i<n_xscale;i++)
		{
			x_scale[i] = i*inter+p_begin;
		}
		break;
	case 3:////////////////////////////////////////////////////��p_Ϊ��׼
		if(x_scale != NULL)
		{
			delete x_scale;
			x_scale = NULL;
		}
		n_xscale = 5;
		x_scale = new float[n_xscale];
		inter = (float)p_end/n_xscale;
		for(int i = 0;i<n_xscale;i++)
		{
			x_scale[i] = i*inter;
		}
		break;
	}
#pragma endregion
#pragma region y������̶�����
/////////////////////////////////////////////y������̶�����
	float top;
	float bottom;
	float oy;
	if(a_min == a_max)
	{
		if(a_min == 0)
		{
			a_min = -1;
			a_max = 1;
		}
		else if(a_min >0)
		{
			top = a_max * 2;
			bottom = 0;
		}
		else
		{
			bottom = a_max * 2;
			top = 0;
		}
	}
	else
	{
		top = a_max + 0.1f * (a_max - a_min);
		bottom = a_min - 0.1f * (a_max - a_min);
	}
	oy = 6;
	inter = (top - bottom) / oy;
	inx = -(int)floor(log10(inter));
	inter = floor(inter * pow(10.0f,inx))* pow(10.0f,-inx);    //���
	n_yscale = 0;
	firstscale = floor(top * pow(10.0f,inx))* pow(10.0f,-inx);
	n_yscale = (int)((firstscale - bottom)/inter)+1;
	if(y_scale != NULL)
	{
		delete y_scale;
		y_scale = NULL;
	}
	y_scale = new float[n_yscale];
	y_scale[0] = firstscale;
	for(int i = 0;i<n_yscale-1;i++)
	{
		y_scale[i+1] = y_scale[i] - inter;
	}
#pragma endregion
#pragma region ����ӳ��
/////////////////////////////////////////////////////////////////����ӳ��
	CRect clirect;
	this->GetClientRect(&clirect);
	t_height = clirect.Height();
	t_width = clirect.Width();
	ay = (float)t_height *0.85f/(bottom - top);
	by = 0.05f * t_height - top * ay;
	ax = (float)t_width*0.85f /(xData[p_end-1] - xData[p_begin]);
	bx = (float)t_width*0.95f-ax*xData[p_end-1];
#pragma endregion

	this->Invalidate();
	if(p_param != NULL)
	{
		delete p_param;
		p_param = NULL;
	}
	return 0;
}

// MyAxisView ��Ϣ�������
BOOL MyAxisView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return true;
	//	return CView::OnEraseBkgnd(pDC);
}
BOOL MyAxisView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}