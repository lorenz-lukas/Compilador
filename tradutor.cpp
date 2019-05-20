#include <unistd.h>
#include <iostream>
#include <list>
#include <iterator>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <typeinfo>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <algorithm>

// GENERIC TYPE OF TOKEN: -> used in genericType
#define MNEMONIC    1
#define LABEL       2
#define DIRECTIVE   3
#define OPERAND     4
#define CONST       5
#define COMMA       6
#define PLUS        7
#define AMPERSAND   8
//SPECIFIC INFORMATION OF TOKEN: -> used in specificInfo
//-OPERANDS:
#define ADD             1
#define SUB             2
#define MULT            3
#define DIV             4
#define JMP             5
#define JMPN            6
#define JMPP            7
#define JMPZ            8
#define COPY            9
#define LOAD            10
#define STORE           11
#define INPUT           12
#define OUTPUT          13
#define STOP            14
#define BASIC_OPERAND   15   // FOR , + &
//-DIRECTIVES
#define SECTION   1
#define SPACE     2
#define CON       3  // CONST
#define TEXT      4
#define DATA      5

#define INVALID -1


using namespace std;
int error=0;
class Compiler
{
  public:
    struct Token {
      std::string token;
      int lineNumber;
      int tokenPosInLin;
      int genericType;
      int specificInfo;
      int flag;
    };
    typedef struct Token Token;

    struct Symbol {
      string label;
      int position;
    };
    typedef struct Symbol Symbol;

    void getCode(string name);
    void preprocessing();
      void getMacro(string line,int *i);
      string swap(string line, string argLine, string argTable);
      void expMacro(string line, int *i, int j);
      void equIf(string line);
      string brokenLabel(string line, int *i);
      void scaner();//lexic error

        void markSintaxError(vector<Token>::iterator it);
       	int parserMnemonic(vector<Token>::iterator it);
        int parserDirective(vector<Token>::iterator it);
        int parserOperand(vector<Token>::iterator it);
        int parserConst(vector<Token>::iterator it);
        int parserAmpersand(vector<Token>::iterator it);
        int parserLabel(vector<Token>::iterator it);
			void parser();//sintatic error

				int duplicateLabel ();
				int sectionPlacement ();
				int checkSymbolsFromData();
				int checkForDataNeed();
				int defasLabel();
				int invalidLabel();
				int noLabel();
				int labelExist();
				int wrongSection();
      void semanticErrorTreat();

    void firstPass();
    void secondPass();

    void writeOutput(string name);

    std::vector<Token> tokenTable, labelTable;
		vector<Token>::iterator data_it;
    std::vector<Symbol> symbleTable;
    std::vector<int> object;

    std::vector<string> codeRaw, code, temp;
    std::vector<int> originalCodeLine;
    std::vector<vector<string> > macrotable, equIfTable;
    string instructions[14] =
    {"ADD","SUB","MULT","DIV","JUMP","JUMPN","JUMPP","JUMPZ","COPY","LOAD","STORE","INPUT","OUTPUT","STOP"};
    int sectionData = 0, sectionText = 0, hasdatasec=0;//, error = 0;

};


void Compiler::getCode(string name)
{
  string line;
  ifstream myfile (name);
  cout<<"###############################"<<endl;
  cout<<"      ORIGINAL CODE:\n"<< endl;
  if (myfile.is_open())
  {
    while (getline(myfile,line))
    {

      cout << line << endl;
      this->codeRaw.push_back(line);
    }
    myfile.close();
  }else
    cout << "Unable to open file";
}
void Compiler::getMacro(string line,int *i){
  std::size_t found = line.find(":");
  std::vector<string> temp1;
  string name,args;
  int index = *i+1;
  int j = 0;
  if(found != string::npos){
    name.append(line, 0, found);
    found = line.find("&");
    if(found!=string::npos){//Arguments not equal to zero
        args.append(line, found, line.length());
    }
    temp1.push_back(name);
    temp1.push_back(args);
    for(; index <= this->codeRaw.size(); index++){
        line = this->codeRaw[index];
        if(line[0]==32){
            string instruction;
            for(j = 0;j<line.length();j++){
              if(line[j]>32)break;
            }
            int size = line.length();
            instruction.append(line, j, size);
            line = instruction;
        }
        if(line.find("END", 0) != std::string::npos)break;
        temp1.push_back(line);
    }
    this->macrotable.push_back(temp1);
    *i = index;
  }else cout<< "Macro definition error! Miss ':' marker at line %d." << index-1 << endl;
}

string Compiler::swap(string line, string argLine, string argTable){
  std::size_t found = line.find(argTable);
  if(found!=std::string::npos){
    string temp1,temp2;
    temp1.append(line,0,found-1);
    temp2.append(line,found + argTable.length(),line.length());
    found = temp2.find(" ");
    found = temp1.find(" ");
    temp1 += " " + argLine;
    line = temp1 + temp2;
  }
  return line;
}

void Compiler::expMacro(string line, int *i, int j){
  int index = *i+1;
  string macroName,arg1,arg2,arg3;
  string macroNameTable = this->macrotable[j][0], argsTable = this->macrotable[j][1], arg1Table, arg2Table ,arg3Table;

  std::size_t found = line.find(" ");
  int k = found+1;
  std::size_t found1,found2,error;

  found1 = line.find(","); // two arguments
  found2 = line.find(",", found1+1);//three arguments
  if(found2!= std::string::npos)error = line.find(",", found2+1);// + three arguments
  else error = std::string::npos;
  if(found!= std::string::npos)macroName.append(line, 0,found);
  else macroName = line;

  if(argsTable.length() == 0){ // None arguments
      if(macroName.length() == line.length()){
        int control = 1;
        std::size_t space;
        for(int k = 2; k < (int)this->macrotable[j].size(); k++){
          control = 1;
          string macro;
          space = line.find(" ");
          line = this->macrotable[j][k];
          if(space!= std::string::npos)macro.append(line, 0,space+1);
          else macro = line;
          for(int j = 0; j < (int)this->macrotable.size(); j++){
              if(macro.compare(this->macrotable[j][0]) == 0){
                  expMacro(line, i, j);
                  control = 0;
              }
          }
          if(control){
            this->temp.push_back(line);
          }
        }
      }else cout<< "[ERROR] Macro with number of parameters greater than necessary at line: " << index << endl;
  }else{ // At least one argument
      if(macroName.length() != line.length()){
          if(error!=std::string::npos){
              cout<< "[ERROR] Macro with more than 3 arguments at line: " << index << endl;
            // QUIT
          }else if(found1 == std::string::npos && found2 == std::string::npos){//one argument
              int arg1Index = argsTable.find("&"); // one
              arg1Table.append(argsTable, arg1Index+1,argsTable.length());
              arg1.append(line,found+1,line.length());

              int control = 1;
              std::size_t space;
              for(int k = 2; k < (int)this->macrotable[j].size(); k++){
                    control = 1;
                    string macro;
                    space = line.find(" ");
                    line = this->macrotable[j][k];
                    if(space!= std::string::npos)macro.append(line, 0,space+1);
                    else macro = line;
                    for(int j = 0; j < (int)this->macrotable.size(); j++){
                        if(macro.compare(this->macrotable[j][0]) == 0){
                            expMacro(line, i, j);
                            control = 0;
                        }
                    }
                    if(control){
                      line = this->swap(line, arg1, arg1Table);
                      this->temp.push_back(line);
                    }
              }

          }else if(found1 != std::string::npos && found2 == std::string::npos){// two arguments
              int arg1Index = argsTable.find("&"); // one
              int found1Index = argsTable.find(",");
              arg1Table.append(argsTable, arg1Index+1,found1Index-1);
              arg2Table.append(argsTable, found1Index+2,argsTable.length());
              for(int m = 0; m < (found1-found-1); m++)         arg1.push_back(line[m+found+1]);
              for(int m = 0; m < (line.length()-found1+1); m++) arg2.push_back(line[m+found1+1]);
              int control = 1;
              std::size_t space;
              for(int k = 2; k < (int)this->macrotable[j].size(); k++){
                    control = 1;
                    string macro;
                    space = line.find(" ");
                    line = this->macrotable[j][k];
                    if(space!= std::string::npos)macro.append(line, 0,space+1);
                    else macro = line;
                    for(int j = 0; j < (int)this->macrotable.size(); j++){
                        if(macro.compare(this->macrotable[j][0]) == 0){
                            expMacro(line, i, j);
                            control = 0;
                        }
                    }
                    if(control){
                        line = this->swap(line, arg1, arg1Table);
                        line = this->swap(line, arg2, arg2Table);
                        this->temp.push_back(line);
                    }
              }

          }else if(found1 != std::string::npos && found2 != std::string::npos){//thre arguments
              int arg1Index = argsTable.find("&");
              int found1Index = argsTable.find(",");
              int found2Index = argsTable.find(",",found1Index+1);
              arg1Table.append(argsTable, arg1Index+1,found1Index-1);
              for(int m = 0; m < (found2Index-found1Index-2); m++) arg2Table.push_back(argsTable[m+found1Index+2]);
              arg3Table.append(argsTable, found2Index+2,argsTable.length());
              for(int m = 0; m < (found1-found-1); m++)         arg1.push_back(line[m+found+1]);
              for(int m = 0; m < (found2-found1-1); m++)        arg2.push_back(line[m+found1+1]);
              for(int m = 0; m < (line.length()-found2+1); m++) arg3.push_back(line[m+found2+1]);

              int control = 1;
              std::size_t space;
              for(int k = 2; k < (int)this->macrotable[j].size(); k++){
                    control = 1;
                    string macro;
                    space = line.find(" ");
                    line = this->macrotable[j][k];
                    if(space!= std::string::npos)macro.append(line, 0,space+1);
                    else macro = line;
                    for(int j = 0; j < (int)this->macrotable.size(); j++){
                        if(macro.compare(this->macrotable[j][0]) == 0){
                            expMacro(line, i, j);
                            control = 0;
                        }
                    }
                    if(control){
                      line = this->swap(line, arg1, arg1Table);
                      line = this->swap(line, arg2, arg2Table);
                      line = this->swap(line, arg3, arg3Table);
                      this->temp.push_back(line);
                    }
              }

          }else cout<< "[ERROR] Macro with wrong number of parameters at line: " << index << endl;
      }else cout<< "[ERROR] Macro with number of parameters less than necessary at line: " << index << endl;
  }
}
string Compiler::brokenLabel(string line, int *i){
  std::size_t found = line.find(":");
  int index = *i;
  int ind = index;
  if( (index+1) >= line.length()){
    for(int j = found+1; index < this->codeRaw.size() ; j++){
      if((j-1) == this->codeRaw[index].length()){
        index++;
        j=0;
      }
      if((int) this->codeRaw[index][j] >= 33) {
        //cout<< ( this->codeRaw[index][j] ) << endl;
        break;
      }
    }
    line = (this->codeRaw[ind] + " " + this->codeRaw[index]);
    *i = index;
  }else{
    if( (int)line[found+2] == 32){ // SPACE
      int j = found+1;
      found+=2;
      string temp = line;
      for(; (int)line[j] == 32; j++);
      int beginEqu = j;
      for(; j < temp.length(); j++, found++)line[found] = line[j];
      line.resize(beginEqu);
    }
  }
  return line;
}

void Compiler::equIf(string line){
  std::vector<string> variable;
  string name, value = "0";
  std::size_t found = line.find(":");
  name.append(line, 0, found);
  if(line.find("1") != std::string::npos)value = "1";

  variable.push_back(name);
  variable.push_back(value);
  this->equIfTable.push_back(variable);
}

void Compiler::preprocessing()
{
  string line;
  int control = 1;
  //std::vector<string> temp;
  std::vector<int> lineIndex;
  int j = 0;
  for(int i = 0; i < this->codeRaw.size() ; i++){
    line = this->codeRaw[i];
    control = 1;
    if(!line.empty()){
      //removes comments
      line = line.substr(0, line.find(';'));
      // Turn all upper.
      for(int j=0; j<line.length(); j++)line.at(j) = toupper(line.at(j));
      // Identify if the line is a macro
      if(line.find("MACRO") != std::string::npos){
          this->getMacro(line, &i);
          control = 0;
      }else{
        // Macro expansion
        string macroName;
        std::size_t found = line.find(" ");
        if(found!= std::string::npos)macroName.append(line, 0,found);
        else macroName = line;
        for(int j = 0; j < (int)this->macrotable.size(); j++){
          if(macroName.compare(this->macrotable[j][0]) == 0){
            this->expMacro(line, &i, j);
            control = 0;
            break;
          }
        }
      }
      if(line.find(":") != std::string::npos && !sectionData){
        line = this->brokenLabel(line, &i);
      }
      if(line.find("EQU", 0) != std::string::npos){
        this->equIf(line);
      }
      if(line.find("IF", 0) != std::string::npos){
        if((int)this->equIfTable.size() > 0){
            string name,temp;
            int j=0;
            std::size_t found = line.find("IF");
            name.append(line, found+3, line.length());

            while(j < (int)this->equIfTable.size()){
              if(name == this->equIfTable[j][0])break;
              j++;
            }
            if(int(name[0]) == 48 || int(name[0]) == 49){
              if(int(name[0]) == 48) temp.push_back('0');
              else temp.push_back('1' );
            }else temp = this->equIfTable[j][1];

            if(int(temp[0]) == 48){ // EQU 0
              i++;
            }
            control = 0;
          }else cout<<"[ERROR] If declaration without an previously EQU at line: " << i << endl;
      }
      if(line.find("SECTION DATA") != std::string::npos){
        this->sectionData = 1;
      }
      if(line[0]==32){
          string instruction;
          for(j = 0;j<line.length();j++){
            if(line[j]>32)break;
          }
          int size = line.length();
          instruction.append(line, j, size);
          line = instruction;
      }
      if(!line.empty() && control){// Qualquer linha ou EQU 1
        this->temp.push_back(line);
        lineIndex.push_back(i+1);
      }

    }
  }
  for(control = 0;control<temp.size();control++){
    line = temp[control];
    if(line.find("SECTION TEXT") != std::string::npos){
      break;
    }
  }

  for(int i = control; i < this->temp.size(); i++) {
    this->code.push_back(this->temp[i]);
    this->originalCodeLine.push_back(lineIndex[i]);
  }
  cout<<"\n\n      CODE CORRECTED:"<< endl;
  for(int i = 0; i < this->code.size() ; i++) {
    cout<<this->code[i]<<endl;
    //cout<<this->originalCodeLine[i]<<endl;
  }
}


//////////////////////////////////////////////////
////////////////////////////////////// SCANER
//////////////////////////////////////////////////

void Compiler::scaner(){

}

//////////////////////////////////////////////////
////////////////////////////////////// PARSER
//////////////////////////////////////////////////

void Compiler::markSintaxError(vector<Token>::iterator it){
  int i = it->lineNumber;
    vector<Token>::iterator newit;
    newit = it;
    while (newit->lineNumber == i){
        newit--;
    }
    newit++;
    while (newit->lineNumber == i){
        newit->flag = -1;
        newit++;
    }
}

int Compiler::parserMnemonic(vector<Token>::iterator it){
  int targetLine, count;

  targetLine = it->lineNumber;
	switch (it->specificInfo){
  case ADD:
  case SUB:
  case MULT:
  case DIV:
  case JMP:
  case JMPN:
  case JMPP:
  case JMPZ:
  case LOAD:
  case STORE:
  case INPUT:
  case OUTPUT:
    it++; count++;
    if (it != tokenTable.end() && targetLine == it->lineNumber ){// check if argument exists.
      if (it->genericType == AMPERSAND) {// ignores argument indicator.
        it++; count++;
        if(it == tokenTable.end() || targetLine != it->lineNumber){
          cerr << "Sintax Error @ Line " << targetLine << " -missing argument." << endl;
				  markSintaxError(it);
				  error = 1;
				  it->flag = -1;

        }
      }
      if(it->genericType == OPERAND){  // check if valid argument.
        it++; count++;
        if (it != tokenTable.end() && targetLine == it->lineNumber ){ // check extra operation.
          if(it->genericType ==PLUS){  // check if valid operation +.
            it++; count++;
            if (it != tokenTable.end() && targetLine == it->lineNumber ){ // check if + argument exist.
              if(it->genericType ==CONST){  // check if + valid argument.
                it++; count++;
                if (it != tokenTable.end() && targetLine == it->lineNumber ){ // check if too much arguments.
                  cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
				          markSintaxError(it);
				          error = 1;
				          it->flag = -1;
                  do{ // get out of line.
                    it++; count++;
                  }while(it != tokenTable.end() && targetLine == it->lineNumber);
                }
              }else {
                cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
				        markSintaxError(it);
				        error = 1;
				        it->flag = -1;
                do{ // get out of line.
                  it++; count++;
                }while(it != tokenTable.end() && targetLine == it->lineNumber);
              }
            }else{
              cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
				      markSintaxError(it);
				      error = 1;
				      it->flag = -1;
            }
          }else {
            cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
				    markSintaxError(it);
				    error = 1;
				    it->flag = -1;
            do{ // get out of line.
              it++; count++;
            }while(it != tokenTable.end() && targetLine == it->lineNumber);
          }
        }
      }else{
        cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
        markSintaxError(it);
        error = 1;
        it->flag = -1;
        do{ // get out of line.
          it++; count++;
        }while(it != tokenTable.end() && targetLine == it->lineNumber);
      }
    } else {
				cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
				markSintaxError(it);
				error = 1;
				it->flag = -1;
    }
  break;

  case COPY:
      it++; count++;
			if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if arguments exists.

				// first argument.
				if (it->genericType == AMPERSAND){		// ignores argument indicator.
					it++; count++;
					if (it == tokenTable.end() || targetLine != it->lineNumber){
						markSintaxError(it);
						error = 1;
						it->flag = -1;
						cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
					}
				}
				if (it->genericType == OPERAND){										// check if valid argument.
					it++; count++;
					if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if next argument exist.
						if (it->genericType == PLUS){									// check if it is + operation.
							it++; count++;
							if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if + argument exist.
								if (it->genericType == CONST){									// check if + valid argument.
									it++; count++;
									if (it == tokenTable.end() || targetLine != it->lineNumber){	// check if next argument exist.
										cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
										markSintaxError(it);
										error = 1;
										it->flag = -1;
										break;		// get out of switch.
									}
								} else {
									cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
									markSintaxError(it);
									error = 1;
									it->flag = -1;
									do {		// get out of line.
										it++; count++;
									} while(it != tokenTable.end() && targetLine == it->lineNumber);
									break;		// get out of switch.
								}
							} else {
								cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
								markSintaxError(it);
								error = 1;
								it->flag = -1;
								break;		// get out of switch.
							}
						}
					} else {
						cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
						markSintaxError(it);
						error = 1;
						it->flag = -1;
						break;		// get out of switch.
					}
				} else {
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					markSintaxError(it);
					error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
					break;		// get out of switch.
				}

				// comma argument.
				if (it->genericType == COMMA){									// check if valid argument.
					it++; count++;
					if (it == tokenTable.end() || targetLine != it->lineNumber){	// check if next argument exist.
						cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
						markSintaxError(it);
						error = 1;
						it->flag = -1;
						break;		// get out of switch.
					}
				} else {
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					markSintaxError(it);
					error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
					break;
				}

				// second argument.
				if (it->genericType == AMPERSAND){		// ignores argument indicator.
					it++; count++;
					if (it == tokenTable.end() || targetLine != it->lineNumber){
						markSintaxError(it);
										error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
					}
				}
				if (it->genericType == OPERAND){										// check if valid argument.
					it++; count++;
					if (it != tokenTable.end() && targetLine == it->lineNumber){	// check extra operation.
						if (it->genericType == PLUS){											// check if valid operation +.
							it++; count++;
							if (it != tokenTable.end() && targetLine == it->lineNumber){			// check if + argument exist.
								if (it->genericType == CONST){											// check if + valid argument.
									it++; count++;
									if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if too much arguments.
										cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
										markSintaxError(it);
										error = 1;
										it->flag = -1;
										do {		// get out of line.
											it++; count++;
										} while(it != tokenTable.end() && targetLine == it->lineNumber);
									}
								} else {
									cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
									markSintaxError(it);
									error = 1;
									it->flag = -1;
									do {		// get out of line.
										it++; count++;
									} while(it != tokenTable.end() && targetLine == it->lineNumber);
								}
							} else {
								cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
								markSintaxError(it);
								error = 1;
								it->flag = -1;
							}
						} else {
							cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
							markSintaxError(it);
							error = 1;
							it->flag = -1;
							do {		// get out of line.
								it++; count++;
							} while(it != tokenTable.end() && targetLine == it->lineNumber);
						}
					}
				} else {
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					markSintaxError(it);
					error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
				}

			} else {
				cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
				markSintaxError(it);
				error = 1;
				it->flag = -1;
			}
  break;

  case STOP:
    it++; count++;
    if (it != tokenTable.end() && targetLine == it->lineNumber ){ // check if too much arguments.
      cerr << "Sintax Error @ Line " << targetLine << " - unexpected argument." << endl;
			markSintaxError(it);
			error = 1;
			it->flag = -1;
      do{ // get out of line.
        it++; count++;
      }while(it != tokenTable.end() && targetLine == it->lineNumber);
    }
  break;

  default:
    cerr << "Parser: unknowm mnemonic type (" << it->token << ")." << endl;
    markSintaxError(it);
    error=1;
    it->flag = -1;
    it++; count++;
  break;
  }

  return count;
}

int Compiler::parserDirective(vector<Token>::iterator it){
	int targetLine, count;

	targetLine = it->lineNumber;
	switch (it->specificInfo){
		case SECTION:
			it++; count++;
			if (it != tokenTable.end() && targetLine == it->lineNumber){										// check if arguments exists.
				if (it->genericType == DIRECTIVE && (it->specificInfo == TEXT || it->specificInfo == DATA)){	// check if argument is valid.
					it++; count++;
					if (it != tokenTable.end() && targetLine == it->lineNumber){								// check if too much arguments.
						cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
						markSintaxError(it);
						error = 1;
						it->flag = -1;
						do {		// get out of line.
							it++; count++;
						} while(it != tokenTable.end() && targetLine == it->lineNumber);
					}
				} else {
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					markSintaxError(it);
					error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
				}
			} else {
				cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
				markSintaxError(it);
				error = 1;
				it->flag = -1;
			}
		break;

		case SPACE:
			it++; count++;
			if (it != tokenTable.end() && targetLine == it->lineNumber){			// check if argument exist.
				if (it->genericType == CONST && it->specificInfo > 0){											// check if argument is valid
					it++; count++;
					if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if too much arguments.
						cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
						markSintaxError(it);
						error = 1;
						it->flag = -1;
						do {		// get out of line.
							it++; count++;
						} while(it != tokenTable.end() && targetLine == it->lineNumber);
					}
				} else {
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					markSintaxError(it);
					error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
				}
			}
		break;

		case CON:
			it++; count++;
			if (it != tokenTable.end() && targetLine == it->lineNumber){			// check if argument exist.
				if (it->genericType == CONST){											// check if argument is valid
					it++; count++;
					if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if too much arguments.
						cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
						markSintaxError(it);
						error = 1;
						it->flag = -1;
						do {		// get out of line.
							it++; count++;
						} while(it != tokenTable.end() && targetLine == it->lineNumber);
					}
				} else {
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					markSintaxError(it);
					error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
				}
			} else {
				cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
				markSintaxError(it);
				error = 1;
				it->flag = -1;
			}
		break;

    case TEXT:
		case DATA:
			cerr << "Sintax Error @ Line " << targetLine << " - invalid use of directive." << endl;
			markSintaxError(it);
			error = 1;
			it->flag = -1;
			do {		// get out of line.
				it++; count++;
			} while(it != tokenTable.end() && targetLine == it->lineNumber);
		break;

		/*case IF:
			it++; count++;
			if (it != tokenTable.end() && targetLine == it->lineNumber){			// check if argument exist.
				if (it->genericType == AMPERSAND){		// ignores argument indicator.
					it++; count++;
					if (it == tokenTable.end() || targetLine != it->lineNumber){
						markSintaxError(it);
										error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
					}
				}
				if (it->genericType == CONST || it->genericType == OPERAND){				// check if argument is valid
					it++; count++;
					if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if too much arguments.
						cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
						markSintaxError(it);
						error = 1;
						it->flag = -1;
						do {		// get out of line.
							it++; count++;
						} while(it != tokenTable.end() && targetLine == it->lineNumber);
					}
				} else {
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					markSintaxError(it);
					error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
				}
			} else {
				cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
				markSintaxError(it);
				error = 1;
				it->flag = -1;
			}
		break;*/

		/*case MACRO:
			it++; count++;
			// first argument.
			if (it != tokenTable.end() && targetLine == it->lineNumber){		// check if argument exist.
				if (it->genericType == AMPERSAND){							// check argument operator.
					it++; count++;
					if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if argument is present.
						if (it->genericType == OPERAND){								// check if argument is valid.
							it++; count++;
						} else {
							markSintaxError(it);
										error = 1;
										it->flag = -1;
							cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
							do {		// get out of line.
								it++; count++;
							} while(it != tokenTable.end() && targetLine == it->lineNumber);
							break;
						}
					} else {
						markSintaxError(it);
										error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
						break;
					}
				} else {
					markSintaxError(it);
										error = 1;
										it->flag = -1;
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
					break;
				}
			} else {
				break;
			}

			// second argument.
			if (it != tokenTable.end() && targetLine == it->lineNumber){		// check if argument exist.
				if (it->genericType == COMMA){								// check argument separator indicator.
					it++; count++;
					if (it == tokenTable.end() || targetLine != it->lineNumber){
						markSintaxError(it);
										error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
						break;
					}
				} else {
					markSintaxError(it);
										error = 1;
										it->flag = -1;
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
					break;
				}
				if (it->genericType == AMPERSAND){							// check argument operator.
					it++; count++;
					if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if argument is present.
						if (it->genericType == OPERAND){								// check if argument is valid.
							it++; count++;
						} else {
							markSintaxError(it);
										error = 1;
										it->flag = -1;
							cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
							do {		// get out of line.
								it++; count++;
							} while(it != tokenTable.end() && targetLine == it->lineNumber);
							break;
						}
					} else {
						markSintaxError(it);
										error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
						break;
					}
				} else {
					markSintaxError(it);
										error = 1;
										it->flag = -1;
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
					break;
				}
			} else {
				break;
			}

			// third argument.
			if (it != tokenTable.end() && targetLine == it->lineNumber){		// check if argument exist.
				if (it->genericType == COMMA){								// check argument separator indicator.
					it++; count++;
					if (it == tokenTable.end() || targetLine != it->lineNumber){
						markSintaxError(it);
										error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
						break;
					}
				} else {
					markSintaxError(it);
										error = 1;
										it->flag = -1;
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
					break;
				}
				if (it->genericType == AMPERSAND){							// check argument operator.
					it++; count++;
					if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if argument is present.
						if (it->genericType == OPERAND){								// check if argument is valid.
							it++; count++;
						} else {
							markSintaxError(it);
										error = 1;
										it->flag = -1;
							cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
							do {		// get out of line.
								it++; count++;
							} while(it != tokenTable.end() && targetLine == it->lineNumber);
							break;
						}
					} else {
						markSintaxError(it);
										error = 1;
										it->flag = -1;
						cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
						break;
					}
				} else {
					markSintaxError(it);
										error = 1;
										it->flag = -1;
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
					break;
				}
			} else {
				break;
			}

			if (it != tokenTable.end() && targetLine == it->lineNumber){		// check if too much arguments.
				markSintaxError(it);
										error = 1;
										it->flag = -1;
				cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
				do {		// get out of line.
					it++; count++;
				} while(it != tokenTable.end() && targetLine == it->lineNumber);
			}
		break;*/

		/*case END:
			it++; count++;
			if (it != tokenTable.end() && targetLine == it->lineNumber){		// check if argument exist.
				markSintaxError(it);
										error = 1;
										it->flag = -1;
				cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
				do {		// get out of line.
					it++; count++;
				} while(it != tokenTable.end() && targetLine == it->lineNumber);
			}
		break;*/


		/*case EQU:	// if found in this stage, it must be an invalid EQU.
			do {		// get out of line.
				it++; count++;
			} while(it != tokenTable.end() && targetLine == it->lineNumber);
		break;*/

		default:
			cerr << "Parser: unknowm directive token (" << it->token << ")." << endl;
			markSintaxError(it);
			error = 1;
			it->flag = -1;
			it++; count++;
		break;
	}

	return count;
}

/*int Compiler::parserOperand  (vector<Token>::iterator it){
  int targetLine, count;

  targetLine = it->lineNumber;
	// expects to be a macro calling.
	it++; count++;
	// first argument.
	if (it != tokenTable.end() && targetLine == it->lineNumber){		// check if argument exist.
		if (genericType == AMPERSAND){		// ignores argument indicator.
			it++; count++;
			if (it == tokenTable.end() || targetLine != it->lineNumber){
				return count;
			}
		}
		if (genericType == OPERAND){									// check if argument is valid.
			it++; count++;
		} else {
			markSintaxError(it);
			error = 1;
			it->flag = -1;
			cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
			do {		// get out of line.
				it++; count++;
			} while(it != tokenTable.end() && targetLine == it->lineNumber);
			return count;
		}
	} else {
		return count;
	}

	// second argument.
	if (it != tokenTable.end() && targetLine == it->lineNumber){		// check if argument exist.
		if (genericType == COMMA){							// check argument separator operator.
			it++; count++;
			if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if argument is present.
				if (genericType == AMPERSAND){		// ignores argument indicator.
					it++; count++;
					if (it == tokenTable.end() || targetLine != it->lineNumber){
						return count;
					}
				}
				if (genericType == OPERAND){								// check if argument is valid.
					it++; count++;
				} else {
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					markSintaxError(it);
					error = 1;
					it->flag = -1;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
					return count;
				}
			} else {
				markSintaxError(it);
				error = 1;
				it->flag = -1;
				cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
				return count;
			}
		} else {
			cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
			markSintaxError(it);
			error = 1;
			it->flag = -1;
			do {		// get out of line.
				it++; count++;
			} while(it != tokenTable.end() && targetLine == it->lineNumber);
			return count;
		}
	} else {
		return count;
	}

	// third argument.
	if (it != tokenTable.end() && targetLine == it->lineNumber){		// check if argument exist.
		if (genericType == COMMA){							// check argument separator operator.
			it++; count++;
			if (it != tokenTable.end() && targetLine == it->lineNumber){	// check if argument is present.
				if (genericType == AMPERSAND){		// ignores argument indicator.
					it++; count++;
					if (it == tokenTable.end() || targetLine != it->lineNumber){
						return count;
					}
				}
				if (genericType == OPERAND){								// check if argument is valid.
					it++; count++;
				} else {
					markSintaxError(it);
          error = 1;
          it->flag = -1;
					cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
					do {		// get out of line.
						it++; count++;
					} while(it != tokenTable.end() && targetLine == it->lineNumber);
					return count;
				}
			} else {
				markSintaxError(it);
        error = 1;
        it->flag = -1;
				cerr << "Sintax Error @ Line " << targetLine << " - missing argument." << endl;
				return count;
			}
		} else {
			markSintaxError(it);
      error = 1;
      it->flag = -1;
			cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
			do {		// get out of line.
				it++; count++;
			} while(it != tokenTable.end() && targetLine == it->lineNumber);
			return count;
		}
	} else {
		return count;
	}

	if (it != tokenTable.end() && targetLine == it->lineNumber){		// check if too much arguments.
		markSintaxError(it);
    error = 1;
    it->flag = -1;
		cerr << "Sintax Error @ Line " << targetLine << " - invalid argument." << endl;
		do {		// get out of line.
			it++; count++;
		} while(it != tokenTable.end() && targetLine == it->lineNumber);
	}
	return count;
}*/

int Compiler::parserConst(vector<Token>::iterator it){
  int targetLine, count;
  targetLine = it->lineNumber;

  cerr << "Sintax Error @ Line " << targetLine << " - unexpected value." << endl;
  markSintaxError(it);
  error = 1;
  it->flag = -1;
	do {		// get out of line.
		it++; count++;
	} while(it != tokenTable.end() && targetLine == it->lineNumber);

	return count;
}

int Compiler::parserAmpersand(vector<Token>::iterator it){
  int targetLine, count;
	targetLine = it->lineNumber;

	it++; count++;
	if (it == tokenTable.end() || targetLine != it->lineNumber){
		markSintaxError(it);
		error = 1;
		it->flag = -1;
		cerr << "Sintax Error @ Line " << targetLine << " - ." << endl;
	}

	return count;
}

int Compiler::parserLabel(vector<Token>::iterator it){
	int targetLine, count;
	targetLine = it->lineNumber;

	it++; count++;
	if (it == tokenTable.end() || targetLine != it->lineNumber){
		markSintaxError(it);
		error = 1;
		it->flag = -1;
		cerr << "Sintax Error @ Line " << targetLine << " - empty label." << endl;
	} else {
		if (it->genericType == LABEL){
			markSintaxError(it);
			error = 1;
			it->flag = -1;
			cerr << "Sintax Error @ Line " << targetLine << " - multiple labels in line." << endl;
			do {		// get out of line.
				it++; count++;
			} while(it != tokenTable.end() && targetLine == it->lineNumber);
		}
	}

	return count;
}

void Compiler::parser(){
  vector<Token>::iterator it;

  it = tokenTable.begin();
  while (it != tokenTable.end())
  {
    switch (it->genericType){
      case MNEMONIC:
        advance(it,parserMnemonic(it));
      break;

      case LABEL:
        advance(it,parserLabel(it));
      break;

      case DIRECTIVE:
        advance(it,parserDirective(it));
      break;

      /*case OPERAND:
          advance(it,parserOperand(it));
      break;*/

      case COMMA:
      case PLUS:
      case CONST:
        advance(it,parserConst(it));
      break;

      case AMPERSAND:
        advance(it,parserAmpersand(it));
      break;

      default:
        cerr << "Parser: unknowm token type (" << it->token << ")." << endl;
        markSintaxError(it);
        error=1;
        it->flag = -1;
        it++;
      break;
    }
  }

}

//////////////////////////////////////////////////
////////////////////////////////////// SEMANTIC ANALYSIS
//////////////////////////////////////////////////

int Compiler::duplicateLabel (){ //ALTERAR PARA FUNCIONAR CORETAMENTE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	vector<Token>::iterator it, aux, end;//										T
	int err = 0;//																						|
 //																									  			|
	end=labelTable.end();//																		|
	//end--;              //stops loop one before  <-----------
	for (it=labelTable.begin(); it != end; it++){
			aux=it;
			aux++;
			while(aux != labelTable.end()){
					if (it->token == aux->token){
							cerr << "Semantic Error @ Line " << aux->lineNumber << " - multiple declarations of '" << aux->token <<"'." << endl;
							cerr << "\t\tPrevious declaration @ line " << it->lineNumber << "." << endl;
							error = 1;
							err++;
					}
					aux++;
			}
	}
	return err;

}

int Compiler::sectionPlacement (){
	vector<Token>::iterator it = tokenTable.begin();
    int err = 0;
    int count = 0;

    while (it != tokenTable.end()){
        if (it->genericType == DIRECTIVE && it->specificInfo == SECTION && it->flag != -1){     //if section
            it++;
            if (count == 0){    //if first section
                if (!(it->genericType == DIRECTIVE && it->specificInfo == TEXT)){     //if not section text
                    fprintf(stderr, "Semantic error @ line %d - Expected 'TEXT' section!\n", it->lineNumber);
                    error = 1;
                    err++;
                }
            }else if (count == 1 && err == 0){      //second section
                if (!(it->genericType == DIRECTIVE && it->specificInfo == DATA)){     //not data section
                    fprintf(stderr, "Semantic error @ line %d - Expected 'DATA' section!\n", it->lineNumber);
                    error = 1;
                    err++;
                }else{
                    data_it = it;
                }
            }else if (err == 0){    //third+ section
                fprintf(stderr, "Semantic error @ line %d - Too many sections!\n", it->lineNumber);
                error = 1;
                err++;
            }
            count++;
        }
        it++;
    }
    if (it == tokenTable.end() && count == 0){   //no section
        fprintf(stderr, "Semantic error - No section found!\n");
        error = 1;
        err++;
    }
    if (count == 2){
        hasdatasec = 1;
    }
    return err;
}

int Compiler::checkSymbolsFromData(){
    int err = 0;
    int i =0,konst;
    vector<Token>::iterator it, data_begin, aux,aux2;

    for (it = tokenTable.begin(); it != tokenTable.end(); it++){
        if (it->genericType == OPERAND && it->specificInfo != -1 && it->flag != -1){
            for (data_begin = data_it; data_begin != tokenTable.end(); data_begin++){
                if (data_begin->genericType == LABEL){
                    if (data_begin->token.substr(0, data_begin->token.find(":")) == it->token){
                        aux2=it;
                        aux2++;

                        if (aux2->genericType == PLUS && aux2->lineNumber == it->lineNumber)
                        {
                            aux2++;
                            if (aux2->genericType == CONST && aux2->lineNumber == it->lineNumber)
                            {
                                konst = aux2->specificInfo;
                                aux2 = data_begin;
                                advance(aux2,2);
                                if (aux2->genericType == CONST && aux2->lineNumber == data_begin->lineNumber && aux2->specificInfo <= konst){
                                    fprintf(stderr, "Semantic error @ line %d -Argument '%s' not reserved in vector '%s' in DATA section.\n", it->lineNumber, it->token.c_str(), data_begin->token.c_str());
                                    error = 1;
                                    err++;
                                }

                            }
                        }
                        data_begin->flag = 100;        //marks data flags that are related to an operand
                        break;
                    }
                }
            }
            /*if (data_begin == tokenTable.end()){
                aux = it;
                aux--;
                if (!(aux->genericType == MNEMONIC && (aux->specificInfo == JMP ||\
                                                aux->specificInfo == JMPN ||\
                                                aux->specificInfo == JMPP ||\
                                                aux->specificInfo == JMPZ))){
                    if (aux->lineNumber == it->lineNumber){
                        for (aux = it; aux != tokenTable.begin(); aux--){
                            if (aux->genericType == DIRECTIVE && aux->specificInfo == MACRO){
                                i=1;
                                break;
                            }
                        }
                        for (aux = it; aux != tokenTable.begin(); aux++){
                            if (aux->genericType == DIRECTIVE && aux->specificInfo == END){
                                i++;
                                break;
                            }
                        }
                        if(i!=2){
                            //cout << "Token: " << it->token << "..   \tLine: " << it->lineNumber << "   \tPosition in line: " << it->tokenPosInLin << "    \tType: " << it->genericType << "        \taddt_info: " << it->specificInfo << "    \tflag: " << it->flag << "     \tinfo str: " << it->info_str << endl;  //print list element
                            fprintf(stderr, "Semantic error @ line %d - Argument '%s' not declared in DATA section.\n", it->lineNumber, it->token.c_str());
                            error = 1;
                            err++;
                        }
                    }
                }
            }*/
        }
    }
    return err;
}

int Compiler::checkForDataNeed(){
    int err = 0;
    int i = 0;
    vector<Token>::iterator it, aux;
    for (it = tokenTable.begin(); it != tokenTable.end(); it++){
        if (it->genericType == OPERAND && it->specificInfo != -1 && it->flag != -1){
            aux = it;
            aux--;
            if (!(aux->genericType == MNEMONIC && (aux->specificInfo == JMP ||\
                                            aux->specificInfo == JMPN ||\
                                            aux->specificInfo == JMPP ||\
                                            aux->specificInfo == JMPZ))){
                fprintf(stderr, "Semantic error @ line %d - No DATA section - Argument '%s' not declared.\n", it->lineNumber, it->token.c_str());
                error = 1;
                err++;
                i = 1;
            }
        }
    }
    if (i==1){
        fprintf(stderr, "Semantic error - Expected 'DATA' section!\n");
    }
    return err;
}

/*int Compiler::defasLabel(){
    int err = 0;
    vector<Token>::iterator it, newit, aux, auxx;
    int i = 0;
    for (it = tokenTable.begin(); it != tokenTable.end(); it++){
        if (it->genericType == OPERAND && it->specificInfo != -1 && it->flag != -1){
            aux = it;
            aux--;
            if (aux->lineNumber == it->lineNumber){
                if (!(aux->genericType == MNEMONIC && (aux->specificInfo == JMP ||\
                                                aux->specificInfo == JMPN ||\
                                                aux->specificInfo == JMPP ||\
                                                aux->specificInfo == JMPZ))){
                    for (newit=tokenTable.begin();newit != data_it; newit++){
                        if ( (newit->token.substr(0, newit->token.find(":")) == it->token) && \
                                            (newit->genericType == LABEL) && \
                                            (newit->lineNumber != it->lineNumber) && \
                                            (newit->tokenPosInLin != it->tokenPosInLin) ){
                            for (auxx = it; auxx != tokenTable.begin(); auxx--){
                                if (auxx->genericType == DIRECTIVE && auxx->specificInfo == MACRO){
                                    i=1;
                                    break;
                                }
                            }
                            for (auxx = it; auxx != tokenTable.begin(); auxx++){
                                if (auxx->genericType == DIRECTIVE && auxx->specificInfo == END){
                                    i++;
                                    break;
                                }
                            }
                            if (i!=2){
                                fprintf(stderr, "Semantic error @ line %d - Definition from line %d (%s) declared as TEXT label in line %d.\n", it->lineNumber, it->lineNumber, it->token.c_str(), newit->lineNumber);
                                error = 1;
                                err++;
                            }
                        }
                    }
                }
            }
        }
    }
    return err;
}*/

int Compiler::invalidLabel(){
    int err = 0;
    vector<Token>::iterator it, newit, aux;
    for (it = tokenTable.begin(); it != tokenTable.end(); it++){
        if (it->genericType == OPERAND && it->specificInfo != -1 && it->flag != -1){
            aux = it;
            aux--;
            if (aux->genericType == MNEMONIC && (aux->specificInfo == JMP ||\
                                            aux->specificInfo == JMPN ||\
                                            aux->specificInfo == JMPP ||\
                                            aux->specificInfo == JMPZ)){
                for (newit=tokenTable.begin();newit != data_it; newit++){
                    if ( (newit->token.substr(0, newit->token.find(":")) == it->token) && \
                                        (newit->genericType == LABEL) && \
                                        (newit->lineNumber != it->lineNumber) && \
                                        (newit->tokenPosInLin != it->tokenPosInLin) ){
                        break;
                    }
                }
                if (newit != data_it){
                    aux = newit;
                    aux++;
                    if (aux->genericType == DIRECTIVE){
                        fprintf(stderr, "Semantic error @ line %d - Jump to invalid Label ('%s' - previous declaration @ line %d).\n", newit->lineNumber, it->token.c_str(), it->lineNumber);
                        error = 1;
                        err++;
                    }
                }
            }
        }
    }
    return err;
}

int Compiler::noLabel(){
    int err = 0;
    vector<Token>::iterator it, newit, aux;
    for (it = tokenTable.begin(); it != tokenTable.end(); it++){
        if (it->genericType == OPERAND && it->specificInfo != -1 && it->flag != -1){
            aux = it;
            aux--;
            if (aux->genericType == MNEMONIC && (aux->specificInfo == JMP ||\
                                            aux->specificInfo == JMPN ||\
                                            aux->specificInfo == JMPP ||\
                                            aux->specificInfo == JMPZ)){
                for (newit=tokenTable.begin();newit != data_it; newit++){
                    if ( (newit->token.substr(0, newit->token.find(":")) == it->token) && \
                                        (newit->genericType == LABEL) && \
                                        (newit->lineNumber != it->lineNumber) && \
                                        (newit->tokenPosInLin != it->tokenPosInLin) ){
                        break;
                    }
                }
                if (newit == data_it){
                    fprintf(stderr, "Semantic error @ line %d - Label '%s' not declared in TEXT section.\n", it->lineNumber, it->token.c_str());
                    error = 1;
                    err++;
                    while (newit != tokenTable.end()){
                        newit++;
                        if ( (newit->token.substr(0, newit->token.find(":")) == it->token) && \
                                            (newit->genericType == LABEL) && \
                                            (newit->lineNumber != it->lineNumber) && \
                                            (newit->tokenPosInLin != it->tokenPosInLin) ){
                            fprintf(stderr, "Semantic error @ line %d - Label '%s' defined in DATA section, but a TEXT section label was expected (previous declaration @ line %d).\n", newit->lineNumber, it->token.c_str(), it->lineNumber);
                            error = 1;
                            err++;
                        }
                    }
                }
            }
        }
    }
    return err;
}

int Compiler::labelExist(){
    vector<Token>::iterator itt, itl;
    int err = 0;
    int flag = 0;

    for (itt = tokenTable.begin(); itt != tokenTable.end(); itt++){
        if (itt->genericType == OPERAND){
            for (itl = labelTable.begin(); itl != labelTable.end(); itl++){
                if (itt->token == itl->token){
                    flag = 1;
                    break;
                }
            }
            if (!flag) {
                cerr << "Semantic Error @ Line " << itt->lineNumber << " - no definition of label (" << itt->token << ") found." << endl;
                error = 1;
                err++;
            }
            flag = 0;
        }
    }

    return err;
}

int Compiler::wrongSection(){
    int err = 0;
    vector<Token>::iterator it;
    for (it = tokenTable.begin(); it != data_it; it++){
        if (it->genericType == DIRECTIVE && (it->specificInfo == CON || it->specificInfo == SPACE)){
            fprintf(stderr, "Semantic error @ line %d - Atempt to use the directive '%s' in the wrong section.\n", it->lineNumber, it->token.c_str());
            error = 1;
            err++;
        }
    }
    while (it != tokenTable.end()){
        if (it->genericType == MNEMONIC){
            fprintf(stderr, "Semantic error @ line %d - Atempt to use the mnemonic '%s' in the wrong section.\n", it->lineNumber, it->token.c_str());
            error = 1;
            err++;
        }
        it++;
    }
    return err;
}

void Compiler::semanticErrorTreat(){
    int err = 0;

    err+=duplicateLabel();
    err+=sectionPlacement();
    if (err == 0 && hasdatasec) err+=checkSymbolsFromData();
    else if (err == 0) err+=checkForDataNeed();
    //err+=defasLabel();
    err+=invalidLabel();
    err+=noLabel();
    err+=labelExist();
    err+=wrongSection();

}

//////////////////////////////////////////////////
////////////////////////////////////// SYNTHESIS
//////////////////////////////////////////////////

void Compiler::firstPass(){ //done
  std::vector<Token>::iterator it;
  Symbol symbol;
  int posCounter=0;
  for (it = tokenTable.begin(); it != tokenTable.end(); it++)
  {
    switch (it->genericType)
    {
    case MNEMONIC:
    case OPERAND:
      posCounter++;
      break;
    case DIRECTIVE:
      switch (it->specificInfo)
      {
        case SPACE:
          it++;
          if (it->genericType == CONST)
          {
            posCounter+= it->specificInfo;
          } else {
            posCounter++;
            it--;
          }

        break;

        case CON:
          posCounter++;
        break;

        default:
        break;
      }
      break;

    case LABEL:
        symbol.label = it->token.substr(0,it->token.length()-1);//store label in variable
        symbol.position = posCounter;
        symbleTable.insert(symbleTable.end(), symbol);
      break;

    default:
      break;
    }
  }
}

void Compiler::secondPass(){ //done
  vector<Token>::iterator it_to;
  vector<Symbol>::iterator it_sy;
  vector<int>::iterator it_ob;

  for (it_to = tokenTable.begin();it_to != tokenTable.end();it_to++)
  {
    switch (it_to->genericType)
    {
        case MNEMONIC:
          object.insert(object.end(),it_to->specificInfo);
        break;

        case OPERAND:
          for ( it_sy = symbleTable.begin(); it_sy != symbleTable.end(); it_sy++)
          {
            if (it_to->token == it_sy->label)
            {
              it_to++;
              if (it_to->genericType == PLUS)
              {
                it_to++;
                if (it_to->genericType == CONST)
                {
                  object.insert(object.end(),it_sy->position + it_to->specificInfo);
                  break;
                }else
                {
                  it_to--;
                  cout << "second pass: error!" << endl;
    								break;
                }
              }
              else
              {
                object.insert(object.end(),it_sy->position);
                it_to--;
                break;
              }
            }
          }
        break;
        case DIRECTIVE:
            switch (it_to->specificInfo)
            {
              case SPACE:
                it_to++;
                if (it_to->genericType == CONST && it_to != tokenTable.end())
                {
                  for ( int i = 0; i < it_to->specificInfo; i++)
                  {
                    object.insert(object.end(),0);
                  }

                } else {
                    object.insert(object.end(),0);
                }
                break;
                case CON:
                  it_to++;
                  if (it_to->genericType == CONST)
                  {
                    object.insert(object.end(),it_to->specificInfo);
                  }
                  else
                  {
                    it_to--;
                    cout << "second pass: error!" << endl;
                  }
                break;

                default:
                break;
              }
        break;

        default:
        break;
    }
    if (it_to == tokenTable.end()){
			break;
		}
  }
}

void Compiler::writeOutput(string name){
  std::size_t found = name.find(".asm");
  std::size_t found2 = name.find("/");
  string extension, newName, filepre, fileobj;

  extension.append(name, 0, found);
  cout<<extension<<endl;
  newName.append(extension, found2+1, extension.length());
  filepre = "outputFiles/" + newName + ".pre";
  fileobj = "outputFiles/" + newName + ".obj";
  ofstream myfile (filepre);
  if (myfile.is_open()){
    for(int i=0;i < (int)this->code.size();i++){
      myfile<<this->code[i];
      myfile<<"\n";
    }
    myfile.close();
  }else cout << "Unable to open the preprocessed file (file.pre).";

  /*
  ofstream myfile (fileobj);
  if (myfile.is_open()){
    for(int i=0;i < (int)this->code.size();i++){
      myfile<<this->code[i];
      myfile<<"\n";
    }
    myfile.close();
  }else cout << "Unable to open the object file (file.obj).";
  */

}

int main(int argc, char* argv[])
{
  if(argc<0){
    cout<< "Too few parameters!" << endl;
  }
  string fileName = argv[1];
  std::vector<string> code;
  Compiler com;

  com.getCode(fileName);
  com.preprocessing();
  com.scaner();
  com.parser();
  com.semanticErrorTreat();
  if(error==1) return 0;
  com.firstPass();
  com.secondPass();

  com.writeOutput(fileName);
  return 0;
}
