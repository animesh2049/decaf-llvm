#include <iostream>
#include <fstream>
#include <cstdio>

extern "C" FILE *yyin;
extern "C" int yyparse();

std::fstream flex_outfile, bison_outfile;

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
	flex_outfile.open("flex_output.txt",std::ios_base::out);
	bison_outfile.open("bison_output.txt",std::ios_base::out);
	while(!feof(yyin)){
		yyparse();
	}
	std::cout << "Success!" << std::endl;
}
