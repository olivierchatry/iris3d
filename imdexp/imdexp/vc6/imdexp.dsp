# Microsoft Developer Studio Project File - Name="imdexp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=imdexp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "imdexp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "imdexp.mak" CFG="imdexp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "imdexp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "imdexp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "imdexp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IMDEXP_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IMDEXP_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "imdexp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IMDEXP_EXPORTS" /YX /FD /GZ  /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\lib\devil\include" /I "..\..\src\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib  core.lib geom.lib maxutil.lib mesh.lib odbc32.lib odbccp32.lib comctl32.lib d3dx8.lib d3d8.lib /nologo /base:"0x64660000" /dll /debug /machine:I386 /out:"c:\3dsmax5\plugins\imdexp.dle" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "imdexp - Win32 Release"
# Name "imdexp - Win32 Debug"
# Begin Group "resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Resource\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\Resource\resource.rc
# End Source File
# End Group
# Begin Group "dll"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\imdexp.def
# End Source File
# Begin Source File

SOURCE=..\..\src\imdexp_main.cpp
# End Source File
# End Group
# Begin Group "devil"

# PROP Default_Filter ""
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\devil\lib\DevIL.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\devil\lib\il_wrap.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\devil\lib\ILU.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\devil\lib\ILUT.lib
# End Source File
# End Group
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\devil\include\il\config.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\devil\include\il\devil_internal_exports.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\devil\include\il\il.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\devil\include\il\il_wrap.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\devil\include\il\ilu.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\devil\include\il\ilut.h
# End Source File
# End Group
# End Group
# Begin Group "stripper"

# PROP Default_Filter ""
# Begin Group "zappy"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\strip\zappy\Adjacency.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\strip\zappy\Adjacency.h
# End Source File
# Begin Source File

SOURCE=..\..\src\strip\zappy\CustomArray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\strip\zappy\CustomArray.h
# End Source File
# Begin Source File

SOURCE=..\..\src\strip\zappy\RevisitedRadix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\strip\zappy\RevisitedRadix.h
# End Source File
# Begin Source File

SOURCE=..\..\src\strip\zappy\Striper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\strip\zappy\Striper.h
# End Source File
# End Group
# Begin Group "nvidia"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\strip\nvidia\NvTriStrip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\strip\nvidia\NvTriStrip.h
# End Source File
# Begin Source File

SOURCE=..\..\src\strip\nvidia\NvTriStripObjects.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\strip\nvidia\NvTriStripObjects.h
# End Source File
# Begin Source File

SOURCE=..\..\src\strip\nvidia\VertexCache.h
# End Source File
# End Group
# End Group
# Begin Group "exporter"

# PROP Default_Filter ""
# Begin Group "callback"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\callback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\callback.h
# End Source File
# End Group
# Begin Group "viewer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\viewer\vb_wrap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\viewer\vb_wrap.h
# End Source File
# Begin Source File

SOURCE=..\..\src\viewer\viewer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\viewer\viewer.h
# End Source File
# End Group
# Begin Group "logger"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\log\loger.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\log\loger.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\def.h
# End Source File
# Begin Source File

SOURCE=..\..\src\ImdExp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ImdExp.h
# End Source File
# Begin Source File

SOURCE=..\..\src\ImdExpBone.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ImdExpLight.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ImdExpMaterial.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ImdExpMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ImdExpSkin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ImdExpTag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\stdafx.h
# End Source File
# End Group
# End Target
# End Project
