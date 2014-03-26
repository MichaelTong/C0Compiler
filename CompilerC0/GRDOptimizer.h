// GRDOptimizer.h: interface for the GRDOptimizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRDOPTIMIZER_H__06D9ABF7_8BF4_4068_A444_FD5E096A95B2__INCLUDED_)
#define AFX_GRDOPTIMIZER_H__06D9ABF7_8BF4_4068_A444_FD5E096A95B2__INCLUDED_
#include "BBSetMgr.h"
#include "ConflictTable.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class SymbolTableMgr;
class QuadrupleMgr;
class Quadruple;
class BBSetMgr;
class ConflictTable;
class Item;
using namespace std;

class GRDOptimizer  //Global Register Distribute
{
public:
	SymbolTableMgr* SMgr;
	QuadrupleMgr* QMgr;
	BBSetMgr* bbs[200];
	int bbsNumber;
	ConflictTable* ct[200];
	int ctNumber;

	Item* regitem[200][500];
	int regNumber[200];
	string reg[200][500];

	int quadNumber;
	Quadruple* quad[1000];

public:
	GRDOptimizer(SymbolTableMgr* aSMgr, QuadrupleMgr* aQMgr, BBSetMgr* abbs[], int abbsNumber);
	virtual ~GRDOptimizer();
	void CreateConflict();
	void DisplayCT();
	void DisplayRegItem();
	void AddRegItem(int n, Item* i);
	void RegisterSingleDistribute(int i);
	void RegisterDistribute();
	string FindString(int n, Item* item);
	void Generate();

};

#endif // !defined(AFX_GRDOPTIMIZER_H__06D9ABF7_8BF4_4068_A444_FD5E096A95B2__INCLUDED_)
