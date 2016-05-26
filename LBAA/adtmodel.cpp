#include "StdAfx.h"
#include "adtmodel.h"
#include "PCH8522.h"
#include <exception>

#define M_LENGTH 320 //120//320    
#define N_LENGTH 880  //280//1280
#define N_ADDMAX 400 //叠加次数                 62,125,250...
#define DEV_C 8   //与叠加次数对应单位变换   8,9,10

HANDLE rankMutex;
static UINT calthread(LPVOID lParam)
{
	//////////////////////////////some thing no use
	//CNiReal64Vector tem6;
	//double dd1;
	//double dd2;


	/////////////////////////////////////////////////////////////////////////////////////
	points* temq;
	/////逻辑控制参数

	/////数学计算参数
	float data_x;
	float data_x2;
	float data_xy;
	float data_y;

	/////////////////////////////

	////////////////////////////////线程参数初始化
	float BaseLine;                                         //基线
	float* predata = NULL;                                  //拟合区间曲线y
	float *data_rd = NULL;                                  //曲线数据y
	float* xdata = NULL;                                    //曲线数据x
	USHORT cut_c[2];                                        //拟合衰当曲线的截取范围   0 - 最小值   1 - 最大值   2 - 是否需要重新分配内存
	adtmodel *model =(adtmodel *)lParam;                    //线程维护模型对象
	//double damp;                                            //斜率       
	float dRDValue;                                         //衰当时间
	float dRDValue_max = 0;                                 //衰当时间最高值
	float dRDValue_min = 9999;                              //衰当时间最低值
	time_t curtime;                                         //时间戳
//	float dRDValues[100]                                    //衰当时间平均缓存
	int num_dRDValues = 0;                                  //dRDValues的计数器
	float m_RDValues[100] = {0};                            //
	float meanRDValue = 0;                                  //
	char buf[100];                                          //字符串缓存 
	byte num_sam = 0;                                        //样品标号
	FILE *pf;                                               //数据文件
	float *uu;                                               //界面数据传输参数
	float *m_RDValuess = NULL;                               //持续测试的缓存 
	int num_dRDValuess = 0;                                  //持续测试的计数器
	float sum = 0;                                           //计算平均值
	float x2sum = 0;                                         //计算方差
	CString timet;
////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////等待初始化
	while(model->m_ControlForm == NULL||model->m_RDView == NULL||model->m_TView == NULL)
	{
		Sleep(100);
	}
	CString file_str; //获取系统时间 　
	CTime tmm;
	tmm=CTime::GetCurrentTime();
	file_str=tmm.Format("d:/data/%Y_%m_%d %H.%M.csv");
	::SendMessageA(model->m_ControlForm,WMSETPARTEXT,(WPARAM)&file_str,3);
	///////////////////////////////////////////衰当曲线x轴数值初始化
	xdata = new float[M_LENGTH+N_LENGTH-200];
	for(int i = 0;i<M_LENGTH+N_LENGTH-200;i++)
	{
		xdata[i] = (float)i/80;
	}
	m_RDValuess = new float[8640000];
///////////////////////////////////////////////////////////////////

	while(true)
	{
		
		data_rd = model->pop();

////////////////////////////////////////提取数据
		if(data_rd == NULL)
		{
			Sleep(100);
			continue;
		}
		if(model->isfinishing())
		{
			break;
		}
///////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////基线减除
		BaseLine = 0;   //基线赋初值
		for(int i = M_LENGTH+N_LENGTH-400, j = 0;j<150;i++,j++)
			BaseLine += data_rd[i];//加和
		BaseLine /= 150;//求平均
		for(int i = 0;i<M_LENGTH+N_LENGTH-200;i++)
		{
			data_rd[i] -= BaseLine;
		}
/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////拟合曲线剪切
		model->getcut(cut_c);
		if(predata != NULL)
		{
			delete predata;
			predata = NULL;
		}
		predata = new float[cut_c[1]];
		memcpy(predata,data_rd+cut_c[0],cut_c[1]*sizeof(float));
/////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////曲线拟合1
		//CNiReal32Vector x_cal(cut_c[1],xdata);
		//CNiReal32Vector y_cal(cut_c[1],predata);
		//CNiMath::ExpFit(x_cal,
		//	y_cal,
		//	CNiMath::LeastSquare,
		//	tem6,
		//	dd1,
		//	damp,
		//	dd2);

		//dRDValue = -1/damp;
////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////曲线拟合	2	
		data_x = 0;
		data_x2 = 0;
		data_xy = 0;
		data_y = 0;
		float temlogy;
		for(int i = 0;i<cut_c[1];i++)
		{
			temlogy = log(abs(predata[i]));
			data_y += temlogy;
			data_x += xdata[i];
			data_x2 += xdata[i]*xdata[i];
			data_xy += xdata[i]*temlogy;
		}
		dRDValue = -(cut_c[1]*data_x2-data_x*data_x)/(cut_c[1]*data_xy-data_x*data_y);



//////////////////////////////////////////////////////////////////////////////












//////////////////////////////////////////////////////////////////////////////
//
#pragma region 显示及保存


///////////////////////////////////////////////////////////////////////是否重置显示数据

		if(model->isreset)
		{
			temq = new points;
			temq->mode = 4;
			model->isreset = false;
			::PostMessage(model->m_TView,WM_ONPLOT,(WPARAM)temq,NULL);
			dRDValue_max = 0;                                 //衰当时间最高值
			dRDValue_min = 9999;                              //衰当时间最低值
			num_dRDValuess = 0;
		}
////////////////////////////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////数据显示及后处理
		temq = new points;
		temq->px = new float[M_LENGTH+N_LENGTH-200];
		memcpy(temq->px, xdata, sizeof(float)*(M_LENGTH+N_LENGTH-200));
		temq->py = data_rd;
		temq->mode = 1;
		temq->n = M_LENGTH+N_LENGTH-200;
		temq->min = -500;
		temq->max = 50;
		::PostMessageA(model->m_RDView,WM_ONPLOT,(WPARAM)temq,NULL);
/////////////////////////////////////////////////////////////////////////////获取当前时间
		curtime = time(0); 
////////////////////////////////////////////////////////////////////
		if(model->ispost2view)
		{
///////////////////////////////////////////////////////////////////计算衰当时间最大最小值
			if(dRDValue_max < dRDValue)
				dRDValue_max = dRDValue;
			if(dRDValue_min > dRDValue)
				dRDValue_min = dRDValue;
/////////////////////////////////////////////////////////////////////////////////////生成消息包
			temq = new points;
			temq->py =  new float;
			temq->py[0] = dRDValue;
			temq->px = new float;
			temq->px[0] = 0;
			temq->min = dRDValue_min;
			temq->max = dRDValue_max;
			temq->mode = 3;
			temq->time = curtime;
			temq->n = 240;
			::PostMessageA(model->m_TView,WM_ONPLOT,(WPARAM)temq,NULL);
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////生成消息包
			//temq = new points;
			//temq->py =  new float;
			//temq->py[0] = RD_tem;
			//temq->px = new float;
			//temq->px[0] = 0;
			//temq->min = dRDValue_min;
			//temq->max = dRDValue_max;
			//temq->mode = 3;
			//temq->time = curtime;
			//temq->n = 240;
			//::PostMessageA(model->m_TView,WM_ONPLOT,(WPARAM)temq,NULL);		
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////数据处理  计算平均值以及方差
			m_RDValuess[num_dRDValuess]= dRDValue;                  ///累加器
			num_dRDValuess++;
			sum = 0;
			x2sum = 0;
			for(int i = 0; i<num_dRDValuess;i++)
			{
				sum += m_RDValuess[i];
			}
			sum /= num_dRDValuess;
			meanRDValue = sum;
			for(int i = 0;i<num_dRDValuess;i++)
			{
				x2sum += (m_RDValuess[i]-sum)*(m_RDValuess[i]-sum);
			}
			x2sum /= (num_dRDValuess-1);
			x2sum = sqrt(x2sum)/sum*100;
			uu = new float(sum);
			::PostMessageA(model->m_ControlForm,WMSETPARTEXT,(WPARAM)uu,1);
			uu = new float(x2sum);
			::PostMessageA(model->m_ControlForm,WMSETPARTEXT,(WPARAM)uu,2);
		}
////////////////////////////////////////////////////////////////////////////////////记录数据文件
		if(model->isstore)
		{
			tm tim;
			localtime_s(&tim, &curtime);
			char title[25];
			sprintf_s(title,25, "%02d:%02d:%02d %0.3f\n",tim.tm_hour, tim.tm_min, tim.tm_sec,dRDValue);
			FILE *pfile;
			fopen_s(&pfile,model->filename,"a");
			fwrite(title,1,strlen(title),pfile);
			fclose(pfile);
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion
		//数值累加,显示
		if(model->istest)     //receiveRDdata
		{
			model->ispost2view = false;
			temq = new points;
			temq->mode = 2;
			temq->n = 240;
			temq->py = new float;
			temq->py[0] =  dRDValue;
			temq->px = new float;
			temq->px[0] = 0;
			::PostMessage(model->m_TView,WM_ONPLOT,(WPARAM)temq,NULL);
///////////////////////////////////////////////////////写入文件
			tmm = CTime::GetCurrentTime();
			timet = tmm.Format("%H:%M:%S");
			sprintf_s(buf,100,"%d,%f,%s\n",num_dRDValues,dRDValue,timet);	
			fopen_s(&pf,file_str,"a");
			fwrite(buf,1,strlen(buf),pf);
			fclose(pf);
//////////////////////////////////////////////////////////
			m_RDValues[num_dRDValues]= dRDValue;              ///累加器
			num_dRDValues++;
		}
		//累加结果处理，计算吸收系数
		if(num_dRDValues >= 20)
		{
//////////////////////////////////////////////////////////计算均数及标准差
			sum = 0;
			x2sum = 0;
			for(int i = 0; i<num_dRDValues;i++)
			{
			   sum += m_RDValues[i];
			}
			sum /= num_dRDValues;
			meanRDValue = sum;
			for(int i = 0;i<num_dRDValues;i++)
			{
				x2sum += (m_RDValues[i]-sum)*(m_RDValues[i]-sum);
			}
			x2sum /= (num_dRDValues-1);
			x2sum = sqrt(x2sum)/sum*100;
			uu = new float(sum);
			::PostMessageA(model->m_ControlForm,WMSETPARTEXT,(WPARAM)uu,1);
			uu = new float(x2sum);
			::PostMessageA(model->m_ControlForm,WMSETPARTEXT,(WPARAM)uu,2);
////////////////////////////////////////////////写入文件
			tmm = CTime::GetCurrentTime();
			timet = tmm.Format("%H:%M:%S");
			sprintf_s(buf,100,"平均数,%f\n标准差,%f,%s\n",meanRDValue,x2sum,timet);	
			fopen_s(&pf,file_str,"a");
			fwrite(buf,1,strlen(buf),pf);
			fclose(pf);
/////////////////////////////////////////////////
			model->RDValues[model->pRD] = meanRDValue;
			num_dRDValues = 0;
			meanRDValue = 0;
			model->pRD++;
			model->istest = false;
			float tems = 0;
///////////////////////////////////////////////////////case2参数初始化
			int meanum;  //采集次数
			float Abkg;
			byte *aa;
			char *aaa;
			UINT16 CRC;
			union
			{
				float ft;
				unsigned char ss[4];
			}ppm;
			switch(model->A_flag)
			{
			case 1:
				//背景
				meanum = model->pRD/2; //采集次数
				for(int i=0;i<meanum;i++)
				{
					tems +=0.0014f*( 1/model->RDValues[2*i+1] - 1/model->RDValues[2*i]);
				}
				tems /= meanum;

				//////////////////////////////////////////////写入文件
				tmm = CTime::GetCurrentTime();
				timet = tmm.Format("%H:%M:%S");
				sprintf_s(buf,100,"背景吸光度,%f\n,%s",tems,timet);	
				fopen_s(&pf,file_str,"a");
				fwrite(buf,1,strlen(buf),pf);
				fclose(pf);
				///////////////////////////////////////////////////////
				model->pRD = 0;
				sprintf_s(buf,100,"%f",tems);
				::WritePrivateProfileString(_T("LBAA"),_T("Abkg"),buf,_T(".//setting.ini"));
				aaa = new char[10];
				strcpy_s(aaa,10,"Bak_OK;");
				::PostMessage(model->m_ControlForm,ON_SENDCOM,(WPARAM)aaa,7);
				break;
			case 2:
				//检测气体
				meanum = model->pRD/2; //采集次数
				for(int i=0;i<meanum;i++)
				{
					tems +=0.0014f*( 1/model->RDValues[2*i+1] - 1/model->RDValues[2*i]);
				}
				tems /= meanum;
				//////////////////////////////////////////////写入文件
				tmm = CTime::GetCurrentTime();
				timet = tmm.Format("%H:%M:%S");
				sprintf_s(buf,100,"呼吸气体吸光度,%f,%s\n",tems,timet);	
				fopen_s(&pf,file_str,"a");
				fwrite(buf,1,strlen(buf),pf);
				fclose(pf);
				///////////////////////////////////////////////////////
				model->pRD = 0;
				::GetPrivateProfileStringA(_T("LBAA"),_T("Abkg"),_T("0"),buf,20,_T(".//setting.ini"));
				Abkg = (float)atof(buf);


				num_sam++;

				ppm.ft=(float)(72.1*(25+273.15)*(tems - Abkg));

				//////////////////////////////////////////////////////////////////////////////发送信息到界面
				uu = new float(ppm.ft);
				::PostMessageA(model->m_ControlForm,WMSETPARTEXT,(WPARAM)uu,6);
				//////////////////////////////////////////////写入文件
				tmm = CTime::GetCurrentTime();
				timet = tmm.Format("%H:%M:%S");
				sprintf_s(buf,100,"呼吸气体丙酮浓度,%f,%s\n",ppm.ft,timet);	
				fopen_s(&pf,file_str,"a");
				fwrite(buf,1,strlen(buf),pf);
				fclose(pf);
///////////////////////////////////////////////////////
				aa = new byte[9];
				aa[0]=0x01;
				aa[1]=num_sam;
				memcpy(aa+2,&ppm.ss[3],1);
				memcpy(aa+3,&ppm.ss[2],1);
				memcpy(aa+4,&ppm.ss[1],1);
				memcpy(aa+5,&ppm.ss[0],1);
				CRC=0xffff;
				for(int j=0;j<6;j++)
				{
					UCHAR  i,TT;
					CRC = CRC^aa[j];
					for(i=0;i<8;i++)
					{
						TT = CRC&1;
						CRC = CRC>>1;
						CRC = CRC&0x7FFF;
						if(TT==1)
							CRC = CRC^0xA001;
						CRC = CRC&0xFFFF;
					}
				}
				memcpy(aa+6,&CRC,2);
				aa[8]=0x3B;
				::PostMessage(model->m_ControlForm,ON_SENDCOM,(WPARAM)aa,9);
				break;
			default:

				aaa = new char[20];
				strcpy_s(aaa,20,"RDdata_OK;");
				::PostMessage(model->m_ControlForm,ON_SENDCOM,(WPARAM)aaa,10);
				//	
				break;
			}
		}
	}
	return 0;
}
static UINT workthread(LPVOID lParam)  
{

//////////////////////////////////////////////////////////////////////////////////////
	//     section：变量初始化，设备初始化
	//     
	//


	adtmodel *model = (adtmodel *)lParam;//模型
	USHORT ADBuffer[8096];
	HANDLE hDevice = NULL;
	ULONG ulDDR2Length;//DDR2容量
	PCH8522_PARA_AD ADPara; // AD转换器参数
	PCH8522_STATUS_AD ADStatus;//AD转换器状态
    bool isstarted = false;   //是否已经开始数据捕捉
	long offsetAddr = 0;      //AD内存指针（有正负)
//	ULONG ulStartAddr;        //AD内存指针（校正）
	long nReadSizeWords = M_LENGTH + N_LENGTH;//读取数据数
	int pos = 0;
//	USHORT source[N_ADDMAX][M_LENGTH+N_LENGTH-200];//叠加缓冲区
	USHORT **source;//叠加缓冲区
	float data_rd[M_LENGTH+N_LENGTH-200]; //RD曲线数据
	ULONG temdata;   //计算RD曲线数据缓存
	float lam_de = 500.0f/2048.0f/N_ADDMAX;  //换算系数
	source = new USHORT*[N_ADDMAX];
	for(int i = 0;i<N_ADDMAX; i++)
	{
		source[i] = new USHORT[M_LENGTH+N_LENGTH-200];
	}

#pragma region PCH8522初始化
	hDevice = PCH8522_CreateDevice(); // 创建设备对象
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		::MessageBox(NULL,_T("数据采集卡初始化失败，请重新启动程序!"),_T("警告"),0);
		::MessageBox(NULL,_T("工作线程已停止，请重新启动程序!"),_T("警告"),0);
		return 0;
	}

	PCH8522_GetDDR2Length(hDevice, &ulDDR2Length);//获取DDR2的容量
	memset(&ADPara, 0x00, sizeof(ADPara)); // 将各项参数复位至确定值0(强烈建议)
	// 预置硬件参数
	ADPara.bChannelArray[0]		= true;
	ADPara.bChannelArray[1]		= false;
	ADPara.Frequency			= 80000000;				// 采样频率(Hz)
	ADPara.InputRange[0]		= 1;			// 接收数据量程：0--1000mv   1--5000mv
	ADPara.InputRange[1]		= 1;			// 接收数据量程：0--1000mv   1--5000mv
	ADPara.CouplingType			= PCH8522_COUPLING_DC;	// 直流耦合
	ADPara.M_Length				= M_LENGTH;			// M长度
	ADPara.N_Length				= N_LENGTH;			// N长度
	ADPara.TriggerMode			= PCH8522_TRIGMODE_MIDL; // 硬件中间触发(包括预触发、后触发功能)
	ADPara.ATRTriggerChannel	= 0;					// 通道0信号作为ATR输入
	ADPara.TriggerSource		= /* PCH8522_TRIGMODE_SOFT;*/PCH8522_TRIGSRC_ATR;	// 选择ATR作为触发源
	ADPara.TriggerDir			= PCH8522_TRIGDIR_NEGATIVE; /*PCH8522_TRIGDIR_POSITIVE;*/	// 下降沿触发
	ADPara.TrigLevelVolt		= -20;
	if(!PCH8522_InitDeviceAD(hDevice, &ADPara)) // 初始化硬件
	{
		::MessageBox(NULL,_T("设备初始化失败"),_T("警告"),0);
		::MessageBox(NULL,_T("工作线程已停止，请重新启动程序!"),_T("警告"),0);
		PCH8522_ReleaseDeviceAD(hDevice);
		return 0;
	}
#pragma endregion



	///////////////////////////////////////////////////////
	//    section：主流程
	//    实现方式：通过while循环进行流程控制，与单片机类似
	//
	//
	while(true)
	{
		if(!model->isworking())
		{
			continue;
			Sleep(100);
		}
		if(model->isfinishing())
		{
			Sleep(50);
			break;
		}
#pragma region PCH8522再初始化
		if(!isstarted)
		{

			PCH8522_StartDeviceAD(hDevice);
			isstarted = true;
		}
		if(!PCH8522_GetDevStatusAD(hDevice,&ADStatus))
			AfxMessageBox(_T("获取AD状态失败"));
		if(!ADStatus.bComplete)
		{
			continue;
		}
		PCH8522_StopDeviceAD(hDevice);
		isstarted = false;

		/////////////////////////////////////////////////////////////////////////////////
		//   section:从AD板卡读取数据到计算机内存
		//
		//
		//
		//
		long offsetAddr=ADStatus.lEndAddr-nReadSizeWords*sizeof(USHORT)+4;
		ULONG ulStartAddr;
		if(offsetAddr<0)
			ulStartAddr=ulDDR2Length*0124*1024+offsetAddr;
		else
			ulStartAddr=(ULONG)offsetAddr;
		PCH8522_ReadDeviceAD(hDevice,ADBuffer,nReadSizeWords,ulStartAddr,NULL);
		///////////////////////////////////////////////////////////////////////////////////

#pragma endregion

#pragma region 对齐 //从ADBuffer中读取数据到缓存，并进行数据对齐

		///////////////////////////////////////////////////////////////////////////////////
		lam_de = 500.0f/2048.0f/model->n_add_t;                     //换算系数
		//USHORT tem = ADBuffer[M_LENGTH-100]&0x0FFF;       //最低值
		//int index = M_LENGTH-100;                         //最低值的索引数
		//for(int i = M_LENGTH - 100;i<M_LENGTH+100;i++)
		//{
		//	if((ADBuffer[i]&0x0FFF) < tem)
		//	{
		//		tem = ADBuffer[i]&0x0FFF;
		//		index = i;
		//	}
		//}
		int mem_start = 200;//index-M_LENGTH+100;
		if(mem_start < 0)
			mem_start = 0;                              //对齐后内存起始点
		//		int mem_end = mem_start + M_LENGTH+N_LENGTH-200;
		//		for(int i = mem_start, j = 0; i<mem_end;i++,j++)
		if(pos<model->n_add_t)
		{
			for(int i = mem_start, j = 0; j < M_LENGTH+N_LENGTH-200 ;i++,j++)
			{

				source[pos][j] = ADBuffer[i]&0x0FFF;

			}
			pos++;
			continue;
		}
		pos = 0;
		////////////////////////////////////////////////////////////////////////////
		//加和取平均
		for(int i=0;i<M_LENGTH+N_LENGTH-200;i++)
		{
			temdata=0;
			for(int j=0;j<model->n_add_t;j++)
				temdata+=source[j][i];
			data_rd[i]=(float)temdata*lam_de-500.0f;
//			data_rd[i]=log(1-(data_rd[i]));//取对数
		}
		
		///////////////////////////////////////////////////////////////////////////////////
		model->pump(data_rd);                                                          //拟合数据入栈
		///////////////////////////////////////////////////////////////////////////////////////
#pragma endregion
	}///循环结束


	PCH8522_ReleaseDeviceAD(hDevice);
	PCH8522_ReleaseDevice(hDevice);
	return 0;
}
//////////////////////////////////////////////////////坐标轴测试线程
static UINT axistest(LPVOID lParam)
{
	adtmodel *model = (adtmodel *)lParam;//模型
	float i = 0;
	points* dd;
	while(true)
	{

		Sleep(50);
		if(model->isfinishing())
		{
			Sleep(50);
			break;
		}
		dd = new points;
		dd->mode = 3;
		dd->n = 330;
		dd->py = new float;
		dd->py[0] =  sin(i);
		dd->px = new float;
		dd->px[0] = 0;
		::PostMessage(model->m_TView,WM_ONPLOT,(WPARAM)dd,NULL);



		dd = new points;
		dd->mode = 1;
		dd->n = 2400;
		dd->py = new float[2400];
		dd->px = new float[2400];
		for(int j = 0;j<2400;j++)
		{
			dd->px[j] = (float)j/100;
			dd->py[j] = sin((float)j/100)+((float)rand()-50)*0.00001f;
		}
		dd->min = -2;
		dd->max = 2;
		::PostMessage(model->m_RDView,WM_ONPLOT,(WPARAM)dd,NULL);
		i=i+0.1f;
	}
}
static UINT caltest(LPVOID lParam)
{
	adtmodel *model = (adtmodel *)lParam;//模型
	float data_rd[M_LENGTH+N_LENGTH-200]; //RD曲线数据
	while(true)
	{
		Sleep(50);

		for(int i = 0;i<M_LENGTH+N_LENGTH-200;i++)
		{
			if(i<200)
			{
				data_rd[i] =(float)rand()/10000+ 20;
			}
			else
			{
				data_rd[i] = (float)rand()/10000+20-exp(-(float)(i-750)/100);
			}
		}
		model->pump(data_rd);

	}

}
adtmodel* adtmodel::m_pInstance = NULL;
adtmodel::adtmodel(void)
: iswork(false),
  isfinish(false),
  m_RDView(NULL),
  m_TView(NULL),
  m_ControlForm(NULL)
  , ispost2view(false)
  , isstore(false)
  , isreset(false)
  , n_add_t(100)
  , pRD(0)
  , istest(false)
  , A_flag(0)
{
	static Deleter deleter;
	char tem_str[20];
	int cut_1,cut_2;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////配置文档初始化
	float Abkg = 0;
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind = finder.FindFile(_T("setting.ini"));
	if( !ifFind )  
	{
		::WritePrivateProfileString(_T("LBAA"),_T("cut1"),_T("240"),_T(".//setting.ini"));
		::WritePrivateProfileString(_T("LBAA"),_T("cut2"),_T("330"),_T(".//setting.ini"));
		::WritePrivateProfileString(_T("LBAA"),_T("com"),_T("1"),_T(".//setting.ini"));
		::WritePrivateProfileString(_T("LBAA"),_T("Abkg"),_T("0"),_T(".//setting.ini"));
	}
	::GetPrivateProfileStringA(_T("LBAA"),_T("cut1"),_T("240"),tem_str,20,_T(".//setting.ini"));
	cut_1 = atoi(tem_str);
	::GetPrivateProfileStringA(_T("LBAA"),_T("cut2"),_T("330"),tem_str,20,_T(".//setting.ini"));
	cut_2 = atoi(tem_str);
	::GetPrivateProfileStringA(_T("LBAA"),_T("com"),_T("1"),tem_str,20,_T(".//setting.ini"));
	::GetPrivateProfileStringA(_T("LBAA"),_T("Abkg"),_T("0"),tem_str,20,_T(".//setting.ini"));
	Abkg = (float)atof(tem_str);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	setcut(cut_1,cut_2);
	rankinitial();
	
	AfxBeginThread(calthread,this);
//	AfxBeginThread(axistest,this);
#ifdef DEBUG
	AfxBeginThread(caltest,this);
#else
	AfxBeginThread(workthread,this);
#endif
	iswork = true;

}
adtmodel::~adtmodel()
{

//////////////////////////////////////////////////////////////////////////配置文档保存
/////////////////////////////////////////////////////////////////////////////////////////////

	isfinish = true;
	Sleep(100);
	destroyrank();
//////////////////////////////////////////////////////////////////////
}
adtmodel *adtmodel::GetInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new adtmodel();
	}
	return m_pInstance;
}
#pragma region 列队维护
//列队初始化
void adtmodel::rankinitial()
{
	rankMutex = CreateMutex(NULL,false,NULL);
	p_start = p_end = 0;
	ranksize = 100;
	for(int i = 0;i < 100;i++)
	{
		fit_rank[i] = NULL;
	}

}

//入栈
bool adtmodel::pump(float *p)
{
	WaitForSingleObject(rankMutex,INFINITE);
	if(((p_end+1)%ranksize) == p_start)
	{
		ReleaseMutex(rankMutex);
		return false;
	}
	fit_rank[p_end] = new float[M_LENGTH+N_LENGTH-200];
	//入列数据
	//
	//
	//
	memcpy(fit_rank[p_end],p,sizeof(float)*(M_LENGTH+N_LENGTH-200));
	p_end = (p_end+1)%ranksize;
	ReleaseMutex(rankMutex);
	return true;
}

//销毁列队
void adtmodel::destroyrank()  
{
	p_start = p_end = 0;
	for(int i = 0;i < 100;i++)
	{
		if(fit_rank[i] != NULL)
		{
			delete fit_rank[i];
			fit_rank[i] = NULL;
		}
	}
}  


/*出栈*/  
float *adtmodel::pop()  
{  
	WaitForSingleObject(rankMutex,INFINITE);
	float *output;  
	if(p_start == p_end)
	{
		ReleaseMutex(rankMutex);
		return NULL;
	}
	output = fit_rank[p_start];
	/////////////////////////////////////出栈时一定要将指针归NULL
	fit_rank[p_start] = NULL;
	p_start = (p_start + 1)%ranksize;
	ReleaseMutex(rankMutex);
	return output;
}  



#pragma endregion
bool adtmodel::isworking()
{
	return iswork;
}
bool adtmodel::isfinishing()
{
	return isfinish;
}
void adtmodel::getcut(USHORT * tem)              //获取拟合范围
{
	tem[0] = cut_c[0];
	tem[1] = cut_c[1];
}
bool adtmodel::setcut(int a,int b)
{
	if(a>0&&b>0&&a<(M_LENGTH+N_LENGTH-200)&&b<(M_LENGTH+N_LENGTH-200))
	{
		if(a<b)
		{
			cut_c[0] = a;
			cut_c[1] = b - a;
			return true;
		}
		else if(a>b)
		{
			cut_c[0] = b;
			cut_c[1] = a - b;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}// 获取cut指针                                                         
void adtmodel::getcutp(USHORT** p_p_cut)
{
	*p_p_cut = cut_c;
}