#pragma once
#include "mscomm1.h"
#include "afxcmn.h"
#include "afxwin.h"



// MyControlForm ������ͼ

class MyControlForm : public CFormView
{
	DECLARE_DYNCREATE(MyControlForm)

protected:
	MyControlForm();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~MyControlForm();

public:
	enum { IDD = IDD_MYCONTROLFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void AdjustToken();  //��ȡ����Ȩ��
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CMscomm1 m_com1;
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	afx_msg void OnBnClickedButton3();
	afx_msg LRESULT Onsendcom(WPARAM,LPARAM); 
	// ���Ӵ���������
	CSliderCtrl slider_add;
//	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	CEdit edit_add;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	afx_msg LRESULT OnWmsetpartext(WPARAM wParam, LPARAM lParam);
public:
	CEdit edit_meanvalue;
	CEdit edit_stdvalue;
	CEdit edit_consen;
private:
	// com�ط�����
	BYTE* comback;
	// �ط������ֽ���
	long comback_num;
public:
	CStatic static_filename;
private:
	CStatic static_filename2;
};


