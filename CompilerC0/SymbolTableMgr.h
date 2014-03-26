// SymbolTableMgr.h: interface for the SymbolTableMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLTABLEMGR_H__D14D2906_025C_4709_8674_D5353C2E35AE__INCLUDED_)
#define AFX_SYMBOLTABLEMGR_H__D14D2906_025C_4709_8674_D5353C2E35AE__INCLUDED_
#include <iostream>
#include "Item.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
using namespace std;
class SymbolTableMgr  
{
public:
	SymbolTableMgr();
	virtual ~SymbolTableMgr();	
	
	int Check(string aname);
	int AllCapsCheck(string aname);
	int LayerCheck(string aname);
	int AllCapsLayerCheck(string aname);
	Item* Find(string aname);
	Item* AllCapsFind(string aname);
	Item* LayerFind(string aname);
	Item* AllCapsLayerFind(string aname);
	int LayerFindNo(string aname);
	int AllCapsLayerFindNo(string aname);
	int IsKeyWord(string aname);
	void Insert(string aname, int akind, int atype, string avalue, int adimension);
	int GetItemNumber();
	void StepIn();
	void StepIn(string aname);
	void StepOut();
	void DisplayAll();
	
public:
	SymbolTable* pCurrentTable;
	SymbolTable* pTopTable;
	Item* pCurrentItem;
};

#endif // !defined(AFX_SYMBOLTABLEMGR_H__D14D2906_025C_4709_8674_D5353C2E35AE__INCLUDED_)
