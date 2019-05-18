#include "first_pass.h"

//#define __DEBUG__

void first_pass(list<Token> & tokenlist, list<Symbol> & symboltable){
	list<Token>::iterator it_tk;
	Symbol symbol;
	int count_atrb = 0;

	for (it_tk = tokenlist.begin(); it_tk != tokenlist.end(); it_tk++){
		switch (it_tk->type){
			case TT_MNEMONIC:
			case TT_OPERAND:
				count_atrb++;
			break;

			case TT_DIRECTIVE:
				switch (it_tk->addit_info){
					case DIR_SPACE:
						it_tk++;
						if (it_tk->type == TT_CONST){
							count_atrb += it_tk->addit_info;
						} else {
							count_atrb++;
							it_tk--;
						}
					break;

					case DIR_CONST:
						count_atrb++;
					break;

					default:
					break;
				}
			break;

			case TT_LABEL:
				symbol.str = it_tk->str.substr(0, it_tk->str.length()-1);
				
				symbol.atrb = count_atrb;
				

				symboltable.insert(symboltable.end(), symbol);
			break;

			default:
			break;
		}
	}
}