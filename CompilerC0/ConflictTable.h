// ConflictTable.h: interface for the ConflictTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFLICTTABLE_H__15C364FC_69A5_4D83_B3DD_8F998C9A3EEA__INCLUDED_)
#define AFX_CONFLICTTABLE_H__15C364FC_69A5_4D83_B3DD_8F998C9A3EEA__INCLUDED_
#include <IOSTREAM>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class SymbolTableMgr;
class QuadrupleMgr;
class Item;
using namespace std;

class ConflictTable  
{
public:
	SymbolTableMgr* SMgr;
	QuadrupleMgr* QMgr;
	Item* row[100];
	int rowNumber;
	int finish[100];
	int table[100][100];
public:
	ConflictTable(SymbolTableMgr* aSMgr,QuadrupleMgr* aQMgr);
	virtual ~ConflictTable();
	int FindItem(Item* item);
	void AddItem(Item* item);
	void AddSet(Item* a[],int na);
	void Display();
	Item* GetNode(int k);
	int DeleteNode();
	int Conflict(Item* a,Item* b);

};

#endif // !defined(AFX_CONFLICTTABLE_H__15C364FC_69A5_4D83_B3DD_8F998C9A3EEA__INCLUDED_)
