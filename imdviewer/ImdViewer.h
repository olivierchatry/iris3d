// ImdViewer.h : main header file for the ImdViewer application
//
#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h" // main symbols

// CImdViewerApp:
// See ImdViewer.cpp for the implementation of this class
//

class CImdViewerApp : public CWinApp
{
public:
  CImdViewerApp();

  // Overrides
public:
  virtual BOOL InitInstance();

  // Implementation
  afx_msg void OnAppAbout();
  DECLARE_MESSAGE_MAP()
  virtual BOOL OnIdle(LONG lCount);
};

extern CImdViewerApp theApp;