#include <iostream>
#include <string>
#include <map>
#include <cstring>
#include "utils/utilities.hpp"
#include "utils/strutils.hpp"
using namespace std;
int main(int argc, char const *argv[])
{
std::map<std::string, std::string> context;
utils::readMapFromFile(".template/map.txt", context);
string __unparsedTemplate__ = utils::readFile(".template/local0.html");

string __variable0;
__variable0 = __unparsedTemplate__.substr(0, 0);
cout << __variable0;
return 0;
}
