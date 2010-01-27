# Microsoft Developer Studio Project File - Name="Meqon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Meqon - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Meqon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Meqon.mak" CFG="Meqon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Meqon - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Meqon - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Meqon - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fr /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\VS6\Meqon.lib"

!ELSEIF  "$(CFG)" == "Meqon - Win32 Debug"

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
# ADD LIB32 /nologo /out:"..\..\Lib\VS6\Meqon.lib"

!ENDIF 

# Begin Target

# Name "Meqon - Win32 Release"
# Name "Meqon - Win32 Debug"
# Begin Group "Terrain"

# PROP Default_Filter ""
# End Group
# Begin Group "Vehicle"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\AutomaticGearBox.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\automaticgearbox.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Car.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Car.h"
# End Source File
# End Group
# Begin Group "Rigid Body"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Joints.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Joints.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\RigidBody.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\RigidBody.h"
# End Source File
# End Group
# Begin Group "Character"

# PROP Default_Filter ""
# End Group
# Begin Group "Types"

# PROP Default_Filter ""
# Begin Group "Frame"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Frame.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Frame.h"
# End Source File
# End Group
# Begin Group "Material"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Material.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Material.h"
# End Source File
# End Group
# End Group
# Begin Group "Cloth"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Cloth.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Cloth.h"
# End Source File
# End Group
# Begin Group "Liquid Surfaces"

# PROP Default_Filter ""
# Begin Group "Internal Liquid"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurface.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurface.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurface_config.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurface_headers.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurfaceelement.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurfaceelement.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurfacemodule.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurfacemodule.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurfacerbconvexpolyhedroninteraction.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurfacerbconvexpolyhedroninteraction.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurfacerbsphereinteraction.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurfacerbsphereinteraction.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\liquidsurface\meq_liquidsurfacetagreader.h"
# End Source File
# End Group
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\LiquidSurfaces.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\LiquidSurfaces.h"
# End Source File
# End Group
# Begin Group "Particles"

# PROP Default_Filter ""
# Begin Group "Internal Particles"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\particlesystem\meq_particlemodule.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\particlesystem\meq_particlemodule.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\particlesystem\meq_particlesystem.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\particlesystem\meq_particlesystem.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\particlesystem\meq_particlesystem_config.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\particlesystem\meq_particlesystemelement.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\particlesystem\meq_particlesystemelement.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\particlesystem\meq_particlesystemrigidbodyinteraction.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\particlesystem\meq_particlesystemrigidbodyinteraction.h"
# End Source File
# End Group
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Particles.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Particles.h"
# End Source File
# End Group
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Globals.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Globals.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Meqon.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\Meqon.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Projects\Expansion Packs\Meqon\Code\MeqonData.h"
# End Source File
# Begin Source File

SOURCE=.\test.txt
# End Source File
# End Target
# End Project
