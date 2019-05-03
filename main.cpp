#include <unistd.h>
#include <iostream>
#include <list>
#include <iterator>
#include <vector>

using namespace std;

class Compiler
{
  public:
    std::vector<string> getCode(string name);
  private:

};

std::vector<string> Compiler::getCode(string name){
  std::vector<string> code;

  //code.push_back();
  return code;
}

int main(int argc, char* argv[])
{
  std::vector<string> code;
  Compiler com;
  code = com.getCode("test.txt");
  cout<< code[0] << endl;

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
