#include <unistd.h>
#include <iostream>
#include <list>
#include <iterator>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <typeinfo>

// GENERIC TOKEN TYPE -> used in genericType
#define MNEMONIC    1
#define LABEL       2
#define DIRECTIVE   3
#define OPERAND     4
#define CONST       5
#define COMMA       6
#define PLUS        7
#define AMPERSAND   8
//SPECIFIC TOKEN INFORMATION -> used in specificInfo
//OPERANDS:
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
//DIRECTIVES
#define SECTION   1
#define SPACE     2
#define CONST     3
#define TEXT      4
#define DATA      5

#define INVALID -1


using namespace std;
class Compiler
{
  public:
    void getCode(string name);
    void preprocessing();
      void getMacro(string line,int *i);
      void expMacro(string line,int *i);
      void equIf(string line);
      string brokenLabel(string line, int *i);
    void errorTreatment()
      void scaner();//lexic error
      void parser();//sintatic error
      void semanErrorDetection();
    void firstPass();
    void secondPass();


    std::vector<string> codeRaw, code;
    std::vector<vector<string> > macrotable, equIfTable;
    string instructions[14] =
    {"ADD","SUB","MULT","DIV","JUMP","JUMPN","JUMPP","JUMPZ","COPY","LOAD","STORE","INPUT","OUTPUT","STOP"};
    int sectionData = 0, sectionText = 0;

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
    
    std::vector<Token> tokenTable, labelTable;
    std::vector<Symbol> symbleTable;
    std::vector<int> object;
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
  string name;
  int index = *i;
  int j = index+1;
  cout<< index << endl;
  if(found != string::npos){
    name.append(line, 0, found);
    found = line.find("&");
    if(found!=string::npos){//Arguments not equal to zero
      name+=" ";
      name.append(line, found, line.length());
    }
    temp1.push_back(name);
    for(; j<= this->codeRaw.size(); j++){
      line = this->codeRaw[j];
       if(line[0]==32){
        string instruction;
        for(j=0;j<line.length();j++){
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
    //*i = j;
    cout<< j << endl;
  }else cout<< "Macro definition error! Miss ':' marker." << endl;
}

void Compiler::expMacro(string line,int *i){

  //cout<< this->codeRaw[i] << endl;
  /*int j = i;
  for(;j < (int)this->codeRaw.size() ; j++){
      for(int k = 0; k < this->codeRaw[j].length(); k++){
          if((int) this->codeRaw[j][k] >= 33) break;
      }
  }
  i=j;
  cout << this->codeRaw[j] << endl;
  */

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
  std::vector<string> temp;
  int j = 0;
  for(int i = 0; i < this->codeRaw.size() ; i++){
    line = this->codeRaw[i];
    control = 1;
    if(!line.empty()){
      //removes comments
      line = line.substr(0, line.find(';'));
      // Turn all upper.
      for(int j=0; j<line.length(); j++)line.at(j) = toupper(line.at(j));
      // Shift correction
      /*if(line[0]==32){
        string instruction;
        for(j=0;j<line.length();j++){
          if(line[j]>32)break;
        }
        int size = line.length();
        instruction.append(line, j, size);
        line = instruction;
      } */
      // Identify if the line is a macro
      if(line.find("MACRO") != std::string::npos){
          this->getMacro(line, &i);
      }
      // Macro expansion
      for(int j = 0; j < (int)this->macrotable.size(); j++){
        if(line.find(this->macrotable[0][j])){
          this->expMacro(line, &i);
          break;
        }
      }
      if(line.find(":") != std::string::npos && !sectionData){
        line = this->brokenLabel(line, &i);
      }
      if(line.find("EQU", 0) != std::string::npos){
        this->equIf(line);
      }
      if(line.find("IF", 0) != std::string::npos){
        string name;
        int j=0;
        std::size_t found = line.find("IF");
        name.append(line, found+3, line.length());

        while(j < (int)this->equIfTable.size()){
          if(name == this->equIfTable[j][0])break;
          j++;
        }

        string temp = this->equIfTable[j][1];

        if(int(temp[0]) == 48){ // EQU 0
          i++;
        }
        control = 0;
      }
      if(line.find("SECTION DATA") != std::string::npos){
        this->sectionData = 1;
      }
      if(!line.empty() && control){// Qualquer linha ou EQU 1
          temp.push_back(line);
      }

    }
  }
  for(control = 0;control<temp.size();control++){
    line = temp[control];
    if(line.find("SECTION TEXT") != std::string::npos){
      break;
    }
  }

  for(int i = control; i < temp.size(); i++) this->code.push_back(temp[i]);
  cout<<"\n\n      CODE CORRECTED:"<< endl;
  for(int i = 0; i < this->code.size() ; i++) cout<<this->code[i]<<endl;
}

//////////////////////////////////////////////////
////////////////////////////////////// FIRST PASS
//////////////////////////////////////////////////

void Compiler::scaner(){

}
void Compiler::parser(){

}
void Compiler::semErroTreat(){
}
void Compiler::firstPass(){ //done
  vector<Token>::iterator it;
  Symbol symbol;
  int posCounter=0;
  for (it = tokenTable.begin();it != tokenTable.end();it++)
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
            posCounte++;
            it--;
          }

        break;

        case CONST:
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
  
  for (it = tokenTable.begin();it != tokenTable.end();it++)
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
        switch (it->specificInfo)
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

          case CONST:
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


int main(int argc, char* argv[])
{
  if(argc<0){
    cout<< "Too few parameters!" << endl;
  }
  string fileName = argv[1];
  std::vector<string> code;
  std::

  Compiler com;

  com.getCode(fileName);
  com.preprocessing();
  com.scaner();
  com.parser();
  com.semErroTreat();
  com.firstPass();
  com.secondPass();
  return 0;
}
