// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ImdViewer.h"

#include "MainFrm.h"
#include "ImdViewerDoc.h"
#include "ImdViewerView.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
ON_WM_CREATE()
ON_COMMAND(ID_ANIMATION_PLAY, OnAnimationPlay)
ON_COMMAND(ID_ANIMATION_STOP, OnAnimationStop)
ON_WM_TIMER()
ON_COMMAND(ID_VIEW_PLUGINSBAR, OnViewPluginsbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_PLUGINSBAR, OnUpdateViewPluginsbar)
ON_COMMAND(ID_VIEW_BONELISTBAR, OnViewBonelistbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_BONELISTBAR, OnUpdateViewBonelistbar)
ON_COMMAND(ID_BONES_ADD, OnBonesAdd)
ON_WM_HSCROLL()
END_MESSAGE_MAP()

static UINT indicators[] =
    {
        ID_SEPARATOR, // status line indicator
        ID_INDICATOR_CAPS,
        ID_INDICATOR_NUM,
        ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
  // TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}
void CMainFrame::Render()
{
  CImdViewerView *pView;
  pView = (CImdViewerView *)this->GetActiveView();
  if (pView)
    pView->Render();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
      !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
  {
    TRACE0("Failed to create toolbar\n");
    return -1; // fail to create
  }

  if (!m_wndStatusBar.Create(this) ||
      !m_wndStatusBar.SetIndicators(indicators,
                                    sizeof(indicators) / sizeof(UINT)))
  {
    TRACE0("Failed to create status bar\n");
    return -1; // fail to create
  }

  if (!m_wndAnimToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
  {
    TRACE0("Failed to create animbar\n");
    return -1; // fail to create
  }

  if (!m_wndPluginsBar.Create(this, CDialogBarPlugins::IDD,
                              CBRS_TOP | CBRS_GRIPPER | CBRS_FLYBY, CDialogBarPlugins::IDD))
  {
    TRACE0("Failed to create dialogbar plugin\n");
    return -1; // fail to create
  }
  if (!m_wndBoneListBar.Create(this, CDialogBarBoneList::IDD,
                               CBRS_TOP | CBRS_GRIPPER | CBRS_FLYBY, CDialogBarBoneList::IDD))
  {
    TRACE0("Failed to create dialogbar bonelist\n");
    return -1; // fail to create
  }

  m_wndToolBar.SetWindowText("File toolbar");
  m_wndAnimToolBar.SetWindowText("Animation toolbar");
  m_wndPluginsBar.SetWindowText("Plugins toolbar");
  m_wndBoneListBar.SetWindowText("Bone list toolbar");
  // TODO: Delete these three lines if you don't want the toolbar to be dockable
  m_wndBoneListBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndAnimToolBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndPluginsBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);
  DockControlBar(&m_wndPluginsBar, AFX_IDW_DOCKBAR_RIGHT);
  DockControlBarLeftOf(&m_wndAnimToolBar, &m_wndToolBar);
  DockControlBarBottomOf(&m_wndBoneListBar, &m_wndPluginsBar);
  m_wndPluginsBar.DisplayPluginsList();
  return 0;
}

void CMainFrame::DockControlBarLeftOf(CControlBar *pBar, CControlBar *pLeftOf)
{
  CRect rect;
  DWORD dw;
  UINT n;

  RecalcLayout(TRUE);
  pLeftOf->GetWindowRect(&rect);
  rect.OffsetRect(1, 0);
  dw = pLeftOf->GetBarStyle();
  n = 0;
  n = (dw & CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
  n = (dw & CBRS_ALIGN_BOTTOM && n == 0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
  n = (dw & CBRS_ALIGN_LEFT && n == 0) ? AFX_IDW_DOCKBAR_LEFT : n;
  n = (dw & CBRS_ALIGN_RIGHT && n == 0) ? AFX_IDW_DOCKBAR_RIGHT : n;
  DockControlBar(pBar, n, &rect);
}

void CMainFrame::DockControlBarBottomOf(CControlBar *pBar, CControlBar *pBottomOf)
{
  CRect rect;
  DWORD dw;
  UINT n;

  RecalcLayout(TRUE);
  pBottomOf->GetWindowRect(&rect);
  rect.OffsetRect(0, 1);
  dw = pBottomOf->GetBarStyle();
  n = 0;
  n = (dw & CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
  n = (dw & CBRS_ALIGN_BOTTOM && n == 0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
  n = (dw & CBRS_ALIGN_LEFT && n == 0) ? AFX_IDW_DOCKBAR_LEFT : n;
  n = (dw & CBRS_ALIGN_RIGHT && n == 0) ? AFX_IDW_DOCKBAR_RIGHT : n;
  DockControlBar(pBar, n, &rect);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT &cs)
{
  if (!CFrameWnd::PreCreateWindow(cs))
    return FALSE;
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
  CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext &dc) const
{
  CFrameWnd::Dump(dc);
}

#endif //_DEBUG

// CMainFrame message handlers

void CMainFrame::OnAnimationPlay()
{
  _timer_id = (int)SetTimer(ID_TIMER_ANIMATION, 1000 / 60, 0);
}

void CMainFrame::OnAnimationStop()
{
  KillTimer(_timer_id);
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
  if (nIDEvent == _timer_id)
  {
    CImdViewerDoc *pDoc = (CImdViewerDoc *)GetActiveDocument();
    if (pDoc == 0)
      return;
    pDoc->Animate();
    UpdateAnimationToolBar();
  }
  CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::SetAnimation(int begin, int end)
{
  m_wndAnimToolBar.m_slider.SetRange(begin, end);
  OnTimer(_timer_id);
}

void CMainFrame::OnViewPluginsbar()
{
  BOOL bShow = m_wndPluginsBar.IsVisible();
  ShowControlBar(&m_wndPluginsBar, !bShow, FALSE);
}

void CMainFrame::OnUpdateViewPluginsbar(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetCheck(m_wndPluginsBar.IsVisible());
}

void CMainFrame::OnViewBonelistbar()
{
  BOOL bShow = m_wndBoneListBar.IsVisible();
  ShowControlBar(&m_wndBoneListBar, !bShow, FALSE);
}

void CMainFrame::OnUpdateViewBonelistbar(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetCheck(m_wndBoneListBar.IsVisible());
}

void CMainFrame::OnBonesAdd()
{
  CFileDialog oFileDialog(TRUE, "imdbone", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Imd bones file (*.imdbone).|*.imdbone||");
  if (oFileDialog.DoModal() == IDOK)
    m_wndBoneListBar.Insert(oFileDialog.GetPathName().GetBuffer());
}

void CMainFrame::UpdateAnimationToolBar()
{
  CImdViewerDoc *pDoc = (CImdViewerDoc *)GetActiveDocument();
  if (pDoc == 0)
    return;
  imd2_object_t *o = pDoc->GetImdObject();
  imd2_bone_file_t *b = pDoc->GetImdBone();
  size_t current_anim = pDoc->GetCurrentAnim();
  CString oStrFrame;
  int total = b == 0 ? o == 0 ? 0 : o->imd2_object_header.num_anim : b->imd2_bone_file_header.anim_count;
  oStrFrame.Format("%d/%d", current_anim, total - 1);
  m_wndAnimToolBar.m_information.SetWindowText(oStrFrame);
  m_wndAnimToolBar.m_slider.SetPos((int)current_anim);
}

void CMainFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
  CFrameWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}
