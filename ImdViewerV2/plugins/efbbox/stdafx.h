// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#define ASSERT_VIEWER(value, str) \
  {                               \
  }

#include <vector>
#include "../../imd2/imd2.hpp"
#include "../../pluginsapi/pluginsapi.h"
#include "magic/MgcCont3DBox.h"
// TODO: reference additional headers your program requires here
