// DialogBarBoneList.cpp : implementation file
//

#include "stdafx.h"
#include "ImdViewer.h"
#include "ImdViewerDoc.h"
#include "DialogBarBoneList.h"
#include "DialogBarBoneTree.h"
#include "DialogBarMeshInfo.h"
#include "DialogBarAnim.h"
#include "DialogBarConfig.h"

#include "MainFrm.h"

// CDialogBarBoneList dialog

IMPLEMENT_DYNAMIC(CDialogBarBoneList, CDialogBar)
CDialogBarBoneList::CDialogBarBoneList(CWnd *pParent /*=NULL*/)
    : CDialogBar()
{
}

CDialogBarBoneList::~CDialogBarBoneList()
{
}

void CDialogBarBoneList::DoDataExchange(CDataExchange *pDX)
{
  CDialogBar::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogBarBoneList, CDialogBar)
ON_LBN_SELCHANGE(IDC_LISTBONES, OnLbnSelchangeListbones)
ON_WM_CONTEXTMENU()
ON_BN_CLICKED(IDC_CHECKBONEANIMATION, OnBnClickedCheckboneanimation)
END_MESSAGE_MAP()

// CDialogBarBoneList message handlers

void CDialogBarBoneList::OnLbnSelchangeListbones()
{
  CListBox *pListBox = (CListBox *)GetDlgItem(IDC_LISTBONES);
  ASSERT(pListBox != 0);
  int iIndex = pListBox->GetCurSel();
  if (iIndex >= 0)
  {
    imd2_bone_file_t *bone = (imd2_bone_file_t *)pListBox->GetItemData(iIndex);
    CMainFrame *pMainFrame = (CMainFrame *)theApp.GetMainWnd();
    ASSERT(pMainFrame != 0);
    pMainFrame->RefreshBoneTree(bone);
  }
}

void CDialogBarBoneList::OnContextMenu(CWnd *pWnd, CPoint point)
{
  CMenu menu;
  CListBox *pListBox = (CListBox *)GetDlgItem(IDC_LISTBONES);
  ASSERT(pListBox != 0);
  menu.CreatePopupMenu();
  menu.AppendMenu(MF_STRING, ID_BONES_OPEN, "Open bones file ...");
  menu.AppendMenu(MF_SEPARATOR, 1, "");
  menu.AppendMenu(MF_STRING, 2, "Remove this bones file");
  menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
  menu.Detach();
}

void CDialogBarBoneList::RefreshBonesList(CImdViewerDoc *pDoc)
{
  CListBox *pListBox = (CListBox *)GetDlgItem(IDC_LISTBONES);
  for (int i = 0; i < pListBox->GetCount(); ++i)
    pListBox->DeleteString(0);
  for (size_t i = 0; i < pDoc->GetBoneFileCount(); ++i)
  {
    imd2_bone_file_t *bones = pDoc->GetBoneFile(i);
    int iItem = pListBox->InsertString(0, pDoc->GetBoneFileName(i).c_str());
    pListBox->SetItemData(iItem, (DWORD_PTR)bones);
  }
}

void CDialogBarBoneList::OnBnClickedCheckboneanimation()
{
  CMainFrame *pMainFrm = (CMainFrame *)theApp.GetMainWnd();
  CButton *pButton = (CButton *)GetDlgItem(IDC_CHECKBONEANIMATION);
  ASSERT(pMainFrm != 0);

  pMainFrm->SetBoneAnimation(pButton->GetCheck() == BST_CHECKED);
}
