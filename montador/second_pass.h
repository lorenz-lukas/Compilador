#ifndef SECOND_PASS_H_
#define SECOND_PASS_H_

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stdlib.h>
#include "pre_processor.h"
#include "first_pass.h"

using namespace std;

void second_pass(list<Token> & tokenlist, list<Symbol> & symboltable,list<int> & object,list<int> & realoc);

#endif /* SECOND_PASS_H_ */