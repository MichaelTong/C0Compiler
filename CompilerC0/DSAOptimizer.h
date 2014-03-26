// DSAOptimizer.h: interface for the DSAOptimizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSAOPTIMIZER_H__04AAD82D_1F26_4B63_B06C_D837EAE57D33__INCLUDED_)
#define AFX_DSAOPTIMIZER_H__04AAD82D_1F26_4B63_B06C_D837EAE57D33__INCLUDED_
#include "QuadrupleMgr.h"
#include "BBSetMgr.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class SymbolTableMgr;
class QuadrupleMgr;
class BBSetMgr;
class Item;
using namespace std;
class DSAOptimizer  
{
public:
	SymbolTableMgr* SMgr;
	QuadrupleMgr* QMgr;
	BBSetMgr* bbs[200];
	int bbsNumber;
public:
	DSAOptimizer(SymbolTableMgr* s,QuadrupleMgr* q);
	virtual ~DSAOptimizer();
	void Initial();
	BBSetMgr* NewBBSMgr(SymbolTableMgr* aSMgr,QuadrupleMgr*aQMgr,Item* aProc);

};

#endif // !defined(AFX_DSAOPTIMIZER_H__04AAD82D_1F26_4B63_B06C_D837EAE57D33__INCLUDED_)
