// DialogBarBoneTree.cpp : implementation file
//

#include "stdafx.h"
#include "ImdViewer.h"
#include "DialogBarBoneTree.h"


// CDialogBarBoneTree dialog

IMPLEMENT_DYNAMIC(CDialogBarBoneTree, CDialogBar)
CDialogBarBoneTree::CDialogBarBoneTree(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
}

CDialogBarBoneTree::~CDialogBarBoneTree()
{
}

void CDialogBarBoneTree::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

}

void CDialogBarBoneTree::RecursiveRefreshTreeView(imd2_bone_file_t *bone_file, imd2_bone_t *bone, HTREEITEM pParentItem)
{
	CTreeCtrl *pTreeCtrl = (CTreeCtrl *) GetDlgItem(IDC_TREEBONE);
	ASSERT(pTreeCtrl != 0);
	HTREEITEM pItem = pTreeCtrl->InsertItem(bone->imd2_bone_header.name, 0, 0, pParentItem);
	for (int iIndex = 0; iIndex < bone_file->imd2_bone_file_header.bone_count; ++iIndex)
		if (bone_file->bones[iIndex].imd2_bone_header.bone_parent == bone->imd2_bone_header.bone_index && bone->imd2_bone_header.bone_index != -1)
			RecursiveRefreshTreeView(bone_file, &(bone_file->bones[iIndex]), pItem);
}

void CDialogBarBoneTree::RefreshTreeView(imd2_bone_file_t *bone)
{
	CTreeCtrl *pTreeCtrl = (CTreeCtrl *) GetDlgItem(IDC_TREEBONE);
	ASSERT(pTreeCtrl != 0);
	pTreeCtrl->DeleteAllItems();	
	DWORD dwLong = GetWindowLong(pTreeCtrl->GetSafeHwnd(), GWL_EXSTYLE);
	dwLong |= TVS_LINESATROOT | TVS_HASBUTTONS | TVS_HASLINES;
	SetWindowLong(pTreeCtrl->GetSafeHwnd(), GWL_EXSTYLE, dwLong);
	for (int iIndex = 0; iIndex < bone->imd2_bone_file_header.bone_count; ++iIndex)
		if (bone->bones[iIndex].imd2_bone_header.bone_parent == -1)
			RecursiveRefreshTreeView(bone, &(bone->bones[iIndex]));
}

BEGIN_MESSAGE_MAP(CDialogBarBoneTree, CDialogBar)

END_MESSAGE_MAP()


// CDialogBarBoneTree message handlers
