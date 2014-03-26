// DAGNode.h: interface for the DAGNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAGNODE_H__3D52FD14_B579_428E_B6D4_E1A660A870D1__INCLUDED_)
#define AFX_DAGNODE_H__3D52FD14_B579_428E_B6D4_E1A660A870D1__INCLUDED_
#include <iostream>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class SymbolTableMgr;
class QuadrupleMgr;
class Item;
class SymbolTable;
using namespace std;

class DAGNode  
{
public:
	bool isleaf;
	Item* name;
	int lc,rc;
	bool inqueue;

	DAGNode* father[100];
	int fanumber;
	int opr;
	DAGNode* brother[1];
	DAGNode* child[2];
	DAGNode* replace[100];
	DAGNode* replaced[100];
	int isReplaced;
	int itype;
	SymbolTable* iParentTable;
	int brnumber;
	int cdnumber;
	int renumber;
	int rdnumber;

public:
	DAGNode();
	DAGNode(DAGNode* abrother, DAGNode* afather);
	virtual ~DAGNode();
	void CreateBrother(DAGNode* abrother, DAGNode* afather);
	void CreateChild(DAGNode* achild);
	void CreateReplace(DAGNode* achild);
	void CreateReplaced(DAGNode* achild);
	int FindFather(DAGNode* abrother,char aopr);
	void Display();

};

#endif // !defined(AFX_DAGNODE_H__3D52FD14_B579_428E_B6D4_E1A660A870D1__INCLUDED_)
