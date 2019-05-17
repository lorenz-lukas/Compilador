#include "synthesizer.h"
using namespace std;


int synthesizer (list<Token> & tokenlist, list<int> & object, list<int> & realoc, list<Symbol> & dt, list<Symbol> & ut){
	list<Symbol> symboltable;

	first_pass(tokenlist, symboltable, dt, ut);
	second_pass(tokenlist, symboltable, object, realoc);

    return 0;
}
