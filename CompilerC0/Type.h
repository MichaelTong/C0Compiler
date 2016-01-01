// Type::h: interface for the Type class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TYPE_H__F846D6E6_9DA3_40F9_AAEB_3E22A26EE0B6__INCLUDED_)
#define AFX_TYPE_H__F846D6E6_9DA3_40F9_AAEB_3E22A26EE0B6__INCLUDED_
#include "Word.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Type  : public Word
{
public:
	static const Type Int,Char,Void;
public:
	Type(std::string s, int t,int w);
	Type();
	virtual ~Type();

};

#endif // !defined(AFX_TYPE_H__F846D6E6_9DA3_40F9_AAEB_3E22A26EE0B6__INCLUDED_)
