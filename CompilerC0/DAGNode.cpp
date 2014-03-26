// DAGNode.cpp: implementation of the DAGNode class.
//
//////////////////////////////////////////////////////////////////////

#include "DAGNode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DAGNode::DAGNode()
{
	brnumber=0;
	cdnumber=0;
	inqueue=0;
	renumber=0;
	rdnumber=0;
	isReplaced=0;
	fanumber = 0;
}

DAGNode::DAGNode(DAGNode* abrother, DAGNode* afather)
{
	opr=0;
	brother[0] = abrother;
	father[0] = afather;
	fanumber = 1;
	brnumber = 1;
	cdnumber = 0;
	inqueue = 0;
	renumber = 0;
	rdnumber = 0;
	isReplaced = 0;
}


DAGNode::~DAGNode()
{

}

void DAGNode::CreateBrother(DAGNode* abrother, DAGNode* afather){
	brother[brnumber] = abrother;
	father[fanumber] = afather;
	brnumber ++;
	fanumber ++;
}

void DAGNode::CreateChild(DAGNode* achild){
	child[cdnumber]=achild;
	cdnumber++;
}

void DAGNode::CreateReplace(DAGNode* achild){
	replace[renumber]=achild;
	renumber++;
}

void DAGNode::CreateReplaced(DAGNode* achild){
	replaced[rdnumber]=achild;
	rdnumber++;
}

int DAGNode::FindFather(DAGNode* abrother,char aopr){
	for(int i=0;i<fanumber;i++){
		if(father[i]->opr==aopr)
		{
			if(abrother==NULL)
				return i;
			for(int j=0;j<father[i]->cdnumber;j++){
				if(father[i]->child[j]==abrother)
					return i;
			}
		}
	}
	return -1;
}

void DAGNode::Display(){
	for(int i=0;i<brnumber;i++)
		cout<<" "<<i<<" brother:"<<brother[i]<<" father:"<<father[i]<<" opr"<<opr<<endl;
}

