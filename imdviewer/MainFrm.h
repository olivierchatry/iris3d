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

  // Implementation
public:
  virtual ~CMainFrame();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext &dc) const;
#endif
  void UpdateD3D();
  void SetImd2Object(imd2_object_t *imd2_object, std::string &path);
  void RefreshBoneTree(imd2_bone_file_t *bone);
  void SetBoneAnimation(bool bone_animation);
  void SetBoneFile(const char *file_path);

  CDialogBarAnim &GetAnimBar() { return m_wndAnimBar; }
  CDialogBarConfig &GetConfigBar() { return m_wndConfigBar; }

protected: // control bar embedded members
  CStatusBar m_wndStatusBar;
  CToolBar m_wndToolBar;
  CDialogBarConfig m_wndConfigBar;
  CDialogBarBoneList m_wndBonesToolBar;
  CDialogBarBoneTree m_wndBoneTreeToolBar;
  CDialogBarMeshInfo m_wndMeshInfoToolBar;
  CDialogBarAnim m_wndAnimBar;
  void DockControlBarBottomOf(CControlBar *pBar, CControlBar *pDownOf);
  imd2_bone_file_t *_bone_file;
  imd2_object_t *_object_file;
  // Generated message map functions
protected:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBonesOpen();
  afx_msg void OnUpdateBonesOpen(CCmdUI *pCmdUI);
  afx_msg void OnAnimPlay();
  afx_msg void OnAnimStop();
  afx_msg void OnViewWeight();
  afx_msg void OnFileRefresh();
  afx_msg void OnViewConfig();
  afx_msg void OnUpdateViewConfig(CCmdUI *pCmdUI);
  afx_msg void OnViewAnimbar();
  afx_msg void OnUpdateViewAnimbar(CCmdUI *pCmdUI);
  afx_msg void OnViewBonelistbar();
  afx_msg void OnUpdateViewBonelistbar(CCmdUI *pCmdUI);
  afx_msg void OnViewBonetreebar();
  afx_msg void OnUpdateViewBonetreebar(CCmdUI *pCmdUI);
  afx_msg void OnViewMeshinfo();
  afx_msg void OnUpdateViewMeshinfo(CCmdUI *pCmdUI);
};
