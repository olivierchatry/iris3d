// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ImdViewer.h"
#include "ImdViewerDoc.h"
#include "ImdViewerView.h"
#include "DialogBarBoneList.h"
#include "DialogBarMeshInfo.h"
#include "DialogBarBoneTree.h"
#include "DialogBarAnim.h"
#include "DialogBarConfig.h"
#include "DialogBoneWeight.h"
#include "MainFrm.h"
#include ".\mainfrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
ON_WM_CREATE()
ON_COMMAND(ID_BONES_OPEN, OnBonesOpen)
ON_UPDATE_COMMAND_UI(ID_BONES_OPEN, OnUpdateBonesOpen)
ON_COMMAND(ID_ANIM_PLAY, OnAnimPlay)
ON_COMMAND(ID_ANIM_STOP, OnAnimStop)
ON_COMMAND(ID_VIEW_WEIGHT, OnViewWeight)
ON_COMMAND(ID_FILE_REFRESH, OnFileRefresh)
ON_COMMAND(ID_VIEW_CONFIG, OnViewConfig)
ON_COMMAND(ID_VIEW_ANIMBAR, OnViewAnimbar)
ON_COMMAND(ID_VIEW_BONELISTBAR, OnViewBonelistbar)
ON_COMMAND(ID_VIEW_BONETREEBAR, OnViewBonetreebar)
ON_COMMAND(ID_VIEW_MESHINFO, OnViewMeshinfo)
ON_UPDATE_COMMAND_UI(ID_VIEW_CONFIG, OnUpdateViewConfig)
ON_UPDATE_COMMAND_UI(ID_VIEW_ANIMBAR, OnUpdateViewAnimbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_BONELISTBAR, OnUpdateViewBonelistbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_BONETREEBAR, OnUpdateViewBonetreebar)
ON_UPDATE_COMMAND_UI(ID_VIEW_MESHINFO, OnUpdateViewMeshinfo)
END_MESSAGE_MAP()

static UINT indicators[] =
    {
        ID_SEPARATOR, // status line indicator
        ID_INDICATOR_CAPS,
        ID_INDICATOR_NUM,
        ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() : _bone_file(0), _object_file(0)
{
  // TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
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
  if (!m_wndConfigBar.Create(this, CDialogBarConfig::IDD, CBRS_TOP | CBRS_GRIPPER | CBRS_FLYBY, CDialogBarConfig::IDD))
  {
    TRACE0("Failed to create dialogbar\n");
    return -1; // fail to create
  }
  if (!m_wndAnimBar.Create(this, CDialogBarAnim::IDD, CBRS_TOP | CBRS_GRIPPER | CBRS_FLYBY, CDialogBarAnim::IDD))
  {
    TRACE0("Failed to create dialogbar\n");
    return -1; // fail to create
  }

  if (!m_wndBonesToolBar.Create(this, CDialogBarBoneList::IDD,
                                CBRS_TOP | CBRS_GRIPPER | CBRS_FLYBY, CDialogBarBoneList::IDD))
  {
    TRACE0("Failed to create dialogbar\n");
    return -1; // fail to create
  }
  if (!m_wndBoneTreeToolBar.Create(this, CDialogBarBoneTree::IDD,
                                   CBRS_TOP | CBRS_GRIPPER | CBRS_FLYBY, CDialogBarBoneTree::IDD))
  {
    TRACE0("Failed to create dialogbar\n");
    return -1; // fail to create
  }

  if (!m_wndMeshInfoToolBar.Create(this, CDialogBarMeshInfo::IDD,
                                   CBRS_TOP | CBRS_GRIPPER | CBRS_FLYBY, CDialogBarMeshInfo::IDD))
  {
    TRACE0("Failed to create dialogbar\n");
    return -1; // fail to create
  }

  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndToolBar.SetWindowText("Toolbar");
  m_wndAnimBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndAnimBar.SetWindowText("Animator");
  m_wndMeshInfoToolBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndMeshInfoToolBar.SetWindowText("Mesh information toolbar");
  m_wndConfigBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndConfigBar.SetWindowText("Config toolbar");
  m_wndConfigBar.SetDispalyTag();
  m_wndConfigBar.SetWalkSpeed(1);
  m_wndConfigBar.SetTurnSpeed(1);
  m_wndConfigBar.SetStraffeSpeed(1);
  m_wndBonesToolBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndBonesToolBar.SetWindowText("Bones toolbar");
  m_wndBoneTreeToolBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndBoneTreeToolBar.SetWindowText("Bones treeview");
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);
  DockControlBar(&m_wndConfigBar, AFX_IDW_DOCKBAR_LEFT);
  DockControlBarBottomOf(&m_wndAnimBar, &m_wndConfigBar);
  DockControlBarBottomOf(&m_wndBonesToolBar, &m_wndAnimBar);
  DockControlBarBottomOf(&m_wndBoneTreeToolBar, &m_wndBonesToolBar);
  DockControlBar(&m_wndMeshInfoToolBar, AFX_IDW_DOCKBAR_FLOAT);
  ilInit();
  iluInit();
  ilutInit();
  return 0;
}

void CMainFrame::RefreshBoneTree(imd2_bone_file_t *bone)
{
  m_wndBoneTreeToolBar.RefreshTreeView(bone);
  CImdViewerView *pView = (CImdViewerView *)this->GetActiveView();
  ASSERT(pView != 0);
  pView->SetBoneFile(bone);
  _bone_file = bone;
}

void CMainFrame::SetBoneAnimation(bool bone_animation)
{
  CImdViewerView *pView = (CImdViewerView *)this->GetActiveView();
  ASSERT(pView != 0);
  pView->SetBoneAnimation(bone_animation);
}

void CMainFrame::DockControlBarBottomOf(CControlBar *pBar, CControlBar *pDownOf)
{
  CRect rect;
  DWORD dw;
  UINT n;

  RecalcLayout(TRUE);
  pDownOf->GetWindowRect(&rect);
  rect.OffsetRect(0, 1);
  dw = pDownOf->GetBarStyle();
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

void CMainFrame::UpdateD3D()
{
  CImdViewerView *pView;
  pView = (CImdViewerView *)this->GetActiveView();
  pView->UpdateScene();
}
void CMainFrame::SetImd2Object(imd2_object_t *imd2_object, std::string &path)
{
  CImdViewerView *pView;
  pView = (CImdViewerView *)this->GetActiveView();
  ASSERT(pView != 0);
  pView->SetImd2Object(imd2_object, path);
  m_wndMeshInfoToolBar.DisplayImd2Info(imd2_object);
  _object_file = imd2_object;
}

// CMainFrame message handlers

void CMainFrame::OnBonesOpen()
{
  CFileDialog oFileDialog(TRUE, "imdbone", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Imd bones file (*.imdbone).|*.imdbone||");
  if (oFileDialog.DoModal() == IDOK)
    SetBoneFile(oFileDialog.GetPathName().GetBuffer());
}

void CMainFrame::SetBoneFile(const char *file_path)
{
  imd2_bone_file_t *file;

  file = load_imd2_bone(file_path);
  if (file == 0)
    AfxMessageBox("Cannot load bones file.", MB_OK | MB_ICONERROR);
  else
  {
    CImdViewerDoc *pDoc = (CImdViewerDoc *)GetActiveDocument();
    ASSERT(pDoc != 0);
    pDoc->InsertBoneFile(file, file_path);
    m_wndBonesToolBar.RefreshBonesList(pDoc);
  }
}

void CMainFrame::OnUpdateBonesOpen(CCmdUI *pCmdUI)
{
}

void CMainFrame::OnAnimPlay()
{
  m_wndAnimBar.Play();
}

void CMainFrame::OnAnimStop()
{
  // TODO: Add your command handler code here
  m_wndAnimBar.Stop();
}

void CMainFrame::OnViewWeight()
{
  CDialogBoneWeight oDialog(_object_file, _bone_file);
  oDialog.DoModal();
  // TODO: Add your command handler code here
}

void CMainFrame::OnFileRefresh()
{
  CImdViewerView *pView;
  pView = (CImdViewerView *)this->GetActiveView();
  ASSERT(pView != 0);
  pView->ResetDevice();
  m_wndMeshInfoToolBar.DisplayImd2Info(_object_file);
}

void CMainFrame::OnViewConfig()
{
  BOOL bShow = m_wndConfigBar.IsVisible();
  ShowControlBar(&m_wndConfigBar, !bShow, FALSE);
}

void CMainFrame::OnUpdateViewConfig(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetCheck(m_wndConfigBar.IsVisible());
}

void CMainFrame::OnViewAnimbar()
{
  BOOL bShow = m_wndAnimBar.IsVisible();
  ShowControlBar(&m_wndAnimBar, !bShow, FALSE);
}

void CMainFrame::OnUpdateViewAnimbar(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetCheck(m_wndAnimBar.IsVisible());
}

void CMainFrame::OnViewBonelistbar()
{
  BOOL bShow = m_wndBonesToolBar.IsVisible();
  ShowControlBar(&m_wndBonesToolBar, !bShow, FALSE);
}

void CMainFrame::OnUpdateViewBonelistbar(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetCheck(m_wndBonesToolBar.IsVisible());
}

void CMainFrame::OnViewBonetreebar()
{
  BOOL bShow = m_wndBoneTreeToolBar.IsVisible();
  ShowControlBar(&m_wndBoneTreeToolBar, !bShow, FALSE);
}

void CMainFrame::OnUpdateViewBonetreebar(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetCheck(m_wndBoneTreeToolBar.IsVisible());
}

void CMainFrame::OnViewMeshinfo()
{
  BOOL bShow = m_wndMeshInfoToolBar.IsVisible();
  ShowControlBar(&m_wndMeshInfoToolBar, !bShow, FALSE);
}

void CMainFrame::OnUpdateViewMeshinfo(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
  pCmdUI->SetCheck(m_wndMeshInfoToolBar.IsVisible());
}
