// CodeGenerator.cpp: implementation of the CodeGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "CodeGenerator.h"
#include "Item.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CodeGenerator::CodeGenerator(SymbolTableMgr* s,QuadrupleMgr* q,string path)
{
	SMgr=s;
	QMgr=q;
	CurrentQuad=0;
	localLabelNumber=0;
	fs.open((char*)path.c_str(),ios::out);
}

CodeGenerator::~CodeGenerator()
{

}

void CodeGenerator::Execute(){
	for(int i=0;i<QMgr->QuadNumber;i++){
		GetNextQuad();
		switch (pCurrentQuad->Opr)
		{
		case CST: GenCon(pCurrentQuad); break;
		case DINT: GenDim(pCurrentQuad); break;
		case DCHR: GenDim(pCurrentQuad); break;
		case BEGIN: GenBegin(pCurrentQuad); break;
		case RET: GenRet(pCurrentQuad); break;
		case NRET: GenNRet(pCurrentQuad); break;
		case END: GenEnd(pCurrentQuad); break;
		case ADD: GenCal(pCurrentQuad); break;
		case SUB: GenCal(pCurrentQuad); break;
		case MUL: GenCal(pCurrentQuad); break;
		case DIV: GenCal(pCurrentQuad); break;
		case ASN: GenCal(pCurrentQuad); break;
		case IDXASN: GenCal(pCurrentQuad); break;
		case BT: GenRelCal(pCurrentQuad); break;
		case LT: GenRelCal(pCurrentQuad); break;
		case EQ: GenRelCal(pCurrentQuad); break;
		case BET: GenRelCal(pCurrentQuad); break;
		case LET: GenRelCal(pCurrentQuad); break;
		case NEQ: GenRelCal(pCurrentQuad); break;
		case EGTO: GenGoto(pCurrentQuad); break;
		case GTO: GenGoto(pCurrentQuad); break;
		case LAB: GenLabel(pCurrentQuad); break;
		case REV: GenReverse(pCurrentQuad); break;
		case ARG: GenArgu(pCurrentQuad); break;
		case CALL: GenCall(pCurrentQuad); break;
		case ECALL: GenCall(pCurrentQuad); break;
		case WR: GenWrite(pCurrentQuad); break;
		case RD: GenRead(pCurrentQuad); break;
		case IDX: GenIdx(pCurrentQuad); break;
		default:break;
		}
	}
}
void CodeGenerator::Gen(string a){
	fs<<a;
}
void CodeGenerator::GenInitial(string include,string lib,string macro){
	fs << ".386\n" << ".model flat,stdcall\n" << "option casemap:none\n"
		<< "include\t" << include << "windows.inc\n"
		<< "include\t" << include << "kernel32.inc\n"
		<< "include\t" << include << "msvcrt.inc\n"
		<< "includelib\t" << lib << "msvcrt.lib\n"
		<< "includelib\t" << lib << "kernel32.lib\n"
		<< "include\t" << macro << "macros.asm\n"
		<<".data\n";
	for(int i=0;i<QMgr->QuadNumber;i++){
		if(QMgr->QuadList[i]->Opr==WR&&QMgr->QuadList[i]->Obj!=NULL)
			fs<<"_str"<<QMgr->QuadList[i]->Src1->lexeme<<"\tbyte\t'"<<QMgr->QuadList[i]->Src1->value<<"',0ah,0"<<endl;
		else if(QMgr->QuadList[i]->Opr==WR&&QMgr->QuadList[i]->Obj==NULL&&QMgr->QuadList[i]->Src1->type==STRING)
			fs<<"_str"<<QMgr->QuadList[i]->Src1->lexeme<<"\tbyte\t'"<<QMgr->QuadList[i]->Src1->value<<"',0ah,0"<<endl;
	}
}
Quadruple* CodeGenerator::GetQuad(int n){
	return QMgr->GetQuad(n);
}
Quadruple* CodeGenerator::GetNextQuad(){
	pCurrentQuad=QMgr->GetQuad(CurrentQuad);
	if(pCurrentQuad==NULL){
		CurrentQuad++;
		return NULL;
	}
	else{
		CurrentQuad++;
		return pCurrentQuad;
	}
}
string CodeGenerator::ResolveItem(Item* i){
	if(i->kind==TEMP){
		return "dword ptr [ebp-0"+i->value+"h]";
	}
	else if(i->kind==CONSTTEMP){
		if(i->type==_CHAR)
			return "'"+i->value+"'";
		else
			return i->value;
	}
	else if(i->kind==LAB){
		return i->value;
	}
	else if(i->kind==VAR){
		if(i->pParentTable->pParentItem==NULL)
			return "_"+i->lexeme;
		else
			return "dword ptr [ebp+0"+i->value+"h]";
	}
	else if(i->kind==INVAR){
		return "dword ptr [ebp-0"+i->value+"h]";
	}
	else if(i->kind==_CONST){
		if(i->type==_CHAR)
			return "'"+i->value+"'";
		else
			return i->value;
	}
	else if(i->kind==PROC||i->kind==FUNC){
		return i->lexeme;
	}
	else if(i->kind==REG)
		return i->value;
	else
		return "";

}
void CodeGenerator::GenCon(Quadruple* i){
	if(i->Src1->pParentTable->pParentItem==NULL){
		if(i->Src1->type==_CHAR)
			fs<<"_"<<i->Src1->lexeme<<"\tequ\t'"<<i->Src1->value<<"'"<<endl;
		else
			fs<<"_"<<i->Src1->lexeme<<"\tequ\t"<<i->Src1->value<<endl;
	}
	else
	{
		if(i->Src1->type==_CHAR)
			fs<<i->Src1->pParentTable->pParentItem->lexeme<<"_"<<i->Src1->lexeme<<"\tequ\t'"<<i->Src1->value<<"'"<<endl;
		else
			fs<<i->Src1->pParentTable->pParentItem->lexeme<<"_"<<i->Src1->lexeme<<"\tequ\t"<<i->Src1->value<<endl;
	}
}
void CodeGenerator::GenDim(Quadruple* i){
	int isoutside=1;
	for(int j=CurrentQuad-1;j>=0;j--){
		if(GetQuad(j)->Opr==BEGIN){
			isoutside=0;
			break;
		}
	}
	if(isoutside){
		fs<<"_"+i->Obj->lexeme<<"\tdword\t"<<i->Src1->value<<"\tdup(?)"<<endl;
	}
}
void CodeGenerator::GenBegin(Quadruple* i){
	int havebegan;
	havebegan=0;
	for(int j=CurrentQuad-2;j>=0;j--){
		if(GetQuad(j)->Opr==BEGIN){
			havebegan=1;
			break;
		}
	}
	if(havebegan==0)
		fs<<".code"<<endl;
	if(i->Src1->lexeme=="a@main"){
		fs<<"start:"<<endl;
		fs<<"\t"<<"push\tebp"<<endl;
		fs<<"\t"<<"mov\tebp,esp"<<endl;
		fs<<"\t"<<"sub\tesp,100h"<<endl;
	}
	else{
		fs<<"_"<<i->Src1->lexeme<<"\t"<<"proc"<<endl;
		fs<<"\t"<<"push\tebp"<<endl;
		fs<<"\t"<<"mov\tebp,esp"<<endl;
		fs<<"\t"<<"sub\tesp,100h"<<endl;
	}
}
void CodeGenerator::GenRet(Quadruple* i){
	int ismain;
	int j;
	for(j=CurrentQuad-1;j>=0;j--){
		if(GetQuad(j)->Opr==BEGIN){
			if(GetQuad(j)->Src1->lexeme=="a@main"){
				ismain=1;
				break;
			}
			else{
				ismain=0;
				break;
			}
		}
	}
	if(ismain){
		if(GetQuad(CurrentQuad-2)->Opr!=RET&&GetQuad(CurrentQuad-2)->Opr!=NRET){
			//string EndLabel="@label_main";
			fs<<"\t"<<"add\tesp,100h"<<endl;
			fs<<"\t"<<"pop\tebp"<<endl;
			fs<<"\t"<<"invoke\tExitProcess,NULL"<<endl;
			//fs<<"\t"<<"jmp\t"<<EndLabel<<endl;
		}
	}
	else{
		if(GetQuad(CurrentQuad-2)->Opr!=RET&&GetQuad(CurrentQuad-2)->Opr!=NRET){
			int k=GetQuad(j)->Src1->dimension;
// 			string EndLabel="@label_";
// 			EndLabel+=GetQuad(j)->Src1->lexeme;
			char c[5];
			itoa(k*4,c,16);
			string s(c);
			fs<<"\t"<<"add\tesp,100h"<<endl;
			fs<<"\t"<<"pop\tebp"<<endl;
			fs<<"\t"<<"ret\t0"<<s<<"h"<<endl;
/*			fs<<"\t"<<"jmp\t"<<EndLabel<<endl;*/
		}
	}
}
void CodeGenerator::GenNRet(Quadruple* i){
	int n;
	int j;
	for(j=CurrentQuad-1;j>=0;j--){
		if(GetQuad(j)->Opr==BEGIN){
			n=GetQuad(j)->Src1->dimension;
			break;
		}
	}
// 	string EndLabel="@label_";
// 	EndLabel+=GetQuad(j)->Src1->lexeme;
	char c[5];
	itoa(n*4,c,16);
	string s(c);
	fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
	fs<<"\t"<<"add\tesp,100h"<<endl;
	fs<<"\t"<<"pop\tebp"<<endl;
	fs<<"\t"<<"ret\t0"<<s<<"h"<<endl;
/*	fs<<"\t"<<"jmp\t"<<EndLabel<<endl;*/
}
void CodeGenerator::GenEnd(Quadruple* i){
	int ismain;
/*	string EndLabel="@label_";*/
	for(int j=CurrentQuad-1;j>=0;j--){
		if(GetQuad(j)->Opr==BEGIN){
			if(GetQuad(j)->Src1->lexeme=="a@main"){
				ismain=1;
/*				EndLabel+="main";*/
				break;
			}
			else{
				ismain=0;
/*				EndLabel+=GetQuad(j)->Src1->lexeme;*/
				break;
			}
		}
	}
	if(ismain){
/*		fs<<EndLabel<<":"<<endl;*/
		fs<<"end\tstart"<<endl;
	}
	else{
/*		fs<<EndLabel<<":"<<endl;*/
		fs<<"_"<<GetQuad(j)->Src1->lexeme<<"\t"<<"endp"<<endl;
	}
}
void CodeGenerator::GenCal(Quadruple* i){
	if(i->Opr==ADD){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"add\teax,"<<ResolveItem(i->Src2)<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",eax"<<endl;
	}
	else if(i->Opr==SUB){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"sub\teax,"<<ResolveItem(i->Src2)<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",eax"<<endl;
	}
	else if(i->Opr==MUL){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"mov\tecx,"<<ResolveItem(i->Src2)<<endl;
		fs<<"\t"<<"imul\tecx"<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",eax"<<endl;
	}
	else if(i->Opr==DIV){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"cdq"<<endl;
		fs<<"\t"<<"mov\tecx,"<<ResolveItem(i->Src2)<<endl;
		fs<<"\t"<<"idiv\tecx"<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",eax"<<endl;
	}
	if(i->Opr==ASN){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",eax"<<endl;
	}
	if(i->Opr==IDXASN){
		Item* Index;
		Item* value;
		Item* Obj;
		value=i->Src1;
		Index=i->Src2;
		Obj=i->Obj;
		string addr="";
		if(Obj->kind==INVAR){
			fs<<"\t"<<"mov\teax,"<<ResolveItem(Index)<<endl;
			fs<<"\t"<<"mov\tecx,4"<<endl;
			fs<<"\t"<<"imul\tecx"<<endl;
			fs<<"\t"<<"add\teax,ebp"<<endl;
			addr="dword ptr [eax-0"+Obj->value+"h]";
			fs<<"\t"<<"mov\tedx,"<<ResolveItem(value)<<endl;
			fs<<"\t"<<"mov\t"<<addr<<",edx"<<endl;
		}
		else if(Obj->kind==VAR){
			fs<<"\t"<<"mov\teax,"<<ResolveItem(Index)<<endl;
			fs<<"\t"<<"mov\tecx,4"<<endl;
			fs<<"\t"<<"imul\tecx"<<endl;
			fs<<"\t"<<"mov\tecx,"<<ResolveItem(value)<<endl;
			fs<<"\t"<<"mov\t_"<<Obj->lexeme<<"[eax],ecx"<<endl;
		}
	}
}
void CodeGenerator::GenRelCal(Quadruple* i){
	string label;
	string l1,l2;
	label="@label";
	char c[5];
	itoa(localLabelNumber,c,10);
	string s(c);
	label+=s;
	l1=label;
	l2=label;
	localLabelNumber++;
	l1+="_1";
	l2+="_2";
	if(i->Opr==BT){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"cmp\teax,"<<ResolveItem(i->Src2)<<endl;
		fs<<"\t"<<"jle\t"<<l1<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",1"<<endl;
		fs<<"\t"<<"jmp\t"<<l2<<endl;
		fs<<l1<<":"<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",0"<<endl;
		fs<<l2<<":"<<endl;
	}
	if(i->Opr==BET){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"cmp\teax,"<<ResolveItem(i->Src2)<<endl;
		fs<<"\t"<<"jl\t"<<l1<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",1"<<endl;
		fs<<"\t"<<"jmp\t"<<l2<<endl;
		fs<<l1<<":"<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",0"<<endl;
		fs<<l2<<":"<<endl;
	}
	if(i->Opr==LT){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"cmp\teax,"<<ResolveItem(i->Src2)<<endl;
		fs<<"\t"<<"jge\t"<<l1<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",1"<<endl;
		fs<<"\t"<<"jmp\t"<<l2<<endl;
		fs<<l1<<":"<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",0"<<endl;
		fs<<l2<<":"<<endl;
	}
	if(i->Opr==LET){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"cmp\teax,"<<ResolveItem(i->Src2)<<endl;
		fs<<"\t"<<"jg\t"<<l1<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",1"<<endl;
		fs<<"\t"<<"jmp\t"<<l2<<endl;
		fs<<l1<<":"<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",0"<<endl;
		fs<<l2<<":"<<endl;
	}
	if(i->Opr==EQ){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"cmp\teax,"<<ResolveItem(i->Src2)<<endl;
		fs<<"\t"<<"jne\t"<<l1<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",1"<<endl;
		fs<<"\t"<<"jmp\t"<<l2<<endl;
		fs<<l1<<":"<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",0"<<endl;
		fs<<l2<<":"<<endl;
	}
	if(i->Opr==NEQ){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"cmp\teax,"<<ResolveItem(i->Src2)<<endl;
		fs<<"\t"<<"je\t"<<l1<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",1"<<endl;
		fs<<"\t"<<"jmp\t"<<l2<<endl;
		fs<<l1<<":"<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",0"<<endl;
		fs<<l2<<":"<<endl;
	}
}
void CodeGenerator::GenGoto(Quadruple* i){
	if(i->Opr==GTO){
		fs<<"\t"<<"jmp\t"<<ResolveItem(i->Src1)<<endl;
	}
	else if(i->Opr==EGTO){
		fs<<"\t"<<"cmp\t"<<ResolveItem(i->Src1)<<",0"<<endl;
		fs<<"\t"<<"jz\t"<<ResolveItem(i->Obj)<<endl;
	}
}
void CodeGenerator::GenLabel(Quadruple* i){
	fs<<ResolveItem(i->Src1)<<":"<<endl;
}
void CodeGenerator::GenReverse(Quadruple* i){
	fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
	fs<<"\t"<<"neg\teax"<<endl;
	fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",eax"<<endl;
}
void CodeGenerator::GenArgu(Quadruple* i){
	fs<<"\t"<<"mov\teax,"<<ResolveItem(i->Src1)<<endl;
	fs<<"\t"<<"push\teax"<<endl;
}
void CodeGenerator::GenCall(Quadruple* i){
	if(i->Opr==CALL){
		fs<<"\t"<<"call\t"<<"_"<<ResolveItem(i->Src1)<<endl;
	}
	else if(i->Opr==ECALL){
		fs<<"\t"<<"call\t"<<"_"<<ResolveItem(i->Src1)<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(i->Obj)<<",eax"<<endl;
	}
}
void CodeGenerator::GenWrite(Quadruple* i){
	if(i->Obj!=NULL){
		fs<<"\t"<<"invoke\tcrt_printf,offset _str"<<i->Src1->lexeme<<endl;
		if (i -> Obj -> type == _INT)
			fs << "\t" << "invoke\tcrt_printf,SADD(\"%d\",0ah,0)," << ResolveItem(i -> Obj) << endl;
		else if (i -> Obj -> type == _CHAR)
			fs << "\t" << "invoke\tcrt_printf,SADD(\"%c\",0ah,0)," << ResolveItem(i -> Obj) << endl;
	}
	else if(i->Src1->type==STRING)
		fs<<"\t"<<"invoke\tcrt_printf,offset _str"<<i->Src1->lexeme<<endl;
	else if(i->Src1->type==_INT)
		fs << "\t" << "invoke\tcrt_printf,SADD(\"%d\",0ah,0)," << ResolveItem(i -> Src1) << endl;
	else if(i->Src1->type==_CHAR)
		fs<<"\t"<<"invoke\tcrt_printf,SADD(\"%c\",0ah,0)," << ResolveItem(i -> Src1) << endl;
}
void CodeGenerator::GenRead(Quadruple* i){
	if(i->Src1->type==_INT)
		fs<<"\t"<<"invoke\tcrt_scanf,SADD(\"%d\"),addr "<<ResolveItem(i->Src1)<<endl;
	else if(i->Src1->type==_CHAR)
		fs<<"\t"<<"invoke\tcrt_scanf,SADD(\"%c\"),addr "<<ResolveItem(i->Src1)<<endl;
}
void CodeGenerator::GenIdx(Quadruple* i){
	Item* Index;
	Item* Array;
	Item* Obj;
	Index=i->Src1;
	Array=i->Src2;
	Obj=i->Obj;
	string addr="";
	if(Array->kind==INVAR){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(Index)<<endl;
		fs<<"\t"<<"mov\tecx,4"<<endl;
		fs<<"\t"<<"imul\tecx"<<endl;
		fs<<"\t"<<"add\teax,ebp"<<endl;
		addr="dword ptr [eax-0"+Array->value+"h]";
		fs<<"\t"<<"mov\teax,"<<addr<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(Obj)<<",eax"<<endl;
	}
	else if(Array->kind==VAR){
		fs<<"\t"<<"mov\teax,"<<ResolveItem(Index)<<endl;
		fs<<"\t"<<"mov\tecx,4"<<endl;
		fs<<"\t"<<"imul\tecx"<<endl;
		fs<<"\t"<<"mov\teax,_"<<Array->lexeme<<"[eax]"<<endl;
		fs<<"\t"<<"mov\t"<<ResolveItem(Obj)<<",eax"<<endl;
	}
}
