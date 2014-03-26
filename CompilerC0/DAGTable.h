// DAGTable.h: interface for the DAGTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAGTABLE_H__ADED02E9_EB30_4C97_B93F_AD4774116166__INCLUDED_)
#define AFX_DAGTABLE_H__ADED02E9_EB30_4C97_B93F_AD4774116166__INCLUDED_
#include <IOSTREAM>
#include "Item.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class SymbolTableMgr;
class QuadrupleMgr;
class Item;
class DAGNode;
using namespace std;

class DAGTable  
{
public:
	Item* item;
	DAGNode* node;
	int nodeNo;

public:
	DAGTable(Item* aitem,DAGNode* anode);
	DAGTable(Item* aitem,int nodei);
	DAGTable();
	virtual ~DAGTable();
	void Display();

};

#endif // !defined(AFX_DAGTABLE_H__ADED02E9_EB30_4C97_B93F_AD4774116166__INCLUDED_)
