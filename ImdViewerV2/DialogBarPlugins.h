#pragma once

// CDialogBarPlugins dialog

class CDialogBarPlugins : public CDialogBar
{
  DECLARE_DYNAMIC(CDialogBarPlugins)

public:
  CDialogBarPlugins(CWnd *pParent = NULL); // standard constructor
  virtual ~CDialogBarPlugins();

  // Dialog Data
  enum
  {
    IDD = IDD_DIALOGPLUGINS
  };
  void DisplayPluginsList();

protected:
  virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
  int InsertInformation(char *name, char *value, char *infotip, CListCtrl *pListCtrl, int &counter);
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnNMDblclkListplugins(NMHDR *pNMHDR, LRESULT *pResult);
};
