// BasicBlock.h: interface for the BasicBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICBLOCK_H__BE915355_A482_4F19_BB4B_AD3F0F563D27__INCLUDED_)
#define AFX_BASICBLOCK_H__BE915355_A482_4F19_BB4B_AD3F0F563D27__INCLUDED_
#include "CompilerC0.h"
#include "define.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class SymbolTableMgr;
class QuadrupleMgr;
class Item;

class BasicBlock  
{
public:
	BasicBlock(SymbolTableMgr* s,QuadrupleMgr* q,int astart,int aend);
	virtual ~BasicBlock();
	int FindItem(Item* a[], int na, Item* item);
	void AddItem(Item* a[], int& na, Item* item);
	void AddDefUse(Item* a[], int& na, Item* b[], int nb, Item* item);
	void Union(Item* a[], int na, Item* b[], int nb, Item* c[], int& nc);
	void Minus(Item* a[], int na, Item* b[], int nb, Item* c[], int& nc);
	void Combine(Item* c[], int& nc);
	void DisplaySet(Item* a[], int na);
	void Initial();
	void Display();
	void AddPreBB(BasicBlock * b);
	void AddNextBB(BasicBlock * b);
	int CalInOut();
	

public:
	SymbolTableMgr* SMgr;
	QuadrupleMgr* QMgr;
	int start;
	int end;
	Item* in[100];
	Item* out[100];
	Item* use[100];
	Item* def[100];
	int inNumber;
	int outNumber;
	int useNumber;
	int defNumber;
	BasicBlock* preBB[10];
	BasicBlock* nextBB[10];
	int preBBNumber;
	int nextBBNumber;

};

#endif // !defined(AFX_BASICBLOCK_H__BE915355_A482_4F19_BB4B_AD3F0F563D27__INCLUDED_)
