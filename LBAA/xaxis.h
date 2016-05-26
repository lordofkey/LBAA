#pragma once

class xaxis
{
public:
	xaxis(void);
	~xaxis(void);

public:
	int t_width;       //映射目标宽度
	int t_height;      //映射目标高度
	unsigned int num_p;
	double ox_points[40]; //原始坐标点
    double oy_points[40]; //原始坐标点
	double* oy_scale;  //纵坐标刻度

	int tx_points[40];
	int ty_points[40];
	int* ty_scale;
	int* tx_scale;

	int n_scale;       //纵坐标刻度数

	double top;        //最大值
	double bottom;     //最小值
	double y_edn;      //纵坐标间隔数
	// 设定映射目标
	void rectset(CRect temrect);
	// 设定原始坐标点
	void opset(float* yData);
	char y_wo[100][100];
};
