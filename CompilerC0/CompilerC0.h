#ifndef COMPILERC0_H
#define COMPILERC0_H
#include "Item.h"
#include "Lexer.h"
#include "Num.h"
#include "Parser.h"
#include "SymbolTable.h"
#include "Tag.h"
#include "Token.h"
#include "Type.h"
#include "Word.h"
#include "QuadrupleMgr.h"
#include "SymbolTableMgr.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
class QuadrupleMgr;
class SymbolTableMgr;
class Parser;

static Token lastId;
static Token lastToken;
void main();
#endif
