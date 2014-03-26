// ConflictTable.cpp: implementation of the ConflictTable class.
//
//////////////////////////////////////////////////////////////////////

#include "ConflictTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ConflictTable::ConflictTable(SymbolTableMgr* aSMgr,QuadrupleMgr* aQMgr)
{
	SMgr=aSMgr;
	QMgr=aQMgr;
	rowNumber=0;
	for(int i=0;i<100;i++){
		row[i]=NULL;
		finish[i]=0;
	}
	for(i=0;i<100;i++){
		for(int j=0;j<100;j++){
			table[i][j]=0;
		}
	}
}

ConflictTable::~ConflictTable()
{

}

int ConflictTable::FindItem(Item* item){
	for(int i=0;i<rowNumber;i++){
		if(row[i]==item)
			return i;
	}
	return -1;
}

void ConflictTable::AddItem(Item* item){
	if(FindItem(item)==-1){
		row[rowNumber]=item;
		rowNumber++;
	}
}

void ConflictTable::AddSet(Item* a[],int na){
	for(int i=0;i<na;i++){
		AddItem(a[i]);
	}
	for(i=0;i<na;i++){
		for(int j=0;j<na;j++){
			table[FindItem(a[i])][FindItem(a[j])]=1;
			table[FindItem(a[j])][FindItem(a[i])]=1;
		}
	}
}

void ConflictTable::Display(){
	for (int i = 0; i < rowNumber; i ++){
		cout << "Item:" << i << "\t";
		for (int j = 0; j < rowNumber; j ++){
			cout << table[i][j] << " ";
		}
		cout << endl;
	}
}

Item* ConflictTable::GetNode(int k){//得到连结数小于k的节点
	int cs;
	for(int i=0;i<rowNumber;i++){
		if(finish[i]==0){
			cs=0;
			for(int j=0;j<rowNumber;j++){
				if(table[i][j]==1&&finish[j]==0)
					cs++;
			}
			if(cs<k){
				finish[i]=1;
				return row[i];
			}
		}
	}
	return NULL;
}

int ConflictTable::DeleteNode(){
	for (int i = 0; i < rowNumber; i ++){
		if (finish[i] == 0){
			finish[i] = 1;
			return 0;
		}
	}
	return -1;
}

int ConflictTable::Conflict(Item* a, Item* b){
	return table[FindItem(a)][FindItem(b)];
}