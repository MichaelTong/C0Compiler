#ifndef LEXER_H
#define LEXER_H
#include "Word.h"
#include<vector>
class Lexer{
public:
	bool finish;
	std::vector<Word> words;
	FILE *in;
private:
	char peek;
	int preSerial;

public:
	Lexer();
	Lexer(FILE *in);
	~Lexer();
	void reserve(Word w);
	void readch(int& serialNo);
	bool readch(char c,int& lineNo,int& serialNo);
	void back(int& lineNo, int& serialNo);
	Token scan(int& lineNo,int& serialNo);
};
#endif