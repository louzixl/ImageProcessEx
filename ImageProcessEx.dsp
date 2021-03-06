# Microsoft Developer Studio Project File - Name="ImageProcessEx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ImageProcessEx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ImageProcessEx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ImageProcessEx.mak" CFG="ImageProcessEx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImageProcessEx - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ImageProcessEx - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ImageProcessEx - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ImageProcessEx - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ImageProcessEx - Win32 Release"
# Name "ImageProcessEx - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BWHighpassDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BWLowpassDlg1.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\dib.cpp
# End Source File
# Begin Source File

SOURCE=.\FourieDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HighPassDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoEquivalize.cpp
# End Source File
# Begin Source File

SOURCE=.\IFourieDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcessEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcessEx.rc
# End Source File
# Begin Source File

SOURCE=.\ImageProcessExDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcessExView.cpp
# End Source File
# Begin Source File

SOURCE=.\LinearTransDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LowPassedDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MiddlePassDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PrewittDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RobertsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RotateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RowToBmpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SobelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Translation.cpp
# End Source File
# Begin Source File

SOURCE=.\TranslationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TransposeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BWHighpassDlg.h
# End Source File
# Begin Source File

SOURCE=.\BWLowpassDlg1.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\dib.h
# End Source File
# Begin Source File

SOURCE=.\FourieDlg.h
# End Source File
# Begin Source File

SOURCE=.\HighPassDlg.h
# End Source File
# Begin Source File

SOURCE=.\HistoEquivalize.h
# End Source File
# Begin Source File

SOURCE=.\IFourieDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessEx.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessExDoc.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessExView.h
# End Source File
# Begin Source File

SOURCE=.\LinearTransDlg.h
# End Source File
# Begin Source File

SOURCE=.\LowPassedDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MiddlePassDlg.h
# End Source File
# Begin Source File

SOURCE=.\PrewittDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RobertsDlg.h
# End Source File
# Begin Source File

SOURCE=.\RotateDlg.h
# End Source File
# Begin Source File

SOURCE=.\RowToBmpDlg.h
# End Source File
# Begin Source File

SOURCE=.\SobelDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Translation.h
# End Source File
# Begin Source File

SOURCE=.\TranslationDlg.h
# End Source File
# Begin Source File

SOURCE=.\TransposeDlg.h
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ImageProcessEx.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImageProcessEx.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ImageProcessExDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
