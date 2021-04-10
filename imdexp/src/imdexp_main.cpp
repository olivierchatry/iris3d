
/********************************************************************
	created:	2003/02/03
	created:	3:2:2003   20:03
	filename: 	e:\_dev\tmp\imdexp\src\imdexp_main.cpp
	file path:	e:\_dev\tmp\imdexp\src
	file base:	imdexp_main
	file ext:	cpp
	author:		Chatry Olivier alias gruiiik
	purpose:	Main source for imd max exporter
*********************************************************************/

#include <stdafx.h>

HINSTANCE hInstance;
int controlsInit = FALSE;

static BOOL showPrompts;
static BOOL exportSelected;

// Class ID. These must be unique and randomly generated!!
// If you use this as a sample project, this is the first thing
// you should change!
#define IMDEXP_CLASS_ID Class_ID(0x26679e2, 0x6b6a040f)

BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
{
  hInstance = hinstDLL;

  // Initialize the custom controls. This should be done only once.
  if (!controlsInit)
  {
    controlsInit = TRUE;
    InitCustomControls(hInstance);
    InitCommonControls();
  }

  return (TRUE);
}

__declspec(dllexport) const TCHAR *LibDescription()
{
  return GetString(IDS_LIBDESCRIPTION);
}

/// MUST CHANGE THIS NUMBER WHEN ADD NEW CLASS
__declspec(dllexport) int LibNumberClasses()
{
  return 1;
}

__declspec(dllexport) ULONG LibVersion()
{
  return VERSION_3DSMAX;
}

// Let the plug-in register itself for deferred loading
__declspec(dllexport) ULONG CanAutoDefer()
{
  return 1;
}

class ImdExpClassDesc : public ClassDesc
{
public:
  int IsPublic() { return 1; }
  void *Create(BOOL loading = FALSE) { return new ImdExp; }
  const TCHAR *ClassName() { return GetString(IDS_IMDEXP); }
  SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
  Class_ID ClassID() { return IMDEXP_CLASS_ID; }
  const TCHAR *Category() { return GetString(IDS_CATEGORY); }
};

static ImdExpClassDesc ImdExpDesc;

ClassDesc *GetImdExpDesc()
{
  return &ImdExpDesc;
}

TCHAR *GetString(int id)
{
  static TCHAR buf[256];

  if (hInstance)
    return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;

  return NULL;
}

__declspec(dllexport) ClassDesc *LibClassDesc(int i)
{
  switch (i)
  {
  case 0:
    return GetImdExpDesc();
  default:
    return 0;
  }
}
