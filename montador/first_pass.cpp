#include "first_pass.h"

//#define __DEBUG__

void first_pass(list<Token> & tokenlist, list<Symbol> & symboltable, list<Symbol> & dt, list<Symbol> & ut){
	list<Token>::iterator it_tk;
	list<Symbol>::iterator it_sb, it_dt;
	list<string> ext;
	list<string>::iterator it_str;
	Symbol symbol, defin;
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

	for (it_dt = dt.begin(); it_dt != dt.end(); it_dt++){
		for (it_sb = symboltable.begin(); it_sb != symboltable.end(); it_sb++){
			if (it_dt->str == it_sb->str){
				it_dt->atrb = it_sb->atrb;
				break;
			}
		}
	}

	count_atrb = 0;
	for (it_tk = tokenlist.begin(); it_tk != tokenlist.end(); it_tk++){
		switch (it_tk->type){
			case TT_MNEMONIC:
			case TT_OPERAND:
				for (it_str = ext.begin(); it_str != ext.end(); it_str++){
					if (it_tk->str == *it_str){
						symbol.str = it_tk->str;
						symbol.atrb = count_atrb;
						ut.insert(ut.end(), symbol);
					}
				}
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

			default:
			break;
		}
	}

#ifdef __DEBUG__
	cout << "Tamanho da Lista: " << symboltable.size() << endl << "-----------------\n";
	for (it_sb = symboltable.begin(); it_sb != symboltable.end(); it_sb++){
		cout << "symbol: " << it_sb->str << ".. \tatrb: " << it_sb->atrb << endl;
	}
	cout << "-----------------\n";
#endif
}