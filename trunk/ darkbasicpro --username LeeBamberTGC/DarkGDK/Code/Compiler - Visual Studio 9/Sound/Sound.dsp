# Microsoft Developer Studio Project File - Name="Sound" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Sound - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Sound.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Sound.mak" CFG="Sound - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Sound - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Sound - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Sound - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "DARKSDK_COMPILE" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\VS6\Sound.lib"

!ELSEIF  "$(CFG)" == "Sound - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "DARKSDK_COMPILE" /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Link\Sound.lib"

!ENDIF 

# Begin Target

# Name "Sound - Win32 Release"
# Name "Sound - Win32 Debug"
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Sound\CSoundC.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Sound\CSoundC.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Sound\CSoundDataC.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Sound\CSoundManagerC.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Sound\CSoundManagerC.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Sound\dsutil.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Sound\dsutil.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Sound\dxutil.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Sound\dxutil.h"
# End Source File
# End Group
# Begin Group "Error"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Error\CError.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Error\CError.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Error\CRuntimeErrors.h"
# End Source File
# End Group
# Begin Group "Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Data\CDataC.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Data\CDataC.h"
# End Source File
# End Group
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Core\globstruct.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Core\globstruct.h"
# End Source File
# End Target
# End Project
