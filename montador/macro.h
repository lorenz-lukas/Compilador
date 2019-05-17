#ifndef MACRO_H_
#define MACRO_H_

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stdlib.h>
#include "pre_processor.h"


using namespace std;

/*--------------STRUCTS---------------*/
struct Macro {
    // MNT.
    string str;
    int argn;
    string arg[4];
    
    // MDT.
    int body_length;
    list<Token>::iterator it_start;

    // argx <- paramx.
    string param[4];
};
typedef struct Macro Macro;
/*------------------------------------*/

void solve_macro(list<Token> & tokenlist);
void create_macrolist(list<Token> & tokenlist, list<Macro> & macrolist);
void expand_macro(list<Token> & tokenlist, list<Macro> & macrolist);
void erase_macro(list<Token> & tokenlist);

extern int pre_error;

#endif /* MACRO_H_ */