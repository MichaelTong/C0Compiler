// QuadrupleMgr.h: interface for the QuadrupleMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUADRUPLEMGR_H__5603C4FA_BEEA_4F55_8109_94B769EBD755__INCLUDED_)
#define AFX_QUADRUPLEMGR_H__5603C4FA_BEEA_4F55_8109_94B769EBD755__INCLUDED_
#include "Quadruple.h"
#include "Item.h"
#include "SymbolTable.h"
#include "DTS.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
using namespace std;
class Item;
class SymbolTable;
class QuadrupleMgr  
{
public:
	QuadrupleMgr();
	virtual ~QuadrupleMgr();
	Quadruple* NewQuad(int aOpr);
	Quadruple* NewQuad(int aOpr, Item* aSrc1);
	Quadruple* NewQuad(int aOpr, Item* aSrc1, Item* aObj);
	Quadruple* NewQuad(int aOpr, Item* aSrc1, Item* aSrc2, Item* aObj);
	Item* NewItem(int atype, SymbolTable* aParentTable);
	Item* NewItem(int atype, SymbolTable* aParentTable, int aProcNumber);
	void NewItemForInVar(Item* i);
	void NewItemForInVarArray(Item* i,int size);
	Item* NewCItem(int atype, string avalue);
	Item* NewLItem();
	Item* NewSItem(string avalue);
	Item* InsertQuad(int aOpr, Item* aSrc1, Item* aSrc2, int type, SymbolTable* aParentTable);
	string DisplayListName(Item* myItem);
	void DisplayAllQuads();
	void DisplayAllItems();
	int FindLItem(string aname);
	Quadruple* GetQuad(int n);

public:
	Quadruple* QuadList[2000];
	int QuadNumber;
	Item* ItemList[500];
	int ItemNumber;
	int TItemNumberInProc[200];
	int TProcNumber;
	Item* CItemList[500];
	int CItemNumber;
	Item* LItemList[500];
	int LItemNumber;
	Item* SItemList[500];
	int SItemNumber;
	Item* RItemList[4];
	int RItemNumber;

};

#endif // !defined(AFX_QUADRUPLEMGR_H__5603C4FA_BEEA_4F55_8109_94B769EBD755__INCLUDED_)
