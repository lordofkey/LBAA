// MyControlForm.cpp : 实现文件
//

#include "stdafx.h"
#include "LBAA.h"
#include "MyControlForm.h"


// MyControlForm

IMPLEMENT_DYNCREATE(MyControlForm, CFormView)

MyControlForm::MyControlForm()
	: CFormView(MyControlForm::IDD)
	, comback(NULL)
	, comback_num(0)
{

}
MyControlForm::~MyControlForm()
{
}
void MyControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_com1);
	DDX_Control(pDX, IDC_SLIDER1, slider_add);
	DDX_Control(pDX, IDC_EDIT2, edit_add);
	DDX_Control(pDX, IDC_EDIT3, edit_meanvalue);
	DDX_Control(pDX, IDC_EDIT4, edit_stdvalue);
	DDX_Control(pDX, IDC_EDIT7, edit_consen);
	DDX_Control(pDX, IDC_FILENAME, static_filename);
	DDX_Control(pDX, IDC_FILENAME2, static_filename2);
}
BEGIN_MESSAGE_MAP(MyControlForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &MyControlForm::OnBnClickedButton1)
	ON_MESSAGE(ON_SENDCOM,&MyControlForm::Onsendcom)
	ON_BN_CLICKED(IDC_BUTTON3, &MyControlForm::OnBnClickedButton3)
	ON_WM_HSCROLL()
	ON_MESSAGE(WMSETPARTEXT, &MyControlForm::OnWmsetpartext)
END_MESSAGE_MAP()


// MyControlForm 诊断

#ifdef _DEBUG
void MyControlForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MyControlForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG
// MyControlForm 消息处理程序
void MyControlForm::OnBnClickedButton1()
{
	CString file_str;           //获取系统时间 　
	CTime tmm;
	tmm = CTime::GetCurrentTime();
	file_str = tmm.Format("d:/data/test_%Y_%m_%d %H.%M.txt");
	static_filename2.SetWindowTextA(file_str);
	strcpy_s(adtmodel::GetInstance()->filename,100,file_str);
	adtmodel::GetInstance()->isreset = true;
	adtmodel::GetInstance()->ispost2view = true;
	adtmodel::GetInstance()->isstore = true;
}
void MyControlForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	if(!m_com1.get_PortOpen())
	{
		m_com1.put_PortOpen(true);
	}
	m_com1.get_Input();
	slider_add.SetRange(1,400);//设置滑动范围
	slider_add.SetTicFreq(40);//每10个单位画一刻度
	slider_add.SetPos(100);
	adtmodel::GetInstance()->m_ControlForm = this->m_hWnd;
	edit_add.SetWindowTextA("100");
	m_com1.put_Output(COleVariant("DAQ_Ready;")); 
	AdjustToken();
	// TODO: 在此添加专用代码和/或调用基类
}
BEGIN_EVENTSINK_MAP(MyControlForm, CFormView)
	ON_EVENT(MyControlForm, IDC_MSCOMM1, 1, MyControlForm::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()
void MyControlForm::OnCommMscomm1()
{

	// TODO: Add your control notification handler code here   
	VARIANT variant_inp;  
	COleSafeArray safearray_inp;  
	LONG len,k;  
	BYTE rxdata[2048]; //设置BYTE数组 An 8-bit integerthat is not signed.   
	CString strtemp,buf;  


	if(m_com1.get_CommEvent()==2) //事件值为2表示接收缓冲区内有字符   
	{             ////////以下你可以根据自己的通信协议加入处理代码   
		variant_inp = m_com1.get_Input(); //读缓冲区   
		safearray_inp = variant_inp; //VARIANT型变量转换为ColeSafeArray型变量   
		len=safearray_inp.GetOneDimSize(); //得到有效数据长度   
		for(k=0;k<len;k++)  
			safearray_inp.GetElement(&k,rxdata+k);//转换为BYTE型数组   
		for(k=0;k<len;k++) //将数组转换为Cstring型变量   
		{  
			BYTE bt=*(char*)(rxdata+k); //字符型   
			strtemp.Format("%c",bt); //将字符送入临时变量strtemp存放   
			buf+=strtemp;
		} 
		if(strcmp(buf,"D0_Fit;")==0)
		{
			adtmodel::GetInstance()->istest = true;
			adtmodel::GetInstance()->A_flag = 0;
		}
		else if(strcmp(buf,"D1_Fit;")==0)
		{
			adtmodel::GetInstance()->istest = true;
			adtmodel::GetInstance()->A_flag = 0;
		}
		else if(strcmp(buf, "B_Over;")==0)
		{
			adtmodel::GetInstance()->istest = true;
			adtmodel::GetInstance()->A_flag = 1;
		}
		else if(strcmp(buf, "G_Over;")==0)
		{
			adtmodel::GetInstance()->istest = true;
			adtmodel::GetInstance()->A_flag = 2;
		}
		else if(strcmp(buf, "Data_RP;") == 0)
		{
			char buf1[30];
			sprintf_s(buf1,30,"repeat");
			OutputDebugString(buf1);
			::PostMessage(this->m_hWnd,ON_SENDCOM,(WPARAM)comback,(LPARAM)comback_num);
		}
		else if(strcmp(buf, "DAQ_OFF;") == 0)
		{
			::ExitWindowsEx(EWX_SHUTDOWN,NULL);
		}
		else
			m_com1.put_Output(COleVariant("Repeat;"));
	}
	//else if(buf.Compare("DSP_OK;")==0)
	//{

	//	dspready=true;

	//}

}
LRESULT MyControlForm::Onsendcom(WPARAM wpar,LPARAM lpar)
{
	BYTE *buf=(BYTE *)wpar;  //传输数据缓存指针
	long byte_num = (long)lpar;
	comback = buf;
	comback_num = byte_num;
	CByteArray bar;
	for(int i_c = 0;i_c < byte_num; i_c++)
	{
		bar.Add(*buf);
		buf++;
	}
	m_com1.put_Output(COleVariant(bar));
	return 0;
}
void MyControlForm::OnBnClickedButton3()
{
	adtmodel::GetInstance()->ispost2view = false;
	adtmodel::GetInstance()->isstore = false;
	// TODO: 在此添加控件通知处理程序代码
}
void MyControlForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int m_int = slider_add.GetPos();//取得当前位置值  
	CString text;
	text.Format("%d",m_int);
	edit_add.SetWindowTextA(text);
	adtmodel::GetInstance()->n_add_t = m_int;
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}
afx_msg LRESULT MyControlForm::OnWmsetpartext(WPARAM wParam, LPARAM lParam)  
{
	float *tempnum = (float*)wParam;
	CString tem;
	switch(lParam)
	{
	case 1:
		tem.Format("%0.4f",tempnum[0]);
		edit_meanvalue.SetWindowTextA(tem);
		delete tempnum;
		break;
	case 2:
		tem.Format("%0.4f",tempnum[0]);
		edit_stdvalue.SetWindowTextA(tem);
	    delete tempnum;
		break;
	case 3:
		tem = *((CString*)wParam);
		static_filename.SetWindowTextA(tem);
		break;
	case 6:
		tem.Format("%0.4f",tempnum[0]);
		edit_consen.SetWindowTextA(tem);
		delete tempnum;
		break;
	}

	return 0;
}
void MyControlForm::AdjustToken()
{
	// 该函数在OnInitDialog()函数中调用
	static HANDLE hToken;
	static TOKEN_PRIVILEGES tp;
	static LUID luid;
	::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
}

