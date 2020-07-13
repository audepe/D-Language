extern int yylineno; 
void yyerror(char *s, ...);
void init_list();

struct symbol {
  char *name;
  int type;
  int scope;
  int iValue;
  double fValue;
  char *sValue;
  struct symlist *syms;
  struct ast *funcs;
};

struct ast {
  int nodetype;
  struct ast *l;
  struct ast *r;
};

struct ref {
  int nodetype;
  struct symbol *s;
};

struct ufncall {                
  int nodetype;                 
  struct ast *l;               
  char *name;
};

struct num {
  int nodetype;
  int type;
  double value;
  char *sValue;
};

struct symasgn {
  int nodetype;                 
  char *name;
  struct ast *v;                
};

struct symlist {
  struct ast *a;
  struct symlist *next;
};

struct flow {
  int nodetype;
  struct ast *cond; 
  struct ast *l;
  struct ast *r;
};

struct forflow{
  int nodetype;
  struct ast *declaration;
  struct ast *cond;
  struct ast *increment;
  struct ast *l;
};

struct symlist *newsymlist(struct ast *a, struct symlist *next);
struct ast * newast(int nodetype, struct ast *l, struct ast *r);
struct ast * newnum(int type, double value, char * sValue);
struct ast * newref(int vartype, char *name);
struct ast * newasgn(char *name, struct ast *v);
struct ast * newcall(char *name, struct ast *l);
struct ast * newif(struct ast *cond,struct ast *l, struct ast *r);
struct ast * newfor(struct ast *declaration, struct ast *cond, struct ast *increment, struct ast * l);
void newfundef(int type, char *name, struct symlist *syms, struct ast *func);

typedef struct node {
  struct symbol *val;
  struct node *next;
}node_t;

node_t *head;
node_t *tail;


struct symbol * eval(struct ast *);

void treefree(struct ast *);

char * substr(char * src, int start, int end);