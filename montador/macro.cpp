#include "macro.h"

// at this stage the code is considered correct.

void solve_macro(list<Token> & tokenlist){
	list<Macro> macrolist; 

	create_macrolist(tokenlist, macrolist);
	expand_macro(tokenlist, macrolist);
	erase_macro(tokenlist);

}

void create_macrolist(list<Token> & tokenlist, list<Macro> & macrolist){
	list<Token>::iterator it_tk, tmp_tk;
	list<Macro>::iterator it_mc, tmp_mc;
	Macro macro;
	int i, j;

	it_tk = tokenlist.begin();
	it_mc = macrolist.begin();
	while (it_tk != tokenlist.end()){		// go through entire token list.
		if (it_tk->type == TT_DIRECTIVE && it_tk->addit_info == DIR_MACRO){	// is macro?
			it_tk--;
			macro.str = it_tk->str.substr(0, it_tk->str.length()-1);		// store macro label.
			
			i = it_tk->line_number;
			j = 0;
			while (i == it_tk->line_number){		// store arg label.
				if (it_tk->type == TT_OPERAND){
					macro.arg[j] = it_tk->str;
					j++;
				}

				it_tk++;
			}
			macro.argn = j;				// store number of arg in macro.
			tmp_tk = it_tk;
			tmp_tk--;
			macro.it_start = tmp_tk;		// store body beginning point;
			i = 0;						// store body length
			while (it_tk->type != TT_DIRECTIVE && it_tk->addit_info != DIR_END){
				i++;
				it_tk++;
			}
			macro.body_length = i;

			macrolist.insert(macrolist.end(), macro);		// store macro.
		}

		it_tk++;
	}

}

void expand_macro(list<Token> & tokenlist, list<Macro> & macrolist){
	list<Token>::iterator it_tk, tmp_tk, it_aux_tk;
	list<Macro>::iterator it_mc, tmp_mc;
	Token token, aux;
	Macro macro;
	int i, j, first, macro_space;

	it_tk = tokenlist.begin();
	it_mc = macrolist.begin();
	macro_space = 0;
	while (it_tk != tokenlist.end()){		// go through entire token list.
		if(it_tk->type == TT_OPERAND  && !macro_space){		// check if macro operand.
			for(it_mc = macrolist.begin(); it_mc != macrolist.end(); it_mc++){
				if (it_tk->str == it_mc->str){		// is macro.
					tmp_tk = it_tk;
					i = it_tk->line_number;
					j = 0;
					it_tk++;
					while (i == it_tk->line_number){		// store parameters label.
						if (it_tk->type == TT_OPERAND){
							it_mc->param[j] = it_tk->str;
							j++;
						}

						it_tk++;
					}

					it_tk = tmp_tk;
					while (i == it_tk->line_number){		// erase calling line.
						it_tk = tokenlist.erase(it_tk);
					}

					tmp_tk = it_mc->it_start;
					first = 1;
					for (tmp_tk++; (tmp_tk->type != TT_DIRECTIVE) || (tmp_tk->addit_info != DIR_END); tmp_tk++){		// expand macro.
						token = *tmp_tk;
						aux = token;		// safety to not change the original code.

						if (tmp_tk->type == TT_OPERAND){				// if operand check if is arg.
							for(j = 0; j < it_mc->argn; j++){			// check if all args.
								if (tmp_tk->str == it_mc->arg[j]){		// is arg.
									tmp_tk--;
									if (tmp_tk->type == TT_AMPERSAND_OPERATOR){		// check the argument indicator
										it_tk--;
										it_tk = tokenlist.erase(it_tk);
										aux.str = it_mc->param[j];			// put param in place of arg.
									}
									tmp_tk++;
								}
							}
						}

						if (first){		// if first.
							it_aux_tk = tokenlist.insert(it_tk, aux);
							first = 0;
						} else {
							tokenlist.insert(it_tk, aux);
						}
						//tmp_tk++;		// advance expansion.
					}

					it_tk = it_aux_tk;		// repositions to the first new element.
					it_tk--;		// compensation.
					break; 		// get out of for.
				}
			}
		} else
		if (it_tk->type == TT_DIRECTIVE && it_tk->addit_info == DIR_MACRO){		// entering macro definition space.
			macro_space = 1;
		} else
		if (it_tk->type == TT_DIRECTIVE && it_tk->addit_info == DIR_END){		// exiting macro definition space.
			macro_space = 0;
		}

		it_tk++;
	}

}

void erase_macro(list<Token> & tokenlist){
	list<Token>::iterator it_tk, tmp_tk;

	it_tk = tokenlist.begin();
	while (it_tk != tokenlist.end()){		// go through entire token list.
		if (it_tk->type == TT_DIRECTIVE && it_tk->addit_info == DIR_MACRO){
			it_tk--;		// get the label.
			while((it_tk->type != TT_DIRECTIVE) || (it_tk->addit_info != DIR_END)){
				it_tk = tokenlist.erase(it_tk);
			}
			it_tk = tokenlist.erase(it_tk);
			it_tk--;		// compensation.
		}

		it_tk++;
	}
}