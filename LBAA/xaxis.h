#pragma once

class xaxis
{
public:
	xaxis(void);
	~xaxis(void);

public:
	int t_width;       //ӳ��Ŀ����
	int t_height;      //ӳ��Ŀ��߶�
	unsigned int num_p;
	double ox_points[40]; //ԭʼ�����
    double oy_points[40]; //ԭʼ�����
	double* oy_scale;  //������̶�

	int tx_points[40];
	int ty_points[40];
	int* ty_scale;
	int* tx_scale;

	int n_scale;       //������̶���

	double top;        //���ֵ
	double bottom;     //��Сֵ
	double y_edn;      //����������
	// �趨ӳ��Ŀ��
	void rectset(CRect temrect);
	// �趨ԭʼ�����
	void opset(float* yData);
	char y_wo[100][100];
};
