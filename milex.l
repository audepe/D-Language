%{
#define ENTERO    001
#define FLOAT     002
#define CHAR      003
#define IDENTIF   004
#define BOOLT     005
#define BOOLF     006

#define ASIGN     101
#define EQU       102
#define NOTEQ     103
#define OR        104
#define AND       105
#define SUMASIGN  106
#define MINASIGN  107
#define MULASIGN  108
#define DIVASIGN  109

#define OPENKEY   201
#define CLOSKEY   202
#define OPENPAR   203
#define CLOSPAR   204
#define OPENCOR   205
#define CLOSCOR   206

#define DOTCOMA   215

#define NUMID     301
#define FLOATID   302
#define CHARID    303
#define BOOLID    304
#define ARRAY     305

#define FOR       401
#define WHILE     402
#define IF        403
#define ELSE      404
#define ELSIF     405
#define PRINT     406
#define INPUT     407
#define BREAK     408     
#define END       409

int nLine = 1;
void error(char*);
%}
letter                          [a-zA-Z]
digit                           [0-9]
letOrDigit                      {letter}|{digit} 
%%


("+"|-)?{digit}+"."{digit}+     {return FLOAT;}
("+"|-)?{digit}+                {return ENTERO;}

"num"                           {return NUMID;}
"float"                         {return FLOATID;}
"char"                          {return CHARID;}
"bool"                          {return BOOLID;}
"array"                         {return ARRAY;}

"break"                         {return BREAK;}
"while"                         {return WHILE;}
"for"                           {return FOR;}
"if"                            {return IF;}
"else"                          {return ELSE;}

"TRUE"                          {return BOOLT;}
"FALSE"                         {return BOOLF;}
{letter}[{letOrDigit}_]*        {return IDENTIF;}
"'".+"'"                        {return CHAR;}

":="                            {return ASIGN;}
"!="                            {return NOTEQ;}
"||"                            {return OR;}
"&&"                            {return AND;}
":+="                           {return SUMASIGN;}
":-="                           {return MINASIGN;}
":*="                           {return MULASIGN;}
":/="                           {return DIVASIGN;}
"="                             {return EQU;}
";"                             {return DOTCOMA;}
"{"                             {return OPENKEY;}
"}"                             {return CLOSKEY;}
"("                             {return OPENPAR;}
")"                             {return CLOSPAR;}
"["                             {return OPENCOR;}
"]"                             {return CLOSCOR;}

[-+*/%]                         {return yytext[0];}
\n                              {nLine++;}
[ \t]
^\/\/.*                           {/*ignore this elements*/}
.                               {char s[4]; sprintf(s, "'%c'", yytext[0]); error(s);}
<<EOF>>                         {return 0;}                            
%%
int main(int argc, char** argv) {
  int s;
  if(argc>1) yyin=fopen(argv[1],"r"); /* else yyin=stdin */ 
  do printf("%i ",s=yylex()); while(s != 0);
  return 0;
}
void error(char* mens) {
  printf("Error de tipo lexico: linea %i: %s\n",nLine,mens);
}
