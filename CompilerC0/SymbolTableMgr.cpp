// SymbolTableMgr.cpp: implementation of the SymbolTableMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "SymbolTableMgr.h"
#include "SymbolTable.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SymbolTableMgr::SymbolTableMgr(){
	pCurrentTable = new SymbolTable();
	pTopTable = pCurrentTable;
	pCurrentItem = NULL;
}
SymbolTableMgr::~SymbolTableMgr(){
	delete pTopTable;
	pTopTable = NULL;
	pCurrentTable = NULL;
}

int SymbolTableMgr::Check(string aname){
	int res = pCurrentTable -> Check(aname);
	if (res != -1){
		pCurrentItem = pCurrentTable -> itemList[res];
		//printf("Found in the current module:%d.\n", res);
		return res;
	}
	else{
		if (pCurrentTable -> pParentItem != NULL){
			int res = pCurrentTable -> pParentItem -> pParentTable -> Check(aname);
			if (res != -1){
				pCurrentItem = pCurrentTable -> pParentItem -> pParentTable -> 
					itemList[res];
				//printf("Found in the father module:%d.\n", res);
				return res;
			}
			else{
				pCurrentItem = NULL;
				//printf("Not found.\n");
				return -1;
			}
		}
		else{
			pCurrentItem = NULL;
			//printf("Not found.\n");
			return -1;
		}
	}
}

int SymbolTableMgr::AllCapsCheck(string aname){
	if (IsKeyWord(aname)){
		//printf("The name conflicts with the key word.\n");
		return 0;
	}
	int res = pCurrentTable -> AllCapsCheck(aname);
	if (res != -1){
		pCurrentItem = pCurrentTable -> itemList[res];
		//printf("Found in the current module:%d.\n", res);
		return res;
	}
	else{
		if (pCurrentTable -> pParentItem != NULL){
			int res = pCurrentTable -> pParentItem -> pParentTable -> AllCapsCheck(aname);
			if (res != -1){
				pCurrentItem = pCurrentTable -> pParentItem -> pParentTable -> itemList[res];
				//printf("Found in the father module:%d.\n", res);
				return res;
			}
			else{
				pCurrentItem = NULL;
				//printf("Not found.\n");
				return -1;
			}
		}
		else{
			pCurrentItem = NULL;
			//printf("Not found.\n");
			return -1;
		}
	}
}

int SymbolTableMgr::LayerCheck(string aname){
	int res = pCurrentTable -> Check(aname);
	if (res != -1){
		pCurrentItem = pCurrentTable -> itemList[res];
		//printf("Found in the current module:%d.\n", res);
	}
	else 
		;//printf("Not found in the current module.\n");
	return res;
}

int SymbolTableMgr::AllCapsLayerCheck(string aname){
	if (IsKeyWord(aname)){
		//printf("The name conflicts with the key word.\n");
		return 0;
	}
	int res = pCurrentTable -> AllCapsCheck(aname);
	if (res != -1){
		pCurrentItem = pCurrentTable -> itemList[res];
		//printf("Found in the current module:%d.\n", res);
	}
	else 
		;//printf("Not found in the current module.\n");
	return res;
}

Item* SymbolTableMgr::Find(string aname){
	if (Check(aname) != -1)
		return pCurrentItem;
	else return NULL;
}

Item* SymbolTableMgr::AllCapsFind(string aname){
	if (AllCapsCheck(aname) != -1)
		return pCurrentItem;
	else return NULL;
}

Item* SymbolTableMgr::LayerFind(string aname){
	if (LayerCheck(aname) != -1)
		return pCurrentItem;
	else return NULL;
}

Item* SymbolTableMgr::AllCapsLayerFind(string aname){
	if (AllCapsLayerCheck(aname) != -1)
		return pCurrentItem;
	else return NULL;
}

int SymbolTableMgr::LayerFindNo(string aname){
	for (int i = 0; i < pCurrentTable -> itemNumber; i ++){
		if (pCurrentTable -> itemList[i] -> lexeme == aname){
			break;
		}
	}
	if (i == pCurrentTable -> itemNumber)
		return -1;
	else return i;
}

int SymbolTableMgr::AllCapsLayerFindNo(string aname){
	for (int i = 0; i < pCurrentTable -> itemNumber; i ++){
		if (pCurrentTable -> itemList[i] -> lexeme == aname){
			break;
		}
	}
	if (i == pCurrentTable -> itemNumber)
		return -1;
	else return i;
}

int SymbolTableMgr::IsKeyWord(string aname){
	if (aname == "const")
		return 1;
	else if (aname == "int")
		return 1;
	else if (aname == "char")
		return 1;
	else if (aname == "void")
		return 1;
	else if (aname == "main")
		return 1;
	else if (aname == "if")
		return 1;
	else if (aname == "else")
		return 1;
	else if (aname == "while")
		return 1;
	else if (aname == "for")
		return 1;
	else if (aname == "scanf")
		return 1;
	else if (aname == "printf")
		return 1;
	else if (aname == "return")
		return 1;
	else
		return 0;
}

void SymbolTableMgr::Insert(string aname, int akind, int atype, string avalue, int adimension){
	pCurrentTable -> Insert(aname, akind, atype, avalue, adimension);
}

int SymbolTableMgr::GetItemNumber(){
	return pCurrentTable -> itemNumber;
}

void SymbolTableMgr::StepIn(){
	if (pCurrentTable -> itemNumber == 0)
		;
		//printf("No item to step in.\n");
	else if (pCurrentTable -> itemList[pCurrentTable -> itemNumber - 1] ->pChildTable != NULL){
		pCurrentTable = pCurrentTable -> itemList[pCurrentTable -> itemNumber - 1] ->pChildTable;
	}
	else 
		;//printf("This item doesn't have a child table.\n");
}

void SymbolTableMgr::StepIn(string aname){
	if (pCurrentTable -> itemNumber == 0)
		;
		//printf("No item to step in.\n");
	else if (Find(aname) -> pChildTable != NULL){
		pCurrentTable = Find(aname) -> pChildTable;
	}
	else 
		;//printf("This item doesn't have a child table.\n");
}

void SymbolTableMgr::StepOut(){
	if (pCurrentTable -> pParentItem != NULL){
		pCurrentTable = pCurrentTable -> pParentItem -> pParentTable;
	}
	else 
		;//printf("Already top table.\n");
}

void SymbolTableMgr::DisplayAll(){
	pTopTable -> DisplayAll();
}
