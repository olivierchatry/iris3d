#pragma once


// CDialogBarMeshInfo dialog

class CDialogBarMeshInfo : public CDialogBar
{
	DECLARE_DYNAMIC(CDialogBarMeshInfo)

public:
	CDialogBarMeshInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogBarMeshInfo();

// Dialog Data
	enum { IDD = IDD_DIALOGMESHINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void	InsertInformation(char *name, char *value, char *infotip, CListCtrl	*pListCtrl);
	DECLARE_MESSAGE_MAP()
	int		_counter;
public:
	afx_msg void OnLvnItemchangedListinfo(NMHDR *pNMHDR, LRESULT *pResult);
	void	DisplayImd2Info(imd2_object_t *object);
};
