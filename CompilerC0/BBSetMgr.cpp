// BBSetMgr.cpp: implementation of the BBSetMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "BBSetMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BBSetMgr::BBSetMgr(SymbolTableMgr* aSMgr, QuadrupleMgr* aQMgr, Item* aProc){
	SMgr = aSMgr;
	QMgr = aQMgr;
	Proc = aProc;
	procHead = 0;
	procEnd = 0;
	bbNumber = 0;
	for (int i = 0; i < 200; i ++){
		bb[i] = NULL;
	}
}

BBSetMgr::~BBSetMgr()
{

}
void BBSetMgr::GetProcArea(int n){
	int time=0;
	for(int i=0;i<QMgr->QuadNumber;i++){
		if(QMgr->QuadList[i]->Opr==BEGIN){
			if(time==n){
				procHead=i;
				for(int j=i+1;j<QMgr->QuadNumber;j++){
					if(QMgr->QuadList[j]->Opr==END){
						procEnd=j;
						break;
					}
				}
			}
			time++;
		}
	}
}

BasicBlock* BBSetMgr::NewBlock(SymbolTableMgr* aSMgr,QuadrupleMgr* aQMgr,int astart,int aend){
	bb[bbNumber]=new BasicBlock(aSMgr,aQMgr,astart,aend);
	bbNumber++;
	return bb[bbNumber-1];
}
void BBSetMgr::DivideBlock(){
	int start[40];
	int end[40];
	int avail[40];
	for (int i = 0; i < 40; i ++){
		avail[i] = 1;
	}
	int startNumber = 0;
	for (int i = procHead + 1; i <= procEnd - 1; i ++){
		if (i == procHead + 1){
			start[startNumber] = i;
			startNumber ++;
		}
		else if (QMgr -> QuadList[i] -> Opr == EGTO || QMgr -> QuadList[i] -> Opr == GTO){
			start[startNumber] = i + 1;
			startNumber ++;
		}
		else if (QMgr -> QuadList[i] -> Opr == LAB){
			start[startNumber] = i + 1;
			startNumber ++;
		}
		else if (QMgr -> QuadList[i] -> Opr == RET || QMgr -> QuadList[i] -> Opr == NRET){
			if ( i + 1 <= procEnd - 1){
				start[startNumber] = i + 1;
				startNumber ++;
			}
		}
		else if (QMgr -> QuadList[i] -> Opr == ARG || QMgr -> QuadList[i] -> Opr == CALL
			|| QMgr -> QuadList[i] -> Opr == ECALL || QMgr -> QuadList[i] -> Opr == RD
			|| QMgr -> QuadList[i] -> Opr == WR){
			start[startNumber] = i + 1;
				startNumber ++;
		}
		else if (QMgr -> QuadList[i] -> Opr == BT || QMgr -> QuadList[i] -> Opr == LT
			|| QMgr -> QuadList[i] -> Opr == EQ || QMgr -> QuadList[i] -> Opr == BET
			|| QMgr -> QuadList[i] -> Opr == LET || QMgr -> QuadList[i] -> Opr == NEQ){
			start[startNumber] = i + 1;
				startNumber ++;
		}
	}
	for (int i = 0; i < startNumber - 1; i ++)
		end[i] = start[i + 1] - 2;
	end[startNumber - 1] = procEnd - 1;
	for (int i = 0; i < startNumber; i ++){
		while (start[i] <= end[i] && (QMgr -> QuadList[start[i]] -> Opr == EGTO || QMgr -> QuadList[start[i]] -> Opr == GTO
			|| QMgr -> QuadList[start[i]] -> Opr == LAB || QMgr -> QuadList[start[i]] -> Opr == RET || QMgr -> QuadList[start[i]] -> Opr == NRET
			|| QMgr -> QuadList[start[i]] -> Opr == ARG || QMgr -> QuadList[start[i]] -> Opr == CALL|| QMgr -> QuadList[start[i]] -> Opr == ECALL
			|| QMgr -> QuadList[start[i]] -> Opr == BT || QMgr -> QuadList[start[i]] -> Opr == LT
			|| QMgr -> QuadList[start[i]] -> Opr == EQ || QMgr -> QuadList[start[i]] -> Opr == BET
			|| QMgr -> QuadList[start[i]] -> Opr == LET || QMgr -> QuadList[start[i]] -> Opr == NEQ
			|| QMgr -> QuadList[start[i]] -> Opr == RD || QMgr -> QuadList[start[i]] -> Opr == WR
			)){
			start[i] ++;
		}
		if (start[i] > end[i])
			avail[i] = 0;
		while (start[i] <= end[i] && (QMgr -> QuadList[end[i]] -> Opr == EGTO || QMgr -> QuadList[end[i]] -> Opr == GTO
			|| QMgr -> QuadList[end[i]] -> Opr == LAB || QMgr -> QuadList[end[i]] -> Opr == RET || QMgr -> QuadList[end[i]] -> Opr == NRET
			|| QMgr -> QuadList[end[i]] -> Opr == ARG || QMgr -> QuadList[end[i]] -> Opr == CALL|| QMgr -> QuadList[end[i]] -> Opr == ECALL
			|| QMgr -> QuadList[end[i]] -> Opr == BT || QMgr -> QuadList[end[i]] -> Opr == LT
			|| QMgr -> QuadList[end[i]] -> Opr == EQ || QMgr -> QuadList[end[i]] -> Opr == BET
			|| QMgr -> QuadList[end[i]] -> Opr == LET || QMgr -> QuadList[end[i]] -> Opr == NEQ)
			|| QMgr -> QuadList[end[i]] -> Opr == RD || QMgr -> QuadList[end[i]] -> Opr == WR
			){
			end[i] --;
		}
		if (start[i] > end[i])
			avail[i] = 0;
	}
	for (int i = 0; i < startNumber; i ++){
		if (avail[i])
			NewBlock(SMgr, QMgr, start[i], end[i]);
	}
	NewBlock(SMgr, QMgr, 0, 0);

	for (int i = procHead + 1; i <= procEnd - 1; i ++){
		if (QMgr -> QuadList[i] -> Opr == GTO){
			Item* label = QMgr -> QuadList[i] -> Src1;
			for (int j = procHead + 1; j <= procEnd - 1; j ++){
				if (QMgr -> QuadList[j] -> Opr == LAB && QMgr -> QuadList[j] -> Src1 == label){
					if (FindBlockFromEnd(i - 1) != NULL && FindBlockFromEnd(j + 1) != NULL){
						FindBlockFromEnd(i - 1) -> AddNextBB(FindBlockFromStart(j + 1));
						FindBlockFromStart(j + 1) -> AddPreBB(FindBlockFromEnd(i - 1));
					}
				}
			}
		}
		else if (QMgr -> QuadList[i] -> Opr == EGTO){
			if (FindBlockFromEnd(i - 1) != NULL && FindBlockFromEnd(i + 1) != NULL){
				FindBlockFromEnd(i - 1) -> AddNextBB(FindBlockFromStart(i + 1));
				FindBlockFromStart(i + 1) -> AddPreBB(FindBlockFromEnd(i - 1));
			}
			Item* label = QMgr -> QuadList[i] -> Obj;
			for (int j = procHead + 1; j <= procEnd - 1; j ++){
				if (QMgr -> QuadList[j] -> Opr == LAB && QMgr -> QuadList[j] -> Src1 == label){
					if (FindBlockFromEnd(i - 1) != NULL && FindBlockFromEnd(j + 1) != NULL){
						FindBlockFromEnd(i - 1) -> AddNextBB(FindBlockFromStart(j + 1));
						FindBlockFromStart(j + 1) -> AddPreBB(FindBlockFromEnd(i - 1));
					}
				}
			}
		}
		else if (QMgr -> QuadList[i] -> Opr == LAB){
			if (FindBlockFromEnd(i - 1) != NULL && FindBlockFromEnd(i + 1) != NULL){
				FindBlockFromEnd(i - 1) -> AddNextBB(FindBlockFromStart(i + 1));
				FindBlockFromStart(i + 1) -> AddPreBB(FindBlockFromEnd(i - 1));
			}
		}
	}
	bb[bbNumber - 2] -> AddNextBB(bb[bbNumber - 1]);
	bb[bbNumber - 1] -> AddPreBB(bb[bbNumber - 2]);
}

BasicBlock* BBSetMgr::FindBlockFromStart(int n){
	for(int i=0;i<bbNumber;i++){
		if(bb[i]->start==n)
			return bb[i];
	}
	return NULL;
}

BasicBlock* BBSetMgr::FindBlockFromEnd(int n){
	for (int i = 0; i < bbNumber; i ++){
		if (bb[i] -> end == n)
			return bb[i];
	}
	return NULL;
}

void BBSetMgr::AnalyzeBlock(){
	int finish;
	do{
		finish = 1;
		for (int i = bbNumber - 1; i >= 0; i --){
			if (bb[i] -> CalInOut() == -1)
				finish = 0;
		}
	}
	while (finish == 0);
	
}

void BBSetMgr::Display(){
	for (int i = 0; i < bbNumber; i ++){
		bb[i] -> Display();
	}
}