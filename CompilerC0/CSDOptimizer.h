// CSDOptimizer.h: interface for the CSDOptimizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSDOPTIMIZER_H__587B5815_2ABE_43A2_9D5D_E9988BACD70A__INCLUDED_)
#define AFX_CSDOPTIMIZER_H__587B5815_2ABE_43A2_9D5D_E9988BACD70A__INCLUDED_
#include "Item.h"
#include "BBSetMgr.h"
#include "DAGNode.h"
#include "DAGTable.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class SymbolTableMgr;
class QuadrupleMgr;
class Quadruple;
class Item;
class DAGNode;
class DAGTable;
class BBSetMgr;
using namespace std;

class CSDOptimizer  
{
public:
	SymbolTableMgr* SMgr;
	QuadrupleMgr* QMgr;
	
	BBSetMgr* bbs[200];
	int bbsNumber;
	
	int nodeNumber;
	int tableNumber;
	int queueNumber;
	int quadNumber;
	
	DAGNode* node[1000];
	DAGTable* table[1000];
	int queue[1000];
	Quadruple* quad[1000];
	int subquad;
	
public:
	CSDOptimizer(SymbolTableMgr* aSMgr, QuadrupleMgr* aQMgr, BBSetMgr* abbs[], int abbsNumber);
	~CSDOptimizer();
	int addNewNode(int op,int alc,int arc,Item* name);
	void Execute();
	void ExecuteOne(int astart, int aend, int aProcNumber);

	DAGNode* NewNode();
	Item* FindItem(int i);
	void CreateQueue();
	void DisplayQueue();
	void NewQuad(int aOpr, Item* aSrc1, Item* aSrc2, Item* aObj);
	void ClearAll();
	void Generate(int aProcNumber);
	void DisplayAllQuads();
	void Replace(int astart, int aend, int wstart, int wend);
	void changeNodeTable(Item* name,int nodei);
	int searchNodeTable(Item* name);
	void Establish(Quadruple* q);
	int searchNode(int opr,int lc,int rc);
};

#endif // !defined(AFX_CSDOPTIMIZER_H__587B5815_2ABE_43A2_9D5D_E9988BACD70A__INCLUDED_)
