// DTS.h: interface for the DTS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DTS_H__8E11518D_6151_4A27_A2E5_348DBE9DB0DC__INCLUDED_)
#define AFX_DTS_H__8E11518D_6151_4A27_A2E5_348DBE9DB0DC__INCLUDED_

#include <iostream>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

class DTS  
{
public:
	DTS();
	virtual ~DTS();
	static string Convert(int def);
	static int str2i(string s,int radix);
};

#endif // !defined(AFX_DTS_H__8E11518D_6151_4A27_A2E5_348DBE9DB0DC__INCLUDED_)
