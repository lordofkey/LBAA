#pragma once

#define XT(TT) (int)(ax * TT + bx)
#define YT(TT) (int)(ay * TT + by)
// MyAxisView 视图

class MyAxisView : public CView
{
	DECLARE_DYNCREATE(MyAxisView)
protected:
	MyAxisView();           // 动态创建所使用的受保护的构造函数
	afx_msg LRESULT OnPlot(WPARAM, LPARAM);
	virtual ~MyAxisView();
public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	DECLARE_MESSAGE_MAP()
	//////坐标变换系统
protected:
	
	//int p_Data;// drawmode = 2时，插入值的指针
	//int n_points_draw;// 需要绘制的点数
	//int n_points;   //点数
	//int p_Data_3;// mode == 3时 数据的指针
	int p_begin;  //坐标轴起始点
	int p_cend;   //绘制曲线的结束点
	int p_end;    //坐标轴结束点
	float *yData;   //坐标实际数值
	float *xData;
	time_t *timeData; //时间戳

	float *xdefultData;   
	float *y_scale;  //纵坐标刻度
	float *x_scale;  //横坐标刻度
	// y轴的scale数
	int n_yscale;
	// x轴的scale数
	int n_xscale;

	//变换系数
	float ax;   //
	float bx;   //
	float ay;
	float by;

	float a_max;  //y轴最大值 最小值
	float a_min;

	int t_width;       //映射目标宽度
	int t_height;      //映射目标高度

	// 显示模式
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