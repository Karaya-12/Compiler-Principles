%{
	#include <stdlib.h>
	#include <stdio.h>
	#define StudentIDNum 201693072

	int num_lines = 0;
	int num_chars = 0;
	int num_vars = 0;
%}

alpha   [A-Za-z]
dig     [0-9]
ws      [ \t]+
string  \"[^\n"]+\"
var     ({alpha}|{dig}|\$)({alpha}|{dig}|[_.\-/$])*
num1    [-+]?{dig}+\.?([eE][-+]?{dig}+)?
num2    [-+]?{dig}*\.{dig}+([eE][-+]?{dig}+)?
number  {num1}|{num2}

%%
\n		        {
	                printf("\n");  //Catch Line Breaks
	                ++num_lines;
	                ++num_chars;
	            }

{ws}		    {
	                printf(" ");  // Catch Whitespaces
                    ++num_chars;
                }

{number}        {
                    int num = atoi(yytext);  //ASCII to Integer
                    if(num == StudentIDNum)
                        printf("Karaya_12");
                    num_chars += yyleng;  //Catch Numbers
                }

{var}		    {
                    printf("%s", yytext);  //Catch Vars
                    ++num_vars;
                    num_chars += yyleng;
                }

{string}        {
	                printf("%s", yytext);  //Catch Strings
                    ++num_vars;
                    num_chars += yyleng;
                }

.               {
	                printf("%s", yytext);  //Catch All Punctuation Marks
                    ++num_chars;
                }

%%
int main(int argc, char* argv[])
{
	yylex();
	printf("\n");
	printf("Number of Lines: %d\n", num_lines);
	printf("Number of Var: %d\n", num_vars);
	printf("Number of Chars: %d\n", num_chars);
}
