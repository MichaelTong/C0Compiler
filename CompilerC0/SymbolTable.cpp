// SymbolTable.cpp: implementation of the SymbolTable class.
//
//////////////////////////////////////////////////////////////////////

#include "SymbolTable.h"
#include "define.h"
#include <iostream>
#include "DTS.h"
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SymbolTable::SymbolTable()
{
	itemNumber=0;
	pParentItem=NULL;
}
SymbolTable::SymbolTable(Item* aParentItem){
	itemNumber = 0;
	pParentItem = aParentItem;
}
SymbolTable::~SymbolTable()
{
	for (int i = 0; i < itemNumber; i ++){
		if (itemList[i] ->kind != INVAR){
			delete itemList[i];
		}
		itemList[i] = NULL;
	}
}
void SymbolTable::error(){
	printf("Insert error! already exist.\n");
}

int SymbolTable::Find(string alexeme){
	for(int i=0;i<itemNumber;i++){
		if(alexeme==itemList[i]->lexeme)
			return i;
	}
	return -1;
}

int SymbolTable::AllCapsFind(string alexeme){
	for(int i=0;i<itemNumber;i++){
		if(alexeme==itemList[i]->lexeme)
			return i;
	}
	return -1;
}

int SymbolTable::Check(string alexeme){
	if(itemNumber==0)
		return -1;
	else{
		for(int i=0;i<itemNumber;i++)
			if(alexeme==itemList[i]->lexeme)
				return i;
			return -1;
	}
}
int SymbolTable::AllCapsCheck(string alexeme){
	if(itemNumber==0)
		return -1;
	else{
		for(int i=0;i<itemNumber;i++)
			if(alexeme==itemList[i]->lexeme)
				return i;
		return -1;
	}
}

void SymbolTable::Increase(){
	itemList[itemNumber] = newItem;
	itemNumber ++;
}

int SymbolTable::Insert(string alexeme, int akind, int atype, string avalue, int adimension){
	newItem = new Item(alexeme, akind, atype, avalue, adimension, this);
	Increase();
	return 0;
}

void SymbolTable::Display(int aItemNo){
	if (aItemNo >= 0 && aItemNo < itemNumber){
		cout << "Item" << aItemNo << " name:" << itemList[aItemNo] -> lexeme << " kind:" << DTS::Convert(itemList[aItemNo] -> kind) 
			<< " type:" << DTS::Convert(itemList[aItemNo] -> type) << " value:" << itemList[aItemNo] -> value  << " dimension:" << 
			itemList[aItemNo] -> dimension << " pParenttable:" << itemList[aItemNo] -> pParentTable << endl;
	}
	else cout << "Display Error." << endl;
}

void SymbolTable::DisplayAll(){
	for (int i = 0; i < itemNumber; i ++){
		if (pParentItem != NULL)
			cout << " ";
		Display(i);
		if (itemList[i] -> pChildTable != NULL)
			itemList[i] -> pChildTable -> DisplayAll();
	}
}