// DialogBarPlugins.cpp : implementation file
//

#include "stdafx.h"
#include "ImdViewer.h"
#include "MainFrm.h"
#include "ImdViewerDoc.h"
#include "ImdViewerView.h"
#include "DialogBarPlugins.h"

// CDialogBarPlugins dialog

IMPLEMENT_DYNAMIC(CDialogBarPlugins, CDialogBar)
CDialogBarPlugins::CDialogBarPlugins(CWnd *pParent /*=NULL*/)
    : CDialogBar()
{
}

CDialogBarPlugins::~CDialogBarPlugins()
{
}

void CDialogBarPlugins::DoDataExchange(CDataExchange *pDX)
{
  CDialogBar::DoDataExchange(pDX);
}

int CDialogBarPlugins::InsertInformation(char *name, char *value, char *infotip, CListCtrl *pListCtrl, int &counter)
{
  int iItem = pListCtrl->InsertItem(LVIF_TEXT | LVIF_STATE, counter++, name, LVIS_SELECTED, LVIS_SELECTED, 0, 0);
  pListCtrl->SetItemText(iItem, 1, value);
  return iItem;
  //	LVSETINFOTIP	InfoTip;
  //	InfoTip.cbSize = sizeof(InfoTip);
  //	InfoTip.dwFlags = 0;
  //	InfoTip.iSubItem = 0;
  //	InfoTip.iItem = iItem;
  //	InfoTip.pszText = infotip;
  //	pListCtrl->SetInfoTip(&InfoTip);
}

void CDialogBarPlugins::DisplayPluginsList()
{
  CListCtrl *pListCtrl = (CListCtrl *)GetDlgItem(IDC_LISTPLUGINS);
  int counter;
  CRect oRect;
  pListCtrl->GetClientRect(oRect);
  pListCtrl->DeleteAllItems();
  pListCtrl->DeleteColumn(0);
  pListCtrl->DeleteColumn(1);
  pListCtrl->SetExtendedStyle(0);
  counter = 0;
  pListCtrl->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
  pListCtrl->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
  long style = GetWindowLong(pListCtrl->GetSafeHwnd(), GWL_STYLE);
  CString oStr;
  style = style | LVS_REPORT;
  SetWindowLong(pListCtrl->GetSafeHwnd(), GWL_STYLE, style);
  int offset = oRect.Width() / 3;
  pListCtrl->InsertColumn(0, "Name", LVCFMT_LEFT, offset);
  pListCtrl->InsertColumn(1, "Description", LVCFMT_LEFT, (oRect.Width() - offset));
  for (size_t i = 0; i < theApp._plugins.size(); ++i)
  {
    Plugins *plugin = theApp._plugins[i];
    int item = InsertInformation(plugin->_info->name, plugin->_info->description, plugin->_info->author, pListCtrl, counter);
    pListCtrl->SetItemData(item, (DWORD_PTR)plugin);
  }
}
BEGIN_MESSAGE_MAP(CDialogBarPlugins, CDialogBar)
ON_NOTIFY(NM_DBLCLK, IDC_LISTPLUGINS, OnNMDblclkListplugins)
END_MESSAGE_MAP()

// CDialogBarPlugins message handlers

void CDialogBarPlugins::OnNMDblclkListplugins(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO: Add your control notification handler code here
  CListCtrl *pListCtrl = (CListCtrl *)GetDlgItem(IDC_LISTPLUGINS);
  UINT uSelectedCount = pListCtrl->GetSelectedCount();
  int nItem = -1;
  CMainFrame *pMainFrame = (CMainFrame *)this->GetParentFrame();
  ASSERT(pMainFrame != 0);
  CImdViewerDoc *pDoc = (CImdViewerDoc *)pMainFrame->GetActiveDocument();
  ASSERT(pDoc != 0);
  // Update all of the selected items.
  if (uSelectedCount > 0)
  {
    nItem = pListCtrl->GetNextItem(nItem, LVNI_SELECTED);
    ASSERT(nItem != -1);
    Plugins *plugin = (Plugins *)pListCtrl->GetItemData(nItem);
    plugin->_plugins->PluginGo(pDoc->GetImdPath().c_str(), pDoc->GetImdObject(), pDoc->GetImdBone());
  }
  *pResult = 0;
}
