// CodeGenerator.h: interface for the CodeGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEGENERATOR_H__62251DC7_7885_4AD0_A521_207663E68AF5__INCLUDED_)
#define AFX_CODEGENERATOR_H__62251DC7_7885_4AD0_A521_207663E68AF5__INCLUDED_
#include "SymbolTableMgr.h"
#include "QuadrupleMgr.h"
#include "Item.h"
#include "define.h"
#include <fstream>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class SymbolTableMgr;
class QuadrupleMgr;
class Item;
using namespace std;

class CodeGenerator  
{
public:
	CodeGenerator(SymbolTableMgr* s,QuadrupleMgr* q,string path);
	virtual ~CodeGenerator();

	void Execute();
	void Gen(string a);
	void GenInitial(string include,string lib,string marco);
	Quadruple* GetQuad(int n);
	Quadruple* GetNextQuad();
	string ResolveItem(Item* i);
	void GenCon(Quadruple* i);
	void GenDim(Quadruple* i);
	void GenBegin(Quadruple* i);
	void GenRet(Quadruple* i);
	void GenNRet(Quadruple* i);
	void GenEnd(Quadruple* i);
	void GenCal(Quadruple* i);
	void GenRelCal(Quadruple* i);
	void GenGoto(Quadruple* i);
	void GenLabel(Quadruple* i);
	void GenReverse(Quadruple* i);
	void GenArgu(Quadruple* i);
	void GenCall(Quadruple* i);
	void GenWrite(Quadruple* i);
	void GenRead(Quadruple* i);
	void GenIdx(Quadruple* i);

public:
	SymbolTableMgr* SMgr;
	QuadrupleMgr* QMgr;
	int CurrentQuad;
	Quadruple* pCurrentQuad;
	int localLabelNumber;
	fstream fs;
};

#endif // !defined(AFX_CODEGENERATOR_H__62251DC7_7885_4AD0_A521_207663E68AF5__INCLUDED_)
