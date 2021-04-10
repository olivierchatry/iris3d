// ImdViewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ImdViewer.h"
#include "MainFrm.h"

#include "ImdViewerDoc.h"
#include "ImdViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImdViewerApp

BEGIN_MESSAGE_MAP(CImdViewerApp, CWinApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
// Standard file based document commands
ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

void CImdViewerApp::FillPluginsList()
{
  char buffer[MAX_PATH];
  bool finished;
  WIN32_FIND_DATA search;
  HANDLE handle;

  GetCurrentDirectory(MAX_PATH, buffer);
  CString str;
  std::string str_path(buffer);
  str_path += "\\plugins\\";
  str = str + str_path.c_str() + "*.dll";
  finished = false;
  _plugins.clear();
  handle = FindFirstFile(str.GetBuffer(0), &search);
  if (handle == INVALID_HANDLE_VALUE)
    return;
  while (!finished)
  {
    std::string str_tmp(str_path);
    str_tmp += search.cFileName;
    _plugins.push_back(new Plugins());
    Plugins *plug = _plugins.back();
    if (!plug->_dll.Load(str_tmp.c_str()))
    {
      delete plug;
      _plugins.pop_back();
    }
    else
    {
      typedef imdviewer_plugin_info_t *(*fct_info_t)();
      fct_info_t fct_info;
      if ((fct_info = (fct_info_t)plug->_dll.Symbole("GetPluginInfo")) != NULL)
      {
        plug->_info = fct_info();
        typedef ImdViewerPlugins *(*fct_creator_t)();
        fct_creator_t fct_creator;
        if ((fct_creator = (fct_creator_t)plug->_dll.Symbole("CreateInstance")) != NULL)
        {
          plug->_plugins = fct_creator();
          plug->_plugins->PluginInit();
        }
        else
        {
          plug->_dll.Unload();
          delete plug;
          _plugins.pop_back();
        }
      }
      else
      {
        plug->_dll.Unload();
        delete plug;
        _plugins.pop_back();
      }
    }
    if (!FindNextFile(handle, &search))
      finished = true;
  }
  FindClose(handle);
}

// CImdViewerApp construction

CImdViewerApp::CImdViewerApp()
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}

// The one and only CImdViewerApp object

CImdViewerApp theApp;

// CImdViewerApp initialization

BOOL CImdViewerApp::InitInstance()
{
  // InitCommonControls() is required on Windows XP if an application
  // manifest specifies use of ComCtl32.dll version 6 or later to enable
  // visual styles.  Otherwise, any window creation will fail.
  InitCommonControls();

  CWinApp::InitInstance();

  // Initialize OLE libraries
  if (!AfxOleInit())
  {
    AfxMessageBox(IDP_OLE_INIT_FAILED);
    return FALSE;
  }
  AfxEnableControlContainer();
  // Standard initialization
  // If you are not using these features and wish to reduce the size
  // of your final executable, you should remove from the following
  // the specific initialization routines you do not need
  // Change the registry key under which our settings are stored
  // TODO: You should modify this string to be something appropriate
  // such as the name of your company or organization
  ilInit();
  iluInit();
  ilutInit();

  SetRegistryKey(_T("ImdVIewer"));
  LoadStdProfileSettings(4); // Load standard INI file options (including MRU)
  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views
  FillPluginsList();
  CSingleDocTemplate *pDocTemplate;
  pDocTemplate = new CSingleDocTemplate(
      IDR_MAINFRAME,
      RUNTIME_CLASS(CImdViewerDoc),
      RUNTIME_CLASS(CMainFrame), // main SDI frame window
      RUNTIME_CLASS(CImdViewerView));
  if (!pDocTemplate)
    return FALSE;
  AddDocTemplate(pDocTemplate);
  // Enable DDE Execute open
  EnableShellOpen();
  RegisterShellFileTypes(TRUE);
  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);
  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;
  // The one and only window has been initialized, so show and update it
  m_pMainWnd->ShowWindow(SW_SHOW);
  m_pMainWnd->UpdateWindow();
  // call DragAcceptFiles only if there's a suffix
  //  In an SDI app, this should occur after ProcessShellCommand
  // Enable drag/drop open
  m_pMainWnd->DragAcceptFiles();
  return TRUE;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

  // Dialog Data
  enum
  {
    IDD = IDD_ABOUTBOX
  };

protected:
  virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

  // Implementation
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CImdViewerApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

// CImdViewerApp message handlers

int CImdViewerApp::ExitInstance()
{
  for (size_t i = 0; i < _plugins.size(); ++i)
  {
    _plugins[i]->_plugins->PluginDestroy();
    _plugins[i]->_plugins = 0;
    _plugins[i]->_dll.Unload();
    delete _plugins[i];
  }
  return CWinApp::ExitInstance();
}

BOOL CImdViewerApp::OnIdle(LONG lCount)
{
  CMainFrame *pMainFrame;
  pMainFrame = (CMainFrame *)this->GetMainWnd();
  if (pMainFrame)
  {
    CImdViewerDoc *pDoc = (CImdViewerDoc *)pMainFrame->GetActiveDocument();
    pMainFrame->Render();
  }
  return CWinApp::OnIdle(lCount);
}
