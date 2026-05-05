%{
#include "ast.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int lineNum = 1;
int colNum = 1;

void updatePos() {
    for (int i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') {
            lineNum++;
            colNum = 1;
        } else {
            colNum++;
        }
    }
}
%}

%option noyywrap

DIGIT [0-9]
NUMBER -?({DIGIT}+)(\.{DIGIT}+)?([eE][+-]?{DIGIT}+)?
STRING \"([^\\"]|\\.)*\"
WS [ \t\r\n]+

%%

"{"         { updatePos(); return '{'; }
"}"         { updatePos(); return '}'; }
"["         { updatePos(); return '['; }
"]"         { updatePos(); return ']'; }
":"         { updatePos(); return ':'; }
","         { updatePos(); return ','; }

"true"      { updatePos(); return TRUE; }
"false"     { updatePos(); return FALSE; }
"null"      { updatePos(); return NULL_TOKEN; }

{NUMBER}    { 
                updatePos(); 
                yylval.numVal = atof(yytext); 
                return NUMBER; 
            }

{STRING}    { 
                updatePos(); 
                // Remove quotes
                int len = strlen(yytext);
                char *str = (char *)malloc(len - 1);
                strncpy(str, yytext + 1, len - 2);
                str[len - 2] = '\0';
                yylval.strVal = str;
                return STRING; 
            }

{WS}        { updatePos(); /* Ignore whitespace */ }

.           { 
                fprintf(stderr, "Error: unexpected character '%s' at line %d, column %d\n", yytext, lineNum, colNum);
                exit(1);
            }

%%
