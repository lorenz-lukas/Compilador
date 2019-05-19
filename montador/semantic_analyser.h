#ifndef SEMANTIC_H_
#define SEMANTIC_H_

#include <cstdio>
#include <iostream>
#include <list>
#include <string>
#include "pre_processor.h"

void semantic_analyser(std::list <Token> & tokenlist, std::list <Token> & labellist);
int duplicate_label (std::list <Token> & labellist);
int section_placement (std::list <Token> & tokenlist,  std::list<Token>::iterator & data, int & hasdatasec);
int check_symbols_from_data(std::list <Token> & tokenlist, std::list<Token>::iterator data_it);
int check_for_data_need(std::list <Token> & tokenlist);
int defaslabel(std::list<Token> & tokenlist, std::list<Token>::iterator data_it);
int nolabel(std::list<Token> & tokenlist, std::list<Token>::iterator data_it);
int invalid_label(std::list<Token> & tokenlist, std::list<Token>::iterator data_it);
int wrong_section(std::list<Token> & tokenlist, std::list<Token>::iterator data_it);
int labelexist(std::list <Token> & tokenlist, std::list <Token> & labellist);

extern int pre_error;
extern int solo;

#endif /* SEMANTIC_H_ */
