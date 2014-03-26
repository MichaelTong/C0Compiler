// Type.cpp: implementation of the Type class.
//
//////////////////////////////////////////////////////////////////////

#include "Type.h"
#include "Tag.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Type::Type(std::string s,int t, int w)
{
	tag=t;
	value=0;
	num=false;
	word=true;
	type=true;
	lexeme=s;
	width=w;
}

Type::Type()
{
}

Type::~Type()
{

}

const Type Type::Int=Type("int",Tag.INTTK,8);
const Type Type::Char=Type("char",Tag.CHARTK,8);
const Type Type::Void=Type("void",Tag.VOIDTK,0);
