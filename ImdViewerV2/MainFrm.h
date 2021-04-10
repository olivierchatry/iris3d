// MainFrm.h : interface of the CMainFrame class
//

#pragma once

class CMainFrame : public CFrameWnd
{

protected: // create from serialization only
  CMainFrame();
  DECLARE_DYNCREATE(CMainFrame)

  // Attributes
public:
  // Operations
public:
  // Overrides
public:
  virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
  void Render();
  // Implementation
public:
  void UpdateAnimationToolBar();
  void SetAnimation(int begin, int end);
  virtual ~CMainFrame();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext &dc) const;
#endif

  CDialogBarBoneList &GetBonesListBar()
  {
    return m_wndBoneListBar;
  }

protected: // control bar embedded members
  CStatusBar m_wndStatusBar;
  CToolBar m_wndToolBar;
  CAnimToolBar m_wndAnimToolBar;
  CDialogBarPlugins m_wndPluginsBar;
  CDialogBarBoneList m_wndBoneListBar;
  int _timer_id;
  // Generated message map functions
  void DockControlBarLeftOf(CControlBar *pBar, CControlBar *pLeftOf);
  void DockControlBarBottomOf(CControlBar *pBar, CControlBar *pBottomOf);

protected:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnAnimationPlay();
  afx_msg void OnAnimationStop();
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg void OnViewPluginsbar();
  afx_msg void OnUpdateViewPluginsbar(CCmdUI *pCmdUI);
  afx_msg void OnViewBonelistbar();
  afx_msg void OnUpdateViewBonelistbar(CCmdUI *pCmdUI);
  afx_msg void OnBonesAdd();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};
