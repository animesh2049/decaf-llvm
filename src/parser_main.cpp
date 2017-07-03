#include <iostream>
#include <fstream>
#include <cstdio>

#include "codegen.h"

extern "C" FILE *yyin;
extern "C" int yyparse();

Program *start;

// Check error reproting in lexer and parser

int main(int argc, char *argv[]){
	if(argc != 2){
		std::cerr << "Usage: parser inputfilepath\n" << std::endl;
	}
	FILE *myfile = fopen(argv[1], "r");
	if(!myfile){
		std::cerr << "Unable to open file" << std::endl;
		return -1;
	}
	else {
		yyin = myfile;
	}

	while(!feof(yyin)){
		yyparse();
	}



	/*Codegen *program_codegen = new Codegen(start);
	program_codegen->generate_code();

	std::cout << "Success!" << std::endl;*/
	return 0;
}
