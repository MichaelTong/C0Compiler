// GRDOptimizer.cpp: implementation of the GRDOptimizer class.
//
//////////////////////////////////////////////////////////////////////

#include "GRDOptimizer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GRDOptimizer::GRDOptimizer(SymbolTableMgr* aSMgr, QuadrupleMgr* aQMgr, BBSetMgr* abbs[], int abbsNumber)
{
	SMgr = aSMgr;
	QMgr = aQMgr;
	quadNumber = 0;
	for (int j = 0; j < 200; j ++){
		regNumber[j] = 0;
		for (int i = 0; i < 500; i ++){
			regitem[j][i] = NULL;
			reg[j][i] = "";
		}
	}
	bbsNumber = abbsNumber;
	for (int i = 0; i < bbsNumber; i ++){
		bbs[i] = abbs[i];
	}
	ctNumber = abbsNumber;
	for (i = 0; i < bbsNumber; i ++){
		ct[i] = new ConflictTable(aSMgr, aQMgr);
	}
}

GRDOptimizer::~GRDOptimizer()
{
}

void GRDOptimizer::CreateConflict(){
	for (int i = 0; i < bbsNumber; i ++){
		bbs[i] -> Display();
		for (int j = 0; j < bbs[i] -> bbNumber; j ++){
			Item* c[100];
			int nc;
			bbs[i] -> bb[j]->Combine(c, nc);
			ct[i] -> AddSet(c, nc);
		}
	}
}

void GRDOptimizer::DisplayCT(){
	for (int i = 0; i < ctNumber; i ++){
		cout << "CT:" << i << endl;
		ct[i] -> Display();
	}
}

void GRDOptimizer::DisplayRegItem(){
	for (int i = 0; i < ctNumber; i ++){
		cout << "RegItem" << i << endl;
		for (int j = 0; j < regNumber[i]; j ++){
			cout << " item:" << regitem[i][j] << " reg:" << reg[i][j];
		}
		cout << endl << endl;
	}
}

void GRDOptimizer::AddRegItem(int n, Item* i){
	int itemHead;
	int itemEnd;
	if(i->arr==true)
		return;
	for (int j = bbs[n] -> procHead + 1; j < bbs[n] -> procEnd - 1; j ++){
		if (QMgr -> QuadList[j] -> Src1 == i || QMgr -> QuadList[j] -> Src2 == i || QMgr -> QuadList[j] -> Obj == i){
			itemHead = j;
			break;
		}
	}
	for (j = bbs[n] -> procEnd - 1; j > bbs[n] -> procHead + 1; j --){
		if (QMgr -> QuadList[j] -> Src1 == i || QMgr -> QuadList[j] -> Src2 == i || QMgr -> QuadList[j] -> Obj == i){
			itemEnd = j;
			break;
		}
	}
	for (j = itemHead; j <= itemEnd; j ++){
		if (QMgr -> QuadList[j] -> Opr == CALL || QMgr -> QuadList[j] -> Opr == ECALL)
			return;
	}
	
	for (j = bbs[n] -> procHead + 1; j < bbs[n] -> procEnd - 1; j ++){
		if (QMgr -> QuadList[j] -> Src1 == i || QMgr -> QuadList[j] -> Src2 == i || QMgr -> QuadList[j] -> Obj == i){
			if (QMgr -> QuadList[j] -> Opr == WR || QMgr -> QuadList[j] -> Opr == RD)
				return;
		}
	}
	
	regitem[n][regNumber[n]] = i;
	regNumber[n] ++;
}

void GRDOptimizer::RegisterSingleDistribute(int i){
	int flag1, flag2;
	do{
		flag2 = 0;
		do{
			flag1 = 0;
			Item* temp = ct[i] -> GetNode(4);
			if (temp != NULL){
				AddRegItem(i, temp);
				flag1 = 1;
			}
		}while (flag1);
		if (ct[i] -> DeleteNode() != -1){
			flag2 = 1;
		}
	}while (flag2);
	int regable[4]; // 0:ebx 1:esi 2:edi 3:ebp
	
	for (int j = regNumber[i] - 1; j >= 0; j --){
		for (int ii = 0; ii < 4; ii ++){
			regable[ii] = 1;
		}
		for (int k = j + 1; k < regNumber[i]; k ++){
			if (ct[i] -> Conflict(regitem[i][j], regitem[i][k]) == 1){
				if (reg[i][k] == "EBX")
					regable[0] = 0;
				else if (reg[i][k] == "ESI")
					regable[1] = 0;
				else if (reg[i][k] == "EDI")
					regable[2] = 0;
				else if (reg[i][k] == "EBP")
					regable[3] = 0;
			}
		}
		if (regable[0] == 1)
			reg[i][j] = "EBX";
		else if (regable[1] == 1)
			reg[i][j] = "ESI";
		else if (regable[2] == 1)
			reg[i][j] = "EDI";
		else if (regable[3] == 1)
			reg[i][j] = "EBP";
		else printf("GRDO error.\n");
	}
}

void GRDOptimizer::RegisterDistribute(){
	for (int i = 0; i < ctNumber; i ++){
		RegisterSingleDistribute(i);
	}
}

string GRDOptimizer::FindString(int n, Item* item){
	for (int k = 0; k < regNumber[n]; k ++){
		if (regitem[n][k] == item){
			return reg[n][k];
		}
	}
	return "";
}

void GRDOptimizer::Generate(){
	string temp;
	for (int i = 0; i < bbsNumber; i ++){
		for (int j = bbs[i] -> procHead + 1; j <= bbs[i] -> procEnd - 1; j ++){
			if (QMgr -> QuadList[j] -> Src1 != NULL && FindString(i, QMgr -> QuadList[j] -> Src1) != ""){
				temp = FindString(i, QMgr -> QuadList[j] -> Src1);
				if (temp == "EBX"){
					QMgr -> QuadList[j] -> Src1 = QMgr -> RItemList[0];
				}
				else if (temp == "ESI"){
					QMgr -> QuadList[j] -> Src1 = QMgr -> RItemList[1];
				}
				else if (temp == "EDI"){
					QMgr -> QuadList[j] -> Src1 = QMgr -> RItemList[2];
				}
				else if (temp == "EBP"){
					QMgr -> QuadList[j] -> Src1 = QMgr -> RItemList[3];
				}
			}
			if (QMgr -> QuadList[j] -> Src2 != NULL && FindString(i, QMgr -> QuadList[j] -> Src2) != ""){
				temp = FindString(i, QMgr -> QuadList[j] -> Src2);
				if (temp == "EBX"){
					QMgr -> QuadList[j] -> Src2 = QMgr -> RItemList[0];
				}
				else if (temp == "ESI"){
					QMgr -> QuadList[j] -> Src2 = QMgr -> RItemList[1];
				}
				else if (temp == "EDI"){
					QMgr -> QuadList[j] -> Src2 = QMgr -> RItemList[2];
				}
				else if (temp == "EBP"){
					QMgr -> QuadList[j] -> Src2 = QMgr -> RItemList[3];
				}
			}
			if (QMgr -> QuadList[j] -> Obj != NULL && FindString(i, QMgr -> QuadList[j] -> Obj) != ""){
				temp = FindString(i, QMgr -> QuadList[j] -> Obj);
				if (temp == "EBX"){
					QMgr -> QuadList[j] -> Obj = QMgr -> RItemList[0];
				}
				else if (temp == "ESI"){
					QMgr -> QuadList[j] -> Obj = QMgr -> RItemList[1];
				}
				else if (temp == "EDI"){
					QMgr -> QuadList[j] -> Obj = QMgr -> RItemList[2];
				}
				else if (temp == "EBP"){
					QMgr -> QuadList[j] -> Obj = QMgr -> RItemList[3];
				}
			}
		}
	}
}