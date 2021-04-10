// DialogBarBoneList.cpp : implementation file
//

#include "stdafx.h"
#include "ImdViewer.h"
#include "MainFrm.h"
#include "ImdViewerDoc.h"
#include "DialogBarBoneList.h"

// CDialogBarBoneList dialog

IMPLEMENT_DYNAMIC(CDialogBarBoneList, CDialogBar)
CDialogBarBoneList::CDialogBarBoneList(CWnd *pParent /*=NULL*/)
    : CDialogBar()
{
  _bone_animation = false;
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
  if (_bone_animation == false)
    return;
  if (iIndex >= 0)
  {
    imd2_bone_file_t *bone = (imd2_bone_file_t *)pListBox->GetItemData(iIndex);
    CMainFrame *pMainFrame = (CMainFrame *)GetParentFrame();
    ASSERT(pMainFrame != 0);
    CImdViewerDoc *pDoc = (CImdViewerDoc *)pMainFrame->GetActiveDocument();
    ASSERT(pDoc != 0);
    pDoc->SetImdBone(bone);
  }
}
void CDialogBarBoneList::Clear()
{
  CMainFrame *pMainFrame = (CMainFrame *)GetParentFrame();
  ASSERT(pMainFrame != 0);
  CImdViewerDoc *pDoc = (CImdViewerDoc *)pMainFrame->GetActiveDocument();
  ASSERT(pDoc != 0);
  pDoc->SetImdBone(0);
  CListBox *pListBox = (CListBox *)GetDlgItem(IDC_LISTBONES);
  ASSERT(pListBox != 0);
  for (int i = 0; i < pListBox->GetCount(); ++i)
  {
    imd2_bone_file_t *bone_file = (imd2_bone_file_t *)pListBox->GetItemDataPtr(0);
    if (bone_file)
      free_imd2_bone(bone_file);
    pListBox->DeleteString(0);
  }
}

void CDialogBarBoneList::Insert(const char *file_name)
{
  imd2_bone_file_t *bone_file;
  CListBox *pListBox = (CListBox *)GetDlgItem(IDC_LISTBONES);
  ASSERT(pListBox != 0);
  bone_file = load_imd2_bone(file_name);
  if (bone_file)
  {
    std::string str(file_name);
    size_t index = str.find_last_of("\\/");
    if (index > 0)
      str = str.substr(index + 1);
    index = str.find_last_of(".");
    str = str.substr(0, index);
    int iItem = pListBox->InsertString(0, str.c_str());
    pListBox->SetItemData(iItem, (DWORD_PTR)bone_file);
  }
}

void CDialogBarBoneList::OnContextMenu(CWnd *pWnd, CPoint point)
{
  CMenu menu;
  menu.CreatePopupMenu();
  menu.AppendMenu(MF_STRING, ID_BONES_ADD, "Insert bone file ...");
  menu.AppendMenu(MF_SEPARATOR, 1, "");
  menu.AppendMenu(MF_STRING, ID_BONES_REMOVE, "Remove selected bones file");
  menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
  menu.Detach();
}

void CDialogBarBoneList::OnBnClickedCheckboneanimation()
{
  CMainFrame *pMainFrm = (CMainFrame *)theApp.GetMainWnd();
  CButton *pButton = (CButton *)GetDlgItem(IDC_CHECKBONEANIMATION);
  _bone_animation = pButton->GetCheck() == BST_CHECKED;
  if (!_bone_animation)
  {
    ASSERT(pMainFrm != 0);
    CImdViewerDoc *pDoc = (CImdViewerDoc *)pMainFrm->GetActiveDocument();
    ASSERT(pDoc != 0);
    pDoc->SetImdBone(0);
  }
  else
  {
    OnLbnSelchangeListbones();
  }

  // pMainFrm->SetBoneAnimation(pButton->GetCheck() == BST_CHECKED);
}
