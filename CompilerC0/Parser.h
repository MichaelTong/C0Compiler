// Parser.h: interface for the Parser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSER_H__7C715CAB_CD29_474C_9580_FAC46FFB8091__INCLUDED_)
#define AFX_PARSER_H__7C715CAB_CD29_474C_9580_FAC46FFB8091__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Lexer.h"
#include "CompilerC0.h"
#include "SymbolTable.h"
#include "QuadrupleMgr.h"
#include "SymbolTableMgr.h"
#include <vector>
#include "Word.h"
#include <stdio.h>

using namespace std;

class Parser  
{
public:
	Parser();
	Parser(FILE *i,SymbolTableMgr* s,QuadrupleMgr* q);

	void reserve(Token w);
	int getFPos(FILE* fp, __int64* pose);
	int setFPos(FILE* fp, __int64* pose);
	bool checkKeyToken(string key);
	bool checkKeyTokenR(string key);
	bool checkId();
	bool checkIdR();

	void Chr(int& s);
	void Program();
	void ConstDec();
	void ConstDef();
	void VarDec();
	void VarDef();
	void ValuedFuncDef();
	void VoidFuncDef();
	void MainDef();
	void Parameters(string);
	void CompStat();
	void StatList();
	void ParaTable(string);
	void Stat();
	void Expr(Item*& i);
	void Term(Item*& i);
	void Factor(Item*& i);
	void Integer(int& s);
	bool checkNumR();
	void AssignStat();
	void CondStat();
	void Cond(Item*& i);
	void loopStat();
	void Step(Item*& i);
	void FuncCall(Item*& j);
	void ProcCall();
	void ArguTable(Item*& i);
	void ReadStat();
	void WriteStat();
	void ReturnStat();
	void Str(Item*& i);
	bool checkChrR();
	virtual ~Parser();

	void PrintNo();
	void getPostion(int cur,int& Ln,int& Col);
	void ConstDef_Insert(int t, string n, string c, int s);
	void VarDef_Insert(int t, string n);
	void VarDef_Insert_Array(int t, string n,int size);
	void VarDef_Alloc(int size,int t, string n,bool arr);
	void Expr_Add(Item* i1, Item* i2,int o, Item*& res);
	void Expr_Sign(Item* in, Item*& i);
	void Term_Mul(Item* i1, Item* i2,int o, Item*& res);
	void Factor_Find(string n, Item*& i);
	void Factor_Array_Find(Item* index,string n, Item*& i);
	void Factor_ConstTemp(string c, int s, Item*& i);
	void ParaTable_Insert(string n, int t, string s, int c);
	void AssignStat_Find(string n, Item*& i);
	void AssignStat_Assign(Item* i1, Item* i2);
	void AssignStat_IndexAssign(Item* i1,Item* index,Item* i2);
	void LogExpr_Rel(Item* i1, Item* i2, string o, Item*& res);
	void CtrlStat_ECBrf(Item* i,Item*& a);
	void CtrlStat_CBrf(Item*& b);
	void CtrlStat_Labprod(Item* a);
	void CtrlStat_Brf(Item* a);
	void CtrlStat_CLabprod(Item*& b);
	void LoopStat_Find(string n, Item*& i);
	void LoopStat_Assign(Item* i1, Item* i2);
	void LoopStat_Add(Item* i1, Item* istep, int o, Item* i2);
	void Step_ConstTemp(string n, Item*& i);
	void FuncDef_Begin(int t, string n, Item*& i);
	void FuncDef_Stepin(string n);
	void FuncDef_Pop(string n);
	void FuncDef_End();
	void FuncDef_Stepout();
	void ProcDef_Begin(string n, Item*& i);
	void ProcDef_Stepin(string n);
	void ProcDef_Pop(string n);
	void ProcDef_End();
	void ProcDef_Stepout();
	void FuncCall_Find(string n, Item*& i);
	void FuncCall_Call(Item* i, Item*& j);
	void ProcCall_Find(string n, Item*& i);
	void ProcCall_Call(Item* i);
	void ArguTable_Check(Item* i, Item* i1, int m);
	void ArguTable_ReversePush(Item* i, Item* item[], int n);
	void ReadStat_Find(string n, Item*& i);
	void ReadStat_Read(Item* i);
	void WriteStat_Write(Item* i);
	void WriteStat_Write(Item* i1, Item* i2);
	void RetStat_NRet(Item* i);
	void RetStat_Ret();
	void MainProc_Begin();
	void MainProc_Stepin();
	void MainProc_End();
	void MainProc_Stepout();
	void Str_Insert(string n, Item*& i);
int Stat_Check(string n);
public:
	FILE *in;
	Lexer lex;
	std::vector<Token> reserveWord;
	fpos_t pos, dpos, tPos;
	Token tempToken;
	int tempLineNo;
	int tempSerialNo;
	int lineNo;
	int serialNo;
	int errorcount;
	bool hasmain;
	bool end;
	bool funcend;
	SymbolTableMgr* SMgr;
	QuadrupleMgr* QMgr;
};

#endif // !defined(AFX_PARSER_H__7C715CAB_CD29_474C_9580_FAC46FFB8091__INCLUDED_)
