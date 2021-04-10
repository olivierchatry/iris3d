// DialogBoneWeight.cpp : implementation file
//

#include "stdafx.h"
#include "ImdViewer.h"
#include "DialogBoneWeight.h"

// CDialogBoneWeight dialog

IMPLEMENT_DYNAMIC(CDialogBoneWeight, CDialog)
CDialogBoneWeight::CDialogBoneWeight(imd2_object_t *pObj, imd2_bone_file_t *pBone, CWnd *pParent /*=NULL*/)
    : CDialog(CDialogBoneWeight::IDD, pParent)
{
  _object = pObj;
  _bones = pBone;
}

CDialogBoneWeight::~CDialogBoneWeight()
{
}

void CDialogBoneWeight::DoDataExchange(CDataExchange *pDX)
{
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogBoneWeight, CDialog)
END_MESSAGE_MAP()

// CDialogBoneWeight message handlers

BOOL CDialogBoneWeight::OnInitDialog()
{
  // DISPLAY All Weight for mesh 0
  std::map<int, bool> bone_used;
  CDialog::OnInitDialog();
  CListCtrl *pListCtrl = (CListCtrl *)GetDlgItem(IDC_LISTBONEWEIGHT);
  ASSERT(pListCtrl != 0);
  if (_object == 0)
    return FALSE;
  pListCtrl->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
  long style = GetWindowLong(pListCtrl->GetSafeHwnd(), GWL_STYLE);
  style = style | LVS_REPORT;
  SetWindowLong(pListCtrl->GetSafeHwnd(), GWL_STYLE, style);
  pListCtrl->InsertColumn(0, "Vertex #", LVCFMT_LEFT, 60);
  // count num bones.
  imd2_mesh_t *mesh = _object->imd2_mesh;
  if (_object->imd2_object_header.have_skin == false)
    return FALSE;
  int bone_count = 0;
  if (_bones == 0)
    for (int i = 0; i < mesh->imd2_mesh_header.num_vertex; ++i)
    {
      imd2_skin_t *skin = &(mesh->imd2_skin[i]);
      for (int w = 0; w < skin->num_bones_assigned; ++w)
      {
        int bone_index = skin->weight[w].bone_index;
        if (bone_used[bone_index] == false)
        {
          bone_count++;
          bone_used[bone_index] = true;
        }
      }
    }
  else
    bone_count = _bones->imd2_bone_file_header.bone_count;
  for (int i = 0; i < bone_count; ++i)
  {
    CString oStr;
    oStr.Format("Bone #%d", 1 + i);
    pListCtrl->InsertColumn(1 + i, oStr, LVCFMT_LEFT, 60);
  }
  int _counter = 0;
  for (int i = 0; i < mesh->imd2_mesh_header.num_vertex; ++i)
  {
    imd2_skin_t *skin = &(mesh->imd2_skin[i]);
    CString oStr;
    oStr.Format("#%d", i);
    int iItem = pListCtrl->InsertItem(LVIF_TEXT | LVIF_STATE, _counter++, oStr,
                                      LVIS_SELECTED, LVIS_SELECTED, 0, 0);
    for (int j = 0; j < 3; ++j)
      pListCtrl->SetItemText(iItem, j + 1, "-");
    for (int j = 0; j < skin->num_bones_assigned; ++j)
    {
      imd2_weight_t *w = &(skin->weight[j]);
      oStr.Format("%0.3f", w->weight);
      pListCtrl->SetItemText(iItem, w->bone_index + 1, oStr);
    }
  }
  return TRUE;
}
