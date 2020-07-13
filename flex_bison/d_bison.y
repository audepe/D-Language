%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "../symbol_table/d_symbols.h"

  int yylex();
%}

%union {
  char *name;
  char *string;
  double d;
  int function;                       
  int i;
  int type;
  struct ast *node;             
  struct symlist *sym;
}

%token <d> REAL
%token <i> NUMBER
%token <string> CHAR
%token <name> NAME
%token <function> FUNCTION
%token <type> TYPE
%token IF ELSE WHILE FOR
%token AND OR
%token PRINT PRINTLN RETURN INPUT BREAK
%token EOL

%nonassoc <function> COMPARISON
%nonassoc UMINUS

%right '='
%left '+' '-'
%left '*' '/' '%'
%left '(' ')'

%type <node> expression expression_list list statement variable_declaration variable_definition
%type <sym> symbol_list

%start main_list

%%

main_list : /* nothing */
          | main_list function_definition EOL 
          | main_list EOL
          | main_list error EOL                                                         { yyerrok;}
          | main_list statement EOL                                                     {
                                                                                          eval($2);
                                                                                          treefree($2);
                                                                                        }
         ;

function_definition : FUNCTION TYPE NAME '(' ')' '{' list '}'                           { newfundef($2, $3, NULL, $7); } 
                    | FUNCTION TYPE NAME '(' symbol_list ')' '{' list '}'               { newfundef($2, $3, $5, $8); } 
                    ;

list : /* nothing */                                                                    { $$ = NULL; }
     | EOL  list                                                                        {;}
     | statement EOL list                                                               { if ($3 == NULL){
                                                                                            $$ = newast('L', $1, NULL);
                                                                                          } else {
                                                                                            $$ = newast('L', $1, $3);
                                                                                          }
                                                                                        }
     ;

symbol_list : variable_declaration                                                      { $$ = newsymlist($1, NULL); }
            | variable_declaration ',' symbol_list                                      { $$ = newsymlist($1, $3); }
            ;

statement : variable_declaration ';'
          | variable_definition ';'
          | expression ';'
          | IF '(' expression ')' '{' list '}'                                          { $$ = newif($3,$6,NULL); }
          | IF '(' expression ')' '{' list '}' ELSE '{' list '}'                        { $$ = newif($3,$6,$10); }
          | FOR '(' variable_declaration ';' expression ';' expression ')' '{' list '}' { $$ = newfor($3,$5,$7,$10); }
          | FOR '(' variable_definition ';' expression ';' expression ')' '{' list '}'  { $$ = newfor($3,$5,$7,$10); }
          | WHILE '(' expression ')' '{' list '}'                                       { $$ = newast('W',$3,$6); }
          | RETURN expression ';'                                                       { $$ = newast('R', $2, NULL); }
          ;

variable_declaration  : TYPE NAME                                                       { $$ = newast('L', newref($1, $2), newasgn($2, newnum($1, 0, ""))); }
                      | TYPE NAME ':''=' expression                                     { $$ = newast('L', newref($1, $2), newasgn($2, $5)); }
                      ;

variable_definition : NAME ':''=' expression                                            { $$ = newasgn($1, $4); }
                    | NAME ':''+''=' expression                                         { $$ = newasgn($1, newast('+', $5, newref(0,$1))); }
                    | NAME ':''-''=' expression                                         { $$ = newasgn($1, newast('-', $5, newref(0,$1))); }
                    | NAME ':''*''=' expression                                         { $$ = newasgn($1, newast('*', $5, newref(0,$1))); }
                    | NAME ':''/''=' expression                                         { $$ = newasgn($1, newast('/', $5, newref(0,$1))); }
                    ;

expression  : NAME                                                                      { $$ = newref(0, $1); }
            | CHAR                                                                      { $$ = newnum(3, 0, $1); }
            | NUMBER                                                                    { $$ = newnum(1,$1, NULL); }
            | REAL                                                                      { $$ = newnum(2, $1, NULL); }
            | NAME '(' ')'                                                              { $$ = newcall($1, NULL);}
            | NAME '(' expression_list ')'                                              { $$ = newcall($1, $3); }
            | PRINT '(' expression ')'                                                  { $$ = newast('P', $3, NULL); }
            | PRINTLN '(' expression ')'                                                { $$ = newast('P', $3, newnum(0, 0, NULL)); }
            | '(' expression ')'                                                        { $$ = $2; }
            | '-' expression %prec UMINUS                                               { $$ = newast('M', $2, NULL); }
            | expression '+' expression                                                 { $$ = newast('+', $1,$3); }
            | expression '-' expression                                                 { $$ = newast('-', $1,$3); }
            | expression '*' expression                                                 { $$ = newast('*', $1,$3); }
            | expression '/' expression                                                 { $$ = newast('/', $1,$3); }
            | expression '%' expression                                                 { $$ = newast('%',$1,$3); }
            | '(' expression ')' AND '(' expression ')'                                 { $$ = newast('Y', $2, $6); }
            | '(' expression ')' OR  '(' expression ')'                                 { $$ = newast('O', $2, $6); }
            | expression COMPARISON expression                                          { $$ = newast($2, $1, $3); }
            ;             
           
expression_list : expression              
                | expression ',' expression_list                                        { $$ = newast('L', $1, $3); }
                ;

%%

void yyerror(char *s, ...)
{
  va_list ap;
  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

int main()
{
  init_list();
  return yyparse();
}
