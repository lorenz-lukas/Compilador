#ifndef FIRST_PASS_H_
#define FIRST_PASS_H_

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stdlib.h>
#include "pre_processor.h"

using namespace std;

/*--------------STRUCTS---------------*/
struct Symbol {
    string str;
    int atrb;
};
typedef struct Symbol Symbol;
/*------------------------------------*/

void first_pass(list<Token> & tokenlist, list<Symbol> & symboltable);

#endif /* FIRST_PASS_H_ */