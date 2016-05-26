#pragma once

#define XT(TT) (int)(ax * TT + bx)
#define YT(TT) (int)(ay * TT + by)
// MyAxisView ��ͼ

class MyAxisView : public CView
{
	DECLARE_DYNCREATE(MyAxisView)
protected:
	MyAxisView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	afx_msg LRESULT OnPlot(WPARAM, LPARAM);
	virtual ~MyAxisView();
public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	DECLARE_MESSAGE_MAP()
	//////����任ϵͳ
protected:
	
	//int p_Data;// drawmode = 2ʱ������ֵ��ָ��
	//int n_points_draw;// ��Ҫ���Ƶĵ���
	//int n_points;   //����
	//int p_Data_3;// mode == 3ʱ ���ݵ�ָ��
	int p_begin;  //��������ʼ��
	int p_cend;   //�������ߵĽ�����
	int p_end;    //�����������
	float *yData;   //����ʵ����ֵ
	float *xData;
	time_t *timeData; //ʱ���

	float *xdefultData;   
	float *y_scale;  //������̶�
	float *x_scale;  //������̶�
	// y���scale��
	int n_yscale;
	// x���scale��
	int n_xscale;

	//�任ϵ��
	float ax;   //
	float bx;   //
	float ay;
	float by;

	float a_max;  //y�����ֵ ��Сֵ
	float a_min;

	int t_width;       //ӳ��Ŀ����
	int t_height;      //ӳ��Ŀ��߶�

	// ��ʾģʽ
	int drawmode;


public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	USHORT* lines;
	int n_lines;
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
public:

private:

};