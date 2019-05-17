#ifndef SCANNER_H_
#define SCANNER_H_

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <list>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include "pre_processor.h"
#include "ifequ.h"

int scanner(char * file_name, std::list<Token> & tokenlist, std::list<Token> & labellist);
int identify_tokens (char * s, std::list<Token> & tokenlist);

void verify_tokens (std::list<Token> & tokenlist, std::list<Token> & labellist);
int categorize_token(Token & token, std::list<Token> & labellist, std::list<Token> & tokenlist);

int is_mnemonic(Token & token);
int is_label(Token & token, std::list<Token> & labellist);
int is_directive(Token & token);
int is_operand(Token & token, std::list<Token> & tokenlist);
int is_decimal(Token & token);
int is_hexadecimal(Token & token);

extern int pre_error;

#endif /* SCANNER_H_ */
