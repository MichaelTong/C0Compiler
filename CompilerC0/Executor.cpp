// Executor.cpp: implementation of the Executor class.
//
//////////////////////////////////////////////////////////////////////

#include "Executor.h"
#include "CompilerC0.h"
#include "define.h"
#include <process.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Executor::Executor(string path,string filename)
{
	TCHAR szModuleFileName[MAX_PATH];
	_path=path;
	_file=filename;
	if (GetModuleFileName(NULL, szModuleFileName, 100) == NULL) 
		printf("GetModuleFileName failed.\n");
	else{
		c = szModuleFileName;
		while (c[c.length() - 1] != '\\'){
			c.resize(c.size() - 1);
		}
		bin = c + "bin\\";
		include = c + "include\\";
		lib = c +"lib\\";
		macro = c + "macros\\";
	}
	mlpara = "ML.EXE /c /coff /Cp /Fo \"" + path + filename + ".obj\" \"" + path + filename + ".asm\"";
	linkpara = "LINK.EXE /SUBSYSTEM:CONSOLE /RELEASE /VERSION:4.0 /OUT:\"" + path + filename + ".exe\" \"" + path + filename + ".obj\"";
}

Executor::~Executor()
{

}

void Executor::Execute(){
	string temp = bin + mlpara;
	printf("Assemblying...\nRunning command:\n");
	cout << temp <<endl;
	LPTSTR cmd = (LPTSTR)temp.c_str();
	system(cmd);
	printf("Linking...\nRunning command:\n");
	temp = bin + linkpara;
	cout << temp <<endl;
	cmd = (LPTSTR)temp.c_str();
	system(cmd);
	cout<<"Do you want to execute the program?(Y/N):\t";
	string rep="";
	cin>>rep;
	while(rep!="Y"&&rep!="y"&&rep!="n"&&rep!="N")
	{
		cout<<"Please enter Y/N:\t";
		cin>>rep;
	}
	if(rep=="Y"||rep=="y"){
		temp=_path+_file+".exe";
		cmd=(LPTSTR)temp.c_str();
		system(cmd);
		cout<<endl;
	}
	else
		cout<<"Existing..."<<endl;
}
