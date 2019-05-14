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
    //std::vector<string> getCode(string name);
    void getCode(string name);
    void preprocessing();
      void getMacro(string line);
      void expMacro(string line);
      void equIf(string line);
      string brokenLabel(string line, int *i);
    void firstPass();
    void secondPass();


    std::vector<string> codeRaw, code;
    std::vector<vector<string> > macrotable, equIfTable;
    int sectionData = 0, sectionText = 0;
};

//std::vector<string> Compiler::getCode(string name){
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
void Compiler::getMacro(string line){

}
void Compiler::expMacro(string line){

}
string Compiler::brokenLabel(string line, int *i){
  //std::vector<string> temp1,temp2;
  std::size_t found = line.find(":");
  int index = *i;
  int ind = index;
  //////  USAR O QUE TA COMENTADO PRA FUNÇÃO GET MACRO
  //if (found != string::npos)
  //    cout << "First occurrence is " << found << endl;
  //for(int i = 0; i<= index; i++)temp1.push_back(this->codeRaw[i]);
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
  //temp1[ind] = line;
  //for(int i = index+1; i< (this->codeRaw.size()); i++) temp2.push_back(this->codeRaw[i]);
  //temp1.insert(temp1.end(),temp2.begin(),temp2.end());

  //cout<< "\n\n   LABEL CORRECTED:" << endl;
  //for(int i = 0; i < temp1.size() ; i++)cout<< temp1[i]<<endl;
  //this->codeRaw = temp1;
  return line;
}

void Compiler::equIf(string line){
  std::vector<string> variable;
  string name, value = "0";
  std::size_t found = line.find(":");
  name.append(line, 0, found);
  if(line.find("0", 0) != std::string::npos) value = "0";
  else value = "1";
  variable.push_back(name);
  variable.push_back(value);
  this->equIfTable.push_back(variable);
}
void Compiler::preprocessing()
{
  string line;
  int control = 1;
  std::vector<string> temp;
  for(int i = 0; i < this->codeRaw.size() ; i++){
    line = this->codeRaw[i];
    control = 1;
    if(!line.empty()){
      //removes comments
      line = line.substr(0, line.find(';'));
      // Turn all upper.
      for(int i=0; i<line.length(); i++)line.at(i) = toupper(line.at(i));
      // Identify if the line is a macro
      if(line.find("BEGINMACRO") != std::string::npos){
          this->getMacro(line);
      }
      // Macro expansion
      for(int i = 0; i < macrotable.size(); i++){
        if(line.find(macrotable[0][i])){
          this->expMacro(line);
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

        for(; j < (int)this->equIfTable.size(); j++){
          if(name == this->equIfTable[0][j])break;
        }

        string temp = this->equIfTable[j][1];
        if(int(temp[0]) == 48){
          i+=2;
          line = this->codeRaw[i];
        }else control = 0;
      }
      if(line.find("SECTION DATA") != std::string::npos){
        this->sectionData = 1;
      }
      if(!line.empty() && control) temp.push_back(line);
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


int main(int argc, char* argv[])
{
  if(argc<0){
    cout<< "Too few parameters!" << endl;
  }
  //cout << argv[1] << endl;
  string fileName = argv[1];
  std::vector<string> code;

  Compiler com;

  com.getCode(fileName);
  com.preprocessing();
  //cout<< com.codeRaw[0] << endl;

  return 0;
}

/*
std::list<int> s;
s.push_back(1);
std::list<int>::iterator it = s.begin();

while(it != s.end())
{
  std::cout<<(*it)<<"  ";
  it++;
}
std::cout<<std::endl;
*/
