#include "stdafx.h"
#include "LBAA.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR, 
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
CMainFrame::CMainFrame()
{
}
CMainFrame::~CMainFrame()
{
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;
	}
	HICON ico = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//显示左上角图标
	SetIcon(ico,true);                               //显示左上角图标
	return 0;
}
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_SYSMENU;
    cs.cx = GetSystemMetrics(SM_CXFULLSCREEN)*9/10;
    cs.cy = GetSystemMetrics(SM_CYFULLSCREEN)*9/10; 
    //移去极大化按钮和拉伸
    cs.style^=WS_MAXIMIZEBOX;     
    cs.style&=~WS_THICKFRAME; 
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif
void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
}
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CRect tem_ClientRect;
	this->GetClientRect(&tem_ClientRect);
	tem_ClientRect.bottom -= 30;
	CSize siz_Control;
	CSize siz_ViewTop;
	CSize siz_ViewBottom;
	siz_Control.cx = 300;
	siz_Control.cy = tem_ClientRect.Height();
	siz_ViewTop.cx = tem_ClientRect.Width() - siz_Control.cx;
	siz_ViewBottom.cx = siz_ViewTop.cx;
	siz_ViewTop.cy = tem_ClientRect.Height()/2;
	siz_ViewBottom.cy = tem_ClientRect.Height() - siz_ViewTop.cy;
	m_srl.CreateStatic(this,1,2);
	m_srl.CreateView(0,0,RUNTIME_CLASS(MyControlForm),siz_Control,pContext);
	m_sud.CreateStatic(&m_srl,2,1,WS_CHILD|WS_VISIBLE,m_srl.IdFromRowCol(0,1));
	m_sud.CreateView(0,0,RUNTIME_CLASS(MyRDView),siz_ViewTop,pContext);
	m_sud.CreateView(1,0,RUNTIME_CLASS(MyTTView),siz_ViewBottom,pContext);
	

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}