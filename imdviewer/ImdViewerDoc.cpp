// ImdViewerDoc.cpp : implementation of the CImdViewerDoc class
//

#include "stdafx.h"
#include "ImdViewer.h"

#include "ImdViewerDoc.h"
#include "ImdViewerView.h"
#include "DialogBarBoneList.h"
#include "DialogBarMeshInfo.h"
#include "DialogBarBoneTree.h"
#include "DialogBarAnim.h"
#include "DialogBarConfig.h"

#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImdViewerDoc

IMPLEMENT_DYNCREATE(CImdViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CImdViewerDoc, CDocument)
END_MESSAGE_MAP()


// CImdViewerDoc construction/destruction

CImdViewerDoc::CImdViewerDoc() : _object(0)
{
}

CImdViewerDoc::~CImdViewerDoc()
{
}

BOOL CImdViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}




// CImdViewerDoc serialization

void CImdViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CImdViewerDoc diagnostics

#ifdef _DEBUG
void CImdViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImdViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImdViewerDoc commands

BOOL CImdViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	std::string	file(lpszPathName);
	std::string	path(file.substr(0, file.find_last_of("\\/")));
	std::string bone_file_name(file.substr(0, file.find_last_of(".")));
	bone_file_name += ".imdbone";
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	_object = load_imd2(lpszPathName);
	if (_object == 0)
	{
		AfxMessageBox("Cannot load IMD2 file. Sorry.", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	CMainFrame *main_frame = (CMainFrame*) this->GetRoutingFrame();
	main_frame->SetImd2Object(_object, path);
	FILE *test_exist = fopen(bone_file_name.c_str(), "rb");
	if (test_exist)
	{
		fclose(test_exist);
		main_frame->SetBoneFile(bone_file_name.c_str());
	}
	return TRUE;
}

void CImdViewerDoc::OnCloseDocument()
{
	if (_object)
		free_imd2(_object);
	for (size_t i = 0; i < _bones_file.size(); ++i)
		free_imd2_bone(_bones_file[i]);
	_bones_file.clear();
	_object = 0;
	CDocument::OnCloseDocument();
}
