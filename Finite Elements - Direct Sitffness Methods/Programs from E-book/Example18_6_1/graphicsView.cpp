// graphicsView.cpp : implementation of the CGraphicsView class
//

#include "stdafx.h"
#include "graphics.h"
#include <fstream>
#include "memdc.h"

#include "graphicsDoc.h"
#include "graphicsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PAGEUP				33
#define PAGEDOWN			34
#define HOME				36
#define	LEFTARROW			37
#define UPARROW				38
#define RIGHTARROW			39		
#define DOWNARROW			40

static int nCalls=0;
/////////////////////////////////////////////////////////////////////////////
// CGraphicsView

IMPLEMENT_DYNCREATE(CGraphicsView, CView)

BEGIN_MESSAGE_MAP(CGraphicsView, CView)
	//{{AFX_MSG_MAP(CGraphicsView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
    ON_WM_RBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphicsView construction/destruction

CGraphicsView::CGraphicsView()
{
	// TODO: add construction code here
	m_nPoints = 0;
	m_nLines = 0;
    m_fMT.SetSize (2,2);
    m_fMT.Set (0.0f);
    m_fMT(1,1) = 1.0f;
    m_fMT(2,2) = 1.0f;
    m_fTheta = 0.0f;
}

CGraphicsView::~CGraphicsView()
{
}

BOOL CGraphicsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

CGraphicsView* CGraphicsView::GetView()
{
    CFrameWnd* pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
    if (!pFrame)
        return NULL;
    CView* pView = pFrame->GetActiveView();

    if (!pView)
        return NULL;

    // Fail if view is of wrong kind
    // (this could occur with splitter windows, or additional
    // views on a single document
    if (!pView->IsKindOf(RUNTIME_CLASS(CGraphicsView)))
        return NULL;

    return (CGraphicsView*) pView;
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicsView drawing

BOOL CGraphicsView::OnEraseBkgnd(CDC* pDC) 
{
      return FALSE;
}

void CGraphicsView::OnDraw(CDC* pDCX)
{
	CGraphicsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    int i;

	// TODO: add draw code for native data here
    CMemDC pDC (pDCX); 	// use mem DC
    CString szMessage;
	nCalls++;
	szMessage.Format ("In OnDraw. Count is %d\n", nCalls);
	TRACE0 (szMessage);	

    // get "viewport" dimensions
	CRect myVP;
	GetClientRect(myVP);

	// Set the color of the viewport
	pDC->FillSolidRect(myVP, RGB(0,0,0));

	// Set pen color 
	long penrgb=0;
	penrgb = (COLORREF) RGB(255,255,255);

	// Select pen
	CPen colorpen, *pOldPen;
	colorpen.CreatePen(PS_SOLID, 1, penrgb);
	pOldPen = pDC->SelectObject(&colorpen);

	// Set the text color
	pDC->SetTextColor (penrgb);

    // get "viewport" dimensions and draw a rectangle
    CPoint TopLeft = myVP.TopLeft();
    CPoint BottomRight = myVP.BottomRight();
    int nHeight = myVP.Height();
    int nWidth = myVP.Width();
    int MinRange = (nHeight < nWidth? nHeight : nWidth);
    pDC->MoveTo (int(TopLeft.x+0.05f*MinRange), int(TopLeft.y+0.05f*MinRange));
    pDC->LineTo (int(TopLeft.x+0.05f*MinRange), int(BottomRight.y-0.05f*MinRange));
    pDC->LineTo (int(BottomRight.x-0.05f*MinRange), int(BottomRight.y-0.05f*MinRange));
    pDC->LineTo (int(BottomRight.x-0.05f*MinRange), int(TopLeft.y+0.05f*MinRange));
    pDC->LineTo (int(TopLeft.x+0.05f*MinRange), int(TopLeft.y+0.05f*MinRange));

	// draw the lines
    int nX, nY;
    for (i=1; i <= m_nLines; i++)
    {
        // move to start point
        ScreenCoordinate (m_nVSP(i), nX, nY);
        pDC->MoveTo(nX, nY);
        // draw to the end point
        ScreenCoordinate (m_nVEP(i), nX, nY);
	    pDC->LineTo(nX, nY);
    }

    // show the point labels
    CString szT;
	for (i=1; i <= m_nPoints; i++)
    {
	    szT.Format ("%d", i);
        ScreenCoordinate (i, nX, nY);
	    pDC->TextOut (nX,nY, szT, szT.GetLength());
    }

    pDC->TextOut (100,100, szMessage, szMessage.GetLength());
	colorpen.DeleteObject();
	pDC->SelectObject(pOldPen); 
}

void CGraphicsView::ScreenCoordinate (int nN, int& nX, int& nY)
{
    float fX = m_fVX(nN);
    float fY = m_fVY(nN);
    nX = int(m_fMT(1,1)*fX + m_fMT(1,2)*fY);
    nY = int(m_fMT(2,1)*fX + m_fMT(2,2)*fY);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicsView printing

BOOL CGraphicsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGraphicsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGraphicsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicsView diagnostics

#ifdef _DEBUG
void CGraphicsView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphicsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphicsDoc* CGraphicsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicsDoc)));
	return (CGraphicsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphicsView message handlers

void CGraphicsView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	static char szFilter[]="Data Files (*.dat)|*.dat||";
	CString selFile;
    int i;

	// open dialog box
	CFileDialog FileDialog (
		TRUE,   // TRUE for FileOpen
		"dat",
		"*.dat",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		szFilter,
		NULL);

	// now open 
	if (FileDialog.DoModal() == IDOK) 
	{
		// get the new title name information
		selFile=FileDialog.GetPathName();
        std::ifstream FileInput;

		// open the file
		FileInput.open (selFile, std::ios::in); 
		                                                   
		// read the data
		FileInput >> m_nPoints >> m_nLines;

        // set the sizes
        m_fVX.SetSize (m_nPoints);
        m_fVY.SetSize (m_nPoints);
        m_nVSP.SetSize (m_nLines);
        m_nVEP.SetSize (m_nLines);

        // read the point data
		for (i=1; i <= m_nPoints; i++)
			FileInput >> m_fVX(i) >> m_fVY(i);

		// read the line data
        for (i=1; i <= m_nLines; i++)
			FileInput >> m_nVSP(i) >> m_nVEP(i);

		// close the file
		FileInput.close ();

        AfxMessageBox ("Finished reading the file", MB_OK, 0);

        // draw the image
        CGraphicsDoc* pDoc = GetDocument();
        pDoc->UpdateAllViews(NULL);
	}
}

void CGraphicsView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGraphicsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if ((nChar <= DOWNARROW && nChar >= HOME) || (nChar == PAGEUP || nChar == PAGEDOWN))	
	{
		if (nChar == HOME)
        {
        }
		else
        {
			if (nChar == RIGHTARROW)		// Right arrow pressed 
            {
                m_fTheta += 5.0f;
                float fRadians = static_cast<float>(m_fTheta*0.0174533);
                m_fMT(1,1) = cos(fRadians);
                m_fMT(1,2) = sin(fRadians);
                m_fMT(2,1) = -sin(fRadians);
                m_fMT(2,2) = cos(fRadians);
            }
			else if (nChar == LEFTARROW)	// Left arrow pressed 
            {
                m_fTheta -= 5.0f;
                float fRadians = static_cast<float>(m_fTheta*0.0174533);
                m_fMT(1,1) = cos(fRadians);
                m_fMT(1,2) = sin(fRadians);
                m_fMT(2,1) = -sin(fRadians);
                m_fMT(2,2) = cos(fRadians);
            }
			else if (nChar == UPARROW)		// Up arrow pressed 
            {
            }
			else if (nChar == DOWNARROW)	// Down arrow pressed 
            {
            }
			else if (nChar == PAGEUP)		// Pageup pressed 
            {
            }
			else if (nChar == PAGEDOWN)		// Pagedown pressed 
            {
            }
		}
	}

    CGraphicsDoc* pDoc = GetDocument();
    pDoc->UpdateAllViews(NULL);
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGraphicsView::OnMouseMove(UINT nFlags, CPoint point) 
{
    // put your code here

	// this calls the base class
    CView::OnMouseMove(nFlags, point);
}

void CGraphicsView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    // put your code here

	// this calls the base class
    CView::OnLButtonDblClk(nFlags, point);
}

void CGraphicsView::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // put your code here

	// this calls the base class
    CView::OnLButtonDown(nFlags, point);
}

void CGraphicsView::OnLButtonUp(UINT nFlags, CPoint point) 
{

	// this calls the base class
	CView::OnLButtonUp(nFlags, point);
}

void CGraphicsView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CView::OnRButtonDblClk(nFlags, point);
}

void CGraphicsView::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CView::OnRButtonDown(nFlags, point);
}

void CGraphicsView::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CView::OnRButtonUp(nFlags, point);
}

BOOL CGraphicsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default

    return CView::OnMouseWheel(nFlags, zDelta, pt);
}
