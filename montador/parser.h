#ifndef PARSER_H_
#define PARSER_H_

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stdlib.h>
#include "pre_processor.h"

using namespace std;

int parser(list <Token> & tokenlist, list <Token> & labellist);
list<Token>::iterator parser_mnemonic(list <Token> & tokenlist, list<Token>::iterator it);
list<Token>::iterator parser_directive(list <Token> & tokenlist, list<Token>::iterator it);
list<Token>::iterator parser_operand(list <Token> & tokenlist, list<Token>::iterator it);
list<Token>::iterator parser_const(list <Token> & tokenlist, list<Token>::iterator it);
list<Token>::iterator parser_ampersand(list <Token> & tokenlist, list<Token>::iterator it);
list<Token>::iterator parser_label(list <Token> & tokenlist, list<Token>::iterator it);

extern int pre_error;

#endif /* PARSER_H_ */