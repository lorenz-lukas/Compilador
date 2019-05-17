#include "ifequ.h"
#include "stdio.h"
using namespace std;

/*
*   IFEQU.CPP
*   Solves the directives "IF" and "EQU" from the assembly code.
*   First it solves for "EQU", uzing the equilizer() function, then solves for "IF" with the conditional_dir() function.
*   Input: tokenlist from scanner.
*   output: tokenlist solved for IF and EQU directives.
*/


void ifequ(list<Token> & tokenlist, list<Token> & labellist){
    equilizer(tokenlist, labellist);
    conditional_dir(tokenlist);

}


int equilizer(list<Token> & tokenlist, list<Token> & labellist){
    if (identify_equ(tokenlist, labellist) == 0)
        solve_equ(tokenlist, labellist);
        
    return 0;
}

int identify_equ(list<Token> & tokenlist, list<Token> & labellist){
    list<Token>::iterator t_it, t_newit, aux, aux2, l_it;
    string label, value, delimiter = ":";
    int i, ad_info, type, equ_error = 0,section=0;
    for (t_it = tokenlist.begin();t_it != tokenlist.end(); t_it++){     //scans all the tokenlist
        if (t_it->type == TT_DIRECTIVE && t_it->addit_info == DIR_SECTION) {
            section = 1; // if it's "section"
            cout << "passou para alguma section, flag: " << section << endl;
        }
        if (t_it->type == TT_DIRECTIVE && t_it->addit_info == DIR_EQU){     //if it's "EQU"
            
            if(section) { //if it's "EQU" after any "section"
                cout << "reconheceu erro: " << section << endl;
                fprintf(stderr, "Sintax error @ line %d - 'EQU' directive declared after TEXT oder DATA section .\n", t_it->line_number);
                mark_sintax_error(tokenlist,t_it);
                pre_error = 1;
                equ_error = 1;
                break;
            }

            t_newit = t_it;
            t_newit --;
            if (t_it != tokenlist.begin()){      //checks if it is the first token
                    if (t_newit->type == TT_LABEL){     //checks if previous is label
                        label = t_newit->str.substr(0, t_newit->str.find(delimiter));
                    }else{                              //if it's not a label marks as sintax error
                        fprintf(stderr, "Sintax error @ line %d - Missing label for 'EQU' directive.\n", t_it->line_number);
                        mark_sintax_error(tokenlist,t_it);
                        pre_error = 1;
                        equ_error = 1;
                        break;
                    }
            }else{      //if it's the first token of the tokenlist
                fprintf(stderr, "Sintax error @ line %d - Missing label for 'EQU' directive.\n", t_it->line_number);
                mark_sintax_error(tokenlist,t_it);
                pre_error = 1;
                equ_error = 1;
                break;
            }
            advance(t_newit,2);
            aux = t_newit;
            aux++;
            if (t_newit != tokenlist.end()){        //checks if it is the last token
                if (t_newit->line_number == t_it->line_number){     //checks if there's an argument for "EQU"
                    if (aux != tokenlist.end()){
                        if (aux->line_number != t_it->line_number){     //checks if there's only one token after "EQU"
                            value = t_newit->str;       //copies information
                            type = t_newit->type;
                            ad_info = t_newit->addit_info;
                            t_it--;
                            i = t_newit->line_number;
                            while (t_it->line_number <= i){  //erases "EQU" line 
                                t_it = tokenlist.erase(t_it);
                            }
                            t_newit = aux;
                            t_it = aux;
                        }else{
                            fprintf(stderr, "Sintax error @ line %d - Too many arguments for 'EQU' directive.\n", t_it->line_number);
                            mark_sintax_error(tokenlist,t_it);
                            pre_error = 1;
                            equ_error = 1;
                            break;
                        }
                    }else{
                        value = t_newit->str;
                        type = t_newit->type;
                        ad_info = t_newit->addit_info;
                        t_it--;
                        i = t_newit->line_number;
                        while (t_it->line_number <= i){  //erases "EQU" line 
                            t_it = tokenlist.erase(t_it);
                        }
                        t_newit = aux;
                        t_it = aux;
                    }
                }else{                      ////if there's no argument marks as sintax error
                    fprintf(stderr, "Sintax error @ line %d - Missing argument for 'EQU' directive.\n", t_it->line_number);
                    mark_sintax_error(tokenlist,t_it);
                    pre_error = 1;
                    equ_error = 1;
                    break;
                }
            }else{                      ////if there's no argument marks as sintax error
                fprintf(stderr, "Sintax error @ line %d - Missing argument for 'EQU' directive.\n", t_it->line_number);
                mark_sintax_error(tokenlist,t_it);
                pre_error = 1;
                equ_error = 1;
                break;
            }
            l_it = labellist.begin();
            while (l_it != labellist.end() && l_it->str != label){      //search in labelist for the label
                l_it++;
            }
            if (l_it == labellist.end()){       //terminates program if there isn't such a label
                fprintf(stderr, "[ERROR]: Malformed label list. Program terminated.\n");
                exit(EXIT_FAILURE);
            }else{
                l_it->info_str = value;     //if there is, stores the argument value in the labellist token
                l_it->type = type;
                l_it->addit_info = ad_info;
                l_it->flag = DIR_EQU;
            }
        }
    }
    return equ_error;
}

int solve_equ (list<Token> & tokenlist, list<Token> & labellist){
    list<Token>::iterator l_it, t_it;
    for (l_it = labellist.begin(); l_it != labellist.end(); l_it++){    //scans whole list of labels
        if (l_it->flag == DIR_EQU){       //flag indicates "EQU"
            for (t_it = tokenlist.begin(); t_it != tokenlist.end(); t_it++){    //scans whole tokenlist
                if (t_it->str == l_it->str){
                    t_it->str = l_it->info_str;     //substitutes content
                    t_it->addit_info = l_it->addit_info;
                    t_it->type = l_it->type;
                    t_it->flag = l_it->flag;
                }
            }
        }
    }
    return 0;

}


int conditional_dir(list<Token> & tokenlist){
    list<Token> iflist;
    if (identify_if(tokenlist, iflist) == 0)
        solve_if(tokenlist, iflist);            //solve if no error is found
    return 0;
}

int identify_if (list<Token> & tokenlist, list<Token> & iflist){
    list<Token>::iterator it, newit;
    int if_err = 0;
    for (it = tokenlist.begin(); it != tokenlist.end(); it++){  //for whole tokenlist
        if(it->type == TT_DIRECTIVE && it->addit_info == DIR_IF){   //if "IF" directive
            it++;
            newit = it;     //sintatic/semanitc analisys
            it--;
            if (it->line_number == newit->line_number){     //does "IF" have one argument?
                if(newit->type == TT_CONST){ //Is it a constant?
                    newit++;
                    if (it->line_number != newit->line_number){ //does it have ONLY one argument?
                        iflist.push_back(*it);
                    }else{
                        fprintf(stderr, "Sintax error @ line %d - Too many arguments for 'IF' directive.\n", newit->line_number);
                        mark_sintax_error(tokenlist,newit);
                        if_err = 1;
                        pre_error = 1;
                    }
                }else{
                    fprintf(stderr, "Semantic error @ line %d - Expected CONST type after 'IF' statement (%s).\n", newit->line_number, newit->str.c_str());
                    if_err = 1;
                    pre_error = 1;
                }
            }else{
                fprintf(stderr, "Sintax error @ line %d - Malformed 'IF' statement - Expected CONST type.\n", it->line_number);
                mark_sintax_error(tokenlist, it);
                if_err = 1;
                pre_error = 1;
            }

        }
    }
    
    return if_err;
}

int solve_if (list<Token> & tokenlist, list<Token> & iflist){
        list<Token>::iterator it, aux, newit = tokenlist.begin();
        int i;
        Token token;
        for (it = iflist.begin(); it != iflist.end(); it++){    //for all the IT->"IF" statements
            for (newit = tokenlist.begin(); newit != tokenlist.end(); newit++){     //for whole NEWIT->tokenlist
                if (newit->line_number == it->line_number && newit->addit_info == DIR_IF){
                    aux = newit;
                    aux++;
                    if (aux->addit_info){                     //if not false
                        aux--;                      //returns to beginning of line
                        i = aux->line_number;
                        while (aux->line_number <= i){  //erases "IF" line 
                            aux = tokenlist.erase(aux);
                        }
                        newit = aux;
                    }else{                     //if false
                        aux--;                      //returns to beginning of line
                        i = aux->line_number;
                        i++;                        //until next line
                        while (aux->line_number <= i){  //erases "IF" line and following line
                            aux = tokenlist.erase(aux);
                            if(aux == tokenlist.end()) break;
                        }
                        newit = aux;
                    }
                }
            }
        }
    return 0;
}
