#pragma once


// CDialogBarBoneTree dialog

class CDialogBarBoneTree : public CDialogBar
{
	DECLARE_DYNAMIC(CDialogBarBoneTree)

public:
	CDialogBarBoneTree(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogBarBoneTree();
// Dialog Data
	enum { IDD = IDD_DIALOGBONETREE };
	void	RefreshTreeView(imd2_bone_file_t *bone);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void RecursiveRefreshTreeView(imd2_bone_file_t *bone_file, imd2_bone_t *bone, HTREEITEM pParentItem = TVI_ROOT);
	DECLARE_MESSAGE_MAP()
};
