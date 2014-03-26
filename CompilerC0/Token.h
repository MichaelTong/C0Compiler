#ifndef TOKEN_H
#define TOKEN_H 0
#include <string>
class Token{
public:
	Token(int t);
	Token();
	std::string toString();

public:
	int tag;
	bool word;
	bool type;
	bool num;
	std::string lexeme;//for word,type,num
	int value;//for num
	int width;//for type
};
#endif