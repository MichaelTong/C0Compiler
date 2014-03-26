// DTS.cpp: implementation of the DTS class.
//
//////////////////////////////////////////////////////////////////////

#include "DTS.h"
#include "define.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DTS::DTS()
{

}

DTS::~DTS()
{

}

string DTS::Convert(int def){
	if (def == ADD)
		return "ADD";
	if (def == SUB)
		return "SUB";
	if (def == MUL)
		return "MUL";
	if (def == DIV)
		return "DIV";
	if (def == BT)
		return "BT";
	if (def == LT)
		return "LT";
	if (def == EQ)
		return "EQ";
	if (def == BET)
		return "BET";
	if (def == LET)
		return "LET";
	if (def == NEQ)
		return "NEQ";
	if (def == ASN)
		return "ASN";
	if (def == EGTO)
		return "EGTO";
	if (def == RD)
		return "RD";
	if (def == WR)
		return "WR";
	if (def == LAB)
		return "LAB";
	if (def == GTO)
		return "GTO";
	if (def == BEGIN)
		return "BEGIN";
	if (def == ARG)
		return "ARG";
	if (def == CALL)
		return "CALL";
	if (def == RET)
		return "RET";
	if (def == NRET)
		return "NRET";
	if (def == END)
		return "END";
	if (def == DINT)
		return "DINT";
	if (def == DCHR)
		return "DCHR";
	if (def == REV)
		return "REV";
	if (def == _IN)
		return "IN";
	if (def == ECALL)
		return "ECALL";
	if (def == CST)
		return "CST";
	if (def == IDX)
		return "IDX";
	if (def == IDXASN)
		return "IDXASN";
	
	if (def == _CONST)
		return "CONST";
	if (def == VAR)
		return "VAR";
	if (def == TEMP)
		return "TEMP";
	if (def == PROC)
		return "PROC";
	if (def == FUNC)
		return "FUNC";
	if (def == CONSTTEMP)
		return "CONSTTEMP";
	if (def == INVAR)
		return "INVAR";
	if (def == REG)
		return "REG";
	
	
	if (def == _VOID)
		return "VOID";
	if (def == ERR)
		return "ERR";
	if (def == _INT)
		return "INT";
	if (def == _CHAR)
		return "CHAR";
	if (def == STRING)
		return "STRING";
	return "";
}
int DTS::str2i(string s,int radix){
	int out=0;
	for(int i=0;i<s.size();i++){
		int temp;
		char temps[1];
		temps[0]=s[i];
		temp=atoi(temps);
		out=out*radix+temp;
	}
	return out;
}