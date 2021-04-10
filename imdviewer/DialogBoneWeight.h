#pragma once

// CDialogBoneWeight dialog

class CDialogBoneWeight : public CDialog
{
  DECLARE_DYNAMIC(CDialogBoneWeight)

public:
  CDialogBoneWeight(imd2_object_t *pObj, imd2_bone_file_t *pBone, CWnd *pParent = NULL); // standard constructor
  virtual ~CDialogBoneWeight();

  // Dialog Data
  enum
  {
    IDD = IDD_DIALOGBONEWEIGHT
  };

protected:
  virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
  imd2_object_t *_object;
  imd2_bone_file_t *_bones;
  DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
};
