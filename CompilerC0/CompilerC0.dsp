# Microsoft Developer Studio Project File - Name="CompilerC0" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=CompilerC0 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CompilerC0.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CompilerC0.mak" CFG="CompilerC0 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CompilerC0 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "CompilerC0 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CompilerC0 - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "CompilerC0 - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CompilerC0 - Win32 Release"
# Name "CompilerC0 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BasicBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\BBSetMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\CompilerC0.cpp
# End Source File
# Begin Source File

SOURCE=.\ConflictTable.cpp
# End Source File
# Begin Source File

SOURCE=.\CSDOptimizer.cpp
# End Source File
# Begin Source File

SOURCE=.\DAGNode.cpp
# End Source File
# Begin Source File

SOURCE=.\DAGTable.cpp
# End Source File
# Begin Source File

SOURCE=.\DSAOptimizer.cpp
# End Source File
# Begin Source File

SOURCE=.\DTS.cpp
# End Source File
# Begin Source File

SOURCE=.\Executor.cpp
# End Source File
# Begin Source File

SOURCE=.\GRDOptimizer.cpp
# End Source File
# Begin Source File

SOURCE=.\Item.cpp
# End Source File
# Begin Source File

SOURCE=.\Lexer.cpp
# End Source File
# Begin Source File

SOURCE=.\Num.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Quadruple.cpp
# End Source File
# Begin Source File

SOURCE=.\QuadrupleMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolTable.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolTableMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Tag.cpp
# End Source File
# Begin Source File

SOURCE=.\Token.cpp
# End Source File
# Begin Source File

SOURCE=.\Type.cpp
# End Source File
# Begin Source File

SOURCE=.\Word.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BasicBlock.h
# End Source File
# Begin Source File

SOURCE=.\BBSetMgr.h
# End Source File
# Begin Source File

SOURCE=.\CodeGenerator.h
# End Source File
# Begin Source File

SOURCE=.\CompilerC0.h
# End Source File
# Begin Source File

SOURCE=.\ConflictTable.h
# End Source File
# Begin Source File

SOURCE=.\CSDOptimizer.h
# End Source File
# Begin Source File

SOURCE=.\DAGNode.h
# End Source File
# Begin Source File

SOURCE=.\DAGTable.h
# End Source File
# Begin Source File

SOURCE=.\define.h
# End Source File
# Begin Source File

SOURCE=.\DSAOptimizer.h
# End Source File
# Begin Source File

SOURCE=.\DTS.h
# End Source File
# Begin Source File

SOURCE=.\Executor.h
# End Source File
# Begin Source File

SOURCE=.\GRDOptimizer.h
# End Source File
# Begin Source File

SOURCE=.\Item.h
# End Source File
# Begin Source File

SOURCE=.\Lexer.h
# End Source File
# Begin Source File

SOURCE=.\Num.h
# End Source File
# Begin Source File

SOURCE=.\Parser.h
# End Source File
# Begin Source File

SOURCE=.\Quadruple.h
# End Source File
# Begin Source File

SOURCE=.\QuadrupleMgr.h
# End Source File
# Begin Source File

SOURCE=.\SymbolTable.h
# End Source File
# Begin Source File

SOURCE=.\SymbolTableMgr.h
# End Source File
# Begin Source File

SOURCE=.\Tag.h
# End Source File
# Begin Source File

SOURCE=.\Token.h
# End Source File
# Begin Source File

SOURCE=.\Type.h
# End Source File
# Begin Source File

SOURCE=.\Windows.h
# End Source File
# Begin Source File

SOURCE=.\Word.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
