#ifndef IFEQU_H_
#define IFEQU_H_

#include <cstdio>
#include <list>
#include "pre_processor.h"
#include "scanner.h"

void ifequ(std::list<Token> & tokenlist, std::list<Token> & labellist);
int equilizer(std::list<Token> & tokenlist, std::list<Token> & labellist);
int conditional_dir(std::list<Token> & tokenlist);
int identify_equ(std::list<Token> & tokenlist, std::list<Token> & labellist);
int solve_equ(std::list<Token> & tokenlist, std::list<Token> & labellist);
int identify_if (std::list<Token> & tokenlist, std::list<Token> & iflist);
int solve_if (std::list<Token> & tokenlist, std::list<Token> & iflist);

extern int pre_error;

#endif
