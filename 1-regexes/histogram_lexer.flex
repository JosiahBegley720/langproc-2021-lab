%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "histogram.hpp"


// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}

/* TODO: get value out of yytext and into yylval.numberValue */
/* TODO: get value out of yytext and into yylval.wordValue */
%%

[+-]?([1-9][0-9]?+\.?[0-9]*)        { fprintf(stderr, "Number : %s\n", yytext);  yylval.numberValue = atof(yytext);  return Number; }

[+-]?([0-9]+\/[0-9]+)        { fprintf(stderr, "Number : %s\n", yytext);

int i = 0;
std::string num = "";
std::string den = "";
std::string input = "";


while(*yytext != NULL){
   input.push_back((*yytext));
   *(++yytext);
}

while(input[i] != 47){
   num.push_back(input[i]);
   i++;
}
i++;

while(i < input.size()){
   den.push_back(input[i]);
   i++;
}

yylval.numberValue = std::stod(num)/std::stod(den);

return Number; }

[a-zA-Z]+          { fprintf(stderr, "Word : %s\n", yytext); yylval.wordValue = new std::string; *yylval.wordValue = yytext; return Word; }


(\[[^(\])]*\]|\[\(\]|\[\)\])  { fprintf(stderr, "Word : %s\n", yytext);
std::string input = "";
std::string output = "";
while(*yytext != NULL){
   input.push_back((*yytext));
   *(++yytext);
}

for(int i = 1; i < input.size()-1; i++){
   output.push_back(input[i]);
}

const char *mychar = output.c_str();

yylval.wordValue = new std::string; *yylval.wordValue = mychar; return Word;} 


\n              { fprintf(stderr, "Newline\n"); }

. {fprintf(stderr, "Word : %s\n", yytext);}

%%

void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
  exit(1);
}