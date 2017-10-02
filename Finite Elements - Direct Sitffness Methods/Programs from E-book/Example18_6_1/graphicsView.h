// graphicsView.h : interface of the CGraphicsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHICSVIEW_H__EEFCD7AD_EC67_11D2_AA6B_00A0249A442A__INCLUDED_)
#define AFX_GRAPHICSVIEW_H__EEFCD7AD_EC67_11D2_AA6B_00A0249A442A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "..\library\vectortemplate.h"
#include "..\library\matrixtemplate.h"
#include "graphicsDoc.h"

class CGraphicsView : public CView
{
protected: // create from serialization only
	CGraphicsView();
	DECLARE_DYNCREATE(CGraphicsView)
    static CGraphicsView* GetView ();

// Attributes
public:
	CGraphicsDoc* GetDocument();
    int m_nPoints;
    int m_nLines;
    float m_fTheta;
    CVector<float> m_fVX;
    CVector<float> m_fVY;
    CVector<int>   m_nVSP;
    CVector<int>   m_nVEP;
    CMatrix<float> m_fMT;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicsView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    //}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphicsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    void ScreenCoordinate (int, int&, int&);

// Generated message map functions
protected:
	//{{AFX_MSG(CGraphicsView)
	afx_msg void OnFileOpen();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in graphicsView.cpp
inline CGraphicsDoc* CGraphicsView::GetDocument()
   { return (CGraphicsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHICSVIEW_H__EEFCD7AD_EC67_11D2_AA6B_00A0249A442A__INCLUDED_)
