#if !defined(AFX_GraphicsLegendView_H__3758D47F_7FBC_4819_8BAC_C8DED4D24B6F__INCLUDED_)
#define AFX_GraphicsLegendView_H__3758D47F_7FBC_4819_8BAC_C8DED4D24B6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphicsLegendView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphicsLegendView view

class CGraphicsLegendView : public CView
{
    protected:
	    CGraphicsLegendView();           // protected constructor used 
                                         // by dynamic creation
	    DECLARE_DYNCREATE(CGraphicsLegendView)

    // Attributes
    public:

    // Operations
    public:
	    CGraphicsDoc* GetDocument();

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CGraphicsLegendView)
	    protected:
	    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	    //}}AFX_VIRTUAL

    // Implementation
    protected:
	    virtual ~CGraphicsLegendView();
    #ifdef _DEBUG
	    virtual void AssertValid() const;
	    virtual void Dump(CDumpContext& dc) const;
    #endif

	    // Generated message map functions
    protected:
	    //{{AFX_MSG(CGraphicsLegendView)
		    // NOTE - the ClassWizard will add and remove member functions here.
	    afx_msg void OnSize(UINT nType, int cx, int cy);
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
};

#ifdef NDEBUG 
inline CGraphicsDoc* CGraphicsLegendView::GetDocument()
   { return (CGraphicsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GraphicsLegendView_H__3758D47F_7FBC_4819_8BAC_C8DED4D24B6F__INCLUDED_)
