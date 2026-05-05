%{
#include <iostream>
#include <string>
#include <vector>
#include "ast.h"

int yylex();
void yyerror(const char *s);
extern int lineNum;
extern int colNum;
AstNode *rootNode = nullptr;
%}

%union {
    char *strVal;
    double numVal;
    AstNode *node;
}

%token <strVal> STRING
%token <numVal> NUMBER
%token TRUE FALSE NULL_TOKEN
%type <node> value object array members member elements

%%

json:
    value { rootNode = $1; }
    ;

value:
    object { $$ = $1; }
    | array { $$ = $1; }
    | STRING { $$ = AstNode::createString($1); free($1); }
    | NUMBER { $$ = AstNode::createNumber($1); }
    | TRUE { $$ = AstNode::createBool(true); }
    | FALSE { $$ = AstNode::createBool(false); }
    | NULL_TOKEN { $$ = AstNode::createNull(); }
    ;

object:
    '{' '}' { $$ = AstNode::createObject(nullptr); }
    | '{' members '}' { $$ = AstNode::createObject($2); }
    ;

members:
    member { $$ = $1; }
    | member ',' members { $1->next = $3; $$ = $1; }
    ;

member:
    STRING ':' value { $$ = AstNode::createKeyValuePair($1, $3); free($1); }
    ;

array:
    '[' ']' { $$ = AstNode::createArray(nullptr); }
    | '[' elements ']' { $$ = AstNode::createArray($2); }
    ;

elements:
    value { $$ = $1; }
    | value ',' elements { $1->next = $3; $$ = $1; }
    ;

%%

void yyerror(const char *s) {
    std::cerr << "Error: " << s << " at line " << lineNum << ", column " << colNum << std::endl;
    exit(1);
}

int main(int argc, char **argv) {
    bool showAst = false;
    if (argc > 1 && std::string(argv[1]) == "--ast") {
        showAst = true;
    }

    if (yyparse() == 0) {
        if (rootNode) {
            if (showAst) {
                std::cout << "--- AST Structure ---" << std::endl;
                rootNode->printAst();
                std::cout << "--- XML Output ---" << std::endl;
            }
            rootNode->printXml();
            delete rootNode;
        }
    }
    return 0;
}
