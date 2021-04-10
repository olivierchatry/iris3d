#pragma once
#pragma message("Creating precompiled header. Please wait ...")
#include <windows.h>
#include <max.h>
#include <istdplug.h>
#include <stdmat.h>
#include <iparamb2.h>
#include <iskin.h>
#include <modstack.h>
#include <decomp.h>

#include <Stdlib.h>
#include <Stdio.h>
#include <String.h>
#include "hack/MAX_Mem.h"
#define null NULL
#define RELEASE(x) \
  {                \
    if (x != null) \
      delete x;    \
    x = null;      \
  }
#define RELEASEARRAY(x) \
  {                     \
    if (x != null)      \
      delete[] x;       \
    x = null;           \
  }

typedef signed char sbyte;
typedef unsigned char ubyte;
typedef signed short sword;
typedef unsigned short uword;
typedef signed int sdword;
typedef unsigned int udword;
typedef signed __int64 sqword;
typedef unsigned __int64 uqword;
typedef float sfloat;

#include "strip/zappy/RevisitedRadix.h"
#include "strip/zappy/CustomArray.h"
#include "strip/zappy/Adjacency.h"
#include "strip/zappy/Striper.h"
#include "strip/nvidia/NvTriStripObjects.h"
#include "strip/nvidia/NvTriStrip.h"
#include "strip/nvidia/VertexCache.h"
#include "il/il.h"
#include "il/ilu.h"
#include "il/ilut.h"
#include "../resource/resource.h"

#include "log/loger.h"
#include "def.h"
#include "imd2/imd2.hpp"
#include "ImdExp.h"
#include <assert.h>
#include <set>
