// DAGTable.cpp: implementation of the DAGTable class.
//
//////////////////////////////////////////////////////////////////////

#include "DAGTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DAGTable::DAGTable(Item* aitem,DAGNode* anode){
	item=aitem;
	node=anode;
}

DAGTable::DAGTable(Item* aitem,int nodei){
	item=aitem;
	nodeNo=nodei;
}

DAGTable::DAGTable()
{

}

DAGTable::~DAGTable()
{

}

void DAGTable::Display(){
	cout<<" item:"<<item<<" item name:"<<item->lexeme<<" node:"<<node<<endl;
}