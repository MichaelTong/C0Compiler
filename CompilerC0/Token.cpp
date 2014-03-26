#include "Token.h"
#include <sstream>

Token:: Token(int t){
	tag=t;
	width=0;
	value=0;
	num=false;
	word=false;
	type=false;
	std::stringstream temp("");
	temp<<(char)tag;
	temp>>lexeme;
}
Token:: Token(){
	tag=0;
}
std::string Token:: toString(){
	return lexeme;
}