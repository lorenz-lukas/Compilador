#include "parser.h"

int parser (list <Token> & tokenlist, list <Token> & labellist){
	list<Token>::iterator it;

	it = tokenlist.begin();
	while (it != tokenlist.end()){		// Check key tokens.
		switch (it->type){
			case TT_MNEMONIC:
				it = parser_mnemonic(tokenlist, it);
			break;

			case TT_LABEL:
				it = parser_label(tokenlist, it);
			break;

			case TT_DIRECTIVE:
				it = parser_directive(tokenlist, it);
			break;

			case TT_OPERAND:
				it = parser_operand(tokenlist, it);
			break;
			
			case TT_COMMA_OPERATOR:
			case TT_PLUS_OPERATOR:
			case TT_CONST:
				it = parser_const(tokenlist, it);
			break;

			case TT_AMPERSAND_OPERATOR:
				it = parser_ampersand(tokenlist, it);
			break;

			default:
				cerr << "Parser: unknowm token type (" << it->str << ")." << endl;
				mark_sintax_error(tokenlist,it);
				pre_error = 1;
				it->flag = -1;
				it++;
			break;
		}
	}

	return 0;
}

list<Token>::iterator parser_mnemonic(list <Token> & tokenlist, list<Token>::iterator it){
	int target_line;

	target_line = it->line_number;
	switch (it->addit_info){
		case OP_ADD:
		case OP_SUB:
		case OP_MULT:
		case OP_DIV:
		case OP_JMP:
		case OP_JMPN:
		case OP_JMPP:
		case OP_JMPZ:
		case OP_LOAD:
		case OP_STORE:
		case OP_INPUT:
		case OP_OUTPUT:
			it++;
			if (it != tokenlist.end() && target_line == it->line_number){			// check if argument exists.
				if (it->type == TT_AMPERSAND_OPERATOR){		// ignores argument indicator.
					it++;
					if (it == tokenlist.end() || target_line != it->line_number){
						mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
					}
				}
				if (it->type == TT_OPERAND){										// check if valid argument.
					it++;
					if (it != tokenlist.end() && target_line == it->line_number){	// check extra operation.
						if (it->type == TT_PLUS_OPERATOR){											// check if valid operation +.
							it++;
							if (it != tokenlist.end() && target_line == it->line_number){			// check if + argument exist.
								if (it->type == TT_CONST){											// check if + valid argument.
									it++;
									if (it != tokenlist.end() && target_line == it->line_number){	// check if too much arguments.
										cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
										mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
										do {		// get out of line.
											it++;
										} while(it != tokenlist.end() && target_line == it->line_number);
									}
								} else {
									cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
									mark_sintax_error(tokenlist,it);
									pre_error = 1;
									it->flag = -1;
									do {		// get out of line.
										it++;
									} while(it != tokenlist.end() && target_line == it->line_number);
								}
							} else {
								cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
								mark_sintax_error(tokenlist,it);
								pre_error = 1;
								it->flag = -1;
							}
						} else {
							cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
							mark_sintax_error(tokenlist,it);
							pre_error = 1;
							it->flag = -1;
							do {		// get out of line.
								it++;
							} while(it != tokenlist.end() && target_line == it->line_number);
						}
					}
				} else {
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					mark_sintax_error(tokenlist,it);
					pre_error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
				}
			} else {
				cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
				mark_sintax_error(tokenlist,it);
				pre_error = 1;
				it->flag = -1;
			}
		break;

		case OP_COPY:
			it++;
			if (it != tokenlist.end() && target_line == it->line_number){	// check if arguments exists.

				// first argument.
				if (it->type == TT_AMPERSAND_OPERATOR){		// ignores argument indicator.
					it++;
					if (it == tokenlist.end() || target_line != it->line_number){
						mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
					}
				}
				if (it->type == TT_OPERAND){										// check if valid argument.
					it++;
					if (it != tokenlist.end() && target_line == it->line_number){	// check if next argument exist.
						if (it->type == TT_PLUS_OPERATOR){									// check if it is + operation.
							it++;
							if (it != tokenlist.end() && target_line == it->line_number){	// check if + argument exist.
								if (it->type == TT_CONST){									// check if + valid argument.
									it++;
									if (it == tokenlist.end() || target_line != it->line_number){	// check if next argument exist.
										cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
										mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
										break;		// get out of switch.
									}
								} else {
									cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
									mark_sintax_error(tokenlist,it);
									pre_error = 1;
									it->flag = -1;
									do {		// get out of line.
										it++;
									} while(it != tokenlist.end() && target_line == it->line_number);
									break;		// get out of switch.
								}
							} else {
								cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
								mark_sintax_error(tokenlist,it);
								pre_error = 1;
								it->flag = -1;
								break;		// get out of switch.
							}
						}
					} else {
						cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
						mark_sintax_error(tokenlist,it);
						pre_error = 1;
						it->flag = -1;
						break;		// get out of switch.
					}
				} else {
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					mark_sintax_error(tokenlist,it);
					pre_error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
					break;		// get out of switch.
				}

				// comma argument.
				if (it->type == TT_COMMA_OPERATOR){									// check if valid argument.
					it++;
					if (it == tokenlist.end() || target_line != it->line_number){	// check if next argument exist.
						cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
						mark_sintax_error(tokenlist,it);
						pre_error = 1;
						it->flag = -1;
						break;		// get out of switch.
					}
				} else {
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					mark_sintax_error(tokenlist,it);
					pre_error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
					break;
				}

				// second argument.
				if (it->type == TT_AMPERSAND_OPERATOR){		// ignores argument indicator.
					it++;
					if (it == tokenlist.end() || target_line != it->line_number){
						mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
					}
				}
				if (it->type == TT_OPERAND){										// check if valid argument.
					it++;
					if (it != tokenlist.end() && target_line == it->line_number){	// check extra operation.
						if (it->type == TT_PLUS_OPERATOR){											// check if valid operation +.
							it++;
							if (it != tokenlist.end() && target_line == it->line_number){			// check if + argument exist.
								if (it->type == TT_CONST){											// check if + valid argument.
									it++;
									if (it != tokenlist.end() && target_line == it->line_number){	// check if too much arguments.
										cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
										mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
										do {		// get out of line.
											it++;
										} while(it != tokenlist.end() && target_line == it->line_number);
									}
								} else {
									cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
									mark_sintax_error(tokenlist,it);
									pre_error = 1;
									it->flag = -1;
									do {		// get out of line.
										it++;
									} while(it != tokenlist.end() && target_line == it->line_number);
								}
							} else {
								cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
								mark_sintax_error(tokenlist,it);
								pre_error = 1;
								it->flag = -1;
							}
						} else {
							cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
							mark_sintax_error(tokenlist,it);
							pre_error = 1;
							it->flag = -1;
							do {		// get out of line.
								it++;
							} while(it != tokenlist.end() && target_line == it->line_number);
						}
					}
				} else {
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					mark_sintax_error(tokenlist,it);
					pre_error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
				}

			} else {
				cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
				mark_sintax_error(tokenlist,it);
				pre_error = 1;
				it->flag = -1;
			}
		break;

		case OP_STOP:
			it++;
			if (it != tokenlist.end() && target_line == it->line_number){		// check if argument exist.
				cerr << "Sintax Error @ Line " << target_line << " - unexpected argument." << endl;
				mark_sintax_error(tokenlist,it);
				pre_error = 1;
				it->flag = -1;
				do {		// get out of line.
					it++;
				} while(it != tokenlist.end() && target_line == it->line_number);
			}
		break;

		default:
			cerr << "Parser: unknowm mnemonic token (" << it->str << ")." << endl;
			mark_sintax_error(tokenlist,it);
			pre_error = 1;
			it->flag = -1;
			it++;
		break;
	}

	return it;
}

list<Token>::iterator parser_directive(list <Token> & tokenlist, list<Token>::iterator it){
	int target_line;

	target_line = it->line_number;
	switch (it->addit_info){
		case DIR_SECTION:
			it++;
			if (it != tokenlist.end() && target_line == it->line_number){										// check if arguments exists.
				if (it->type == TT_DIRECTIVE && (it->addit_info == DIR_TEXT || it->addit_info == DIR_DATA)){	// check if argument is valid.
					it++;
					if (it != tokenlist.end() && target_line == it->line_number){								// check if too much arguments.
						cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
						mark_sintax_error(tokenlist,it);
						pre_error = 1;
						it->flag = -1;
						do {		// get out of line.
							it++;
						} while(it != tokenlist.end() && target_line == it->line_number);
					}
				} else {
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					mark_sintax_error(tokenlist,it);
					pre_error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
				}
			} else {
				cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
				mark_sintax_error(tokenlist,it);
				pre_error = 1;
				it->flag = -1;
			}
		break;

		case DIR_SPACE:
			it++;
			if (it != tokenlist.end() && target_line == it->line_number){			// check if argument exist.
				if (it->type == TT_CONST && it->addit_info > 0){											// check if argument is valid
					it++;
					if (it != tokenlist.end() && target_line == it->line_number){	// check if too much arguments.
						cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
						mark_sintax_error(tokenlist,it);
						pre_error = 1;
						it->flag = -1;
						do {		// get out of line.
							it++;
						} while(it != tokenlist.end() && target_line == it->line_number);
					}
				} else {
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					mark_sintax_error(tokenlist,it);
					pre_error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
				}
			}
		break;

		case DIR_CONST:
			it++;
			if (it != tokenlist.end() && target_line == it->line_number){			// check if argument exist.
				if (it->type == TT_CONST){											// check if argument is valid
					it++;
					if (it != tokenlist.end() && target_line == it->line_number){	// check if too much arguments.
						cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
						mark_sintax_error(tokenlist,it);
						pre_error = 1;
						it->flag = -1;
						do {		// get out of line.
							it++;
						} while(it != tokenlist.end() && target_line == it->line_number);
					}
				} else {
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					mark_sintax_error(tokenlist,it);
					pre_error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
				}
			} else {
				cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
				mark_sintax_error(tokenlist,it);
				pre_error = 1;
				it->flag = -1;
			}
		break;

		case DIR_IF:
			it++;
			if (it != tokenlist.end() && target_line == it->line_number){			// check if argument exist.
				if (it->type == TT_AMPERSAND_OPERATOR){		// ignores argument indicator.
					it++;
					if (it == tokenlist.end() || target_line != it->line_number){
						mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
					}
				}
				if (it->type == TT_CONST || it->type == TT_OPERAND){				// check if argument is valid
					it++;
					if (it != tokenlist.end() && target_line == it->line_number){	// check if too much arguments.
						cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
						mark_sintax_error(tokenlist,it);
						pre_error = 1;
						it->flag = -1;
						do {		// get out of line.
							it++;
						} while(it != tokenlist.end() && target_line == it->line_number);
					}
				} else {
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					mark_sintax_error(tokenlist,it);
					pre_error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
				}
			} else {
				cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
				mark_sintax_error(tokenlist,it);
				pre_error = 1;
				it->flag = -1;
			}
		break;

		case DIR_MACRO:
			it++;
			// first argument.
			if (it != tokenlist.end() && target_line == it->line_number){		// check if argument exist.
				if (it->type == TT_AMPERSAND_OPERATOR){							// check argument operator.
					it++;
					if (it != tokenlist.end() && target_line == it->line_number){	// check if argument is present.
						if (it->type == TT_OPERAND){								// check if argument is valid.
							it++;
						} else {
							mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
							cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
							do {		// get out of line.
								it++;
							} while(it != tokenlist.end() && target_line == it->line_number);
							break;
						}
					} else {
						mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
						break;
					}
				} else {
					mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
					break;
				}
			} else {
				break;
			}

			// second argument.
			if (it != tokenlist.end() && target_line == it->line_number){		// check if argument exist.
				if (it->type == TT_COMMA_OPERATOR){								// check argument separator indicator.
					it++;
					if (it == tokenlist.end() || target_line != it->line_number){
						mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
						break;
					}
				} else {
					mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
					break;
				}
				if (it->type == TT_AMPERSAND_OPERATOR){							// check argument operator.
					it++;
					if (it != tokenlist.end() && target_line == it->line_number){	// check if argument is present.
						if (it->type == TT_OPERAND){								// check if argument is valid.
							it++;
						} else {
							mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
							cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
							do {		// get out of line.
								it++;
							} while(it != tokenlist.end() && target_line == it->line_number);
							break;
						}
					} else {
						mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
						break;
					}
				} else {
					mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
					break;
				}
			} else {
				break;
			}

			// third argument.
			if (it != tokenlist.end() && target_line == it->line_number){		// check if argument exist.
				if (it->type == TT_COMMA_OPERATOR){								// check argument separator indicator.
					it++;
					if (it == tokenlist.end() || target_line != it->line_number){
						mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
						break;
					}
				} else {
					mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
					break;
				}
				if (it->type == TT_AMPERSAND_OPERATOR){							// check argument operator.
					it++;
					if (it != tokenlist.end() && target_line == it->line_number){	// check if argument is present.
						if (it->type == TT_OPERAND){								// check if argument is valid.
							it++;
						} else {
							mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
							cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
							do {		// get out of line.
								it++;
							} while(it != tokenlist.end() && target_line == it->line_number);
							break;
						}
					} else {
						mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
						break;
					}
				} else {
					mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
					break;
				}
			} else {
				break;
			}

			if (it != tokenlist.end() && target_line == it->line_number){		// check if too much arguments.
				mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
				cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
				do {		// get out of line.
					it++;
				} while(it != tokenlist.end() && target_line == it->line_number);
			}
		break;

		case DIR_END:
			it++;
			if (it != tokenlist.end() && target_line == it->line_number){		// check if argument exist.
				mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
				cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
				do {		// get out of line.
					it++;
				} while(it != tokenlist.end() && target_line == it->line_number);
			}
		break;

		case DIR_TEXT:
		case DIR_DATA:
			cerr << "Sintax Error @ Line " << target_line << " - invalid use of directive." << endl;
			mark_sintax_error(tokenlist,it);
			pre_error = 1;
			it->flag = -1;
			do {		// get out of line.
				it++;
			} while(it != tokenlist.end() && target_line == it->line_number);
		break;

		case DIR_EQU:	// if found in this stage, it must be an invalid EQU.
			do {		// get out of line.
				it++;
			} while(it != tokenlist.end() && target_line == it->line_number);
		break;

		/*case DIR_BEGIN:	
		case DIR_EXTERN:
			it--;
			if (it->type == TT_LABEL) {		// check if have label.
				it++;
				it++;

				if (it != tokenlist.end() && target_line == it->line_number){		// check if argument exist.
					cerr << "Sintax Error @ Line " << target_line << " - unexpected argument." << endl;
					mark_sintax_error(tokenlist,it);
					pre_error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
				}
			} else {
				it++;

				cerr << "Sintax Error @ Line " << target_line << " - invalid use of directive." << endl;
				mark_sintax_error(tokenlist,it);
				pre_error = 1;
				it->flag = -1;
				do {		// get out of line.
					it++;
				} while(it != tokenlist.end() && target_line == it->line_number);
			}
		break;

		case DIR_END:	
			it++;

			if (it != tokenlist.end() && target_line == it->line_number){		// check if argument exist.
				cerr << "Sintax Error @ Line " << target_line << " - unexpected argument." << endl;
				mark_sintax_error(tokenlist,it);
				pre_error = 1;
				it->flag = -1;
				do {		// get out of line.
					it++;
				} while(it != tokenlist.end() && target_line == it->line_number);
			}
		break;

		case DIR_PUBLIC:
			it++;

			if (it != tokenlist.end() && target_line == it->line_number){	// check if argument is present.
				if (it->type == TT_OPERAND){								// check if argument is valid.
					it++;

					if (it != tokenlist.end() && target_line == it->line_number){		// check if argument exist.
						cerr << "Sintax Error @ Line " << target_line << " - unexpected argument." << endl;
						mark_sintax_error(tokenlist,it);
						pre_error = 1;
						it->flag = -1;
						do {		// get out of line.
							it++;
						} while(it != tokenlist.end() && target_line == it->line_number);
					}
				} else {
					mark_sintax_error(tokenlist,it);
								pre_error = 1;
								it->flag = -1;
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
					break;
				}
			} else {
				mark_sintax_error(tokenlist,it);
								pre_error = 1;
								it->flag = -1;
				cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
				break;
			}
		break;*/

		default:
			cerr << "Parser: unknowm directive token (" << it->str << ")." << endl;
			mark_sintax_error(tokenlist,it);
			pre_error = 1;
			it->flag = -1;
			it++;
		break;
	}

	return it;
}


list<Token>::iterator parser_operand(list <Token> & tokenlist, list<Token>::iterator it){
	int target_line;
	target_line = it->line_number;

	// expects to be a macro calling.
	it++;
	// first argument.
	if (it != tokenlist.end() && target_line == it->line_number){		// check if argument exist.
		if (it->type == TT_AMPERSAND_OPERATOR){		// ignores argument indicator.
			it++;
			if (it == tokenlist.end() || target_line != it->line_number){
				return it;
			}
		}
		if (it->type == TT_OPERAND){									// check if argument is valid.
			it++;
		} else {
			mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
			cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
			do {		// get out of line.
				it++;
			} while(it != tokenlist.end() && target_line == it->line_number);
			return it;
		}
	} else {
		return it;
	}

	// second argument.
	if (it != tokenlist.end() && target_line == it->line_number){		// check if argument exist.
		if (it->type == TT_COMMA_OPERATOR){							// check argument separator operator.
			it++;
			if (it != tokenlist.end() && target_line == it->line_number){	// check if argument is present.
				if (it->type == TT_AMPERSAND_OPERATOR){		// ignores argument indicator.
					it++;
					if (it == tokenlist.end() || target_line != it->line_number){
						return it;
					}
				}
				if (it->type == TT_OPERAND){								// check if argument is valid.
					it++;
				} else {
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					mark_sintax_error(tokenlist,it);
					pre_error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
					return it;
				}
			} else {
				mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
				cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
				return it;
			}
		} else {
			cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
			mark_sintax_error(tokenlist,it);
			pre_error = 1;
			it->flag = -1;
			do {		// get out of line.
				it++;
			} while(it != tokenlist.end() && target_line == it->line_number);
			return it;
		}
	} else {
		return it;
	}

	// third argument.
	if (it != tokenlist.end() && target_line == it->line_number){		// check if argument exist.
		if (it->type == TT_COMMA_OPERATOR){							// check argument separator operator.
			it++;
			if (it != tokenlist.end() && target_line == it->line_number){	// check if argument is present.
				if (it->type == TT_AMPERSAND_OPERATOR){		// ignores argument indicator.
					it++;
					if (it == tokenlist.end() || target_line != it->line_number){
						return it;
					}
				}
				if (it->type == TT_OPERAND){								// check if argument is valid.
					it++;
				} else {
					mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
					cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
					do {		// get out of line.
						it++;
					} while(it != tokenlist.end() && target_line == it->line_number);
					return it;
				}
			} else {
				mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
				cerr << "Sintax Error @ Line " << target_line << " - missing argument." << endl;
				return it;
			}
		} else {
			mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
			cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
			do {		// get out of line.
				it++;
			} while(it != tokenlist.end() && target_line == it->line_number);
			return it;
		}
	} else {
		return it;
	}

	if (it != tokenlist.end() && target_line == it->line_number){		// check if too much arguments.
		mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
		cerr << "Sintax Error @ Line " << target_line << " - invalid argument." << endl;
		do {		// get out of line.
			it++;
		} while(it != tokenlist.end() && target_line == it->line_number);
	}
	return it;
}

list<Token>::iterator parser_const(list <Token> & tokenlist, list<Token>::iterator it){
	int target_line;
	target_line = it->line_number;

 cerr << "Sintax Error @ Line " << target_line << " - unexpected value." << endl;
 mark_sintax_error(tokenlist,it);
 pre_error = 1;
 it->flag = -1;
	do {		// get out of line.
		it++;
	} while(it != tokenlist.end() && target_line == it->line_number);

	return it;
}

list<Token>::iterator parser_ampersand(list <Token> & tokenlist, list<Token>::iterator it){
	int target_line;
	target_line = it->line_number;

	it++;
	if (it == tokenlist.end() || target_line != it->line_number){
		mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
		cerr << "Sintax Error @ Line " << target_line << " - ." << endl;
	}

	return it;
}

list<Token>::iterator parser_label(list <Token> & tokenlist, list<Token>::iterator it){
	int target_line;
	target_line = it->line_number;

	it++;
	if (it == tokenlist.end() || target_line != it->line_number){
		mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
		cerr << "Sintax Error @ Line " << target_line << " - empty label." << endl;
	} else {
		if (it->type == TT_LABEL){
			mark_sintax_error(tokenlist,it);
										pre_error = 1;
										it->flag = -1;
			cerr << "Sintax Error @ Line " << target_line << " - multiple labels in line." << endl;
			do {		// get out of line.
				it++;
			} while(it != tokenlist.end() && target_line == it->line_number);
		}
	}

	return it;
}