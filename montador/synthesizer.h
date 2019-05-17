#ifndef SYNTHESIZER_H_
#define SYNTHESIZER_H_

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "first_pass.h"
#include "second_pass.h"
#include "pre_processor.h"

using namespace std;

int synthesizer (list<Token> & tokenlist, list<int> & object, list<int> & realoc, list<Symbol> & dt, list<Symbol> & ut);

extern int pre_error;

#endif /* SYNTHESIZER_H__ */
