#include "stdafx.h"
#include "graphics.h"
#include "graphicsDoc.h"
#include "graphicsView.h"
#include "graphicsLegendView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicsLegendView

IMPLEMENT_DYNCREATE(CGraphicsLegendView, CView)

CGraphicsLegendView::CGraphicsLegendView()
// ----------------------------------------------------------------------------
// Function: 
// Input:    
// Output:   
// ----------------------------------------------------------------------------
{
}

CGraphicsLegendView::~CGraphicsLegendView()
{
}


BEGIN_MESSAGE_MAP(CGraphicsLegendView, CView)
	//{{AFX_MSG_MAP(CGraphicsLegendView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphicsLegendView drawing

void CGraphicsLegendView::OnDraw(CDC* pDC)
// ----------------------------------------------------------------------------
// Function: "Draws" the legend that is displayed on the right hand side
// Input:    
// Output:   
// ----------------------------------------------------------------------------
{
    CString szMessage;
    CGraphicsDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    CGraphicsView* pView = CGraphicsView::GetView();
    if (!pView)
        return;

    szMessage.Format ("Number of points: %d.", pView->m_nPoints);
    pDC->TextOut (10, 10, szMessage, szMessage.GetLength());
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicsLegendView diagnostics

#ifdef _DEBUG
void CGraphicsLegendView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphicsLegendView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphicsDoc* CGraphicsLegendView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicsDoc)));
	return (CGraphicsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphicsLegendView message handlers
void CGraphicsLegendView::OnSize(UINT nType, int cx, int cy) 
// ----------------------------------------------------------------------------
// Function: 
// Input:    
// Output:   
// ----------------------------------------------------------------------------
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}
