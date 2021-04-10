// ImdViewerDoc.h : interface of the CImdViewerDoc class
//

#pragma once

class CImdViewerDoc : public CDocument
{
protected: // create from serialization only
  CImdViewerDoc();
  DECLARE_DYNCREATE(CImdViewerDoc)

  // Attributes
public:
  // Operations
public:
  // Overrides
public:
  virtual BOOL OnNewDocument();
  virtual void Serialize(CArchive &ar);

  // Implementation
public:
  virtual ~CImdViewerDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext &dc) const;
#endif

protected:
  // Generated message map functions
protected:
  DECLARE_MESSAGE_MAP()
  imd2_object_t *_object;
  std::vector<imd2_bone_file_t *> _bones_file;
  std::vector<std::string> _bones_file_name;

public:
  void InsertBoneFile(imd2_bone_file_t *file, std::string file_name)
  {
    _bones_file.push_back(file);
    _bones_file_name.push_back(file_name);
  }

  size_t GetBoneFileCount()
  {
    return _bones_file.size();
  }

  imd2_bone_file_t *GetBoneFile(size_t idx)
  {
    return _bones_file[idx];
  }
  std::string &GetBoneFileName(size_t idx)
  {
    return _bones_file_name[idx];
  }
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

  virtual void OnCloseDocument();
};
