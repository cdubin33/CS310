#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int lineno = 1;         // keeps track of the input line number

struct Token
{
  int type;   // will be 256 for identifier; 257 for int; 258 for string
  std::string text;  // the token itself
  int line;  // the line number on which the token occurs
  int value;
  size_t len;
};

size_t  get_len(string text)
{
  size_t len = 0;

  for(size_t i = 0; i < text.length(); i++)
    {
      if(text[i] == '\\' && ((i + 1) < text.length()))
	{
	  if(text[i + 1] == '\\')
	    {
	      len++;
	    }
	}
      else
	{
	  len++;
	}
    }
  return len;
}

int str2int (string str)
{
  int num;
  istringstream s (str);
  s >> num;
  return num;
}

bool is_op(char c)
{
  char ops[] = {'+', '-', '.', '*', '%', ';', ',', '[', ']', '(', ')', '{', '}', '/', '<', '>', '=', '!'};

  for(int i = 0; i < 18; i++)
    {
      if(ops[i] == c)
	{
	  return true;
	}
    }
  return false;
}

void print(string str)
{
  cout << str;
  return;
}

void printn(string str)
{
  cout << str << endl;
  return;
}

void printc(char c)
{
  cout << c;
  return;
}

void printnum(int i)
{
  cout << i;
  return;
}

bool toke(Token t)
{
  return !(t.type == 0);
}

string update_toke(string text)
{
  string new_str = "";
  size_t i = 0;
  
  while(i < text.length())
    {
      if(text[i] == '\\' && ((i + 1) < text.length()))
	{
	  if(text[i + 1] == 'n')
	    {
	      new_str += '\n';
	      i++;
	    }
	  else if(text[i + 1] == '"')
	    {
	      new_str += '\"';
	      i++;
	    }
	  else if(text[i + 1] == 't')
	    {
	      new_str += '\t';
	      i++;
	    }
	  else if(text[i + 1] == '0')
	    {
	      new_str += '\0';
	      i++;
	    }
	}
      else
	{
	  new_str += text[i];
	}
      i++;
    }
  return new_str;
}

int main()
{
  Token int_lit;
  int_lit.type = 0;
  int_lit.value = 0;
  int_lit.line = 0;
  Token ident;
  ident.type = 0;
  ident.text = "";
  ident.line = 0;
  Token string_lit;
  string_lit.type = 0;
  string_lit.text = "";
  string_lit.line = 0;
  char curr_char;
  bool intl = false;
  bool stringl = false;
  bool iden = false;  
  string temp_token = "";

  while(cin.get(curr_char))
    {
      if(stringl)
	{
	  if(curr_char == '"' || curr_char == '\n' || curr_char == '\0')
	    {
	      if(curr_char == '"')
		{
		  if(temp_token[temp_token.length() - 1] == '\\')
		    {
		      temp_token += '"';		      
		    }
		  else
		    {
		      if(toke(string_lit))
			{
			  stringl = false;
			  if(string_lit.len < get_len(temp_token))
			    {
			      string_lit.text = temp_token;
			      string_lit.line = lineno;
			      string_lit.len = get_len(temp_token);
			      temp_token = "";
			    }
			}
		      else
			{
			  stringl = false;
			  string_lit.text = temp_token;
			  string_lit.line = lineno;
			  string_lit.type = 258;
			  string_lit.len = get_len(temp_token);
			  temp_token = "";
			}
		    }
		}
	      else if(toke(string_lit))
		{
		  stringl = false;
		  if(string_lit.len < get_len(temp_token))
		    {
		      string_lit.text = temp_token;
		      string_lit.line = lineno;
		      string_lit.len = get_len(temp_token);
		      temp_token = "";
		    }
		}
	      else
		{
		  stringl = false;
		  string_lit.text = temp_token;
		  string_lit.line = lineno;
		  string_lit.type = 258;
		  string_lit.len = get_len(temp_token);
		  temp_token = "";
		}
	    }
	  else
	    {
	      temp_token += curr_char;
	    }
	}
      else if(curr_char == '"')
	{
	  if(intl)
	    {
	      if(int_lit.value < str2int(temp_token))
		{
		  int_lit.value = str2int(temp_token);
		  int_lit.line = lineno;
		}
	    }
	  else if(iden)
	    {
	      if(ident.len < get_len(temp_token))
		{
		  ident.len = get_len(temp_token);
		  ident.text = temp_token;
		  ident.line = lineno;
		}
	    }  	
	  stringl = true;
	  temp_token = "";
	}
      else if(curr_char == '\n')
	{
	  if(intl)
	    {
	      if(!toke(int_lit))
		{
		  int_lit.type = 257;
		  int_lit.value = str2int(temp_token);
		  int_lit.line = lineno;
		}
	      else if(int_lit.value < str2int(temp_token))
		{
		  int_lit.value = str2int(temp_token);
		  int_lit.line = lineno;
		}
	      intl = false;
	      temp_token = "";
	    }
	  else if(iden)
	    {
	      if(!toke(ident))
		{
		  ident.type = 256;
		  ident.text = temp_token;
		  ident.len = get_len(temp_token);
		  ident.line = lineno;
		}
	      else if(ident.len < get_len(temp_token))
		{
		  ident.len = get_len(temp_token);
		  ident.text = temp_token;
		  ident.line = lineno;
		}
	      iden = false;
	      temp_token = "";
	    }
	  else if(stringl)
	    {
	      cout << "ERROR! in string_lit" << endl; 
	    }
	  lineno++;
	}
      else if(!isspace(curr_char) && !is_op(curr_char))
	{
	  if(isalpha(curr_char))
	    {
	      if(iden)
		{
		  temp_token += curr_char;
		}
	      else if(intl)
		{
		  intl = false;
	    
		  if(!toke(int_lit))
		    {
		      int_lit.type = 257;
		      int_lit.value = str2int(temp_token);
		      int_lit.line = lineno;
		    }
		  else if(int_lit.value < str2int(temp_token))
		    {
		      int_lit.value = str2int(temp_token);
		      int_lit.line = lineno;
		    }
		  intl = false;
		  iden = true;
		  temp_token = curr_char;
		}
	      else
		{
		  iden = true;
		  temp_token = curr_char;
		}
	    }
	  else if(isdigit(curr_char))
	    {
	      if(iden || intl)
		{
		  temp_token += curr_char;
		}
	      else
		{
		  intl = true;
		  temp_token = curr_char;
		}
	    }
	  else if(curr_char == '_')
	    {
	      if(iden)
		{
		  temp_token += curr_char;
		}
	      else
		{
		  intl = false;
		  if(temp_token != "")
		    {	      
		      if(!toke(int_lit))
			{
			  int_lit.type = 257;
			  int_lit.value = str2int(temp_token);
			  int_lit.line = lineno;
			}
		      else if(int_lit.value < str2int(temp_token))
			{
			  int_lit.value = str2int(temp_token);
			  int_lit.line = lineno;
			}
		      temp_token = "";
		    }
		}
	    }
	  else
	    {
	      if(temp_token != "")
		{
		  if(intl)
		    {
		      if(!toke(int_lit))
			{
			  int_lit.type = 257;
			  int_lit.value = str2int(temp_token);
			  int_lit.line = lineno;
			}
		      else if(int_lit.value < str2int(temp_token))
			{
			  int_lit.value = str2int(temp_token);
			  int_lit.line = lineno;
			}
		      intl = false;
		      temp_token = "";
		    }
		  else if(iden)
		    {
		      if(!toke(ident))
			{
			  ident.type = 256;
			  ident.text = temp_token;
			  ident.len = get_len(temp_token);
			  ident.line = lineno;
			}
		      else if(ident.len < get_len(temp_token))
			{
			  ident.len = get_len(temp_token);
			  ident.text = temp_token;
			  ident.line = lineno;
			}
		      iden = false;
		      temp_token = "";
		    }
		  else if(stringl)
		    {
		      if(!toke(string_lit))
			{
			  string_lit.type = 258;
			  string_lit.text = temp_token;
			  string_lit.line = lineno;
			  string_lit.len = get_len(temp_token);
			}
		      else if(string_lit.len < get_len(temp_token))
			{
			  string_lit.text = temp_token;
			  string_lit.line = lineno;
			  string_lit.len = get_len(temp_token);
			}
		    }
		}
	    }
	}
      else
	{
	  if(temp_token != "")
	    {
	      if(intl)
		{
		  if(!toke(int_lit))
		    {
		      int_lit.type = 257;
		      int_lit.value = str2int(temp_token);
		      int_lit.line = lineno;
		    }
		  else if(int_lit.value < str2int(temp_token))
		    {
		      int_lit.value = str2int(temp_token);
		      int_lit.line = lineno;
		    }
		  intl = false;
		  temp_token = "";
		}
	      else if(iden)
		{
		  if(!toke(ident))
		    {
		      ident.type = 256;
		      ident.text = temp_token;
		      ident.len = get_len(temp_token);
		      ident.line = lineno;
		    }
		  else if(ident.len < get_len(temp_token))
		    {
		      ident.len = get_len(temp_token);
		      ident.text = temp_token;
		      ident.line = lineno;
		    }
		  iden = false;
		  temp_token = "";
		}
	      else if(stringl)
		{
		  if(!toke(string_lit))
		    {
		      string_lit.type = 258;
		      string_lit.text = temp_token;
		      string_lit.line = lineno;
		      string_lit.len = get_len(temp_token);
		    }
		  else if(string_lit.len < get_len(temp_token))
		    {
		      string_lit.text = temp_token;
		      string_lit.line = lineno;
		      string_lit.len = get_len(temp_token);
		    }
		}
	      temp_token = "";
	    }
	}
    }

  ident.text = update_toke(ident.text);
  string_lit.text = update_toke(string_lit.text);
  
  if(toke(ident))
    {
      cout << "ID on line " << ident.line << " : " << ident.text << endl;
    }
  else
    {
      printn("No identifiers.");
    }
  if(toke(int_lit))
    {
      cout << "INT on line " << int_lit.line << " : " << int_lit.value << endl;
    }
  else
    {
      printn("No integers.");
    }
  if(toke(string_lit))
    {
      cout << "STRING on line " << string_lit.line << " : " << string_lit.text << endl;
    }
  else
    {
      printn("No strings.");
    }
  return 0;
}
