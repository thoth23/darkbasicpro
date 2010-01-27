# Microsoft Developer Studio Project File - Name="Q3BSP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Q3BSP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Q3BSP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Q3BSP.mak" CFG="Q3BSP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Q3BSP - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Q3BSP - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Dark Basic Pro Shared/Dark Basic Pro SDK/DarkSDK/Q3BSP", WKSAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Q3BSP - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Q3BSP_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Q3BSP_EXPORTS" /YX /FD /EHa /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProWorld3DDebug.dll"

!ELSEIF  "$(CFG)" == "Q3BSP - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Q3BSP_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Q3BSP_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProWorld3DDebug.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Q3BSP - Win32 Release"
# Name "Q3BSP - Win32 Debug"
# Begin Group "Loader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\CBSPFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\CBSPFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\Loader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\Loader.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\Unzip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\Unzip.h
# End Source File
# End Group
# Begin Group "Renderer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\Patch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\Shaders.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\Textures.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\Textures.h
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
# Begin Group "Collision"

# PROP Default_Filter ""
# Begin Group "col"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\SHARED\Q3BSP\col\3DMath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SHARED\Q3BSP\col\3DMath.h
# End Source File
# Begin Source File

SOURCE=..\..\SHARED\Q3BSP\col\col_global.h
# End Source File
# Begin Source File

SOURCE=..\..\SHARED\Q3BSP\col\col_local.h
# End Source File
# Begin Source File

SOURCE=..\..\SHARED\Q3BSP\col\col_response.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SHARED\Q3BSP\col\col_step.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SHARED\Q3BSP\col\col_types.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\Collision.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Q3BSP\Collision.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DLLMain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Core\globstruct.h
# End Source File
# Begin Source File

SOURCE=.\icon.bmp
# End Source File
# Begin Source File

SOURCE=.\Q3BSP.rc
# End Source File
# End Target
# End Project
