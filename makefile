simplelex: lex.o token.o
	c++ -o simplelex lex.o token.o

lex.o: lex.cc token.h
	c++ -c lex.cc

token.o: token.cc token.h
	c++ -c token.cc
