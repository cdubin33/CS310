#include <iostream>

struct Token
{
  int type;   // will be 256 for identifier; 257 for int; 258 for string
  std::string text;  // the token itself
  int line;  // the line number on which the token occurs
};
