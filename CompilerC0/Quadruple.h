// Quadruple.h: interface for the Quadruple class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUADRUPLE_H__3CD8ACF1_6796_4C2D_A85A_C79C30126980__INCLUDED_)
#define AFX_QUADRUPLE_H__3CD8ACF1_6796_4C2D_A85A_C79C30126980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class Item;
class Quadruple  
{
public:
	Quadruple(int aOpr,Item* aSrc1, Item* aSrc2,Item* aObj);
	virtual ~Quadruple();
public:
	int Opr;
	Item* Src1;
	Item* Src2;
	Item* Obj;
};

#endif // !defined(AFX_QUADRUPLE_H__3CD8ACF1_6796_4C2D_A85A_C79C30126980__INCLUDED_)
