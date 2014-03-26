// Item.h: interface for the Item class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__28768797_4351_402F_86F9_12FAE95E79D5__INCLUDED_)
#define AFX_ITEM_H__28768797_4351_402F_86F9_12FAE95E79D5__INCLUDED_
#include <string>
#include "Token.h"
#include <vector>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
using namespace std;
class SymbolTable;
class Item  
{
public:
	int kind;
	int type;
	int dimension;
	std::string lexeme;
	std::string value;
	int addr;
	bool arr;
	SymbolTable* pChildTable;
	SymbolTable* pParentTable;
	bool isAssigned;

public:
	Item();
	Item(string alexeme,int akind,int atype,string avalue,int adimension,SymbolTable* aParentTable);//
	virtual ~Item();

};

#endif // !defined(AFX_ITEM_H__28768797_4351_402F_86F9_12FAE95E79D5__INCLUDED_)
