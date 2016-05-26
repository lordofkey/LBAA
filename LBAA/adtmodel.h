#pragma once


struct points
{
	int mode;  ///��ͼģʽ�� 1-ֱ�ӻ��� 2-�����ƣ����ƣ�  3-�����ƣ�ѹ����
	float* px;
	float* py;
	time_t time;//��������ʱ���
	float min;  //
	float max;  //mode = 1ʱ����ʾ��Ҫ��ʾ�ļ���ֵ
	int n;     // mode = 1ʱ����ʾ����������  mode = 2ʱ����ʾ��������Ҫ��ʾ�ĵ���

};


class adtmodel
{

#pragma region single cls 
private:
	class Deleter
	{
	public:
		~Deleter()
		{
			if (adtmodel::m_pInstance != NULL)
			{
				delete adtmodel::m_pInstance;
				adtmodel::m_pInstance = NULL;
			}
		}
	};
	static adtmodel* m_pInstance;
	adtmodel(void);
	virtual ~adtmodel();   
public:
	static adtmodel *adtmodel::GetInstance();

#pragma endregion



public:
/////////////////////////////�߳��߼����Ʊ���
	bool iswork;//����Ƿ�ʼ����
	bool isfinish;// ����Ƿ��������	
	bool ispost2view;// ����Ƿ����˥��ʱ�����ʾ
	bool isstore;// ����Ƿ����˥��ʱ��Ĵ洢

	USHORT cut_c[2]; //���˥�����ߵĽ�ȡ��Χ0 - ��Сֵ 1-���ֵ 2 - �Ƿ���Ҫ���·����ڴ�
public:
	bool isworking();
	bool isfinishing();
	bool setcut(int a,int b); //�趨��Ϸ�Χ
	void getcut(USHORT* tem);              //��ȡ��Ϸ�Χ




//��������жӿ���
public:
	float *fit_rank[100];                       //��������ж�
	int p_start;                               //�ж���ʼλ��ָ��
	int p_end;                                    //�жӽ�βָ��
	int ranksize;                                 //�ж�����

	void rankinitial();
	bool pump(float*);
	float* pop();
	void destroyrank();
		//�ӿڿ���

	HWND m_RDView;
	HWND m_TView;
	HWND m_ControlForm;
	// ��ȡcutָ��
	void getcutp(USHORT** p_p_cut);
public:

	// �Ƿ���������
	char filename[100];
	bool isreset;
	// ���Ӵ���
	int n_add_t;
	// ���˥������
	float RDValues[100];
	int pRD;
	// ����״̬
	bool istest;
	// �궨����ϵ������� 1-���� 2-�������
	int A_flag;
};
