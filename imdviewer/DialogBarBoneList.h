#pragma once

// CDialogBarBoneList dialog

class CDialogBarBoneList : public CDialogBar
{
  DECLARE_DYNAMIC(CDialogBarBoneList)

public:
  CDialogBarBoneList(CWnd *pParent = NULL); // standard constructor
  virtual ~CDialogBarBoneList();

  // Dialog Data
  enum
  {
    IDD = IDD_DIALOGBONELIST
  };

protected:
  virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  void RefreshBonesList(CImdViewerDoc *pDoc);

  afx_msg void OnLbnSelchangeListbones();
  afx_msg void OnContextMenu(CWnd * /*pWnd*/, CPoint /*point*/);
  afx_msg void OnBnClickedCheckboneanimation();
};
