#include "second_pass.h"

//#define __DEBUG__

void second_pass(list<Token> & tokenlist, list<Symbol> & symboltable, list<int> & object, list<int> & realoc){
	list<Token>::iterator it_tk;
	list<Symbol>::iterator it_sb;
	list<int>::iterator it_ob;
	int i;

	for(it_tk = tokenlist.begin(); it_tk != tokenlist.end(); it_tk++){
		switch(it_tk->type){
			case TT_MNEMONIC:
				realoc.insert(realoc.end(), 0);
				object.insert(object.end(), it_tk->addit_info);
			break;

			case TT_DIRECTIVE:
				switch(it_tk->addit_info){
					case DIR_SPACE:
						it_tk++;
						if(it_tk->type == TT_CONST && it_tk != tokenlist.end()){
							for(i=0; i < it_tk->addit_info; i++){
								realoc.insert(realoc.end(), 0);
								object.insert(object.end(), 0);
							}
						} else {
							realoc.insert(realoc.end(), 0);
							object.insert(object.end(), 0);
						}
					break;

					case DIR_CONST:
						it_tk++;
						if(it_tk->type == TT_CONST){
							realoc.insert(realoc.end(), 0);
							object.insert(object.end(), it_tk->addit_info);
						} else {
							it_tk--;
							cout << "second pass: error!" << endl;
						}
					break;

					//case DIR_PUBLIC:
					//	it_tk++;
					//break;

					default:
					break;
				}
			break;

			case TT_OPERAND:
				for(it_sb = symboltable.begin(); it_sb != symboltable.end(); it_sb++){
					if (it_tk->str == it_sb->str){
						it_tk++;
						if (it_tk->type == TT_PLUS_OPERATOR){
							it_tk++;
							if(it_tk->type == TT_CONST){
								realoc.insert(realoc.end(), 1);
								object.insert(object.end(), it_sb->atrb + it_tk->addit_info);
								break;
							} else {
								it_tk--;
								cout << "second pass: error!" << endl;
								break;
							}
						} else {
							realoc.insert(realoc.end(), 1);
							object.insert(object.end(), it_sb->atrb);
							it_tk--;
							break;
						}
					}
				}
			break;

			default:
			break;
		}
		
		// método heuristicamente comprovado.
		if (it_tk == tokenlist.end()){
			break;
		}
	}


#ifdef __DEBUG__
	cout << "Tamanho da Lista: " << object.size() << endl << "-----------------\n";
	cout << "code:";
	for (it_ob = object.begin(); it_ob != object.end(); it_ob++){
		cout << " " << *it_ob;
	}
	cout << "\n-----------------\n";
#endif
}