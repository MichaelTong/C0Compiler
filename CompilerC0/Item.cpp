// Item.cpp: implementation of the Item class.
//
//////////////////////////////////////////////////////////////////////

#include "Item.h"
#include "define.h"
#include "SymbolTable.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Item::Item()
{
}

Item::Item(string alexeme,int akind,int atype,string avalue,int adimension,SymbolTable* aParentTable)
{
	isAssigned=false;
	lexeme=alexeme;
	kind=akind;
	type=atype;
	value=avalue;
	addr=0;
	arr=false;
	dimension=adimension;
	pParentTable=aParentTable;
	if(kind==PROC||kind==FUNC)
		pChildTable=new SymbolTable(this);
	else
		pChildTable=NULL;
}

Item::~Item()
{
	if (pChildTable != NULL){
		delete pChildTable;
		pChildTable = NULL;
	}
}
