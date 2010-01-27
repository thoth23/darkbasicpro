# Microsoft Developer Studio Project File - Name="Objects" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Objects - Win32 NewCommandsDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Objects.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Objects.mak" CFG="Objects - Win32 NewCommandsDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Objects - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Objects - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Objects - Win32 NewCommandsPreview Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Objects - Win32 NewCommandsPreview Remote Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Objects - Win32 FPSC Game Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Objects - Win32 FPSC Segment Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Objects - Win32 DebugLAUNCH" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Objects - Win32 NewCommandsDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Objects - Win32 Release"

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
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /YX /FD /EHa /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 D3dx9.lib odbc32.lib odbccp32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /dll /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "Objects - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 odbc32.lib odbccp32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib dxguid.lib d3d9.lib dxerr9.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsPreview Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Objects___Win32_NewCommandsPreview_Release"
# PROP BASE Intermediate_Dir "Objects___Win32_NewCommandsPreview_Release"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Objects___Win32_NewCommandsPreview_Release"
# PROP Intermediate_Dir "Objects___Win32_NewCommandsPreview_Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /FR /YX /FD /GZ /EHa /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /D "NEWCOMMANDSPREVIEW" /FR /YX /FD /GZ /EHa /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib d3d8.lib d3dx8.lib dxerr8.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 odbc32.lib odbccp32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib dxguid.lib d3d9.lib dxerr9.lib /nologo /dll /incremental:no /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile /debug

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsPreview Remote Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Objects___Win32_NewCommandsPreview_Remote_Debug"
# PROP BASE Intermediate_Dir "Objects___Win32_NewCommandsPreview_Remote_Debug"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Objects___Win32_NewCommandsPreview_Remote_Debug"
# PROP Intermediate_Dir "Objects___Win32_NewCommandsPreview_Remote_Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /D "NEWCOMMANDSPREVIEW" /FR /YX /FD /GZ /EHa /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /D "NEWCOMMANDSPREVIEW" /FR /YX /FD /GZ /EHa /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib d3d8.lib d3dx8.lib dxerr8.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 odbc32.lib odbccp32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib dxguid.lib d3d9.lib dxerr9.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "Objects - Win32 FPSC Game Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Objects___Win32_FPSC_Game_Debug"
# PROP BASE Intermediate_Dir "Objects___Win32_FPSC_Game_Debug"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Objects___Win32_FPSC_Game_Debug"
# PROP Intermediate_Dir "Objects___Win32_FPSC_Game_Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /D "NEWCOMMANDSPREVIEW" /FR /YX /FD /GZ /EHa /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /D "NEWCOMMANDSPREVIEW" /FR /YX /FD /GZ /EHa /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib d3d8.lib d3dx8.lib dxerr8.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 odbc32.lib odbccp32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib dxguid.lib d3d9.lib dxerr9.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "Objects - Win32 FPSC Segment Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Objects___Win32_FPSC_Segment_Debug"
# PROP BASE Intermediate_Dir "Objects___Win32_FPSC_Segment_Debug"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Objects___Win32_FPSC_Segment_Debug"
# PROP Intermediate_Dir "Objects___Win32_FPSC_Segment_Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /D "NEWCOMMANDSPREVIEW" /FR /YX /FD /GZ /EHa /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /D "NEWCOMMANDSPREVIEW" /FR /YX /FD /GZ /EHa /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib d3d9.lib d3dx9.lib dxerr9.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 odbc32.lib odbccp32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib dxguid.lib d3d9.lib dxerr9.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "Objects - Win32 DebugLAUNCH"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Objects___Win32_DebugLAUNCH"
# PROP BASE Intermediate_Dir "Objects___Win32_DebugLAUNCH"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Objects___Win32_DebugLAUNCH"
# PROP Intermediate_Dir "Objects___Win32_DebugLAUNCH"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib d3d9.lib d3dx9.lib dxerr9.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 odbc32.lib odbccp32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib dxguid.lib d3d9.lib dxerr9.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Objects___Win32_NewCommandsDebug"
# PROP BASE Intermediate_Dir "Objects___Win32_NewCommandsDebug"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Objects___Win32_NewCommandsDebug"
# PROP Intermediate_Dir "Objects___Win32_NewCommandsDebug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /D "NEWCOMMANDSPREVIEW" /FR /YX /FD /GZ /EHa /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OBJECTS_EXPORTS" /D "NEWCOMMANDSPREVIEW" /FR /YX /FD /GZ /EHa /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 odbc32.lib odbccp32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib dxguid.lib d3d9.lib dxerr9.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 odbc32.lib odbccp32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib dxguid.lib d3d9.lib dxerr9.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\Dark Basic Pro\Compiler\plugins\DBProBasic3DDebug.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "Objects - Win32 Release"
# Name "Objects - Win32 Debug"
# Name "Objects - Win32 NewCommandsPreview Release"
# Name "Objects - Win32 NewCommandsPreview Remote Debug"
# Name "Objects - Win32 FPSC Game Debug"
# Name "Objects - Win32 FPSC Segment Debug"
# Name "Objects - Win32 DebugLAUNCH"
# Name "Objects - Win32 NewCommandsDebug"
# Begin Group "Objects"

# PROP Default_Filter ""
# Begin Group "U5.3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\Objects\CObjectsC.h

!IF  "$(CFG)" == "Objects - Win32 Release"

!ELSEIF  "$(CFG)" == "Objects - Win32 Debug"

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsPreview Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsPreview Remote Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 FPSC Game Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 FPSC Segment Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 DebugLAUNCH"

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "U6.0"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\Objects\CObjectsNewC.h

!IF  "$(CFG)" == "Objects - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsPreview Release"

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsPreview Remote Debug"

!ELSEIF  "$(CFG)" == "Objects - Win32 FPSC Game Debug"

!ELSEIF  "$(CFG)" == "Objects - Win32 FPSC Segment Debug"

!ELSEIF  "$(CFG)" == "Objects - Win32 DebugLAUNCH"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsDebug"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Shared\Objects\CObjectDataC.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\CObjectManagerC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\CObjectManagerC.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\CObjectsC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\CommonC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\CommonC.h
# End Source File
# End Group
# Begin Group "Position"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\Objects\CPositionC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\CPositionC.h
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
# Begin Group "CollisionSystem"

# PROP Default_Filter ""
# Begin Group "Mechanics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\Objects\BoxCollision\3DCol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\BoxCollision\3DCol.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\BoxCollision\cBoxCol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\BoxCollision\cBoxCol.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\BoxCollision\CCollision.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\BoxCollision\CCollision.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\BoxCollision\PolyPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\BoxCollision\PolyPool.h
# End Source File
# End Group
# Begin Group "ElipsoidCollision"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\Objects\ElipsoidCollision\col_global.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\ElipsoidCollision\col_local.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\ElipsoidCollision\col_response.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\ElipsoidCollision\col_step.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\ElipsoidCollision\col_types.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\ElipsoidCollision\Collision.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\ElipsoidCollision\Collision.h
# End Source File
# End Group
# End Group
# Begin Group "DBO"

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
# Begin Group "Nodetree / Universe"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\Data\CDataC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Data\CDataC.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\cLightMaps.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\cLightMaps.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Universe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Universe.h
# End Source File
# End Group
# Begin Group "CSG"

# PROP Default_Filter ""
# Begin Group "Compiler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CBounds.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CBounds.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CBSPTree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CBSPTree.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CCompiler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CCompiler.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\Common.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CompilerTypes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CompilerTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CPlane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CPlane.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CPortals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CPortals.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\CVector.h
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\ProcessHSR.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\Compiler\ProcessHSR.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Shared\Objects\CSG.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Objects\CSG.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Shared\Camera\CCameraDataC.h
# End Source File
# Begin Source File

SOURCE=.\DLLMain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shared\Core\globstruct.cpp
# End Source File
# Begin Source File

SOURCE=.\icon.bmp
# End Source File
# Begin Source File

SOURCE=.\Objects.rc

!IF  "$(CFG)" == "Objects - Win32 Release"

!ELSEIF  "$(CFG)" == "Objects - Win32 Debug"

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsPreview Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsPreview Remote Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 FPSC Game Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 FPSC Segment Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 DebugLAUNCH"

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ObjectsPlus.rc

!IF  "$(CFG)" == "Objects - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsPreview Release"

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsPreview Remote Debug"

!ELSEIF  "$(CFG)" == "Objects - Win32 FPSC Game Debug"

!ELSEIF  "$(CFG)" == "Objects - Win32 FPSC Segment Debug"

!ELSEIF  "$(CFG)" == "Objects - Win32 DebugLAUNCH"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Objects - Win32 NewCommandsDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Target
# End Project
