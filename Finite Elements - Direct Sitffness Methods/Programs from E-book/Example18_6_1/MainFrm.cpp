// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "graphics.h"
#include "MainFrm.h"
#include "graphicsView.h"
#include "graphicsLegendView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	//a flag to check if the splitter has been created
	m_bInitSplitter = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	//calculate client size
	CRect cr;
	GetWindowRect( &cr );
	
	// Create the main splitter with 1 row and 2 columns
	if ( !m_mainSplitter.CreateStatic( this, 1, 2 ) )
	{
		MessageBox( "Error setting up m_mainSplitter", "ERROR", MB_OK | MB_ICONERROR );
		return FALSE;
	}

	// The views for each pane must be created 
	if ( !m_mainSplitter.CreateView( 0, 0, RUNTIME_CLASS(CGraphicsView),
		CSize(cr.Width()/2, cr.Height()), pContext ) )
	{
		MessageBox( "Error setting up splitter view", "ERROR", MB_OK | MB_ICONERROR );
		return FALSE;
	}

	if ( !m_mainSplitter.CreateView( 0, 1, RUNTIME_CLASS(CGraphicsLegendView),
		CSize(cr.Width()/2, cr.Height()), pContext ) )
	{
		MessageBox( "Error setting up splitter view", "ERROR", MB_OK | MB_ICONERROR );
		return FALSE;
	}

	//change flag to show splitter created
	m_bInitSplitter = true;

	//return TRUE instead of the parent method since that would
	//not show our window
	return TRUE;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	CRect cr;
	GetWindowRect(&cr);

	if (m_bInitSplitter && nType != SIZE_MINIMIZED )
	{
		m_mainSplitter.SetRowInfo( 0, cy, 0 );
		m_mainSplitter.SetColumnInfo( 0, cr.Width() / 2, 50);
		m_mainSplitter.SetColumnInfo( 1, cr.Width() / 2, 50);
		
		m_mainSplitter.RecalcLayout();
	}	
}
