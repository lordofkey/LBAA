#pragma once


struct points
{
	int mode;  ///绘图模式： 1-直接绘制 2-逐点绘制（后移）  3-逐点绘制（压缩）
	float* px;
	float* py;
	time_t time;//数据生成时间戳
	float min;  //
	float max;  //mode = 1时，表示需要显示的极限值
	int n;     // mode = 1时，表示传入数据量  mode = 2时，表示整个坐标要显示的点数

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
/////////////////////////////线程逻辑控制变量
	bool iswork;//标记是否开始任务
	bool isfinish;// 标记是否结束任务	
	bool ispost2view;// 标记是否进行衰当时间的显示
	bool isstore;// 标记是否进行衰当时间的存储

	USHORT cut_c[2]; //拟合衰当曲线的截取范围0 - 最小值 1-最大值 2 - 是否需要重新分配内存
public:
	bool isworking();
	bool isfinishing();
	bool setcut(int a,int b); //设定拟合范围
	void getcut(USHORT* tem);              //获取拟合范围




//拟合数据列队控制
public:
	float *fit_rank[100];                       //拟合数据列队
	int p_start;                               //列队起始位置指针
	int p_end;                                    //列队结尾指针
	int ranksize;                                 //列队容量

	void rankinitial();
	bool pump(float*);
	float* pop();
	void destroyrank();
		//接口控制

	HWND m_RDView;
	HWND m_TView;
	HWND m_ControlForm;
	// 获取cut指针
	void getcutp(USHORT** p_p_cut);
public:

	// 是否重置数据
	char filename[100];
	bool isreset;
	// 叠加次数
	int n_add_t;
	// 真空衰当曲线
	float RDValues[100];
	int pRD;
	// 测试状态
	bool istest;
	// 标定吸收系数的类别 1-背景 2-检测气体
	int A_flag;
};
