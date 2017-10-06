OBJS = bison.o lex.o parser_main.o
SRC_PATH = ../src
P_DIR = $(PWD)/../src
CC	= g++ --std=c++11 -g
BISON = bison
BISONFLAGS = -d -v
LEX = flex
LLVM_LIBS = `llvm-config-3.8 --cppflags --libs core --ldflags --system-libs`
CXXFLAGS	= -pedantic -I$(P_DIR)

parser:		$(OBJS)
		$(CC) $(CXXFLAGS) $(OBJS) -lfl -o parser $(LLVM_LIBS)
		rm *.o decaf_parser.tab.* lex.yy.c

parser_main.o: $(P_DIR)/parser_main.cpp
		$(CC) $(CXXFLAGS) -c $(P_DIR)/parser_main.cpp -o parser_main.o $(LLVM_LIBS)

lex.o:	lex.yy.c
		$(CC) $(CXXFLAGS) -c  lex.yy.c -o lex.o $(LLVM_LIBS)

lex.yy.c:		$(P_DIR)/decaf_lexer.l
		$(LEX)  $(P_DIR)/decaf_lexer.l

bison.o:	decaf_parser.tab.c
		$(CC) $(CXXFLAGS) -c decaf_parser.tab.c -o bison.o $(LLVM_LIBS)

decaf_parser.tab.c:	$(P_DIR)/decaf_parser.y
		$(BISON) $(BISONFLAGS) $(P_DIR)/decaf_parser.y

lex.o	: decaf_parser.tab.h

clean:
		rm -f *.o *~  parser lex.yy.c decaf_parser.tab.* decaf_parser.output bison_output.txt flex_output.txt
