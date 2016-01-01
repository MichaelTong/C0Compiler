// DSAOptimizer.cpp: implementation of the DSAOptimizer class.
//
//////////////////////////////////////////////////////////////////////

#include "DSAOptimizer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DSAOptimizer::DSAOptimizer(SymbolTableMgr* s,QuadrupleMgr* q)
{
	SMgr=s;
	QMgr=q;
	bbsNumber=0;
	for(int i=0;i<200;i++){
		bbs[i]=NULL;
	}
}

DSAOptimizer::~DSAOptimizer()
{
	for (int i = 0; i < bbsNumber; i ++){
		delete bbs[i];
		bbs[i] = NULL;
	}
}
void DSAOptimizer::Initial(){
	int a = 0;
	BBSetMgr* mb;
	for (int i = 0; i < QMgr -> QuadNumber; i ++){
		if (QMgr -> QuadList[i] -> Opr == BEGIN)
			a ++;
	}
	
	for (int i = 0; i < a; i ++){
		mb = NewBBSMgr(SMgr, QMgr, NULL);
		mb -> GetProcArea(i);
		mb -> DivideBlock();
		mb -> AnalyzeBlock();
	}
}

BBSetMgr* DSAOptimizer::NewBBSMgr(SymbolTableMgr* aSMgr, QuadrupleMgr* aQMgr, Item* aProc){
	bbs[bbsNumber] = new BBSetMgr(aSMgr, aQMgr, aProc);
	bbsNumber ++;
	return bbs[bbsNumber - 1];
}