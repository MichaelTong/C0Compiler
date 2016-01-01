#include "Lexer.h"
#include "Tag.h"
#include "Type.h"
#include "Num.h"
#include "CompilerC0.h"
#include <sstream>
#include <cType.h>

void Lexer::reserve(Word w){
	words.push_back(w);
}
Lexer::Lexer()
{

}
Lexer::Lexer(FILE *i){
	finish=false;
	peek=' ';
	in=i;
	preSerial=0;
	words=std::vector<Word>();
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

Lexer::~Lexer(){
	
}

void Lexer::readch(int& serialNo){
	peek=fgetc(in);
	serialNo++;
}

bool Lexer::readch(char c,int& lineNo,int& serialNo){
	readch(serialNo);
	if(peek!=c){
		back(lineNo,serialNo);
		peek=' ';
		return false;
	}
	peek=' ';
	return true;
}
void Lexer::back(int& lineNo, int& serialNo){
	__int64 tpos;
	fgetpos(in,&tpos);
	tpos--;
	serialNo--;
	if(serialNo<0)
	{
		lineNo--;
		serialNo=preSerial;
	}
	fsetpos(in,&tpos);
}
Token Lexer::scan(int& lineNo, int& serialNo){
	for(;;readch(serialNo)){
		if(peek==EOF){
			finish=true;
			return Token(-1);
		}
		if(peek==' '||peek=='\t')
			continue;
		else if(peek=='\n'){
			lineNo=lineNo+1;
			preSerial=serialNo;
			serialNo=0;
		}
		else
			break;
	}
	switch(peek){
	case'=': if(readch('=',lineNo,serialNo)) return Word::eql; else return Token('=');
	case'!': if(readch('=',lineNo,serialNo)) return Word::neq; else return Token('!');
	case'<': if(readch('=',lineNo,serialNo)) return Word::leq; else return Word::lss;
	case'>': if(readch('=',lineNo,serialNo)) return Word::geq; else return Word::gre;
	}
	if(isdigit(peek)){
		std::string temp="";
		do{
			std::stringstream t("");
			t<<peek;
			std::string a;
			t>>a;
			temp+=a;
			a.~basic_string();
			readch(serialNo);
		}while(isdigit(peek));
		back(lineNo,serialNo);
		peek=' ';
		if(temp.length()!=1&&temp[0]=='0')
			return Word(temp,Tag::ILLEGALNUM);
		return Num(temp);
	}
	if(isalpha(peek)||peek=='_'){
		std::string temp=("");
		do 
		{
			temp+=peek;
			readch(serialNo);
		} while (isdigit(peek)||isalpha(peek)||peek=='_');
		int i;
		for(i=0;i<words.size();i++){
			if(temp==words[i].toString())
				break;
		}
		back(lineNo,serialNo);
		peek=' ';
		if(i<words.size())
			return words[i];
		Word w=Word(temp,Tag::IDEN);
		words.push_back(w);
		return w;
	}
	if(peek=='\''){
		std::string temp=("");
		readch(serialNo);
		while(peek!='\''&&peek!='\n'&&peek!=EOF)
		{
			temp+=peek;
			readch(serialNo);
		}
		if(peek=='\''){
			peek=' ';
			Word w=Word(temp,Tag::CHARCON);
			return w;
		}
		else if(peek=='\n'){
			readch(serialNo);
			back(lineNo,serialNo);
			peek=' ';
			Word w=Word(temp,Tag::ILLEGALCHA);
			return w;
		}
		else{
			back(lineNo,serialNo);
			peek=' ';
			Word w=Word(temp,Tag::ILLEGALCHA);
			return w;
		}
	}
	if(peek=='"'){
		std::string temp=("");
		readch(serialNo);
		while (peek!='"'&&peek!='\n'&&peek!=EOF)
		{
			temp+=peek;
			readch(serialNo);
		} 
		if(peek=='"'){
			Word w=Word(temp,Tag::STRCON);
			peek=' ';
			return w;
		}
		else if(peek=='\n'){
			readch(serialNo);
			back(lineNo,serialNo);
			Word w=Word(temp,Tag::ILLEGALSTR);
			peek=' ';
			return w;
		}
		else{
			back(lineNo,serialNo);
			Word w=Word(temp,Tag::ILLEGALSTR);
			peek=' ';
			return w;
		}
	}
	Token tok=Token(peek);
	peek=' ';
	return tok;
}