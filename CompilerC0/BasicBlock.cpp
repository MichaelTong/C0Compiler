// BasicBlock.cpp: implementation of the BasicBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "BasicBlock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BasicBlock::BasicBlock(SymbolTableMgr* s,QuadrupleMgr* q,int astart,int aend)
{
	SMgr=s;
	QMgr=q;
	start=astart;
	end=aend;
	inNumber=0;
	outNumber=0;
	useNumber=0;
	defNumber = 0;
	preBBNumber = 0;
	nextBBNumber = 0;
	for (int i = 0; i < 100; i ++){
		in[i] = NULL;
		out[i] = NULL;
		use[i] = NULL;
		def[i] = NULL;
	}
	for (int i = 0; i < 10; i ++){
		preBB[i] = NULL;
		nextBB[i] = NULL;
	}
	Initial();
}

BasicBlock::~BasicBlock()
{

}
//Find an item in a[]
int BasicBlock::FindItem(Item* a[], int na, Item* item){
	for (int i = 0; i < na; i ++){
		if (a[i] == item)
			return i;
	}
	return -1;
}
//Add an exclusive item into a[]
void BasicBlock::AddItem(Item* a[], int& na, Item* item){
	if (FindItem(a, na, item) == -1){
		a[na] = item;
		na ++;
	}
}

void BasicBlock::AddDefUse(Item* a[], int& na, Item* b[], int nb, Item* item){
	if (item -> kind == TEMP || item -> kind == INVAR){
		if (FindItem(a, na, item) == -1 && FindItem(b, nb, item) == -1){
			a[na] = item;
			na ++;
		}
	}
}
//a[]+b[]=c[]
void BasicBlock::Union(Item* a[], int na, Item* b[], int nb, Item* c[], int& nc){
	for (int i = 0; i < na; i ++)
		c[i] = a[i];
	nc = na;
	for (int i = 0; i < nb; i ++){
		if (FindItem(c, nc, b[i]) == -1){
			c[nc] = b[i];
			nc ++;
		}
	}
}
//a[]-b[]=c[]
void BasicBlock::Minus(Item* a[], int na, Item* b[], int nb, Item* c[], int& nc){
	int k;
	for (int i = 0; i < na; i ++)
		c[i] = a[i];
	nc = na;
	for (int i = 0; i < nb; i ++){
		if (FindItem(c, nc, b[i]) != -1){
			k = FindItem(c, nc, b[i]);
			for (int j = k + 1; j < nc; j ++)
				c[j - 1] = c[j];
			c[nc - 1] = NULL;
			nc --;
		}
	}
}
//in+use+def=c[]
void BasicBlock::Combine(Item* c[], int& nc){
	Union(in, inNumber, use, useNumber, c, nc);
	Union(c, nc, def, defNumber, c, nc);
}

void BasicBlock::Initial(){
	for(int i=start;i<=end;i++){
		int Opr=QMgr->QuadList[i]->Opr;
		if(Opr==ADD||Opr==SUB||Opr==MUL||Opr==DIV||
			Opr==BT||Opr==LT||Opr==EQ||Opr==BET||Opr==LET||Opr==NEQ){
			AddDefUse(use,useNumber,def,defNumber,QMgr->QuadList[i]->Src1);
			AddDefUse(use,useNumber,def,defNumber,QMgr->QuadList[i]->Src2);
			AddDefUse(def,defNumber,use,useNumber,QMgr->QuadList[i]->Obj);
		}
		else if(Opr==ASN||Opr==REV){
			AddDefUse(use,useNumber,def,defNumber,QMgr->QuadList[i]->Src1);
			AddDefUse(def,defNumber,use,useNumber,QMgr->QuadList[i]->Obj);
		}
		else if(Opr==NRET||Opr==ARG){
			AddDefUse(use,useNumber,def,defNumber,QMgr->QuadList[i]->Src1);
		}
		else if(Opr==DINT||Opr==DCHR||Opr==RD||Opr==_IN){
			AddDefUse(def,defNumber,use,useNumber,QMgr->QuadList[i]->Src1);
		}
		else if(Opr==WR&&QMgr->QuadList[i]->Obj!=NULL){
			AddDefUse(def,defNumber,use,useNumber,QMgr->QuadList[i]->Obj);
		}
		else if(Opr==IDX){
			AddDefUse(use,useNumber,def,defNumber,QMgr->QuadList[i]->Src1);
			AddDefUse(use,useNumber,def,defNumber,QMgr->QuadList[i]->Src2);
			AddDefUse(def,defNumber,use,useNumber,QMgr->QuadList[i]->Obj);
		}
		else if(Opr==IDXASN){
			AddDefUse(use,useNumber,def,defNumber,QMgr->QuadList[i]->Src1);
			AddDefUse(use,useNumber,def,defNumber,QMgr->QuadList[i]->Src2);
			AddDefUse(def,defNumber,use,useNumber,QMgr->QuadList[i]->Obj);
		}
	}
}

void BasicBlock::AddPreBB(BasicBlock *b){
	preBB[preBBNumber]=b;
	preBBNumber++;
}

void BasicBlock::AddNextBB(BasicBlock * b){
	nextBB[nextBBNumber]=b;
	nextBBNumber++;
}

int BasicBlock::CalInOut(){
	Item* temp[2000];
	int tempNumber=0;
	Item* pre[2000];
	int preNumber=0;
	for (int i=0;i<inNumber;i++)
		pre[i]=in[i];
	preNumber=inNumber;
	for(int i=0;i<nextBBNumber;i++){
		Union(out,outNumber,nextBB[i]->in,nextBB[i]->inNumber,out,outNumber);
	}
	Minus(out,outNumber,def,defNumber,temp,tempNumber);
	Union(use,useNumber,temp,tempNumber,in,inNumber);
	if(preNumber==inNumber){
		for(int i=0;i<inNumber;i++){
			if(FindItem(pre,preNumber,in[i])==-1)
				return -1;
		}
		return 0;
	}
	else
		return -1;
}

void BasicBlock::Display(){
	cout << "start:" << start << " end:" << end << endl;
	cout << "use: ";
	for (int i = 0; i < useNumber - 1; i ++){
		cout << use[i] -> lexeme << ", ";
	}
	if (useNumber >= 1)
		cout << use[useNumber - 1] -> lexeme;
	cout << endl;
	
	cout << "def: ";
	for (int i = 0; i < defNumber - 1; i ++){
		cout << def[i] -> lexeme << ", ";
	}
	if (defNumber >= 1)
		cout << def[defNumber - 1] -> lexeme;
	cout << endl;
	
	cout << "in: ";
	for (int i = 0; i < inNumber - 1; i ++){
		cout << in[i] -> lexeme << ", ";
	}
	if (inNumber >= 1)
		cout << in[inNumber - 1] -> lexeme;
	cout << endl;
	
	cout << "out: ";
	for (int i = 0; i < outNumber - 1; i ++){
		cout << out[i] -> lexeme << ", ";
	}
	if (outNumber >= 1)
		cout << out[outNumber - 1] -> lexeme;
	cout << endl << endl;
}