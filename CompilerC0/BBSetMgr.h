// BBSetMgr.h: interface for the BBSetMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BBSETMGR_H__BA6D4A0F_69EF_428F_B178_E13E807DC50B__INCLUDED_)
#define AFX_BBSETMGR_H__BA6D4A0F_69EF_428F_B178_E13E807DC50B__INCLUDED_
#include "QuadrupleMgr.h"
#include "BasicBlock.h"
#include "define.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class SymbolTableMgr;
class QuadrupleMgr;
class Item;
class BasicBlock;
using namespace std;

class BBSetMgr  
{
public:
	SymbolTableMgr* SMgr;
	QuadrupleMgr* QMgr;
	Item* Proc;
	int procHead;
	int procEnd;
	BasicBlock* bb[200];
	int bbNumber;
public:
	BBSetMgr(SymbolTableMgr* aSMgr,QuadrupleMgr* aQMgr,Item* aProc);
	virtual ~BBSetMgr();
	void GetProcArea(int n);
	BasicBlock* NewBlock(SymbolTableMgr* aSMgr,QuadrupleMgr* aQMgr,int astart,int aend);
	void DivideBlock();
	BasicBlock* FindBlockFromStart(int n);
	BasicBlock* FindBlockFromEnd(int n);
	void AnalyzeBlock();
	void Display();
};

#endif // !defined(AFX_BBSETMGR_H__BA6D4A0F_69EF_428F_B178_E13E807DC50B__INCLUDED_)
