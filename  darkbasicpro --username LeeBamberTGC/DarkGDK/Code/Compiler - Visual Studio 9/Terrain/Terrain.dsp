# Microsoft Developer Studio Project File - Name="Terrain" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Terrain - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Terrain.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Terrain.mak" CFG="Terrain - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Terrain - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Terrain - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Terrain - Win32 Release"

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
# ADD LIB32 /nologo /out:"..\..\Lib\VS6\Terrain.lib"

!ELSEIF  "$(CFG)" == "Terrain - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "DARKSDK_COMPILE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Link\Terrain.lib"

!ENDIF 

# Begin Target

# Name "Terrain - Win32 Release"
# Name "Terrain - Win32 Debug"
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Basic\Basic.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\cBitmap.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\cBitmap.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\cTerrain.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\cTerrain.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\DynamicArray2D.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\DynamicArray2D.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\fastlib.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\fastlib.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\Image.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\Image.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\pvs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\pvs.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\TGAImage.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\TGAImage.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\utility.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\Expansion Packs\Nature\Shared\utility.h"
# End Source File
# End Target
# End Project
