// DialogBarMeshInfo.cpp : implementation file
//

#include "stdafx.h"
#include "ImdViewer.h"
#include "DialogBarMeshInfo.h"
#include <commctrl.h>

// CDialogBarMeshInfo dialog

IMPLEMENT_DYNAMIC(CDialogBarMeshInfo, CDialogBar)
CDialogBarMeshInfo::CDialogBarMeshInfo(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
}

CDialogBarMeshInfo::~CDialogBarMeshInfo()
{
}

void CDialogBarMeshInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogBarMeshInfo, CDialogBar)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTINFO, OnLvnItemchangedListinfo)
END_MESSAGE_MAP()


// CDialogBarMeshInfo message handlers

void CDialogBarMeshInfo::OnLvnItemchangedListinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void	CDialogBarMeshInfo::InsertInformation(char *name, char *value, char *infotip, CListCtrl	*pListCtrl)
{
	int iItem = pListCtrl->InsertItem(LVIF_TEXT|LVIF_STATE, _counter++, name, LVIS_SELECTED, LVIS_SELECTED,	0, 0);
	pListCtrl->SetItemText(iItem, 1, value);
/*
	LVSETINFOTIP	InfoTip;
	InfoTip.cbSize = sizeof(InfoTip);
	InfoTip.dwFlags = 0;
	InfoTip.iSubItem = 0;
	InfoTip.iItem = iItem;
	InfoTip.pszText = infotip;
	pListCtrl->SetInfoTip(&InfoTip);
*/
}

void	CDialogBarMeshInfo::DisplayImd2Info(imd2_object_t *object)
{
	CListCtrl	*pListCtrl = (CListCtrl	*) GetDlgItem(IDC_LISTINFO);
	ASSERT(pListCtrl != 0);
	if (object != 0)
	{
		pListCtrl->DeleteAllItems();
		pListCtrl->DeleteColumn(0);
		pListCtrl->DeleteColumn(1);
		pListCtrl->SetExtendedStyle(0);
		_counter = 0;
		pListCtrl->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
		long style = GetWindowLong(pListCtrl->GetSafeHwnd(),GWL_STYLE);
		CString	oStr;
		style = style | LVS_REPORT ;
		SetWindowLong(pListCtrl->GetSafeHwnd(), GWL_STYLE, style);
		pListCtrl->InsertColumn(0, "Information", LVCFMT_LEFT, 70);
		pListCtrl->InsertColumn(1, "Value", LVCFMT_LEFT, 70 + 36);
		InsertInformation("Name", object->imd2_object_header.name, "Name of the imd object.", pListCtrl);
		oStr.Format("%d", object->imd2_object_header.num_mesh);
		InsertInformation("Number of mesh", oStr.GetBuffer(), "Number of mesh in the imd object.", pListCtrl);
		int	temp = 0;
		for (int i = 0; i < object->imd2_object_header.num_mesh; ++i)
			temp += object->imd2_mesh[i].imd2_mesh_header.num_vertex;
		oStr.Format("%d", temp);
		InsertInformation("Number of vertices", oStr.GetBuffer(), "Total number of vertices in the imd object.", pListCtrl);
		oStr.Format("%d", object->imd2_object_header.num_material);
		InsertInformation("Number of material", oStr.GetBuffer(), "Total number of material in the imd object.", pListCtrl);
		oStr.Format("%d", object->imd2_object_header.num_anim);
		InsertInformation("Number of animation", oStr.GetBuffer(), "Total number of frame in the imd object.", pListCtrl);
		oStr.Format("%d", object->imd2_object_header.num_tag);
		InsertInformation("Number of tag", oStr.GetBuffer(), "Total number of tag in the imd object.", pListCtrl);
	}
}
