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
class Dll
{
public:
  Dll() : _handle(0) {}
  virtual ~Dll() { Unload(); }

  virtual bool Load(const std::string &path)
  {
    if (_handle)
      return false;
    _handle = LoadLibrary(path.c_str());
    return (IsLoaded());
  }

  virtual void *Symbole(const std::string &id)
  {
    if (!_handle)
      return NULL;
    return GetProcAddress(_handle, id.c_str());
  }

  virtual void Unload()
  {
    if (_handle)
    {
      FreeLibrary(_handle);
      _handle = 0;
    }
  }

  virtual bool IsLoaded() { return _handle != 0; }

private:
  HMODULE _handle;
};

class Plugins
{
public:
  Plugins() : _plugins(0) {}
  ~Plugins()
  {
    if (_plugins)
      _plugins->PluginDestroy();
  }
  Dll _dll;
  ImdViewerPlugins *_plugins;
  imdviewer_plugin_info_t *_info;
};

class CImdViewerApp : public CWinApp
{
public:
  CImdViewerApp();

  // Overrides
public:
  std::vector<Plugins *> _plugins;
  virtual BOOL InitInstance();
  void FillPluginsList();
  // Implementation
  afx_msg void OnAppAbout();
  DECLARE_MESSAGE_MAP()
  virtual int ExitInstance();
  virtual BOOL OnIdle(LONG lCount);
};

extern CImdViewerApp theApp;