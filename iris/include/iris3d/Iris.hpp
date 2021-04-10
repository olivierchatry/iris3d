/*********************************************************
**  File name : Iris.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
// TODO : look for precompiled header with gcc ...
#ifndef __IRIS_H__
#define __IRIS_H__

#define IRIS_DEGTORAD(deg) ((deg * F_PI) / 180)

typedef unsigned char uchar;
typedef unsigned int uint;

// -- KOS --
#include <kos.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <png/png.h>
#include <dc/fmath.h>
#include <stdio.h>
#include <stdlib.h>

// -- STL --
#include <vector>
#include <set>

// -- IRIS - math
// -- IRIS - Support Classes --
#include "math/macro.hpp"
#include "math/matrix.hpp"
#include "math/polar.hpp"
#include "math/vect3d.hpp"
#include "math/quat.hpp"
#include "math/vertex2dtl.hpp"
#include "IrisColor.hpp"

// -- IRIS --
// FIXME: reorganize the order or everything below

#include "model/imd.hpp"
#include "model/imd2.hpp"
#include "IrisPlane.hpp"
#include "IrisFrustrum.hpp"
#include "IrisManager.hpp"
#include "IrisMaterial.hpp"
#include "IrisMaterialManager.hpp"
#include "model/IrisImportObject.hpp"
#include "model/IrisObjectManager.hpp"
#include "animation/IrisSkeletal.hpp"
#include "animation/IrisSkeletalManager.hpp"

#include "spline/spline.hpp"
#include "IrisCamera.hpp"
#include "IrisGeometryPipeline.hpp"
#include "IrisLight.hpp"
#include "IrisOmniLight.hpp"
#include "IrisSmousseLight.hpp"
#include "IrisLightingPipeline.hpp"
#include "input/IrisInputManager.hpp"
#include "IrisContext.hpp"
#include "IrisFaceClipper.hpp"
#include "entity/IrisApplicationPart.hpp"
#include "entity/IrisApplication.hpp"
#include "model/IrisObject.hpp"
#include "model/IrisManualObject.hpp"

// opcode
#include "../opcode/Opcode.h"
#include "collision/ColModel.hpp"
#include "collision/ColModelModel.hpp"
#include "collision/ColSphereModel.hpp"
#include "collision/ColRayModel.hpp"

// helper
// #include	"helper/IrisHelper.hpp"
#endif
