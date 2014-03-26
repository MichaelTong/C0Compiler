// QuadrupleMgr.cpp: implementation of the QuadrupleMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "QuadrupleMgr.h"
#include "define.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QuadrupleMgr::QuadrupleMgr()
{
	QuadNumber = 0;
	ItemNumber = 0;
	for (int i = 0; i < 200; i ++){
		TItemNumberInProc[i] = 0;
	}
	TProcNumber = 0;
	CItemNumber = 0;
	LItemNumber = 0;
	SItemNumber = 0;
	RItemNumber = 4;
	RItemList[0] = new Item("@EBX", REG, _INT, "ebx", 0, NULL);
	RItemList[1] = new Item("@ESI", REG, _INT, "esi", 0, NULL);
	RItemList[2] = new Item("@EDI", REG, _INT, "edi", 0, NULL);
	RItemList[3] = new Item("@EBP", REG, _INT, "ebp", 0, NULL);

}

QuadrupleMgr::~QuadrupleMgr()
{
	for (int i = 0; i < QuadNumber; i ++){
		delete QuadList[i];
		QuadList[i] = NULL;
	}
	for (i = 0; i < ItemNumber; i ++){
		delete ItemList[i];
		ItemList[i] = NULL;
	}

}

Quadruple* QuadrupleMgr::NewQuad(int aOpr){
	QuadList[QuadNumber] = new Quadruple(aOpr, NULL, NULL, NULL);
	QuadNumber ++;
	return QuadList[QuadNumber - 1];
}

Quadruple* QuadrupleMgr::NewQuad(int aOpr, Item* aSrc1){
	QuadList[QuadNumber] = new Quadruple(aOpr, aSrc1, NULL, NULL);
	QuadNumber ++;
	return QuadList[QuadNumber - 1];
}

Quadruple* QuadrupleMgr::NewQuad(int aOpr, Item* aSrc1, Item* aObj){
	QuadList[QuadNumber] = new Quadruple(aOpr, aSrc1, NULL, aObj);
	QuadNumber ++;
	return QuadList[QuadNumber - 1];
}

Quadruple* QuadrupleMgr::NewQuad(int aOpr, Item* aSrc1, Item* aSrc2, Item* aObj){
	QuadList[QuadNumber] = new Quadruple(aOpr, aSrc1, aSrc2, aObj);
	QuadNumber ++;
	return QuadList[QuadNumber - 1];
}

Item* QuadrupleMgr::NewItem(int atype, SymbolTable* aParentTable){
	string temp = "t";
	char c[5];
	itoa(ItemNumber, c, 10);
	string s(c);
	temp += s;
	if (ItemNumber != 0 && ItemList[ItemNumber - 1] -> pParentTable == aParentTable){
		char c[5];
		itoa(TItemNumberInProc[TProcNumber] * 4 + 4, c, 16);
		string s(c);
		ItemList[ItemNumber] = new Item(temp, TEMP, atype, s, 0, aParentTable);
		TItemNumberInProc[TProcNumber] ++;
	}
	else{
		if (ItemNumber != 0)
			TProcNumber ++;
		char c[5];
		itoa(TItemNumberInProc[TProcNumber] * 4 + 4, c, 16);
		string s(c);
		ItemList[ItemNumber] = new Item(temp, TEMP, atype, s, 0, aParentTable);
		TItemNumberInProc[TProcNumber] ++;
	}
	ItemNumber ++;
	return ItemList[ItemNumber - 1];
}
Item* QuadrupleMgr::NewItem(int atype, SymbolTable* aParentTable, int aProcNumber){
	string temp = "tt";
	char c[5];
	itoa(ItemNumber, c, 10);
	string s(c);
	temp += s;

	itoa(TItemNumberInProc[aProcNumber] * 4 + 4, c, 16);
	string ss(c);
	ItemList[ItemNumber] = new Item(temp, TEMP, atype, ss, 0, aParentTable);
	TItemNumberInProc[aProcNumber] ++;

	ItemNumber ++;
	return ItemList[ItemNumber - 1];
}
void QuadrupleMgr::NewItemForInVar(Item* i){
	if (ItemNumber != 0 && ItemList[ItemNumber - 1] -> pParentTable == i ->pParentTable){
		char c[5];
		itoa(TItemNumberInProc[TProcNumber] * 4 + 4, c, 16);
		string s(c);
		i ->value = s;
		ItemList[ItemNumber] = i;
		TItemNumberInProc[TProcNumber] ++;
	}
	else{
		if (ItemNumber != 0)
			TProcNumber ++;
		char c[5];
		itoa(TItemNumberInProc[TProcNumber] * 4 + 4, c, 16);
		string s(c);
		i ->value = s;
		ItemList[ItemNumber] = i;
		TItemNumberInProc[TProcNumber] ++;
	}
	ItemNumber ++;
}
void QuadrupleMgr::NewItemForInVarArray(Item* i,int size){
	if (ItemNumber != 0 && ItemList[ItemNumber - 1] -> pParentTable == i ->pParentTable){
		char c[5];
		itoa(TItemNumberInProc[TProcNumber] * 4 + size*4, c, 16);
		string s(c);
		i ->value = s;
		ItemList[ItemNumber] = i;
		TItemNumberInProc[TProcNumber] +=size;
	}
	else{
		if (ItemNumber != 0)
			TProcNumber ++;
		char c[5];
		itoa(TItemNumberInProc[TProcNumber] * 4 + size*4, c, 16);
		string s(c);
		i ->value = s;
		ItemList[ItemNumber] = i;
		TItemNumberInProc[TProcNumber] +=size;
	}
	ItemNumber ++;
}


Item* QuadrupleMgr::NewCItem(int atype, string avalue){//const temp
	string temp = "c";
	char c[5];
	itoa(CItemNumber, c, 10);
	string s(c);
	temp += s;
	CItemList[CItemNumber] = new Item(temp, CONSTTEMP, atype, avalue, 0, NULL);
	CItemNumber ++;
	return CItemList[CItemNumber - 1];
}

Item* QuadrupleMgr::NewLItem(){//label
	string temp = "@l";
	char c[5];
	itoa(LItemNumber, c, 10);
	string s(c);
	temp += s;
	LItemList[LItemNumber] = new Item(temp, LAB, 0, temp, 0, NULL);
	LItemNumber ++;
	return LItemList[LItemNumber - 1];
}

Item* QuadrupleMgr::NewSItem(string avalue){
	string temp = "s";
	char c[5];
	itoa(SItemNumber, c, 10);
	string s(c);
	temp += s;
	SItemList[SItemNumber] = new Item(temp, CONSTTEMP, STRING, avalue, 0, NULL);
	SItemNumber ++;
	return SItemList[SItemNumber - 1];
}

Item* QuadrupleMgr::InsertQuad(int aOpr, Item* aSrc1, Item* aSrc2, int type, SymbolTable* aParentTable){
	Quadruple* myQuad;
	myQuad = NewQuad(aOpr, aSrc1, aSrc2, NULL);
	myQuad -> Obj = NewItem(type, aParentTable);
	return myQuad -> Obj;
}

string QuadrupleMgr::DisplayListName(Item* myItem){
	if (myItem != NULL)
		if (myItem ->kind == CONSTTEMP)
			return myItem -> value + "(" + myItem -> lexeme + ")";
		else if (myItem ->type == STRING)
			return myItem -> value + "(" + myItem -> lexeme + ")";
		else 
			return myItem -> lexeme;
	else return "NULL";
}

void QuadrupleMgr::DisplayAllQuads(){
	for (int i = 0; i < QuadNumber; i ++){
		cout << "Quadruple" << i << " Opr:" << DTS::Convert(QuadList[i] -> Opr) << " Src1:" << DisplayListName(QuadList[i] -> Src1) <<
			" Src2:" << DisplayListName(QuadList[i] -> Src2) << " Obj:" << DisplayListName(QuadList[i] -> Obj) << endl;
	}
}

void QuadrupleMgr::DisplayAllItems(){
	for (int i = 0; i < ItemNumber; i ++){
		cout << "Item" << i << " name:" << ItemList[i] -> lexeme << " kind:" << DTS::Convert(ItemList[i] -> kind) 
			<< " type:" << DTS::Convert(ItemList[i] -> type) << " value:" << ItemList[i] -> value  << " dimension:" << 
			ItemList[i] -> dimension << " pParentTable:" << ItemList[i] -> pParentTable << endl;
	}
}

int QuadrupleMgr::FindLItem(string aname){
	for (int i = 0; i < LItemNumber; i ++){
		if (LItemList[i] -> lexeme == aname){
			return i;
		}
	}
	return -1;
}

Quadruple* QuadrupleMgr::GetQuad(int n){
	if (n >= 0 && n < QuadNumber)
	return QuadList[n];
	else return NULL;
}