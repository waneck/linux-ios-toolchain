/*
 *  xcodeproj.l
 *  parser
 *
 *  Created by Jonny Yu on 11/17/10.
 *  Copyright 2010 Autodesk Inc. All rights reserved.
 *
 */

%{

#include <stdio.h>
#include <ctype.h>
#include "pbxproj.hpp"

%}

%option yylineno

%%
\/\/.*$                     {   yylval.string = strdup(yytext);
                                return PREAMBLE;    
                            } // one line
                            
"/*"((\*+[^/*])|([^*]))*\**"*/"            {   yylval.string = strdup(yytext);
                                return COMMENT;     
                            } // inline
                            
[0-9A-F]{24,24}             {   yylval.string = strdup(yytext);
                                return ID;          
                            }
                            
-?[0-9]+                    {   yylval.intValue = atoi(yytext);
                                return INTEGER;     
                            }
\"?[0-9a-zA-Z\.\/\$\-_\*=\<\>\[\]]+\"?  {   yylval.string = strdup(yytext); /*"CODE_SIGN_IDENTITY[sdk=iphoneos*]" = "iPhone Developer"; */
                                return WORD;        
                            }
                            
[  ]=[  ]                           {   return ASSIGN/* we have to add a space before and after =*/;      }


\"([^"\\]*(\\.[^"\\]*)*)\"  {   yylval.string = strdup(yytext);
                                return STRING;      
                            }
                                                        
,                           {   return COMMA;       }
;                           {   return SEMICOLON;   }
\{                          {   return OBRACE;      }
\}                          {   return EBRACE;      }
\(                          {   return LBRACKET;    }
\)                          {   return RBRACKET;    }
\n                          /*ignore EOL*/;
[ \t]+                      /*ignore whitespace*/;

%%
