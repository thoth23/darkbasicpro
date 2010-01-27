# Microsoft Developer Studio Project File - Name="ConvX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ConvX - Win32 Debug_FPSCGame
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ConvX.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ConvX.mak" CFG="ConvX - Win32 Debug_FPSCGame"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ConvX - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ConvX - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ConvX - Win32 Debug_FPSCGame" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ConvX - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CONVX_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CONVX_EXPORTS" /YX /FD /EHa /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 D3DX9.lib d3dxof.lib d3d9.lib d3dx9.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\ConvX.dll"

!ELSEIF  "$(CFG)" == "ConvX - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CONVX_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CONVX_EXPORTS" /FR /YX /FD /GZ /EHa /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3dx9dt.lib d3dxof.lib d3d9.lib d3dx9.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\ConvX.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "ConvX - Win32 Debug_FPSCGame"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ConvX___Win32_Debug_FPSCGame"
# PROP BASE Intermediate_Dir "ConvX___Win32_Debug_FPSCGame"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ConvX___Win32_Debug_FPSCGame"
# PROP Intermediate_Dir "ConvX___Win32_Debug_FPSCGame"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CONVX_EXPORTS" /FR /YX /FD /GZ /EHa /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CONVX_EXPORTS" /FR /YX /FD /GZ /EHa /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 d3dx9dt.lib d3dxof.lib d3d9.lib d3dx9.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\ConvX.dll" /pdbtype:sept
# ADD LINK32 d3dx9dt.lib d3dxof.lib d3d9.lib d3dx9.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\ConvX.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ConvX - Win32 Release"
# Name "ConvX - Win32 Debug"
# Name "ConvX - Win32 Debug_FPSCGame"
# Begin Group "Convert"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\ConvX\ConvX.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\ConvX\ConvX.h
# End Source File
# End Group
# Begin Group "DBOFormat"

# PROP Default_Filter ""
# Begin Group "Extras"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\Extras\NVMeshMender.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\Extras\NVMeshMenderD3DX.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOBlock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOBlock.h
# End Source File
# Begin Source File

SOURCE=..\..\SHARED\DBOFORMAT\DBOData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SHARED\DBOFORMAT\DBOData.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOEffects.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOEffects.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOExternals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOExternals.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOFormat.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOFrame.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBOMesh.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBORawMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\DBOFormat\DBORawMesh.h
# End Source File
# End Group
# Begin Group "Error"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\Error\CError.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Error\CError.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Error\CRuntimeErrors.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Shared\Core\globstruct.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Core\globstruct.h
# End Source File
# End Target
# End Project
