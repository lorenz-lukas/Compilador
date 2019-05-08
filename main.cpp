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
      void expMacro();
      void equIf();
      void brokenLabel();
    void firstPass();
    void secondPass();
    std::vector<string> codeRaw, code;
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
      /*for (int i=0; i<line.length(); i++)     // Turn all upper.
        line.at(i) = toupper(line.at(i));
      line = line.substr(0, line.find(';'));*/  //removes comments
      cout << line << '\n';
      this->codeRaw.push_back(line);
    }
    myfile.close();
  }else
    cout << "Unable to open file";
}

void Compiler::preprocessing()
{
  string line;
  for(int i = 0; i < this->codeRaw.size() ; i++){
   line = this->codeRaw[i];
   line = line.substr(0, line.find(';'));  //removes comments
   for (int i=0; i<line.length(); i++)     // Turn all upper.
    line.at(i) = toupper(line.at(i));
   // this->expMacro(line);
    //this->equIf(line);
    //this->brokenLabel(line);
   cout << line << '\n';
  }
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
  cout<< com.codeRaw[0] << endl;

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
