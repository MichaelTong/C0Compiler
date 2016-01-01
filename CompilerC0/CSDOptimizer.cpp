// CSDOptimizer.cpp: implementation of the CSDOptimizer class.
//
//////////////////////////////////////////////////////////////////////

#include "CSDOptimizer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSDOptimizer::CSDOptimizer(SymbolTableMgr* aSMgr, QuadrupleMgr* aQMgr, BBSetMgr* abbs[], int abbsNumber){
	SMgr = aSMgr;
	QMgr = aQMgr;
	nodeNumber = 0;
	tableNumber = 0;
	queueNumber = 0;
	quadNumber = 0;
	subquad = 0;

	bbsNumber = abbsNumber;
	for (int i = 0; i < bbsNumber; i ++){
		bbs[i] = abbs[i];
	}

	for (int i = 0; i < 1000; i ++)
		node[i] = NULL;
	for (int i = 0; i < 1000; i ++)
		table[i] = NULL;
	for (int i = 0; i < 1000; i ++)
		queue[i] = NULL;
	for (int i = 0; i < 1000; i ++)
		quad[i] = NULL;
}

CSDOptimizer::~CSDOptimizer(){
}

void CSDOptimizer::Execute(){
	for (int i = 0; i < bbsNumber; i ++){
		for (int j = 0; j < bbs[i] -> bbNumber - 1; j ++){
			ExecuteOne(bbs[i] -> bb[j] -> start - subquad, bbs[i] -> bb[j] -> end - subquad, i);
		}
	}
}

void CSDOptimizer::ExecuteOne(int astart, int aend, int aProcNumber){
	for (int i = astart; i <= aend; i ++){
		Establish(QMgr -> QuadList[i]);
	}
	if (astart <= aend){
		CreateQueue();
		Generate(aProcNumber);
		Replace(0, quadNumber - 1, astart, aend);
	}
	ClearAll();
}

Item* CSDOptimizer::FindItem(int i){
	for (int j = 0; j < tableNumber; j ++){
		if (table[j] -> nodeNo == i)
			return table[j] -> item;
	}
	return NULL;
}

void CSDOptimizer::DisplayQueue(){
	for (int i = 0; i < queueNumber; i ++){
		cout << "Queue node:" << i << " " << queue[i] << endl;
	}
	cout << endl;
}

void CSDOptimizer::NewQuad(int aOpr, Item* aSrc1, Item* aSrc2, Item* aObj){
	quad[quadNumber] = new Quadruple(aOpr, aSrc1, aSrc2, aObj);
	quadNumber ++;
}

void CSDOptimizer::ClearAll(){
	for (int i = 0; i < nodeNumber; i ++)
		node[i] = NULL;
	nodeNumber = 0;
	for (int i = 0; i < tableNumber; i ++)
		table[i] = NULL;
	tableNumber = 0;
	for (int i = 0; i < queueNumber; i ++)
		queue[i] = -1;
	queueNumber = 0;
	for (int i = 0; i < quadNumber; i ++)
		quad[i] = NULL;
	quadNumber = 0;
	
}
void CSDOptimizer::Replace(int astart, int aend, int wstart, int wend){
	while (wstart < wend && (QMgr -> QuadList[wstart] -> Opr == DINT ||
		QMgr -> QuadList[wstart] -> Opr == DCHR || QMgr -> QuadList[wstart] -> Opr == _IN))
		wstart ++;
	int sub;
	if (wend - wstart >= aend - astart){
		sub = (wend - wstart) - (aend - astart);
		int j = astart;
		for (int i = wstart; i < wstart + aend - astart + 1 ; i ++){
			QMgr -> QuadList[i] = quad[j];
			j ++;
		}
		int k = wend;
		for (int i = wstart + aend - astart + 1; i < QMgr -> QuadNumber - sub; i++){
			QMgr -> QuadList[i] = QMgr -> QuadList[k + 1];
			k ++;
		}
		QMgr -> QuadNumber -= sub;
	}
	else printf("After CSDO the code length increased.\n");
	subquad += sub;
}

int CSDOptimizer::addNewNode(int opr,int alc,int arc,Item* name){
	if(opr==-1){
		node[nodeNumber]=new DAGNode();
		node[nodeNumber]->isleaf=true;
		node[nodeNumber]->name=name;

		node[nodeNumber]->lc=-1;
		node[nodeNumber]->rc=-1;
		node[nodeNumber]->opr=-1;
	}
	else{
		node[nodeNumber]=new DAGNode();
		node[nodeNumber]->isleaf=false;
		node[nodeNumber]->name=name;
		
		node[nodeNumber]->lc=alc;
		node[nodeNumber]->rc=arc;
		node[nodeNumber]->opr=opr;
		if(opr==IDXASN)
			node[nodeNumber]->name=name;
	}
	node[nodeNumber]->inqueue=false;
	nodeNumber++;
	return nodeNumber-1;
}

void CSDOptimizer::changeNodeTable(Item* name,int nodei){
	int t;
	t=searchNodeTable(name);
	if(t==-1){
		table[tableNumber]=new DAGTable(name,nodei);
		tableNumber++;
	}
	else{
		for(int i=0;i<tableNumber;i++){
			if(table[i]->item==name){
				table[i]->nodeNo=nodei;
				break;
			}
		}
	}
}

int CSDOptimizer::searchNodeTable(Item* name){
	if(name==NULL)
		return -1;
	for(int i=0;i<tableNumber;i++){
		if(table[i]->item==name)
				return table[i]->nodeNo;		
	}
	return -1;
}

void CSDOptimizer::Establish(Quadruple* q){
	if(q->Opr==ASN){
		int temp=searchNodeTable(q->Src1);
		if(temp==-1){
			temp=addNewNode(-1,-1,-1,q->Src1);
			changeNodeTable(q->Src1,temp);
		}
		changeNodeTable(q->Obj,temp);
	}
	else if(q->Opr==IDXASN){
		int temp=searchNodeTable(q->Src1);
		if(temp==-1){
			temp=addNewNode(-1,-1,-1,q->Src1);
			changeNodeTable(q->Src1,temp);
		}
		int t1=searchNodeTable(q->Src2);
		if(t1==-1){
			t1=addNewNode(-1,-1,-1,q->Src2);
			changeNodeTable(q->Src2,t1);
		}
		int tt=addNewNode(q->Opr,t1,temp,q->Obj);
		changeNodeTable(q->Obj,tt);
	}
	else if(q->Opr==ADD||q->Opr==SUB||q->Opr==MUL||q->Opr==DIV){
		int temp=searchNodeTable(q->Src1);
		if(temp==-1){
			temp=addNewNode(-1,-1,-1,q->Src1);
			changeNodeTable(q->Src1,temp);
		}
		int t1=searchNodeTable(q->Src2);
		if(t1==-1){
			t1=addNewNode(-1,-1,-1,q->Src2);
			changeNodeTable(q->Src2,t1);
		}
		int tt=searchNode(q->Opr,temp,t1);
		if(tt==-1)
			tt=addNewNode(q->Opr,temp,t1,NULL);
		changeNodeTable(q->Obj,tt);
	}
	else if(q->Opr==IDX){
		int temp=searchNodeTable(q->Src1);
		if(temp==-1){
			temp=addNewNode(-1,-1,-1,q->Src1);
			changeNodeTable(q->Src1,temp);
		}
		int t1=searchNodeTable(q->Src2);
		if(t1==-1){
			t1=addNewNode(-1,-1,-1,q->Src2);
			changeNodeTable(q->Src2,t1);
		}
		int tt=searchNode(q->Opr,temp,t1);
		if(tt==-1)
			tt=addNewNode(q->Opr,temp,t1,NULL);
		changeNodeTable(q->Obj,tt);
	}
}
int CSDOptimizer::searchNode(int opr,int lc,int rc){
	for(int i=0;i<nodeNumber;i++){
		if(node[i]->opr==opr&&node[i]->lc==lc&&node[i]->rc==rc)
			return i;
	}
	return -1;
}

void CSDOptimizer::CreateQueue(){
	bool hasparent[1000];
	int i;
	while(queueNumber<nodeNumber){
		for(i=0;i<nodeNumber;i++)
			hasparent[i]=false;
		for(i=0;i<nodeNumber;i++){
			if(node[i]->inqueue==false&&node[i]->isleaf==false){
				hasparent[node[i]->lc]=true;
				hasparent[node[i]->rc]=true;
			}
		}
		for(i=nodeNumber-1;i>=0;i--){
			if(!node[i]->inqueue&&!hasparent[i]){
				int j;
				j=i;
				while(true){
					queue[queueNumber]=j;
					node[j]->inqueue=true;
					queueNumber++;
					
					if(node[j]->isleaf)
						break;

					int lc=node[j]->lc;
					int rc=node[j]->rc;
					hasparent[lc]=false;
					hasparent[rc]=false;
					int k;
					for(k=0;k<nodeNumber;k++){
						if(node[k]->inqueue==false&&node[k]->isleaf==false){
							if(node[k]->lc==lc||node[k]->rc==lc){
								hasparent[lc]=true;
							}
							if(node[k]->lc==rc||node[k]->rc==rc){
								hasparent[rc]=true;
							}
						}
					}
					
					j=node[j]->lc;
					if(hasparent[j])
						break;
				}
				break;
			}
		}
	}
// 	for(int k=0;k<queueNumber;k++)
// 	{
// 		for(int h=k+1;h<queueNumber;h++)
// 		{
// 			if((node[queue[k]]->opr==IDX||node[queue[k]]->opr==IDXASN)&&(node[queue[h]]->opr==IDX||node[queue[h]]->opr==IDXASN))
// 			{
// 				for(int l=k+1;l<h;l++)
// 				{
// 					if(node[queue[l]]->opr!=-1)
// 						break;
// 				}
// 				if(l==h&&queue[k]>queue[h])
// 				{
// 					int temp=queue[k];
// 					queue[k]=queue[h];
// 					queue[h]=temp;
// 				}
// 			}
// 		}
// 	}
}

void CSDOptimizer::Generate(int aProcNumber){
	int listi;
	int i;
	for(listi=nodeNumber-1;listi>=0;listi--){
		i=queue[listi];
		if(node[i]->isleaf){
			Item* temp=FindItem(i);
			for(int j=0;j<tableNumber;j++)
				if(table[j]->nodeNo==i){
					if(table[j]->item!=temp)
						NewQuad(ASN,temp,NULL,table[j]->item);
				}
			continue;
		}
		int il,ir;
		il=node[i]->lc;
		ir=node[i]->rc;
		Item *Src1;Item *Src2;Item *Obj;
		if(node[il]->isleaf){
			Src1=node[il]->name;
		}
		else{
			Src1=FindItem(il);
		}
		if(node[ir]->isleaf||node[i]->opr==IDX){
			Src2=node[ir]->name;
		}
		else{
			Src2=FindItem(ir);
		}
		if(node[i]->opr==IDXASN){
			Obj=node[i]->name;
			NewQuad(node[i]->opr,Src2,Src1,Obj);
		}
		else{
			int caled=0;
			for(int j=0;j<tableNumber;j++){
				if(table[j]->nodeNo==i){
					Obj=table[j]->item;
					if(caled==0){
						NewQuad(node[i]->opr,Src1,Src2,Obj);
						caled=1;
					}
					else
						NewQuad(ASN,FindItem(i),NULL,Obj);
				}
			}
		}
	}
}