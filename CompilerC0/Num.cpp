// Num.cpp: implementation of the Num class.
//
//////////////////////////////////////////////////////////////////////
#include "Num.h"
#include "Tag.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Num::Num(std::string nn)
{
	std::stringstream temp("");
	temp<<nn;
	temp>>value;	
	tag=Tag.INTCON;
	width=0;
	num=true;
	word=false;
	type=false;
	lexeme=nn;
}

Num::Num(int v)
{
	tag=Tag.INTCON;
	value=v;
	std::stringstream temp("");
	temp<<value;
	temp>>lexeme;
	num=true;
	word=false;
	type=false;
}
Num::Num(){

}
Num::~Num()
{

}
std::string Num::toString(){
	return lexeme;
}
