#include <unistd.h>
#include <iostream>
#include <list>
#include <iterator>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <typeinfo>

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
    void firstPass();
      void scaner();
      void parser();
      void semErroTreat();
    void secondPass();


    std::vector<string> codeRaw, code;
    std::vector<vector<string> > macrotable, equIfTable;
    string instructions[14] =
    {"ADD","SUB","MULT","DIV","JUMP","JUMPN","JUMPP","JUMPZ","COPY","LOAD","STORE","INPUT","OUTPUT","STOP"};
    int sectionData = 0, sectionText = 0;

    struct Token {
      std::string str;
      int lineNumber;
      int tokenPosLin;
      int type;
      int extraInfo;
      int flag;
    };

    typedef struct Token Token;
    std::vector<Token> tokenList;
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
  int index = *i+1;
  int j = 0;
  if(found != string::npos){
    name.append(line, 0, found);
    found = line.find("&");
    if(found!=string::npos){//Arguments not equal to zero
        name+=" ";
        name.append(line, found, line.length());
        //cout<< name << endl;
    }
    temp1.push_back(name);
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
        //cout<< line << endl;
        temp1.push_back(line);
    }
    this->macrotable.push_back(temp1);
    *i = index;
    //cout<< j << endl;
  }else cout<< "Macro definition error! Miss ':' marker at line %d." << index-1 << endl;
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
          control = 0;
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
void Compiler::firstPass(){

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
  com.semErroTreat();
  com.firstPass();
  return 0;
}
