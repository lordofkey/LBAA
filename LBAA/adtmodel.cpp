#include "StdAfx.h"
#include "adtmodel.h"
#include "PCH8522.h"
#include <exception>

#define M_LENGTH 320 //120//320    
#define N_LENGTH 880  //280//1280
#define N_ADDMAX 400 //���Ӵ���                 62,125,250...
#define DEV_C 8   //����Ӵ�����Ӧ��λ�任   8,9,10

HANDLE rankMutex;
static UINT calthread(LPVOID lParam)
{
	//////////////////////////////some thing no use
	//CNiReal64Vector tem6;
	//double dd1;
	//double dd2;


	/////////////////////////////////////////////////////////////////////////////////////
	points* temq;
	/////�߼����Ʋ���

	/////��ѧ�������
	float data_x;
	float data_x2;
	float data_xy;
	float data_y;

	/////////////////////////////

	////////////////////////////////�̲߳�����ʼ��
	float BaseLine;                                         //����
	float* predata = NULL;                                  //�����������y
	float *data_rd = NULL;                                  //��������y
	float* xdata = NULL;                                    //��������x
	USHORT cut_c[2];                                        //���˥�����ߵĽ�ȡ��Χ   0 - ��Сֵ   1 - ���ֵ   2 - �Ƿ���Ҫ���·����ڴ�
	adtmodel *model =(adtmodel *)lParam;                    //�߳�ά��ģ�Ͷ���
	//double damp;                                            //б��       
	float dRDValue;                                         //˥��ʱ��
	float dRDValue_max = 0;                                 //˥��ʱ�����ֵ
	float dRDValue_min = 9999;                              //˥��ʱ�����ֵ
	time_t curtime;                                         //ʱ���
//	float dRDValues[100]                                    //˥��ʱ��ƽ������
	int num_dRDValues = 0;                                  //dRDValues�ļ�����
	float m_RDValues[100] = {0};                            //
	float meanRDValue = 0;                                  //
	char buf[100];                                          //�ַ������� 
	byte num_sam = 0;                                        //��Ʒ���
	FILE *pf;                                               //�����ļ�
	float *uu;                                               //�������ݴ������
	float *m_RDValuess = NULL;                               //�������ԵĻ��� 
	int num_dRDValuess = 0;                                  //�������Եļ�����
	float sum = 0;                                           //����ƽ��ֵ
	float x2sum = 0;                                         //���㷽��
	CString timet;
////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////�ȴ���ʼ��
	while(model->m_ControlForm == NULL||model->m_RDView == NULL||model->m_TView == NULL)
	{
		Sleep(100);
	}
	CString file_str; //��ȡϵͳʱ�� ��
	CTime tmm;
	tmm=CTime::GetCurrentTime();
	file_str=tmm.Format("d:/data/%Y_%m_%d %H.%M.csv");
	::SendMessageA(model->m_ControlForm,WMSETPARTEXT,(WPARAM)&file_str,3);
	///////////////////////////////////////////˥������x����ֵ��ʼ��
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

////////////////////////////////////////��ȡ����
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

//////////////////////////////////////////////////////////////���߼���
		BaseLine = 0;   //���߸���ֵ
		for(int i = M_LENGTH+N_LENGTH-400, j = 0;j<150;i++,j++)
			BaseLine += data_rd[i];//�Ӻ�
		BaseLine /= 150;//��ƽ��
		for(int i = 0;i<M_LENGTH+N_LENGTH-200;i++)
		{
			data_rd[i] -= BaseLine;
		}
/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////������߼���
		model->getcut(cut_c);
		if(predata != NULL)
		{
			delete predata;
			predata = NULL;
		}
		predata = new float[cut_c[1]];
		memcpy(predata,data_rd+cut_c[0],cut_c[1]*sizeof(float));
/////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////�������1
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
///////////////////////////////////////////////////////�������	2	
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
#pragma region ��ʾ������


///////////////////////////////////////////////////////////////////////�Ƿ�������ʾ����

		if(model->isreset)
		{
			temq = new points;
			temq->mode = 4;
			model->isreset = false;
			::PostMessage(model->m_TView,WM_ONPLOT,(WPARAM)temq,NULL);
			dRDValue_max = 0;                                 //˥��ʱ�����ֵ
			dRDValue_min = 9999;                              //˥��ʱ�����ֵ
			num_dRDValuess = 0;
		}
////////////////////////////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////������ʾ������
		temq = new points;
		temq->px = new float[M_LENGTH+N_LENGTH-200];
		memcpy(temq->px, xdata, sizeof(float)*(M_LENGTH+N_LENGTH-200));
		temq->py = data_rd;
		temq->mode = 1;
		temq->n = M_LENGTH+N_LENGTH-200;
		temq->min = -500;
		temq->max = 50;
		::PostMessageA(model->m_RDView,WM_ONPLOT,(WPARAM)temq,NULL);
/////////////////////////////////////////////////////////////////////////////��ȡ��ǰʱ��
		curtime = time(0); 
////////////////////////////////////////////////////////////////////
		if(model->ispost2view)
		{
///////////////////////////////////////////////////////////////////����˥��ʱ�������Сֵ
			if(dRDValue_max < dRDValue)
				dRDValue_max = dRDValue;
			if(dRDValue_min > dRDValue)
				dRDValue_min = dRDValue;
/////////////////////////////////////////////////////////////////////////////////////������Ϣ��
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
/////////////////////////////////////////////////////////////////////////////////////������Ϣ��
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
///////////////////////////////////////////////////////////////////////////////���ݴ���  ����ƽ��ֵ�Լ�����
			m_RDValuess[num_dRDValuess]= dRDValue;                  ///�ۼ���
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
////////////////////////////////////////////////////////////////////////////////////��¼�����ļ�
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
		//��ֵ�ۼ�,��ʾ
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
///////////////////////////////////////////////////////д���ļ�
			tmm = CTime::GetCurrentTime();
			timet = tmm.Format("%H:%M:%S");
			sprintf_s(buf,100,"%d,%f,%s\n",num_dRDValues,dRDValue,timet);	
			fopen_s(&pf,file_str,"a");
			fwrite(buf,1,strlen(buf),pf);
			fclose(pf);
//////////////////////////////////////////////////////////
			m_RDValues[num_dRDValues]= dRDValue;              ///�ۼ���
			num_dRDValues++;
		}
		//�ۼӽ��������������ϵ��
		if(num_dRDValues >= 20)
		{
//////////////////////////////////////////////////////////�����������׼��
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
////////////////////////////////////////////////д���ļ�
			tmm = CTime::GetCurrentTime();
			timet = tmm.Format("%H:%M:%S");
			sprintf_s(buf,100,"ƽ����,%f\n��׼��,%f,%s\n",meanRDValue,x2sum,timet);	
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
///////////////////////////////////////////////////////case2������ʼ��
			int meanum;  //�ɼ�����
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
				//����
				meanum = model->pRD/2; //�ɼ�����
				for(int i=0;i<meanum;i++)
				{
					tems +=0.0014f*( 1/model->RDValues[2*i+1] - 1/model->RDValues[2*i]);
				}
				tems /= meanum;

				//////////////////////////////////////////////д���ļ�
				tmm = CTime::GetCurrentTime();
				timet = tmm.Format("%H:%M:%S");
				sprintf_s(buf,100,"���������,%f\n,%s",tems,timet);	
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
				//�������
				meanum = model->pRD/2; //�ɼ�����
				for(int i=0;i<meanum;i++)
				{
					tems +=0.0014f*( 1/model->RDValues[2*i+1] - 1/model->RDValues[2*i]);
				}
				tems /= meanum;
				//////////////////////////////////////////////д���ļ�
				tmm = CTime::GetCurrentTime();
				timet = tmm.Format("%H:%M:%S");
				sprintf_s(buf,100,"�������������,%f,%s\n",tems,timet);	
				fopen_s(&pf,file_str,"a");
				fwrite(buf,1,strlen(buf),pf);
				fclose(pf);
				///////////////////////////////////////////////////////
				model->pRD = 0;
				::GetPrivateProfileStringA(_T("LBAA"),_T("Abkg"),_T("0"),buf,20,_T(".//setting.ini"));
				Abkg = (float)atof(buf);


				num_sam++;

				ppm.ft=(float)(72.1*(25+273.15)*(tems - Abkg));

				//////////////////////////////////////////////////////////////////////////////������Ϣ������
				uu = new float(ppm.ft);
				::PostMessageA(model->m_ControlForm,WMSETPARTEXT,(WPARAM)uu,6);
				//////////////////////////////////////////////д���ļ�
				tmm = CTime::GetCurrentTime();
				timet = tmm.Format("%H:%M:%S");
				sprintf_s(buf,100,"���������ͪŨ��,%f,%s\n",ppm.ft,timet);	
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
	//     section��������ʼ�����豸��ʼ��
	//     
	//


	adtmodel *model = (adtmodel *)lParam;//ģ��
	USHORT ADBuffer[8096];
	HANDLE hDevice = NULL;
	ULONG ulDDR2Length;//DDR2����
	PCH8522_PARA_AD ADPara; // ADת��������
	PCH8522_STATUS_AD ADStatus;//ADת����״̬
    bool isstarted = false;   //�Ƿ��Ѿ���ʼ���ݲ�׽
	long offsetAddr = 0;      //AD�ڴ�ָ�루������)
//	ULONG ulStartAddr;        //AD�ڴ�ָ�루У����
	long nReadSizeWords = M_LENGTH + N_LENGTH;//��ȡ������
	int pos = 0;
//	USHORT source[N_ADDMAX][M_LENGTH+N_LENGTH-200];//���ӻ�����
	USHORT **source;//���ӻ�����
	float data_rd[M_LENGTH+N_LENGTH-200]; //RD��������
	ULONG temdata;   //����RD�������ݻ���
	float lam_de = 500.0f/2048.0f/N_ADDMAX;  //����ϵ��
	source = new USHORT*[N_ADDMAX];
	for(int i = 0;i<N_ADDMAX; i++)
	{
		source[i] = new USHORT[M_LENGTH+N_LENGTH-200];
	}

#pragma region PCH8522��ʼ��
	hDevice = PCH8522_CreateDevice(); // �����豸����
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		::MessageBox(NULL,_T("���ݲɼ�����ʼ��ʧ�ܣ���������������!"),_T("����"),0);
		::MessageBox(NULL,_T("�����߳���ֹͣ����������������!"),_T("����"),0);
		return 0;
	}

	PCH8522_GetDDR2Length(hDevice, &ulDDR2Length);//��ȡDDR2������
	memset(&ADPara, 0x00, sizeof(ADPara)); // �����������λ��ȷ��ֵ0(ǿ�ҽ���)
	// Ԥ��Ӳ������
	ADPara.bChannelArray[0]		= true;
	ADPara.bChannelArray[1]		= false;
	ADPara.Frequency			= 80000000;				// ����Ƶ��(Hz)
	ADPara.InputRange[0]		= 1;			// �����������̣�0--1000mv   1--5000mv
	ADPara.InputRange[1]		= 1;			// �����������̣�0--1000mv   1--5000mv
	ADPara.CouplingType			= PCH8522_COUPLING_DC;	// ֱ�����
	ADPara.M_Length				= M_LENGTH;			// M����
	ADPara.N_Length				= N_LENGTH;			// N����
	ADPara.TriggerMode			= PCH8522_TRIGMODE_MIDL; // Ӳ���м䴥��(����Ԥ�������󴥷�����)
	ADPara.ATRTriggerChannel	= 0;					// ͨ��0�ź���ΪATR����
	ADPara.TriggerSource		= /* PCH8522_TRIGMODE_SOFT;*/PCH8522_TRIGSRC_ATR;	// ѡ��ATR��Ϊ����Դ
	ADPara.TriggerDir			= PCH8522_TRIGDIR_NEGATIVE; /*PCH8522_TRIGDIR_POSITIVE;*/	// �½��ش���
	ADPara.TrigLevelVolt		= -20;
	if(!PCH8522_InitDeviceAD(hDevice, &ADPara)) // ��ʼ��Ӳ��
	{
		::MessageBox(NULL,_T("�豸��ʼ��ʧ��"),_T("����"),0);
		::MessageBox(NULL,_T("�����߳���ֹͣ����������������!"),_T("����"),0);
		PCH8522_ReleaseDeviceAD(hDevice);
		return 0;
	}
#pragma endregion



	///////////////////////////////////////////////////////
	//    section��������
	//    ʵ�ַ�ʽ��ͨ��whileѭ���������̿��ƣ��뵥Ƭ������
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
#pragma region PCH8522�ٳ�ʼ��
		if(!isstarted)
		{

			PCH8522_StartDeviceAD(hDevice);
			isstarted = true;
		}
		if(!PCH8522_GetDevStatusAD(hDevice,&ADStatus))
			AfxMessageBox(_T("��ȡAD״̬ʧ��"));
		if(!ADStatus.bComplete)
		{
			continue;
		}
		PCH8522_StopDeviceAD(hDevice);
		isstarted = false;

		/////////////////////////////////////////////////////////////////////////////////
		//   section:��AD�忨��ȡ���ݵ�������ڴ�
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

#pragma region ���� //��ADBuffer�ж�ȡ���ݵ����棬���������ݶ���

		///////////////////////////////////////////////////////////////////////////////////
		lam_de = 500.0f/2048.0f/model->n_add_t;                     //����ϵ��
		//USHORT tem = ADBuffer[M_LENGTH-100]&0x0FFF;       //���ֵ
		//int index = M_LENGTH-100;                         //���ֵ��������
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
			mem_start = 0;                              //������ڴ���ʼ��
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
		//�Ӻ�ȡƽ��
		for(int i=0;i<M_LENGTH+N_LENGTH-200;i++)
		{
			temdata=0;
			for(int j=0;j<model->n_add_t;j++)
				temdata+=source[j][i];
			data_rd[i]=(float)temdata*lam_de-500.0f;
//			data_rd[i]=log(1-(data_rd[i]));//ȡ����
		}
		
		///////////////////////////////////////////////////////////////////////////////////
		model->pump(data_rd);                                                          //���������ջ
		///////////////////////////////////////////////////////////////////////////////////////
#pragma endregion
	}///ѭ������


	PCH8522_ReleaseDeviceAD(hDevice);
	PCH8522_ReleaseDevice(hDevice);
	return 0;
}
//////////////////////////////////////////////////////����������߳�
static UINT axistest(LPVOID lParam)
{
	adtmodel *model = (adtmodel *)lParam;//ģ��
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
	adtmodel *model = (adtmodel *)lParam;//ģ��
	float data_rd[M_LENGTH+N_LENGTH-200]; //RD��������
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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////�����ĵ���ʼ��
	float Abkg = 0;
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����  
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

//////////////////////////////////////////////////////////////////////////�����ĵ�����
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
#pragma region �ж�ά��
//�жӳ�ʼ��
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

//��ջ
bool adtmodel::pump(float *p)
{
	WaitForSingleObject(rankMutex,INFINITE);
	if(((p_end+1)%ranksize) == p_start)
	{
		ReleaseMutex(rankMutex);
		return false;
	}
	fit_rank[p_end] = new float[M_LENGTH+N_LENGTH-200];
	//��������
	//
	//
	//
	memcpy(fit_rank[p_end],p,sizeof(float)*(M_LENGTH+N_LENGTH-200));
	p_end = (p_end+1)%ranksize;
	ReleaseMutex(rankMutex);
	return true;
}

//�����ж�
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


/*��ջ*/  
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
	/////////////////////////////////////��ջʱһ��Ҫ��ָ���NULL
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
void adtmodel::getcut(USHORT * tem)              //��ȡ��Ϸ�Χ
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
}// ��ȡcutָ��                                                         
void adtmodel::getcutp(USHORT** p_p_cut)
{
	*p_p_cut = cut_c;
}