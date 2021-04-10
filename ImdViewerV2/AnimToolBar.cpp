// AnimToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "ImdViewer.h"
#include "AnimToolBar.h"
#include "MainFrm.h"
#include "ImdViewerDoc.h"
#include "ImdViewerView.h"

// CAnimToolBar

IMPLEMENT_DYNAMIC(CAnimToolBar, CToolBar)
CAnimToolBar::CAnimToolBar()
{
  EnableActiveAccessibility();
}

CAnimToolBar::~CAnimToolBar()
{
}

BEGIN_MESSAGE_MAP(CAnimToolBar, CToolBar)
ON_WM_CREATE()
ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CAnimToolBar message handlers

int CAnimToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CToolBar::OnCreate(lpCreateStruct) == -1)
    return -1;
  LoadToolBar(IDR_ANIMBAR);
  SetButtonInfo(3, ID_SLIDDER, TBBS_SEPARATOR, 200);
  SetButtonInfo(4, ID_INFORMATION, TBBS_SEPARATOR, 60);
  CRect rect;
  GetItemRect(3, &rect);
  if (!m_slider.Create(WS_VISIBLE | WS_TABSTOP, rect, this, ID_SLIDDER))
  {
    TRACE0("Failed to create anim-bar\n");
    return FALSE;
  }
  GetItemRect(4, &rect);
  if (!m_information.Create("0/0", WS_VISIBLE | WS_TABSTOP, rect, this, ID_INFORMATION))
  {
    TRACE0("Failed to create anim-bar\n");
    return FALSE;
  }
  LOGFONT logFont;
  memset(&logFont, 0, sizeof(logFont));

  if (!::GetSystemMetrics(SM_DBCSENABLED))
  {
    // Since design guide says toolbars are fixed height so is the font.
    logFont.lfHeight = -12;
    logFont.lfWeight = FW_LIGHT;
    logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
    CString strDefaultFont;
    strDefaultFont.LoadString(IDS_DEFAULT_FONT);
    lstrcpy(logFont.lfFaceName, strDefaultFont);
    if (!m_font.CreateFontIndirect(&logFont))
      TRACE0("Could Not create font for anim-bar\n");
    else
      m_information.SetFont(&m_font);
  }
  else
  {
    m_font.Attach(::GetStockObject(SYSTEM_FONT));
    m_slider.SetFont(&m_font);
    m_information.SetFont(&m_font);
  }
  m_slider.SetRange(0, 0, TRUE);

  return 0;
}

void CAnimToolBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
  // TODO: Add your message handler code here and/or call default
  CSliderCtrl *pSlidder = (CSliderCtrl *)pScrollBar;
  ASSERT(pSlidder != 0);
  CMainFrame *pMainFrame = (CMainFrame *)theApp.GetMainWnd();
  ASSERT(pMainFrame != 0);
  CImdViewerDoc *pDoc = (CImdViewerDoc *)pMainFrame->GetActiveDocument();
  ASSERT(pDoc != 0);
  pDoc->SetCurrentAnim(pSlidder->GetPos());
  pMainFrame->UpdateAnimationToolBar();
  CToolBar::OnHScroll(nSBCode, nPos, pScrollBar);
}
