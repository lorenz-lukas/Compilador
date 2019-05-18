
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "validate_arguments.h"
#include "pre_processor.h"
#include "scanner.h"
#include "parser.h"
#include "macro.h"
#include "synthesizer.h"
#include "semantic_analyser.h"
using namespace std;

int pre_error = 0; // flag to indicate if pre-processor had any error.
int solo = 0;

/*
* MAIN: validates arguments and calls specifc functions, acording to arguments.
* INPUTS: - argc -> argument count from terminal
*         - argv -> argument vector from terminal
* OUTPUT: none
*/
int main (int argc, char** argv){

    list<Token> tokenlist;
    list<Token>::iterator it, aux;
    list<Symbol> dt, ut;
    list<Symbol>::iterator its;
    list<int> object, realoc;
    list<int>::iterator it_ob;

    validate_arguments(argc, argv);
    
    string file_name;
    string object_fn, pre_fn;


    
    solo = 1;
    file_name = file_name = string(argv[1]);
    pre_processor(strdup(file_name.c_str()), tokenlist);
    
    /*create pre_processing file*/
    pre_fn = string(argv[1]);
    pre_fn = pre_fn.substr(0, pre_fn.find('.')) ;
    pre_fn = pre_fn + ".pre";
    ofstream fpo (pre_fn);  //opens output file

    it = tokenlist.begin();
    int line = it->line_number;
    for (it = tokenlist.begin(); it != tokenlist.end(); it++){
        if(line != it->line_number) {
            fpo  << "\n";
            line = it->line_number;
        }
        aux=it;
        aux++;
        if(aux->type == TT_COMMA_OPERATOR || it->type == TT_COMMA_OPERATOR) fpo << it->str;
        else fpo << it->str << " ";
    }
    fpo.close();   //closes output file


    if (pre_error){
        cout << endl << "Error = true. The process will terminate before creating object file." << endl << endl;
        return 0;
    }

    //solve_macro(tokenlist);
    synthesizer(tokenlist, object);
    
    /*create object file*/
    object_fn = string(argv[1]);
    object_fn = object_fn.substr(0, object_fn.find('.')) ;
    object_fn = object_fn + ".obj";
    ofstream fpo1 (object_fn);  //opens output file
    
    for (it_ob = object.begin(); it_ob != object.end(); it_ob++){
        fpo1 << *it_ob << " ";
    }
    fpo1.close();   //closes output file
    
    return 0;
    
}
