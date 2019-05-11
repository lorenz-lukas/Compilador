#include <unistd.h>
#include <iostream>
#include <list>
#include <iterator>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>

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
    std::vector<vector<string> > macrotable;
};

//std::vector<string> Compiler::getCode(string name){
void Compiler::getCode(string name)
{
  string line;
  ifstream myfile (name);
  if (myfile.is_open())
  {
    while (getline(myfile,line))
    {

      cout << line << '\n';
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
  //if (found != string::npos)
  //    cout << "First occurrence is " << found << endl;
  //for(int i = 0; i<= index; i++)temp1.push_back(this->codeRaw[i]);

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
  //temp1[ind] = line;
  //for(int i = index+1; i< (this->codeRaw.size()); i++) temp2.push_back(this->codeRaw[i]);
  //temp1.insert(temp1.end(),temp2.begin(),temp2.end());

  //cout<< "\n\n   LABEL CORRECTED:" << endl;
  //for(int i = 0; i < temp1.size() ; i++)cout<< temp1[i]<<endl;
  //this->codeRaw = temp1;
  return line;
}

void Compiler::equIf(string line){

}
void Compiler::preprocessing()
{
  string line;
  for(int i = 0; i < this->codeRaw.size() ; i++){
    line = this->codeRaw[i];
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
    if(line.find(":") != std::string::npos){
      line = this->brokenLabel(line, &i);
    }
    if(line.find("EQU", 0) != std::string::npos){
      this->equIf(line);
    }
    this->code.push_back(line);
    
  }
  cout<<"      CODE CORRECTED:"<< endl;
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
