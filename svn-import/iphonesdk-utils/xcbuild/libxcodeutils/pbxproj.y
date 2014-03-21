/*
 *  parser.y
 *  parser
 *
 *  Created by Jonny Yu on 11/17/10.
 *  Copyright 2010 Autodesk Inc. All rights reserved.
 *
 */

%{
#include <stdio.h>
#include <string.h>
#include <list>
#include "pbxprojdef.h"

extern "C"
{
    extern int yylineno;
    extern FILE* yyin;
    int yyparse(void);
    int yylex(void);
    int yywrap(void);
    void yyerror(const char *str);
    
    int yywrap()
    {
        //only one file
        return 1;
    }
    
    static bool sFailed = false;
    void yyerror(const char *str)
    {
        sFailed = true;
        fprintf(stderr, "[%d]error: %s\n", yylineno, str);
    }
    
    static PBXFile* gpDoc = NULL;
    bool loadDocument(const char* filePath, PBXFile **pDoc)
    {
        //init
        *pDoc = NULL;
        gpDoc = new PBXFile;
        sFailed = false;
        
        //parse
        yyin = fopen(filePath, "r");
		if (yyin != NULL)
		{
			yyparse();
		}
        
        if (sFailed)
            delete gpDoc;
        else
            *pDoc = gpDoc;
        
        fclose(yyin);
        gpDoc = NULL;
        yyin = NULL;
                
        return !sFailed;
    }
}
    
%}

%union
{
    /* C types shared with lex */
    int intValue;
    char * string;
    #ifdef __cplusplus
    PBXFile     * document;
    PBXItem         * statement;
    PBXBlock        * block;
    PBXArray        * array;
    PBXValue        * value;
    #endif
}

%token <intValue>       INTEGER
%token <string>         ID
%token <string>         WORD
%token <string>         STRING
%token <string>         PREAMBLE
%token <string>         COMMENT


%token COMMA LBRACKET RBRACKET OBRACE EBRACE SEMICOLON QUOTE ASSIGN

%type <document>    document
%type <block>       block
%type <array>       array
%type <statement>   statement
%type <block>       statements
%type <value>       value
%type <array>       values

%start document

%%

document    :   PREAMBLE block
                {
                    gpDoc->setPreamble($1);
                    gpDoc->setBlock($2);
                    $$ = gpDoc;
                }
                ;

block       :   OBRACE statements EBRACE
                {
                    $$ = $2;
                }
                ;

statements  :   /* Empty */
                {
                    $$ = new PBXBlock;
                }
                ;
              | statements statement
                {
                    $1->addStatement($2);
                    $$ = $1;
                }
                ;
                
statement   :   WORD         ASSIGN value         SEMICOLON
                {
                    $$ = new PBXAssignment($1, $3);
                }
                ;
              | WORD COMMENT ASSIGN value         SEMICOLON
                {
                    PBXAssignment *assign = new PBXAssignment($1, $4);
                    assign->setKeyComment($2);
                    $$ = assign;
                }
                ;              
              | WORD         ASSIGN value COMMENT SEMICOLON
                {
                    $3->setComment($4);
                    $$ = new PBXAssignment($1, $3);
                }
                ;              
              | WORD COMMENT ASSIGN value COMMENT SEMICOLON  
                {
                    $4->setComment($5);
                    PBXAssignment *assign = new PBXAssignment($1, $4);
                    assign->setKeyComment($2);
                    $$ = assign;
                }
                ;
              | ID         ASSIGN value         SEMICOLON
                {
                    $$ = new PBXAssignment($1, $3);
                }
                ;
              | ID COMMENT ASSIGN value         SEMICOLON
                {
                    PBXAssignment *assign = new PBXAssignment($1, $4);
                    assign->setKeyComment($2);
                    $$ = assign;
                }
                ;              
              | ID         ASSIGN value COMMENT SEMICOLON
                {
                    $3->setComment($4);
                    $$ = new PBXAssignment($1, $3);
                }
                ;              
              | ID COMMENT ASSIGN value COMMENT SEMICOLON  
                {
                    $4->setComment($5);
                    PBXAssignment *assign = new PBXAssignment($1, $4);
                    assign->setKeyComment($2);
                    $$ = assign;
                }
                ;
              | COMMENT
                {
                    $$ = new PBXCommentItem($1);
                }
                ;
                
value       :   ID
                {
                    $$ = new PBXValueRef($1);
                }
                ;
              | INTEGER
                {
                    $$ = new PBXInteger($1);
                }
                ;
              | WORD
                {
                    $$ = new PBXText($1);
                }
                ;
              | STRING
                {
                    $$ = new PBXText($1);
                }
                ;
              | block
                {
                    $$ = $1;
                }
                ;
              | array
                {
                    $$ = $1;
                }
                ;

values      :   /* Empty */
                {
                    $$ = new PBXArray;
                }
                ;
              | values value
                {
                    $1->addValue($2);
                    $$ = $1;
                }
                ;
              | values value COMMA
                {
                    $1->addValue($2);
                    $$ = $1;
                }
              |  values value COMMENT COMMA
                {
                    $2->setComment($3);
                    $1->addValue($2);
                    $$ = $1;
                }
                ;

array       :   LBRACKET values RBRACKET
                {
                    $$ = $2;
                }
                ;
%%
