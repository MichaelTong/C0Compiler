// Parser.cpp: implementation of the Parser class.
//
//////////////////////////////////////////////////////////////////////

#include "Parser.h"
#include <cType.h>
#include "define.h"
#include "CompilerC0.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Parser::Parser()
{

}
Parser::Parser(FILE *i,SymbolTableMgr* s,QuadrupleMgr* q)
{
	in=i;
	tempLineNo=0;
	tempSerialNo=0;
	lineNo=0;
	serialNo=0;
	SMgr=s;
	QMgr=q;
	end=false;
	errorcount=0;
	hasmain=false;
	funcend=false;
	lex=Lexer(i);
	reserve(Word("do",Tag::DOTK));
	reserve(Word("const",Tag::CONSTTK));
	reserve(Word("main",Tag::MAINTK));
	reserve(Word("if",Tag::IFTK));
	reserve(Word("else",Tag::ELSETK));
	reserve(Word("while",Tag::WHILETK));
	reserve(Word("for",Tag::FORTK));
	reserve(Word("scanf",Tag::SCANFTK));
	reserve(Word("printf",Tag::PRINTFTK));
	reserve(Word("return",Tag::RETURNTK));
	reserve(Type::Char);
	reserve(Type::Int);
	reserve(Type::Void);
}

Parser::~Parser()
{

}

void Parser::reserve(Token w){
	reserveWord.push_back(w);
}

int Parser::getFPos(FILE* fp, __int64* pose){
	tempToken = lastToken;
	tempLineNo = lineNo;
	tempSerialNo = serialNo;
	return fgetpos(in, pose);
}

int Parser::setFPos(FILE* fp, __int64* pose){
	
	lastToken = tempToken;
	lineNo = tempLineNo;
	serialNo = tempSerialNo;
	dpos = *pose;
	return fsetpos(in, pose);
}

bool Parser::checkKeyToken(string key){
	Token preToken=lex.scan(lineNo,serialNo);
	if (preToken.lexeme == key){
		return true;
	}
	else{
		return false;
	}
}

bool Parser::checkKeyTokenR(string key){
	__int64 tpos;
	fgetpos(in, &tpos);
	int tl=lineNo;
	int ts=serialNo;
	Token preToken=lex.scan(lineNo,serialNo);
	if (preToken.lexeme == key){
		lineNo=tl;
		serialNo=ts;
		fsetpos(in, &tpos);
		return true;
	}
	else{
		lineNo=tl;
		serialNo=ts;
		fsetpos(in, &tpos);
		return false;
	}
}

bool Parser::checkId(){
	Token preToken=lex.scan(lineNo,serialNo);
	if(preToken.tag==Tag::IDEN)
		return true;
	else
		return false;
}
bool Parser::checkIdR(){
	__int64 tpos;
	fgetpos(in,&tpos);
	int tl=lineNo;
	int ts=serialNo;
	Token preToken=lex.scan(lineNo,serialNo);
	if(preToken.tag==Tag::IDEN){
		lineNo=tl;
		serialNo=ts;
		fsetpos(in,&tpos);
		return true;
	}
	else{
		fsetpos(in,&tpos);
		lineNo=tl;
		serialNo=ts;
		return false;
	}
}
void Parser::Chr(int& s){
	lastToken=lex.scan(lineNo,serialNo);
	string temp=lastToken.toString();
	if(lastToken.tag==Tag::ILLEGALCHA){
		PrintNo();
		printf("Missing '.\n");
		s=ERR;
	}
	else if(temp.size()>1){
		PrintNo();
		printf("Illegal character.\n");
		s=ERR;
	}
	else{
		char c=temp[0];
		if(c!='*'&&c!='/'&&c!='+'&&c!='-'&&c!='_'&&!isdigit(c)&&!isalpha(c)){
			PrintNo();
			printf("Illegal character.\n");
			s=ERR;
		}
		else
			s=_CHAR;
	}
	return;
}
void Parser::Program(){
	getFPos(in,&pos);
	if(checkKeyTokenR("const")){
		ConstDec();
	}
	getFPos(in,&pos);
	if(checkKeyTokenR("int")||checkKeyTokenR("char")){
		checkKeyToken("");
		if(!checkIdR()){
			printf("Missing an identifier.\n");
		}
		else //if(!(checkKeyTokenR("int")||checkKeyTokenR("char")||checkKeyTokenR("void")))
			checkId();
		if(checkKeyTokenR("[")||checkKeyTokenR(",")||checkKeyTokenR(";")){
			setFPos(in,&pos);
			VarDec();
		}
		else if(checkKeyTokenR("(")){
			setFPos(in,&pos);
			ValuedFuncDef();
		}
		else{
			PrintNo();
			printf("Missing an identifier or a',' or a';' for var, or a '[' for array, or a'('for function.\n");
		}
	}
	do{
		getFPos(in,&pos);
		if(checkKeyTokenR("int")||checkKeyTokenR("char")){
			checkKeyToken("");
			if(!checkIdR()){
				printf("Missing an identifier.\n");
			}
			else //if(!(checkKeyTokenR("int")||checkKeyTokenR("char")||checkKeyTokenR("void")))
				checkId();
			if(checkKeyTokenR("(")){
				setFPos(in,&pos);
				ValuedFuncDef();
			}
			else{
				PrintNo();
				printf("Missing an identifier or a',' or a';' for var, or a '[' for array, or a'('for function.\n");
			}
		}
		else if(checkKeyTokenR("void")){
			checkKeyToken("void");
			if(checkKeyTokenR("main")){
				setFPos(in,&pos);
				MainDef();
				hasmain=true;
				break;
			}
			else if(checkIdR()){
				setFPos(in,&pos);
				VoidFuncDef();
			}
			else{
				PrintNo();
				printf("Missing a function name.\n");
				//break;
			}
		}
		else{
			string temp;
			lastToken=lex.scan(lineNo,serialNo);
			temp=lastToken.toString();
			if(temp[0]==-1){
				end=true;
			}
			if(temp=="const"){
				PrintNo();
				cout<<"Constants should be declared at the very first.\n";
			}
			else
			{
				PrintNo();
				printf("Missing a type name.\n");
			}
			//break;
		}
		if(end){
			if(!hasmain){
				PrintNo();
				cout<<"No main function.\n";
				}
			break;
		}
	}while(1);
}
void Parser::ConstDec(){
	do{
		if(!checkKeyToken("const")){
			PrintNo();
			printf("Expected \"const\"\n");
		}
		ConstDef();
		if(!checkKeyTokenR(";")){
			PrintNo();
			printf("Expected ';'\n");
		}
		else
			checkKeyToken(";");
	}while(checkKeyTokenR("const"));
}
void Parser::ConstDef(){
	int t;
	int s;
	bool flag=false;
	getFPos(in,&pos);
	if(!(checkKeyTokenR("int")||checkKeyTokenR("char"))){
		PrintNo();
		printf("Const Type Error.\n");
		t=ERR;
	}
	else{
		lastId=lex.scan(lineNo,serialNo);
		if(lastId.tag==Tag::INTTK)
			t=_INT;
		else if(lastId.tag==Tag::CHARTK)
			t=_CHAR;
	}
	do{
		flag=false;
		getFPos(in,&pos);
		if(!checkIdR()){
			PrintNo();
			printf("Missing an Identifier.\n");
		}
		else{
			lastId=lex.scan(lineNo,serialNo);
		}
		getFPos(in,&pos);
		if(!checkKeyTokenR("=")){
			PrintNo();
			printf("Missing a '='.\n");
		}
		else
			checkKeyToken("=");
		if(checkChrR()){
			Chr(s);
		}
		else if(checkKeyTokenR("+")||checkKeyTokenR("-")||checkNumR()){
			Integer(s);
		}
		if(SMgr->AllCapsLayerCheck(lastId.toString())!=-1)
		{
			PrintNo();
			cout<<"Const redefinition.\n";
		}
		else
			ConstDef_Insert(t,lastId.toString(),lastToken.toString(),s);
		flag=checkKeyTokenR(",");
		if(flag)
			checkKeyToken(",");
	}while(flag);
}
void Parser::VarDec(){
	do{
		VarDef();
		if(!checkKeyTokenR(";")){
			PrintNo();
			printf("expected a ;\n");
		}
		else
			checkKeyToken(";");
		getFPos(in,&pos);
		if(checkKeyTokenR("int")||checkKeyTokenR("char"))
			checkKeyToken("");
		else
			break;
		if(!checkId()){
			PrintNo();
			printf("Illegal identifier\n");
		}
		if(!(checkKeyTokenR("[")||checkKeyTokenR(",")||checkKeyTokenR(";"))){
			setFPos(in,&pos);
			break;
		}
		setFPos(in,&pos);
	}while(1);
}
void Parser::VarDef(){
	int t;
	int size;
	bool flag=false;
	bool arrf=false;
	if(!(checkKeyTokenR("int")||checkKeyTokenR("char"))){
		PrintNo();
		printf("Unexpected type name\n");
		t=ERR;
	}
	else{
		lastId=lex.scan(lineNo,serialNo);
		if(lastId.tag==Tag::INTTK)
			t=_INT;
		else if(lastId.tag==Tag::CHARTK)
			t=_CHAR;
		else
			t=ERR;
	}
	do{
		flag=false;
		arrf=false;
		size=0;
		if(!checkIdR()){
			PrintNo();
			printf("illegal identifier");
		}
		else
			lastId=lex.scan(lineNo,serialNo);
		if(checkKeyTokenR("[")){
			//array
			checkKeyToken("[");
			if(!checkNumR()){
				lastToken=lex.scan(lineNo,serialNo);
				PrintNo();
				printf("Here should be a unsigned-integer\n");
				size=0;
			}
			else{
				arrf=true;
				Integer(size);
				Num temp=Num(lastToken.toString());
				size=temp.value;
			}
			if(!arrf)
				VarDef_Insert(t,lastId.toString());
			else
				VarDef_Insert_Array(t,lastId.toString(),size);
			VarDef_Alloc(size,t,lastId.toString(),arrf);
			if(!checkKeyTokenR("]")){
				PrintNo();
				printf("Expected a ']'");
			}
			else
				checkKeyToken("]");
		}
		else{
			size=1;
			VarDef_Insert(t,lastId.toString());
			VarDef_Alloc(size,t,lastId.toString(),arrf);
		}
		flag=checkKeyTokenR(",");
		if(flag)
			checkKeyToken(",");
	}while(flag);	
}
void Parser::ValuedFuncDef(){
	funcend=false;
	int t;
	Item* i;
	string n="";
	if(!(checkKeyTokenR("int")||checkKeyTokenR("char"))){
		printf("There should be a type name.\n");
		t=ERR;
	}
	else{
		lastId=lex.scan(lineNo,serialNo);
		if(lastId.tag==Tag::INTTK)
			t=_INT;
		else if(lastId.tag==Tag::CHARTK)
			t=_CHAR;
		else
			t=ERR;
	}
	if(!checkIdR()){
		PrintNo();
		printf("There should be an identifier.\n");
		if(checkKeyTokenR("int")||checkKeyTokenR("char")||checkKeyTokenR("void")){
			return;
		}
	}
	else{
		lastId=lex.scan(lineNo,serialNo);//FuncDefBegin
		n=lastId.toString();
	}
	FuncDef_Begin(t,n,i);
	if(checkKeyTokenR("=")){
		PrintNo();
		checkKeyToken("=");
		cout<<"Cannot do assignment here.\n";
// 		FuncDef_Stepin(n);
// 		FuncDef_Pop(n);
// 		FuncDef_End();
// 		FuncDef_Stepout();
		return;
	}
	if(checkKeyTokenR(",")||checkKeyTokenR("[")){
		PrintNo();
		checkKeyTokenR(",");
		cout<<"Cannot declare vars here.\n";
		// 		FuncDef_Stepin(n);
		// 		FuncDef_Pop(n);
		// 		FuncDef_End();
		// 		FuncDef_Stepout();
		return;
	}
	if(!checkKeyTokenR("(")){
		PrintNo();
		printf("Expected a '('\n");
	}
	else
		checkKeyToken("(");
	if(!checkKeyTokenR(")")){
		if(checkKeyTokenR("{")){
			PrintNo();
			printf("Expected a ')'\n");
		}
		else{
			Parameters(n);
		}
	}
	if(!checkKeyTokenR(")")){
		PrintNo();
		printf("Expected a ')'\n");
	}
	else
		checkKeyToken(")");//Stepin Pop
	FuncDef_Stepin(n);
	FuncDef_Pop(n);
	if(!checkKeyTokenR("{")){
		PrintNo();
		printf("Need a '{' to start the body\n");
	}
	else
		checkKeyToken("{");
	CompStat();
	if(!checkKeyTokenR("}")){
		PrintNo();
		printf("Expected a '}'\n");
	}
	else
		checkKeyToken("}");//FuncDef_End();Stepout();
	FuncDef_End();
	FuncDef_Stepout();
}
void Parser::VoidFuncDef(){
	funcend=false;
	Item* i;
	string n="";
	if(!checkKeyTokenR("void")){
		PrintNo();
		printf("There should be a type name.\n");
	}
	else
		checkKeyToken("void");
	if(!checkIdR()){
		PrintNo();
		printf("There should be a indentifier.\n");
	}
	else{
		lastId=lex.scan(lineNo,serialNo);//FuncDefBegin
		n=lastId.toString();
	}
	ProcDef_Begin(n,i);
	if(!checkKeyTokenR("(")){
		PrintNo();
		printf("Expected a '('\n");
	}
	else
		checkKeyToken("(");
	if(!checkKeyTokenR(")")){
		if(checkKeyTokenR("{")){
			PrintNo();
			printf("Expected a ')'\n");
		}
		else{
			Parameters(n);
		}
	}
	if(!checkKeyTokenR(")")){
		PrintNo();
		printf("Expected a ')'\n");
	}
	else
		checkKeyToken(")");//Stepin Pop
	ProcDef_Stepin(n);
	ProcDef_Pop(n);
	if(!checkKeyTokenR("{")){
		PrintNo();
		printf("Need a '{' to start the body\n");
	}
	else
		checkKeyToken("{");
	CompStat();
	if(!checkKeyTokenR("}")){
		PrintNo();
		printf("Expected a '}'\n");
	}
	else
		checkKeyToken("}");//FuncDef_End();Stepout();
	ProcDef_End();
	ProcDef_Stepout();
}
void Parser::MainDef(){
	funcend=false;
	if(!checkKeyTokenR("void")){
		PrintNo();
		printf("There should be a type name.\n");
	}
	else
		checkKeyToken("void");
	if(!checkKeyTokenR("main")){
		PrintNo();
		printf("There should be a \"main\".\n");
	}
	else
		lastId=lex.scan(lineNo,serialNo);//MainProc_Begin();
	MainProc_Begin();
	if(!checkKeyTokenR("(")){
		PrintNo();
		printf("Expected a '('\n");
	}
	else
		checkKeyToken("(");//insert? no t
	if(!checkKeyTokenR(")")){
		PrintNo();
		printf("Expected a ')'\n");
	}
	else
		checkKeyToken(")");//Stepin Pop
	MainProc_Stepin();
	if(!checkKeyTokenR("{")){
		PrintNo();
		printf("Need a '{' to start the body\n");
	}
	else
		checkKeyToken("{");
	CompStat();
	if(!checkKeyTokenR("}")){
		PrintNo();
		printf("Expected a '}'\n");
	}
	else
		checkKeyToken("}");//FuncDef_End();Stepout();
	MainProc_End();
	MainProc_Stepout();
}
void Parser::Parameters(string n){
	ParaTable(n);
}
void Parser::CompStat(){
	getFPos(in,&pos);
	if(checkKeyTokenR("const")){
		ConstDec();
	}
	if(checkKeyTokenR("int")||checkKeyTokenR("char"))
		VarDec();
	StatList();
}
void Parser::StatList(){
	while(!checkKeyTokenR("}")){
		Stat();
		if(end||funcend)
			break;
	}
}
void Parser::ParaTable(string n){
	int c=0;
	int t;
	string s="";
	bool flag=false;
	if(!checkKeyTokenR(")")){
		do{
			flag=false;
			if(!checkKeyTokenR("int")&&!checkKeyTokenR("char")){
				PrintNo();
				printf("There should be a type name.\n");
				t=ERR;
				lex.scan(lineNo,serialNo);
			}
			else{
				lastId=lex.scan(lineNo,serialNo);
				if(lastId.tag==Tag::INTTK)
					t=_INT;
				else if(lastId.tag==Tag::CHARTK)
					t=_CHAR;
				else
					t=ERR;
			}
			if(!checkIdR()){
				PrintNo();
				printf("There should be an identifier.\n");
				lex.scan(lineNo,serialNo);
				s="";
			}
			else{
				lastId=lex.scan(lineNo,serialNo);
				s=lastId.toString();
			}
			//ParaTable_Insert; FuncName, t,c lastId
			ParaTable_Insert(n,t,s,c);
			c++;
			flag=checkKeyTokenR(",");
			if(flag)
				checkKeyToken(",");
		}while(flag);
		SMgr -> AllCapsFind(n) -> dimension = c;
	}
}
void Parser::Stat(){
	getFPos(in,&pos);
	if(checkKeyTokenR("{")){
		checkKeyToken("{");
		StatList();
		if(!checkKeyTokenR("}")){
			PrintNo();
			printf("Expected a '}'\n");
		}
		else
			checkKeyToken("}");
	}
	else if(checkKeyTokenR("if")){
		CondStat();
	}
	else if(checkKeyTokenR("do")||checkKeyTokenR("for")){
		loopStat();
	}
	else if(checkKeyTokenR("scanf")){
		ReadStat();
		if(!checkKeyTokenR(";")){
			PrintNo();
			printf("Expected a ';'\n");
		}
		else
			checkKeyToken(";");
	}
	else if(checkKeyTokenR("printf")){
		WriteStat();
		if(!checkKeyTokenR(";")){
			PrintNo();
			printf("Expected a ';'\n");
		}
		else
			checkKeyToken(";");
	}
	else if(checkKeyTokenR("return")){
		ReturnStat();
		if(!checkKeyTokenR(";")){
			PrintNo();
			printf("Expected a ';'\n");
		}
		else
			checkKeyToken(";");
	}
	else if(checkIdR()){
		lastId=lex.scan(lineNo,serialNo);
		if(checkKeyTokenR("=")||checkKeyTokenR("[")){
			setFPos(in,&pos);
			AssignStat();
		}
		else if(checkKeyTokenR("(")){
			Item* temp;
			setFPos(in,&pos);
			if(Stat_Check(lastId.toString()))
				FuncCall(temp);
			else
				ProcCall();
		}
		if(!checkKeyTokenR(";")){
			if(SMgr->AllCapsCheck(lastId.toString())==-1)
			{
				PrintNo();
				cout<<"\""<<lastId.toString()<<"\" is an undeclared identifier."<<endl;
			}
			else{
				PrintNo();
				cout<<"Error use of \""<<lastId.toString()<<"\"."<<endl;
				
				PrintNo();
				printf("Expected a ';'\n");
			}
		}
		else{
			if(SMgr->AllCapsCheck(lastId.toString())==-1)
			{
				PrintNo();
				cout<<"\""<<lastId.toString()<<"\" is an undeclared identifier."<<endl;
			}
			checkKeyToken(";");
		}
	}
	else{
		if(checkKeyTokenR("void")||checkKeyTokenR("int")||checkKeyTokenR("char")){
			funcend=true;
			return;
		}
		if(!checkKeyTokenR(";")){
			lastToken=lex.scan(lineNo,serialNo);
			string temp=lastToken.toString();
			if(temp[0]==-1){
				PrintNo();
				cout<<"Unexpected end of file. Missing a '}' or ')' ?"<<endl;
				end=true;
			}
			else{
				PrintNo();
				cout<<"\""<<lastToken.toString()<<"\" is not a legal statement: Unrecognized symbol or undefined syntax."<<endl;
			}
		}
		else
			checkKeyToken(";");
	}
}
void Parser::Expr(Item*& i){
	Item* item[100];
	int itemNumber=0;
	int sign;
	int op;
	bool flag;
	if(checkKeyTokenR("-")){
		checkKeyToken("-");
		sign=0;
	}
	else if(checkKeyTokenR("+")){
		checkKeyToken("+");
		sign=1;
	}
	else
		sign=1;
	Term(item[itemNumber]);
	itemNumber++;
	if(sign==0){
		Expr_Sign(item[itemNumber-1],item[itemNumber]);
		itemNumber++;
	}
	do{
		flag=false;
		if(checkKeyTokenR("+")||checkKeyTokenR("-")){
			lastToken=lex.scan(lineNo,serialNo);
			if(lastToken.toString()=="+")
				op=1;
			else if(lastToken.toString()=="-")
				op=0;
			Term(item[itemNumber]);
			itemNumber++;
			Expr_Add(item[itemNumber-2],item[itemNumber-1],op,item[itemNumber]);
			itemNumber++;
			flag=true;		
		}
	}while(flag);
	i=item[itemNumber-1];
}
void Parser::Term(Item*& i){
	Item* item[100];
	int itemNumber=0;
	int op;
	bool flag;
	Factor(item[itemNumber]);
	itemNumber++;
	do{
		flag=false;
		if(checkKeyTokenR("*")||checkKeyTokenR("/")){
			lastToken=lex.scan(lineNo,serialNo);
			if(lastToken.toString()=="*")
				op=1;
			else if(lastToken.toString()=="/")
				op=0;
			Factor(item[itemNumber]);
			itemNumber++;
			Term_Mul(item[itemNumber-2],item[itemNumber-1],op,item[itemNumber]);
			itemNumber++;
			flag=true;
		}
	}while(flag);
	i=item[itemNumber-1];
}
void Parser::Factor(Item*& i){
	string n;
	string c;
	int s;
	Item* index;
	getFPos(in,&pos);
	if(checkKeyTokenR("(")){
		checkKeyToken("(");
		Expr(i);
		if(!checkKeyTokenR(")")){
			PrintNo();
			printf("Expected a ')'\n");
		}
		else
			checkKeyToken(")");
	}
	else if(checkChrR()){
		Chr(s);
		c=lastToken.toString();
		Factor_ConstTemp(c,s,i);
	}
	else if(checkIdR()){
		getFPos(in,&pos);
		lastId=lex.scan(lineNo,serialNo);
		if(checkKeyTokenR("(")){
			setFPos(in,&pos);
			FuncCall(i);
		}
		else if(checkKeyTokenR("[")){
			//Array
			setFPos(in,&pos);
			lastId=lex.scan(lineNo,serialNo);
			n=lastId.toString();
			checkKeyToken("[");
			Expr(index);
			Factor_Array_Find(index,n,i);
			if(!checkKeyTokenR("]")){
				PrintNo();
				printf("Expected a ']'\n");
			}
			else
				checkKeyToken("]");
		}
		else{
			setFPos(in,&pos);
			lastId=lex.scan(lineNo,serialNo);
			n=lastId.toString();
			Factor_Find(n,i);
		}
	}
	else if(checkKeyTokenR("+")||checkKeyTokenR("-")||checkNumR()){
		Integer(s);
		Factor_ConstTemp(lastToken.toString(),s,i);
	}
	else
		i=NULL;
}
bool Parser::checkChrR(){
	__int64 tpos;
	fgetpos(in,&tpos);
	int tl=lineNo;
	int ts=serialNo;
	Token preToken=lex.scan(lineNo,serialNo);
	if(preToken.tag==Tag::ILLEGALCHA||preToken.tag==Tag::CHARCON){
		lineNo=tl;
		serialNo=ts;
		fsetpos(in,&tpos);
		return true;
	}
	else{
		lineNo=tl;
		serialNo=ts;
		fsetpos(in,&tpos);
		return false;
	}
}
bool Parser::checkNumR(){
	__int64 tpos;
	int tl=lineNo;
	int ts=serialNo;
	fgetpos(in,&tpos);
	Token preToken=lex.scan(lineNo,serialNo);
	if (preToken.tag == Tag::ILLEGALNUM||preToken.tag==Tag::INTCON){
		lineNo=tl;
		serialNo=ts;
		fsetpos(in, &tpos);
		return true;
	}
	else{
		lineNo=tl;
		serialNo=ts;
		fsetpos(in, &tpos);
		return false;
	}
}
void Parser::Integer(int& s){
	Num temp;
	bool signFlag=false;
	if(checkKeyTokenR("+"))
		checkKeyToken("+");
	else if(checkKeyTokenR("-")){
		signFlag=checkKeyToken("-");
	}
	lastToken=lex.scan(lineNo,serialNo);
	if(lastToken.tag==Tag::ILLEGALNUM){
		temp=Num(0);
		PrintNo();
		printf("doulbe 0\n");
		lastToken=temp;
		s=_INT;
	}
	else if(lastToken.tag==Tag::INTCON){
		if(signFlag)
			temp=Num("-"+lastToken.toString());
		else
			temp=Num(lastToken.toString());
		lastToken=temp;
		s=_INT;
	}
	else{
		PrintNo();
		printf("There should be an integer.\n");
		s=ERR;
	}
}
void Parser::AssignStat(){
	Item* i1 = NULL;
	Item* index=NULL;
	Item* i2 = NULL;
	bool flag=false;
	lastId=lex.scan(lineNo,serialNo);
	AssignStat_Find(lastId.toString(),i1);
	if(checkKeyTokenR("[")){
		checkKeyToken("[");
		Expr(index);/////
		flag=true;
		if(!checkKeyTokenR("]")){
			PrintNo();
			printf("Expected a ']'\n");
		}
		else
			checkKeyToken("]");
	}
	if(checkKeyTokenR("=")){
		checkKeyToken("=");
		Expr(i2);
	}
	else{
		PrintNo();
		printf("Expected a '='\n");
	}
	if(flag)
		AssignStat_IndexAssign(i1,index,i2);
	else
		AssignStat_Assign(i1,i2);
}
void Parser::CondStat(){
	Item* i;
	Item* a;
	Item* b;
	if(!checkKeyTokenR("if")){
		PrintNo();
		printf("Expected \"if\"\n");
	}
	else
		checkKeyToken("if");
	if(!checkKeyTokenR("(")){
		PrintNo();
		printf("Expected a '('\n");
	}
	else
		checkKeyToken("(");
	Cond(i);
	if(!checkKeyTokenR(")")){
		PrintNo();
		printf("Expected a ')'\n");
	}
	else
		checkKeyToken(")");
	CtrlStat_ECBrf(i,a);
	Stat();
	getFPos(in,&pos);
	if(checkKeyTokenR("else")){
		checkKeyToken("else");
		CtrlStat_CBrf(b);
		CtrlStat_Labprod(a);
		Stat();
		CtrlStat_Labprod(b);
	}
	else
		CtrlStat_Labprod(a);
}
void Parser::Cond(Item*& i){
	Item* i1;
	Item* i2;
	string lop;
	Expr(i1);
	getFPos(in,&pos);
	if(checkKeyTokenR("==")||checkKeyTokenR("!=")||checkKeyTokenR(">")||checkKeyTokenR(">=")||checkKeyTokenR("<")||checkKeyTokenR("<=")){
		lastToken=lex.scan(lineNo,serialNo);
		lop=lastToken.toString();
		Expr(i2);
		LogExpr_Rel(i1,i2,lop,i);
	}
	else if(checkKeyTokenR(")")||checkKeyTokenR(";")){
		i=i1;
	}
	else{
		PrintNo();
		printf("Error\n");
	}
}
void Parser::loopStat(){
	Item* i;
	Item* a;
	Item* b;
	Item* c;
	Item* d;
	string n;
	string n1;
	string n2;
	Item* i1;
	Item* i2;
	Item* istep;
	int op;
	getFPos(in,&pos);
	if(checkKeyTokenR("for")){
		checkKeyToken("for");
		if(!checkKeyTokenR("(")){
			PrintNo();
			printf("Expected a '('\n");
		}
		else
			checkKeyToken("(");
		if(!checkIdR()){
			PrintNo();
			printf("Expected a identifier.\n");
		}
		lastId=lex.scan(lineNo,serialNo);
		n=lastId.toString();
		LoopStat_Find(n,i1);
		if(!checkKeyTokenR("=")){
			PrintNo();
			printf("Expected a '='\n");
		}
		else
			checkKeyToken("=");
		Expr(i2);
		if(!checkKeyTokenR(";")){
			PrintNo();
			printf("Expected a ';'\n");
		}
		else
			checkKeyToken(";");
		LoopStat_Assign(i1,i2);
		CtrlStat_CLabprod(b);
		Cond(i);
		CtrlStat_ECBrf(i,a);
		CtrlStat_CBrf(c);
		CtrlStat_CLabprod(d);
		if(!checkKeyTokenR(";")){
			PrintNo();
			printf("Expected a ';'\n");
		}
		else
			checkKeyToken(";");
		if(!checkIdR()){
			PrintNo();
			printf("Expected a identifier.\n");
		}
		lastId=lex.scan(lineNo,serialNo);
		n1=lastId.toString();
		LoopStat_Find(n1,i1);
		if(!checkKeyTokenR("=")){
			PrintNo();
			printf("Expected a '='\n");
		}
		else
			checkKeyToken("=");
		if(!checkIdR()){
			PrintNo();
			printf("Expected a identifier.\n");
		}
		lastId=lex.scan(lineNo,serialNo);
		n2=lastId.toString();
		LoopStat_Find(n2,i2);
		if(checkKeyTokenR("+")){
			checkKeyToken("+");
			op=1;
		}
		else if(checkKeyTokenR("-")){
			checkKeyToken("-");
			op=0;
		}
		else{
			PrintNo();
			printf("Expected a '+' or '-'\n");
		}
		Step(istep);
		LoopStat_Add(i1,istep,op,i2);
		CtrlStat_Brf(b);
		if(!checkKeyTokenR(")")){
			PrintNo();
			printf("Expected a ')'\n");
		}
		else 
			checkKeyToken(")");
		CtrlStat_Labprod(c);
		Stat();
		CtrlStat_Brf(d);
		CtrlStat_Labprod(a);
	}
	else if(checkKeyTokenR("do")){
		checkKeyToken("do");
		CtrlStat_CLabprod(b);
		Stat();
		if(!checkKeyTokenR("while")){
			PrintNo();
			printf("Expected \"while\".\n");
		}
		else
			checkKeyToken("while");
		if(!checkKeyTokenR("(")){
			PrintNo();
			printf("Expected a '('\n");
		}
		else 
			checkKeyToken("(");
		Cond(i);
		if(!checkKeyTokenR(")")){
			PrintNo();
			printf("Expected a ')'\n");
		}
		else 
			checkKeyToken(")");
		CtrlStat_ECBrf(i,a);
		CtrlStat_Brf(b);
		CtrlStat_Labprod(a);
	}
}
void Parser::Step(Item*& i){
	lastToken=lex.scan(lineNo,serialNo);
	if(lastToken.tag!=Tag::ILLEGALNUM&&lastToken.tag!=Tag::INTCON){
		PrintNo();
		printf("Only integer!\n");
	}
	else if(lastToken.tag==Tag::ILLEGALNUM){
		PrintNo();
		printf("double 0\n");
	}
	Num step=Num(lastToken.toString());
	Step_ConstTemp(step.toString(),i);
}
void Parser::FuncCall(Item*& j){
	Item* i;
	lastId=lex.scan(lineNo,serialNo);
	FuncCall_Find(lastId.toString(),i);
	if(!checkKeyTokenR("(")){
		PrintNo();
		printf("Expected '('\n");
	}
	else
		checkKeyToken("(");
	ArguTable(i);
	if(!checkKeyTokenR(")")){
		PrintNo();
		printf("Expected ')'\n");
	}
	else
		checkKeyToken(")");
	FuncCall_Call(i,j);
}
void Parser::ProcCall(){
	Item* i;
	lastId=lex.scan(lineNo,serialNo);
	ProcCall_Find(lastId.toString(),i);
	if(!checkKeyTokenR("(")){
		PrintNo();
		printf("Expected '('\n");
	}
	else
		checkKeyToken("(");
	ArguTable(i);
	if(!checkKeyTokenR(")")){
		PrintNo();
		printf("Expected ')'\n");
	}
	else
		checkKeyToken(")");
	ProcCall_Call(i);
}
void Parser::ArguTable(Item*& i){
	Item* item[100];
	int itemNumber=0;
	bool flag;
	if(!checkKeyTokenR(")")){
		Expr(item[itemNumber]);
		itemNumber++;
		ArguTable_Check(i,item[itemNumber-1],itemNumber-1);
		do{
			flag=false;
			if(checkKeyTokenR(",")){
				checkKeyToken(",");
				Expr(item[itemNumber]);
				itemNumber++;
				ArguTable_Check(i,item[itemNumber-1],itemNumber-1);
				flag=true;
			}
		}while(flag);
		ArguTable_ReversePush(i,item,itemNumber);
	}
}
void Parser::ReadStat(){
	Item* i;
	bool flag;
	if(!checkKeyTokenR("scanf")){
		PrintNo();
		printf("Expected \"scanf\"\n");
	}
	else
		checkKeyToken("scanf");
	if(!checkKeyTokenR("(")){
		PrintNo();
		printf("Expected '('\n");
	}
	else
		checkKeyToken("(");
	lastId=lex.scan(lineNo,serialNo);
	ReadStat_Find(lastId.toString(),i);
	ReadStat_Read(i);
	do{
		flag=false;
		if(checkKeyTokenR(",")){
			checkKeyToken(",");
			lastId=lex.scan(lineNo,serialNo);
			ReadStat_Find(lastId.toString(),i);
			ReadStat_Read(i);
			flag=true;
		}
	}while(flag);
	if(!checkKeyTokenR(")")){
		PrintNo();
		printf("Expected a ')'\n");
	}
	else
		checkKeyToken(")");
}
void Parser::WriteStat(){
	Item* i1;
	Item* i2;
	if(!checkKeyTokenR("printf")){
		PrintNo();
		printf("Expected \"printf\"\n");
	}
	else
		checkKeyToken("printf");
	if(!checkKeyTokenR("(")){
		PrintNo();
		printf("Expected a '('\n");
	}
	else 
		checkKeyToken("(");
	getFPos(in,&pos);
	lastToken=lex.scan(lineNo,serialNo);
	if(lastToken.tag==Tag::STRCON||lastToken.tag==Tag::ILLEGALSTR){
		setFPos(in,&pos);
		Str(i1);
		if(checkKeyTokenR(",")){
			checkKeyToken(",");
			Expr(i2);
			WriteStat_Write(i1,i2);
		}
		else
			WriteStat_Write(i1);
		if(!checkKeyTokenR(")")){
			PrintNo();
			printf("Expected ')'\n");
		}
		else
			checkKeyToken(")");
	}
	else{
		setFPos(in,&pos);
		Expr(i2);
		WriteStat_Write(i2);
		if(!checkKeyTokenR(")")){
			PrintNo();
			printf("Expected ')'\n");
		}
		else
			checkKeyToken(")");
	}
}
void Parser::ReturnStat(){
	Item* i;
	if(!checkKeyTokenR("return")){
		PrintNo();
		printf("Expected \"return\"\n");
	}
	else
		checkKeyToken("return");
	if(checkKeyTokenR("(")){
		checkKeyToken("(");
		if(checkKeyTokenR(")")){
			PrintNo();
			cout<<"There should be an expression between the '(' and ')'.\n";
			i=NULL;
			checkKeyToken(")");
		}
		else
			Expr(i);
		RetStat_NRet(i);
		if(!checkKeyTokenR(")")){
			PrintNo();
			printf("Expected ')'\n");
		}
		else
			checkKeyToken(")");
	}
	else{
		RetStat_Ret();	
	}
}
void Parser::Str(Item*& i){
	int j;
	lastToken=lex.scan(lineNo,serialNo);
	string temp=lastToken.toString();
	if(lastToken.tag==Tag::ILLEGALSTR){
		PrintNo();
		printf("Expected \"\n");
	}
	for(j=0;j<temp.size();j++){
		if(temp[j]!=32&&temp[j]!=33&&!(temp[j]>=35&&temp[j]<=126)){
			PrintNo();
			printf("Illegal string.\n");
			temp="";
			break;
		}
	}
	Str_Insert(temp,i);
}

/*==================================================================================*/

void Parser::ConstDef_Insert(int t, string n, string c, int s){
	if(t==ERR){
		PrintNo();
		printf("Type id error.\n");
	}
	else if(s==ERR){
		PrintNo();
		printf("Type of value error.\n");
	}
	else if(t!=s){
		PrintNo();
		printf("Data type doesn't match.\n");
	}
	else{
		if(SMgr->AllCapsLayerCheck(n)==-1){
			SMgr->Insert(n,_CONST,s,c,0);
			QMgr->NewQuad(CST,SMgr->AllCapsFind(n));
		}
		else{
			PrintNo();
			printf("The const name has already been declared.\n");
		}
	}
}
void Parser::VarDef_Insert(int t, string n){
	if(t==ERR){
		PrintNo();
		printf("Type id error.\n");
	}
	else{
		if(SMgr->AllCapsLayerCheck(n)==-1){
			if(SMgr->pCurrentTable->pParentItem!=NULL){
				SMgr->Insert(n,INVAR,t,"",0);
				SMgr->AllCapsLayerFind(n)->pParentTable=SMgr->pCurrentTable;
				QMgr->NewItemForInVar(SMgr->AllCapsLayerFind(n));
			}
			else{
				SMgr->Insert(n,VAR,t,"",0);
			}
		}
		else{
			PrintNo();
			printf("This var has already been declared.\n");
		}
	}
}
void Parser::VarDef_Insert_Array(int t, string n,int size){
	if(t==ERR){
		PrintNo();
		printf("Type id error.\n");
	}
	else{
		if(SMgr->AllCapsLayerCheck(n)==-1){
			if(SMgr->pCurrentTable->pParentItem!=NULL){
				SMgr->Insert(n,INVAR,t,"",0);
				SMgr->AllCapsLayerFind(n)->pParentTable=SMgr->pCurrentTable;
				QMgr->NewItemForInVarArray(SMgr->AllCapsLayerFind(n),size);
			}
			else{
				SMgr->Insert(n,VAR,t,"",0);
			}
		}
		else{
			PrintNo();
			printf("This var has already been declared.\n");
		}
	}
}
void Parser::VarDef_Alloc(int size,int t, string n,bool arr){
	if(t==ERR){
		PrintNo();
		printf("Type id error.\n");
	}
	else if(size==0){
		PrintNo();
		printf("Array size error.\n");
	}
	else{
		char cc[5];
		_itoa_s(size,cc,10);
		string ss(cc);
		Item* tSize=QMgr->NewCItem(_INT,cc);
		if(t==_INT)
			QMgr->NewQuad(DINT,tSize,SMgr->AllCapsFind(n));
		else if(t==_CHAR)
			QMgr->NewQuad(DCHR,tSize,SMgr->AllCapsFind(n));
		if(size>=1){
			Item* temp=SMgr->AllCapsFind(n);
			temp->dimension=size;
			temp->arr=arr;
		}
	}
}
void Parser::Expr_Add(Item* i1, Item* i2,int o, Item*& res){
	if(o==1){
		res=QMgr->InsertQuad(ADD,i1,i2,_INT,SMgr->pCurrentTable);
	}
	else if(o==0){
		res=QMgr->InsertQuad(SUB,i1,i2,_INT,SMgr->pCurrentTable);
	}
}
void Parser::Expr_Sign(Item* in, Item*& i){
	if(in==NULL){
		PrintNo();
		cout<<"Cannot execute operation '-' due to previous error(s)"<<endl;
		i=NULL;
		return;
	}
	Item* newItem;
	newItem=QMgr->NewItem(in->type,SMgr->pCurrentTable);
	QMgr->NewQuad(REV,in,newItem);
	i=newItem;
}
void Parser::Term_Mul(Item* i1, Item* i2,int o, Item*& res){
	if(o==1)
		res=QMgr->InsertQuad(MUL,i1,i2,_INT,SMgr->pCurrentTable);
	else if(o==0){
		res=QMgr->InsertQuad(DIV,i1,i2,_INT,SMgr->pCurrentTable);
	}
}
void Parser::Factor_Find(string n, Item*& i){
	if(SMgr->AllCapsCheck(n)!=-1){
		if(SMgr->AllCapsFind(n)->arr){
			PrintNo();
			printf("Cannot operate on an array as a whole.\n");
			i=NULL;
		}
		else
			i=SMgr->AllCapsFind(n);
	}
	else{
		PrintNo();
		printf("Undeclared identifier.\n");
		i=NULL;
	}
}
void Parser::Factor_Array_Find(Item* index,string n, Item*& i){
	if(SMgr->AllCapsCheck(n)!=-1){
		if(!SMgr->AllCapsFind(n)->arr){
			PrintNo();
			printf("This var is not an array.\n");
			i=NULL;
		}
		else{
			Item* j=SMgr->AllCapsFind(n);
			if(index->kind==_CONST||index->kind==CONSTTEMP){
				if(index->type==_INT){
					int ind=atoi(index->value.c_str());
					if(ind>=j->dimension||ind<0)
					{
						PrintNo();
						cout<<"Reference out of range.\n";
					}
				}
				else if(index->type=_CHAR){
					char ind=index->value[0];
					if((int)ind>=j->dimension)
					{
						PrintNo();
						cout<<"Reference out of range.\n";
					}
				}
			}
			
			i=QMgr->NewItem(j->type,SMgr->pCurrentTable);
			QMgr->NewQuad(IDX,index,j,i);
		}
	}
	else{
		PrintNo();
		printf("Undeclared identifier.\n");
		i=NULL;
	}
}
void Parser::Factor_ConstTemp(string c, int s, Item*& i){
	if(s==ERR){
		PrintNo();
		printf("Wrong char.\n");
		i=NULL;
	}
	else
		i=QMgr->NewCItem(s,c);
}
void Parser::ParaTable_Insert(string n, int t, string s, int c){
	SMgr->StepIn(n);
	if(t==ERR){
		PrintNo();
		printf("Type id error.\n");
	}
	else{
		if(SMgr->AllCapsLayerCheck(s)==-1){
			char cc[5];
			_itoa_s(c*4+8,cc,16);
			string ss(cc);
			SMgr->Insert(s,VAR,t,ss,0);
			QMgr->NewQuad(_IN,SMgr->AllCapsFind(s));
		}
		else{
			PrintNo();
			printf("This agrument name has already been declared.\n");
		}
	}
	SMgr->StepOut();
}
void Parser::AssignStat_Find(string n, Item*& i){
	if(SMgr->AllCapsCheck(n)!=-1)
		i=SMgr->AllCapsFind(n);
	else{
		PrintNo();
		printf("Undeclared identifier.\n");
		i=NULL;
	}
}

void Parser::AssignStat_Assign(Item* i1, Item* i2){
	if(i1==NULL||i2==NULL){
		PrintNo();
		cout<<"Cannot assign due to previous error(s).\n";
		return;
	}
	if(i1->arr){
		PrintNo();
		cout<<"Cannot assign an array as a whole.\n";
	}
	else{
		if(i2->kind==INVAR&&i2->isAssigned==false){
			PrintNo();
			cout<<i1->lexeme<<" is used without initialized.\n";
		}
		QMgr->NewQuad(ASN,i2,i1);
		i1->isAssigned=true;
	}
}
void Parser::AssignStat_IndexAssign(Item* i1,Item* index,Item* i2){
	if(i1==NULL||index==NULL||i2==NULL)
	{
		PrintNo();
		cout<<"Cannot assign due to previous error(s).\n";
		return;
	}
	if(index->kind==_CONST||index->kind==CONSTTEMP){
		if(index->type==_INT){
			int ind=atoi(index->value.c_str());
			if(ind>=i1->dimension||ind<0)
			{
				PrintNo();
				cout<<"Reference out of range.\n";
			}
		}
		else if(index->type=_CHAR){
			char ind=index->value[0];
			if((int)ind>=i1->dimension)
			{
				PrintNo();
				cout<<"Reference out of range.\n";
			}
		}
	}
	if(i1->arr){
		if(i2->kind==INVAR&&i2->isAssigned==false){
			PrintNo();
			cout<<i2->lexeme<<" is used without initialized.\n";
		}
		if(index->kind==INVAR&&index->isAssigned==false){
			PrintNo();
			cout<<index->lexeme<<" is used without initialized.\n";
		}
		QMgr->NewQuad(IDXASN,i2,index,i1);
	}
	else{
		PrintNo();
		cout<<i1->lexeme<<" is not an array.\n";
	}
}
void Parser::LogExpr_Rel(Item* i1, Item* i2, string o, Item*& res){
	if(i1==NULL||i2==NULL||o==""){
		PrintNo();
		cout<<"Cannot compare due to previous error(s).\n";
		res=NULL;
		return;
	}
	if(i1->kind==INVAR&&i1->isAssigned==false){
		PrintNo();
		cout<<i1->lexeme<<" is used without initialized.\n";
	}
	if(i2->kind==INVAR&&i2->isAssigned==false){
		PrintNo();
		cout<<i2->lexeme<<" is used without initialized.\n";
	}
	if (o == ">"){
		if (i1 -> type == i2 -> type){
			res = QMgr -> InsertQuad(BT, i1, i2, i1 -> type, SMgr -> pCurrentTable);
		}
		else printf("The type between the relation operator doesn't match.\n");
	}
	else if (o == ">="){
		if (i1 -> type == i2 -> type){
			res = QMgr -> InsertQuad(BET, i1, i2, i1 -> type, SMgr -> pCurrentTable);
		}
		else printf("The type between the relation operator doesn't match.\n");
	}
	else if (o == "<"){
		if (i1 -> type == i2 -> type){
			res = QMgr -> InsertQuad(LT, i1, i2, i1 -> type, SMgr -> pCurrentTable);
		}
		else printf("The type between the relation operator doesn't match.\n");
	}
	else if (o == "<="){
		if (i1 -> type == i2 -> type){
			res = QMgr -> InsertQuad(LET, i1, i2, i1 -> type, SMgr -> pCurrentTable);
		}
		else printf("The type between the relation operator doesn't match.\n");
	}
	else if (o == "=="){
		if (i1 -> type == i2 -> type){
			res = QMgr -> InsertQuad(EQ, i1, i2, i1 -> type, SMgr -> pCurrentTable);
		}
		else printf("The type between the relation operator doesn't match.\n");
	}
	else if (o == "!="){
		if (i1 -> type == i2 -> type){
			res = QMgr -> InsertQuad(NEQ, i1, i2, i1 -> type, SMgr -> pCurrentTable);
		}
		else printf("The type between the relation operator doesn't match.\n");
	}
}
void Parser::CtrlStat_ECBrf(Item* i,Item*& a){
	a=QMgr->NewQuad(EGTO,i,QMgr->NewLItem())->Obj;
}
void Parser::CtrlStat_CBrf(Item*& b){
	b=QMgr->NewQuad(GTO,QMgr->NewLItem())->Src1;
}
void Parser::CtrlStat_Labprod(Item* a){
	if(QMgr->FindLItem(a->lexeme)!=-1)
		QMgr->NewQuad(LAB,a);
	else cout<<"Doesn't define the label:"<< a->lexeme<<".\n"<<endl;
}
void Parser::CtrlStat_Brf(Item* a){
	if (QMgr -> FindLItem(a -> lexeme) != -1)
		QMgr -> NewQuad(GTO, a);
	else cout << "Doesn't define the label:" << a -> lexeme << ".\n" << endl;

}
void Parser::CtrlStat_CLabprod(Item*& b){
	b = QMgr -> NewQuad(LAB, QMgr -> NewLItem()) -> Src1;
}
void Parser::LoopStat_Find(string n, Item*& i){
	if(SMgr->AllCapsCheck(n)!=-1)
		i=SMgr->AllCapsFind(n);
	else{
		PrintNo();
		printf("Undeclared identifier.\n");
		i=NULL;
	}
}
void Parser::LoopStat_Assign(Item* i1, Item* i2){
	if(i1==NULL||i2==NULL){
		PrintNo();
		cout<<"Cannot assign due to previous error(s).\n";
		return;
	}
	if(i2->kind==INVAR&&i2->isAssigned==false){
		PrintNo();
		cout<<i2->lexeme<<" is used without initialized.\n";
	}
	if(i1->type!=i2->type){
		PrintNo();
		printf("The type between the equal operator doesn't match.\n");
	}
	else{
		QMgr->NewQuad(ASN,i2,i1);
		i1->isAssigned=true;
	}
}
void Parser::LoopStat_Add(Item* i1, Item* istep, int o, Item* i2){
	if(i1==NULL||istep==NULL||i2==NULL){
		PrintNo();
		cout<<"Cannot add step due to previous error(s).\n";
		return;
	}
	if(i1->kind==INVAR&&i1->isAssigned==false){
		PrintNo();
		cout<<i1->lexeme<<" is used without initialized.\n";
	}
	if(istep->kind==INVAR&&istep->isAssigned==false){
		PrintNo();
		cout<<istep->lexeme<<" is used without initialized.\n";
	}
	if(o==1){
		QMgr->NewQuad(ADD,i1,istep,i2);
	}
	else if(o==0){
		QMgr->NewQuad(SUB,i1,istep,i2);
	}
	i2->isAssigned=true;
}
void Parser::Step_ConstTemp(string n, Item*& i){
	i=QMgr->NewCItem(_INT,n);
}
void Parser::FuncDef_Begin(int t, string n, Item*& i){
	if(SMgr->AllCapsCheck(n)==-1){
		SMgr->Insert(n,FUNC,t,"",0);
		i=SMgr->AllCapsFind(n);
		QMgr->NewQuad(BEGIN,i);
	}
	else{
		PrintNo();
		printf("cannot redeclare a function.\n");
	}
}
void Parser::FuncDef_Stepin(string n){
	SMgr->StepIn(n);
}
void Parser::FuncDef_Pop(string n){

}
void Parser::FuncDef_End(){
	QMgr->NewQuad(RET);
	QMgr->NewQuad(END);
}
void Parser::FuncDef_Stepout(){
	SMgr->StepOut();
}
void Parser::ProcDef_Begin(string n, Item*& i){
	if(SMgr->AllCapsCheck(n)==-1){
		SMgr->Insert(n,PROC,_VOID,"",0);
		i=SMgr->AllCapsFind(n);
		QMgr->NewQuad(BEGIN,i);
	}
	else{
		PrintNo();
		printf("cannot redeclare a function.\n");
	}
}
void Parser::ProcDef_Stepin(string n){
	SMgr->StepIn(n);
}
void Parser::ProcDef_Pop(string n){

}
void Parser::ProcDef_End(){
	QMgr->NewQuad(RET);
	QMgr->NewQuad(END);
}
void Parser::ProcDef_Stepout(){
	SMgr->StepOut();
}
void Parser::FuncCall_Find(string n, Item*& i){
	if(SMgr->AllCapsCheck(n)!=-1)
		i=SMgr->AllCapsFind(n);
	else{
		PrintNo();
		printf("Undeclared funciton.\n");
		i=NULL;
	}
}
void Parser::FuncCall_Call(Item* i, Item*& j){
	if(i==NULL){
		j=NULL;
		PrintNo();
		cout<<"Cannot call an undeclared function.\n";
		return;
	}
	j=QMgr->NewItem(i->type,SMgr->pCurrentTable);
	QMgr->NewQuad(ECALL,i,j);
}
void Parser::ProcCall_Find(string n, Item*& i){
	if(SMgr->AllCapsCheck(n)!=-1)
		i=SMgr->AllCapsFind(n);
	else{
		PrintNo();
		printf("Undeclared function.\n");
		i=NULL;
	}
}
void Parser::ProcCall_Call(Item* i){
	if(i==NULL){
		PrintNo();
		cout<<"Cannot call an undeclared function.\n";
		return;
	}
	QMgr->NewQuad(CALL,i);
}
void Parser::ArguTable_Check(Item* i, Item* i1, int m){
	if(i==NULL||i1==NULL){
		PrintNo();
		cout<<"Invalid argument."<<endl;
		return;
	}
	string temp="";
	if(SMgr->pCurrentTable->pParentItem!=NULL){
		temp=SMgr->pCurrentTable->pParentItem->lexeme;
		SMgr->StepOut();
	}
	SMgr->StepIn(i->lexeme);
	if(i1->type!=_INT&&i1->type!=_CHAR){
	//if(SMgr->pCurrentTable->itemList[m]->type!=i1->type){
		PrintNo();
		cout<<"Cannot convert type "<<DTS::Convert(i1->type)<<" to "<<DTS::Convert(SMgr->pCurrentTable->itemList[m]->type)<<"."<<endl;
		printf("\tArgument's type conflicts with the definition.\n");
	}
	SMgr->StepOut();
	if(temp!="")
		SMgr->StepIn(temp);
}
void Parser::ArguTable_ReversePush(Item* i, Item* item[], int n){
	if(i==NULL){
		PrintNo();
		cout<<"Cannot push arguments due to the previous error(s)."<<endl;
		return ;
	}
	string temp="";
	if(SMgr->pCurrentTable->pParentItem!=NULL){
		temp=SMgr->pCurrentTable->pParentItem->lexeme;
		SMgr->StepOut();
	}
	SMgr->StepIn(i->lexeme);
	if(SMgr->pCurrentTable->pParentItem->dimension!=n){
		PrintNo();
		printf("Argument's number conflicts with the definition,\n");
	}
	else{
		for(int k=n-1;k>=0;k--){
			if(item[k]->kind==INVAR&&item[k]->isAssigned==false){
				PrintNo();
				cout<<item[k]->lexeme<<" is used without initialized.\n";
			}
			QMgr->NewQuad(ARG,item[k]);
		}
	}
	SMgr->StepOut();
	if(temp!="")
		SMgr->StepIn(temp);
}
void Parser::ReadStat_Find(string n, Item*& i){
	if(SMgr->AllCapsCheck(n)!=-1)
		i=SMgr->AllCapsFind(n);
	else{
		PrintNo();
		printf("Undeclared identifier.\n");
		i=NULL;
	}
}
void Parser::ReadStat_Read(Item* i){
	QMgr->NewQuad(RD,i);
	i->isAssigned=true;
}
void Parser::WriteStat_Write(Item* i){
	QMgr->NewQuad(WR,i);
}
void Parser::WriteStat_Write(Item* i1, Item* i2){
	QMgr->NewQuad(WR,i1,i2);
}
void Parser::RetStat_NRet(Item* i){
	if(SMgr->pCurrentTable->pParentItem->type==_VOID){
		PrintNo();
		cout<<"Void function should not have a return value.\n";
	}
	else
		QMgr->NewQuad(NRET,i);
}
void Parser::RetStat_Ret(){
	if(SMgr->pCurrentTable->pParentItem->type!=_VOID){
		PrintNo();
		cout<<"NonVoid function should have a return value.\n";
	}
	QMgr -> NewQuad(RET);
}
void Parser::MainProc_Begin(){
	SMgr -> Insert("a@main", PROC, _VOID, "", 0);
	QMgr -> NewQuad(BEGIN, SMgr -> Find("a@main"));
}
void Parser::MainProc_Stepin(){
	SMgr -> StepIn("a@main");
}
void Parser::MainProc_End(){
	QMgr -> NewQuad(RET);
	QMgr -> NewQuad(END);
}
void Parser::MainProc_Stepout(){
	SMgr -> StepOut();
}
void Parser::Str_Insert(string n, Item*& i){
	i=QMgr->NewSItem(n);
}
int Parser::Stat_Check(string n){
	int res;
	string temp="";
	if(SMgr->pCurrentTable->pParentItem!=NULL){
		temp=SMgr->pCurrentTable->pParentItem->lexeme;
		SMgr->StepOut();
	}
	if(SMgr->AllCapsCheck(n)!=-1){
		if(SMgr->AllCapsFind(n)->kind==FUNC)
			res=1;
		else if(SMgr->AllCapsFind(n)->kind==PROC)
			res=0;
		else{
			PrintNo();
			cout<<"The name "<<SMgr->AllCapsFind(n)->lexeme<<" should be a function."<<endl;
			res=-1;
		}
	}
	else
		res=-1;
	if(temp!="")
		SMgr->StepIn(temp);
	return res;
}

void Parser::PrintNo(){
 	errorcount++;
	cout<<"Error("<<errorcount<<"): (Line "<<lineNo+1<<" Column "<<serialNo+1<<") ";		
}
