%option noyywrap nodefault yylineno
%{
# include <string.h>
# include "../symbol_table/d_symbols.h"
# include "../build/d_bison.tab.h"
%}

%%
"+" |
"-" |
"*" |
"/" |
"%" |
"=" |
":" |
")" |
"(" |
"}" |
"{" |
"," |
";"                     { return yytext[0]; }


">"                     { yylval.function = 1; return COMPARISON; }
"<"                     { yylval.function = 2; return COMPARISON; }
"!="                    { yylval.function = 3; return COMPARISON; }
"=="                    { yylval.function = 4; return COMPARISON; }
">="                    { yylval.function = 5; return COMPARISON; }
"<="                    { yylval.function = 6; return COMPARISON; }

"&&"                    { return AND; }
"||"                    { return OR; }

"num"                   { yylval.type = 1; return TYPE; }
"float"                 { yylval.type = 2; return TYPE; }
"char"                  { yylval.type = 3; return TYPE; }
"bool"                  { yylval.type = 4; return TYPE; }
"void"                  { yylval.type = 5; return TYPE; }

"if"                    { return IF;}
"else"                  { return ELSE;}
"for"                   { return FOR;}
"while"                 { return WHILE;}
"break"                 { return BREAK; }

"function"              { return FUNCTION;}
"return"                { return RETURN; }
"input"                 { return INPUT; }
"print"                 { return PRINT; }
"println"               { return PRINTLN; }

[a-zA-Z][a-zA-Z0-9_]*   { yylval.name = substr(yytext, 0, yyleng); return NAME; }
"'"(.|\\.)"'"           { yylval.string = substr(yytext, 1, yyleng-1); return CHAR; } 
[0-9]+                  { yylval.i = atoi(yytext); return NUMBER; }
[0-9]+"."[0-9]*         { yylval.d = atof(yytext); return REAL; }


"//".*  
[ \t]

\\\n                    { printf("c> "); }

\n                      { return EOL; }

.                       { yyerror("Unknown character: %c\n", *yytext); }
%%