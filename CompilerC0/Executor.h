// Executor.h: interface for the Executor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXECUTOR_H__71CC1828_FACC_4679_B4F1_297E0740F03C__INCLUDED_)
#define AFX_EXECUTOR_H__71CC1828_FACC_4679_B4F1_297E0740F03C__INCLUDED_
#include <iostream>
#include <Windows.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
using namespace std;
class Executor  
{
public:
	Executor(string path,string filename);
	virtual ~Executor();

	void Execute();
public:
	string c;
	string bin;
	string include;
	string lib;
	string mlpara;
	string linkpara;
	string macro;
	string _path;
	string _file;

};

#endif // !defined(AFX_EXECUTOR_H__71CC1828_FACC_4679_B4F1_297E0740F03C__INCLUDED_)
