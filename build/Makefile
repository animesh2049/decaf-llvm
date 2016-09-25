OBJS	= bison.o lex.o
SRC_PATH = ../src
CC	= g++
CXXFLAGS	= -pedantic
P_DIR = $(PWD)/../src
#BISON =

a.out:		$(OBJS)
		$(CC) $(CXXFLAGS) $(OBJS) -lfl
		rm *.o decaf_parser.tab.* lex.yy.c
		
lex.o:	lex.yy.c
		$(CC) $(CXXFLAGS) -c  lex.yy.c -o lex.o

lex.yy.c:		$(P_DIR)/decaf_lexer.l
		flex  $(P_DIR)/decaf_lexer.l

bison.o:	decaf_parser.tab.c
		$(CC) $(CXXFLAGS) -c decaf_parser.tab.c -o bison.o

decaf_parser.tab.c:	$(P_DIR)/decaf_parser.y
		bison -d -v $(P_DIR)/decaf_parser.y

lex.o	: decaf_parser.tab.h

clean:
	rm -f *.o *~  a.out lex.yy.c decaf_parser.tab.* decaf_parser.output bison_output.txt flex_output.txt
