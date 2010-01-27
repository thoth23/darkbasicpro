# Microsoft Developer Studio Project File - Name="Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Library.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Library.mak" CFG="Library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Library - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Library - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Library - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\VS6\DarkSDK.lib"

!ELSEIF  "$(CFG)" == "Library - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\VS6\DarkSDK.lib"

!ENDIF 

# Begin Target

# Name "Library - Win32 Release"
# Name "Library - Win32 Debug"
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKCore.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKCore.h
# End Source File
# End Group
# Begin Group "Text"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKText.h
# End Source File
# End Group
# Begin Group "Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKInput.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKInput.h
# End Source File
# End Group
# Begin Group "File"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKFile.h
# End Source File
# End Group
# Begin Group "Display"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKDisplay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKDisplay.h
# End Source File
# End Group
# Begin Group "Basic2D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKBasic2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKBasic2D.h
# End Source File
# End Group
# Begin Group "Bitmap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKBitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKBitmap.h
# End Source File
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKSound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKSound.h
# End Source File
# End Group
# Begin Group "Music"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKMusic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKMusic.h
# End Source File
# End Group
# Begin Group "Sprite"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKSprites.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKSprites.h
# End Source File
# End Group
# Begin Group "Image"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKImage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKImage.h
# End Source File
# End Group
# Begin Group "Light"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKLight.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKLight.h
# End Source File
# End Group
# Begin Group "Camera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKCamera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKCamera.h
# End Source File
# End Group
# Begin Group "Basic3D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKBasic3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKBasic3D.h
# End Source File
# End Group
# Begin Group "Matrix"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKMatrix.h
# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKWorld.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKWorld.h
# End Source File
# End Group
# Begin Group "Particles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKParticles.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKParticles.h
# End Source File
# End Group
# Begin Group "3DMaths"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDK3DMaths.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDK3DMaths.h
# End Source File
# End Group
# Begin Group "FTP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKFtp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKFtp.h
# End Source File
# End Group
# Begin Group "Memblocks"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKMemblocks.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKMemblocks.h
# End Source File
# End Group
# Begin Group "Multiplayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKMultiplayer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKMultiplayer.h
# End Source File
# End Group
# Begin Group "System"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKSystem.h
# End Source File
# End Group
# Begin Group "Animation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKAnimation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKAnimation.h
# End Source File
# End Group
# Begin Group "Terrain"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DarkSDKTerrain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDKTerrain.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Source\DarkSDK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\DarkSDK.h
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Core\globstruct.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Dark Basic Pro SDK\SHARED\Core\globstruct.h"
# End Source File
# Begin Source File

SOURCE=.\Info.txt
# End Source File
# End Target
# End Project
