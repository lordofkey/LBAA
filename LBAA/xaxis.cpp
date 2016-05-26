#include "StdAfx.h"
#include "xaxis.h"

xaxis::xaxis(void)
{

	oy_scale = NULL;  //纵坐标刻度
	ty_scale = NULL;
	tx_scale = NULL;
	y_edn = 6;//初始纵坐标间隔
	oy_scale = new double[(unsigned int)y_edn*10];
}

xaxis::~xaxis(void)
{
}

// 设定映射目标
void xaxis::rectset(CRect temrect)
{
	t_width = temrect.Width();
	t_height = temrect.Height();
}

// 设定原始坐标点
void xaxis::opset(float* yData)
{
#pragma region 坐标点初始化
	num_p = RDdata.GetSize();
	if(RDdata.GetSize() == 0)return;
	double max;
	double min;


	if(num_p <40)
	{
		max = RDdata[0];
		min = RDdata[0];

		for(unsigned int i = 0;i < num_p;i++)
		{
			ox_points[i] = i;
			oy_points[i] = RDdata[i];
			if(max < RDdata[i])
			{
				max = oy_points[i];
			}
			if(min > RDdata[i])
			{
				min = oy_points[i];
			}
		}
	}
	else
	{
		max = RDdata[num_p - 40];
		min = RDdata[num_p - 40];

		for(unsigned int i = num_p - 40,j = 0;i < num_p;i++,j++)
		{
			ox_points[j] = j;
			oy_points[j] = RDdata[i];
			if(max < RDdata[i])
			{
				max = oy_points[j];
			}
			if(min > RDdata[i])
			{
				min = oy_points[j];
			}
		}
		num_p = 40;
	}
#pragma endregion


#pragma region 计算top bottom
	if(min == max)
	{
		top = max * 2;
		bottom = 0;
	}
	else
	{
		top = max + 0.5 * (max - min);
		bottom = min - 0.5 * (max - min);
	}
#pragma endregion

#pragma region 定位坐标刻度

	double inter = (top - bottom) / y_edn;
	int inx = -(int)floor(log10(inter));
	inter = floor(inter * pow(10.0f,inx))* pow(10.0f,-inx);    //间隔
	oy_scale[0] = floor(top * pow(10.0f,inx))* pow(10.0f,-inx);
	n_scale = 0;
	while(oy_scale[n_scale] > bottom)
	{
		oy_scale[n_scale+1] = oy_scale[n_scale] - inter;
		n_scale++;
	}
#pragma endregion


#pragma region 绘图目标映射
//纵坐标映射

	double ya = t_height /(bottom - top);
	double yb = -top * ya;

	double xa = t_width / 45;
	double xb = 5 * xa;

	if(ty_scale == NULL)ty_scale = new int[(unsigned int)y_edn*10];
	if(tx_scale == NULL)tx_scale = new int[8];
	for(unsigned int i = 0;i < num_p;i++)
	{
		tx_points[i] = (int)(xa * ox_points[i] + xb);
		ty_points[i] = (int)(ya * oy_points[i] + yb);
	}
	for(int i = 0; i < n_scale; i++)
	{

		sprintf_s(y_wo[i],100,"%g",oy_scale[i]);
		ty_scale[i] = (int)(ya * oy_scale[i] + yb);
	}
	for(int i = 0; i < 8; i++)
	{
		tx_scale[i] = xa * i * 5 + xb;
	}

#pragma endregion
}
