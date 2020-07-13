#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "d_symbols.h"

int free_scope = 0;
int return_called = 0;
struct symbol *return_result;

void init_list(){
  if (!head){
    free(head);
  }

  head = (node_t *)malloc(sizeof(node_t));
  
  if (head == NULL){
    fprintf(stderr, "Error: Out of space");
    exit(0);
  }

  tail = head;
  head->next = NULL;
}


int get_int_digits(int value)
{
  int count = 0;
  count = value < 0 ? 1 : 0;
  value = abs(value);

  while(value != 0){
    count++;
    value /= 10;
  }

  return count;
}

int get_float_digits(long double value)
{
  int intPart = (int)value;
  int intDigits = get_int_digits(intPart);
  int digits = 1;
  value  = value < 0 ? -value : value;

  do{
    digits++;
    value *= 10;
    
    value = value - floor(value);
  }while(value > 0.00000001 && digits < 9);
  
  return digits + intDigits;
}

void set_free_scope(){
  
  if (head == NULL) {
    fprintf(stderr, "Error: Symbol table not initialized");
    exit(1);
  }

  node_t *aux = head;

  int free = 0;

  while(aux->next != NULL){
    aux = aux->next;
    if (aux->val->scope > free){
      free = aux->val->scope;
    }
  }

  free_scope = free + 1;
}

struct symbol *lookup(char *sym)
{
  if (head == NULL) {
    fprintf(stderr, "Error: Symbol table not initialized");
    exit(1);
  }

  node_t *aux = head;

  while(aux->next != NULL){
    aux = aux->next;
    if (aux->val != NULL && aux->val->scope == free_scope && (strcmp(aux->val->name, sym) == 0)){
      return aux->val;
    }
  }

  return NULL;
}

static void remove_scope_variables(int scope, char *func_name)
{
  if (head == NULL) {
    fprintf(stderr, "Error: Symbol table not initialized");
    exit(1);
  }

  node_t *aux = head;
  node_t *previus = aux;

  while(aux->next != NULL){
    previus = aux;
    aux = aux->next;
    if (aux->val != NULL && aux->val->scope == scope &&
        (strcmp(aux->val->name, func_name))){
            
      previus->next = aux->next;
      free(aux);
      aux = previus;
    }
  }

  tail = aux;
}

static struct symbol *lookup_func(char *name)
{
  if (head == NULL) {
    fprintf(stderr, "Error: Symbol table not initialized");
    exit(1);
  }

  node_t *aux = head;

  while(aux->next != NULL){
    aux = aux->next;
    if (aux->val != NULL && aux->val->scope > 0 && (strcmp(aux->val->name, name) == 0)){
      return aux->val;
    }
  }
 
  return NULL;
}

struct ast *newif(struct ast *cond,struct ast *l, struct ast *r)
{
  struct flow  *a= malloc(sizeof(struct flow));

  if(!a) {
    fprintf(stderr, "Error: Out of space");
    exit(0);
  }

  a-> nodetype = 'I';
  a->cond = cond;
  a->l = l;
  a->r = r;

  return (struct ast *)a;
}

struct ast *newcall(char *name, struct ast *l)
{
  struct ufncall *a = malloc(sizeof(struct ufncall));
  
  if(!a) {
    fprintf(stderr, "Error: Out of space");
    exit(0);
  }
  a->nodetype = 'C';
  a->l = l;
  a->name = name;
  
  return (struct ast *)a;
}

static struct symbol *calluser(struct ufncall *f)
{
  struct symbol *result, *auxResult;
  struct symbol *func = lookup_func(f->name);
  struct ast *a, *aux;
  struct symlist *sl;
  struct ast *paramValues;
  int nInputArgs = 0;
  int nParams = 0;
  int i = 0;
  int lastFree = free_scope;

  if (func == NULL){
    fprintf(stderr, "Error: Function %s not defined\n", f->name);
    exit(1);
  }
  
  if (f->l != NULL){
    nInputArgs++;
    a = f->l;
    while (a->nodetype == 'L'){
      nInputArgs++;
      a = a->r;
    }
  }

  if (func->syms != NULL){
    sl = func->syms;
    while (sl != NULL){
      nParams++;
      sl = sl->next;
    }
  }

  if (nInputArgs != nParams){
    fprintf(stderr, "Error: Expected %d params in function %s, got %d\n", nParams, f->name, nInputArgs);
    exit(1);
  }

  if (func->funcs == NULL){
    return NULL;
  }

  free_scope = func->scope;
  

  a = f->l;
  sl = func->syms;
  for (i = 0; i < nParams; i++){
    eval(sl->a);
    char *name = ((struct ref *)sl->a->l)->s->name;
    sl = sl->next;
    if (a->nodetype == 'L'){ 
     eval(newasgn(name, a->l));
     a = a->r;
    }else{
      eval(newasgn(name, a));
    } 
  }
  
  eval(func->funcs);

  if (return_called != 0){
    result = return_result;
    return_called = 0;
    if (result->type != func->type){
      fprintf(stderr, "Return type differs from function type\n");
      exit(1);
    }
  }

  remove_scope_variables(func->scope, f->name);
  free_scope = lastFree;

  return result; 
}

struct ast *newnum(int type, double value, char *sValue){

  struct num *a = malloc(sizeof(struct num));

  if(!a) {
    fprintf(stderr, "Error: Out of space");
    exit(0);
  }

  a->value = value;
  a->type = type;
  a->nodetype = 'K';
  a->sValue = sValue;

  return (struct ast *)a;
}


struct ast *newref(int vartype, char *name){

  struct ref *reference = malloc(sizeof(struct ref));
  
  if(!reference) {
    fprintf(stderr,"Error: Out of space");
    exit(0);
  }

  reference->nodetype = 'N';
  struct symbol *sym = malloc(sizeof(struct symbol));

  if(!sym) {
    fprintf(stderr, "Error: Out of space");
    exit(0);
  }

  reference->s = sym;
  sym->type = vartype;
  sym->name = name;

  return (struct ast *)reference;
}

struct ast *newast(int nodetype, struct ast *l, struct ast *r)
{
  struct ast *a = malloc(sizeof(struct ast));
  
  if(!a) {
    fprintf(stderr, "out of space");
    exit(0);
  }
  a->nodetype = nodetype;
  a->l = l;
  a->r = r;
  return a;
}


struct ast *newasgn(char *name, struct ast *v)
{
  struct symasgn *a = malloc(sizeof(struct symasgn));
  
  if(!a) {
    fprintf(stderr, "Error: Out of space");
    exit(0);
  }

  a->nodetype = '=';
  a->name = name;
  a->v = v;
  return (struct ast *)a;
}

void newfundef(int type, char *name, struct symlist *syms, struct ast *funcs)
{
  struct symbol *function = lookup_func(name);

  if (function != NULL){
    fprintf(stderr, "Error: Function %s already defined", name);
    exit(1);
  }

  set_free_scope();

  struct symbol *sym = malloc(sizeof(struct symbol));

  if(!sym) {
    fprintf(stderr, "Error: Out of space");
    exit(0);
  }

  sym->name = name;
  sym->type = type;
  sym->syms = syms;
  sym->scope = free_scope;
  sym->funcs = funcs;

  tail->next = (node_t *)malloc(sizeof(node_t));
  tail = tail->next;
  tail->val = sym;
  free_scope = 0;
}

struct symlist *newsymlist(struct ast *a, struct symlist *next)
{
  struct symlist *sl = malloc(sizeof(struct symlist));

  if(!sl) {
    fprintf(stderr, "Error: Out of space");
    exit(0);
  }

  sl->a = a;
  sl->next = next;
  return sl;
}


struct symbol *asignValue(struct ast *a)
{
  struct symasgn *asign = (struct symasgn *)a;
  struct symbol *s = lookup(asign->name);

  if (s == NULL){
    fprintf(stderr, "Error: Varible %s not found", asign->name);
    exit(1);
  }
  struct symbol *value = eval(asign->v);

  switch(s->type){
    case 1: s->iValue = (int)(value->fValue);
    case 2: s->fValue = value->fValue; break;
    case 3: s->sValue = value->sValue; break;
    default: fprintf(stderr, "Error: Error asigning value to %s\n", s->name);
  }

  return s;
}


struct symbol *numCalculator(int operation, struct symbol *a, struct symbol *b)
{

  struct symbol *result =  malloc(sizeof(struct symbol));

  if (a->type == 1 && b->type == 1){
    result->type = 1;
  }else{
    result->type = 2;
  }

  switch(operation){
    case '+': 
      result->fValue = a->fValue + b->fValue;
      result->iValue = a->iValue + b->iValue;
      break;
    case '-':
      result->fValue = a->fValue - b->fValue;
      result->iValue = a->iValue - b->iValue;
      break;
    case '*':
      result->fValue = a->fValue * b->fValue;
      result->iValue = a->iValue * b->iValue;
      break;
    case '/':
      if((result->type == 1 && !b->iValue) || (result->type == 2 && !b->fValue)){
        fprintf(stderr, "Error: Can not divide by zero\n");
        exit(1);
      }

      result->fValue = a->fValue / b->fValue;
      result->iValue = (int)(result->fValue);
      
      break;
    case '%':
      if(result->type != 1 || b->iValue <= 1 || a->iValue < 0){
        fprintf(stderr,"Error: Bad module operators\n");
        exit(1);
      }

      result->iValue = a->iValue % b->iValue;
      result->fValue = a->iValue % b->iValue;
      break;
  }

  return result;
}

struct symbol *orAndStmt(struct ast *input)
{
  struct symbol *result = malloc(sizeof(struct symbol));
  result->type = 1;

  if (result == NULL){
    fprintf(stderr, "Out of Space\n");
    exit(1);
  }

  struct symbol *a = eval(input->l);
  struct symbol *b = eval(input->r);

  if (!((a->type ==1 || a->type == 2) && (b->type == 1 || b->type == 2))){
    fprintf(stderr, "Or and And operations can only be performed with float and integer values\n");
    exit(1);
  }

  double aValue = a->type == 2 ? a->fValue : a->iValue;
  double bValue = b->type == 2 ? b->fValue : b->iValue;

  result->iValue = input->nodetype == 'O' ? (aValue) || (bValue) : (aValue) && (bValue); 

  return result;
}


struct symbol *logicalCalculator(int operation, struct symbol *a, struct symbol *b)
{

  struct symbol *result =  malloc(sizeof(struct symbol));
  result->type = 1;
  double aValue = a->type == 2 ? a->fValue : a->iValue;
  double bValue = b->type == 2 ? b->fValue : b->iValue;

  switch(operation){
    case 1: result->iValue = aValue > bValue; break;
      // <
    case 2: result->iValue = aValue < bValue; break;
      // <>
    case 3: result->iValue = aValue != bValue; break;
      // ==
    case 4: result->iValue = aValue == bValue; break;
      // >=
    case 5: result->iValue = aValue >= bValue; break;
      // <= 
    case 6: result->iValue = aValue <= bValue; break;
  }

    return result;
}

struct symbol *stringCalculator(int operation, struct symbol *a, struct symbol *b)
{
  struct symbol *result = malloc(sizeof(struct symbol));
  char *sValue;
  result->type = 3;
  
  switch(operation){
    case '+':
      if(a->type != 3 && b->type != 3){
        fprintf(stderr, "Error: Strings can only concatenate with another String");
        exit(1);  
      }
      int aLen = strlen(a->sValue);
      int bLen = strlen(b->sValue);
      sValue = malloc(sizeof(char) * (aLen+bLen));
      strcpy(sValue,a->sValue);
      strcat(sValue,b->sValue);
      result->sValue = sValue;
      break;
    case '*':
      if (a->type != 3 && b->type != 1 && b->iValue >= 0) {
        fprintf(stderr, "Error: Strings multiplication is only posible when the second expresion is an integer");
        exit(1);  
      }
      int sLen = strlen(a->sValue);
      sValue = malloc(sLen * b->iValue * sizeof(char));
      for(int i = 0; i < b->iValue; i++){
        strcat(sValue, a->sValue);
      }
      result->sValue = sValue;
      break;
    //>
    case 1:
      if(a->type != 3 && b->type != 3){
        fprintf(stderr, "Error: Strings can only be compared with another String");
        exit(1);  
      }
      result->type = 1;
      result->iValue = strcmp(a->sValue,b->sValue) > 0;
      break;
    // <
    case 2:
      if(a->type != 3 && b->type != 3){
        fprintf(stderr, "Error: Strings can only be compared with another String");
        exit(1);  
      }
      result->type = 1;
      result->iValue = strcmp(a->sValue,b->sValue) < 0;
      break;
    // <>
    case 3  :
      if(a->type != 3 && b->type != 3){
        fprintf(stderr, "Error: Strings can only be compared with another String");
        exit(1);  
      }
      result->type = 1;
      result->iValue = strcmp(a->sValue,b->sValue) != 0;
      break;
    // ==
    case 4  :
      if(a->type != 3 && b->type != 3){
        fprintf(stderr, "Error: Strings can only be compared with another String");
        exit(1);  
      }
      result->type = 1;
      result->iValue = strcmp(a->sValue, b->sValue) == 0;
      break;
    default : 
      fprintf(stderr, "Error: Strings does not support this type of operations");
      exit(1);
      break;
  }

  return result;
}

struct symbol *calculator(int operation, struct ast *a, struct ast *b)
{
  struct symbol *result;
  struct symbol *aux1 = eval(a);
  struct symbol *aux2 = eval(b);
   
  if(aux1->type == 3 || aux2->type==3){
    result = stringCalculator(operation, aux1, aux2);
    return result;
  }
  switch(operation){
    case '+': case '-': case '*': case '/': case '%':
      result = numCalculator(operation,aux1,aux2);
      break;
    // >
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
      result = logicalCalculator(operation, aux1, aux2);
      break;
  }

  return result;
}

struct ast *newfor(struct ast *declaration, struct ast *cond, struct ast *increment, struct ast * l)
{
  struct forflow *a = malloc(sizeof(struct forflow));

  if(!a){
    fprintf(stderr,"Error: Out of space");
    exit(0);
  }
  
  a->nodetype = 'F';
  a->declaration = declaration;
  a->cond = cond;
  a->increment = increment;
  a->l = l;

  return (struct ast *)a;
}

int isValidSymbol(struct ast *a){

  struct symbol *s = eval(a);
  
  switch(s->type){
    case 1: return s->iValue;
    case 2: return s->fValue;
    default: printf("Warning: Invalid condition\n");
  }

  return 0;
}

struct symbol *returnstm(struct ast *a)
{
  return_result = eval(a->l);
  return_called = 1;
  return return_result;
}

int isReturnInFunction(){
  return return_called != 0 && free_scope > 0;
}

struct symbol *negativeNumber(struct ast *a)
{
  struct symbol * b = eval(a->l);
  if(b->type == 1){
    b->iValue = -(b->iValue);
  }else if(b->type == 2){
    b->fValue = -(b->fValue);
  }
  return b;
}

struct symbol *forloop(struct ast *a)
{
  struct forflow *f = (struct forflow *)a;
  struct ast *inc;

  if(f->l != NULL){
    if(f->increment == NULL){
      struct symbol *sym = ((struct ref *)f->declaration->l)->s;      
      inc = newasgn(sym->name, newast('+', newref(sym->type, sym->name), newnum(1, 1, "")));
    }else{
      inc = f->increment;
    }
    
    for(eval(f->declaration); isValidSymbol(f->cond); eval(inc)){
      eval(f->l);
      if (isReturnInFunction()){
        break;        
      }
    }
  }
  
  return NULL;  
}

struct symbol *evalif(struct ast *a)
{
  struct flow *f= (struct flow *)a;
  
  if(isValidSymbol(f->cond)){
    eval(f->l);
  }else if(f->r != NULL){
    eval(f->r);
  }

  return NULL;
}

struct symbol *whileloop(struct ast *a)
{
   
  if(a->r != NULL){
    
    while(isValidSymbol(a->l) ){
      eval(a->r);
      if (isReturnInFunction()){
        break;        
      }
    }
  }

  return NULL;
}

struct symbol *evalRef(struct ast *a)
{
  struct ref *reference = (struct ref *)a;
    
  struct symbol *sym = lookup(reference->s->name);

  
  if (sym != NULL){
    if (reference->s->type != 0) sym->type = reference->s->type;
    return sym;
  }

  if (reference->s->type == 0){
    fprintf(stderr, "Error: Variable %s not defined\n", reference->s->name);
    exit(1);
  }

  reference->s->scope = free_scope;

  tail->next = (node_t *)malloc(sizeof(node_t));

  if(!tail->next) {
    fprintf(stderr, "Error: Out of space\n");
    exit(0);
  }

  tail = tail->next;
  tail->val = reference->s;


  return reference->s;
}

void evalPrint(struct ast* a)
{

  struct symbol *sym = eval(a->l);
  char *CRLF = "";
  if( a->r != NULL){
    CRLF = "\n";
  }
  
  switch(sym->type){
    case 1: printf("%d%s", sym->iValue, CRLF); break;
    case 2: printf("%0.4g%s", sym->fValue, CRLF); break;
    case 3: printf("%s%s", sym ->sValue,CRLF); break;
    default: fprintf(stderr, "Error: Bad varible type\n"); break;
  }
  
}

struct symbol *evalConstant(struct ast *a){
  struct symbol *sym = malloc(sizeof(struct symbol));
  struct num *value = (struct num *)a;

  sym->type = value->type;
  switch(value->type){
    case 1: sym->iValue = value->value;
    case 2: sym->fValue = value->value;
    case 3: sym->sValue = value->sValue;
      break;
    default:
      fprintf(stderr, "Error: Unkdown variable type\n");
      exit(1);
  }

  return sym;
}

struct symbol *evalList(struct ast *a)
{
  struct symbol *result;

  result = eval(a->l);
  if (isReturnInFunction()){
    return result;
  }

  return a->r != NULL ? eval(a->r) : result;
}

struct symbol *toString(struct ast *a)
{
  struct symbol *result = malloc(sizeof(struct symbol));
  int digitSize;
  result->type = 3;
  struct symbol *aux = eval(a->l);

  switch(aux->type){
    case 1:
      digitSize =  get_int_digits(aux->iValue);
      result->sValue = malloc((digitSize + 1) * sizeof(char));
      snprintf(result->sValue, digitSize + 1 , "%d", aux->iValue);
      break;
    case 2:
      digitSize =  get_float_digits(aux->fValue);
      result->sValue = malloc((digitSize + 1) * sizeof(char));
      snprintf(result->sValue, digitSize + 1 , "%f", aux->fValue);
      break;
    case 3:
      result->sValue = aux->sValue;
      break;
  }

  return result;
}

struct symbol *eval(struct ast *a)
{
  
  if(!a) {
    fprintf(stderr, "Error: Internal error, null eval");
    exit(1);
  }

  struct symbol *v = malloc(sizeof(struct symbol));

  switch(a->nodetype) {
    /* constant */
  case 'K': 
    v = evalConstant(a);
    break;

    /* name reference */
  case 'N': 
    v = evalRef(a);
    break;
  
    /* assignment */
  case '=': 
    v = asignValue(a);;
    break;

    /* expressions */
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case '+': 
  case '-': 
  case '*': 
  case '/': 
  case '%':
    v = calculator(a->nodetype, a->l, a->r);
    break;
  case 'P': 
    evalPrint(a);
    break;
  case 'M':
    v = negativeNumber(a);
    break;
  /* list of statements */
  case 'I':
    evalif(a);
    break;
  case 'L': v=evalList(a); break;
  case 'W':
    whileloop(a);
    break;
  case 'C': 
    v = calluser((struct ufncall *)a);
    break;  
  case 'F':
    forloop(a);
    break;
  case 'R':
    returnstm(a);
    break;
  case 'O':
    v = orAndStmt(a);
    break;  
  case 'Y':
    v = orAndStmt(a);
    break;
  case 'S':
    v =  toString(a);
    break;
  default: fprintf(stderr, "Warning: internal error: bad node %c\n", a->nodetype);
  }
  return v;
}

void treefree(struct ast *a)
{
  switch(a->nodetype) {

    /* two subtrees */
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case 'L': case 'Y': case 'O':
    treefree(a->r);
    treefree(a->l);
    break;
    /* no subtree */
  case 'K': case 'N':
    break;
  case 'P': case 'I': case 'W': case 'F': case 'S': case 'M':
    treefree(a->l);
    break;
  case 'C':
    if (a->l != NULL) treefree(a->l);
    break;
  case '=':
    free( ((struct symasgn *)a)->v);
    break;
  default: fprintf(stderr, "Warning: internal error: free bad node %c\n", a->nodetype);
  }       
  
  free(a); /* always free the node itself */
}

char *substr(char * src, int start, int end){
  
	int len = end - start;

	char *dest = (char*)malloc(sizeof(char) * (len + 1));

	// extracts characters between m'th and n'th index from source string
	// and copy them into the destination string
	for (int i = start; i < end && (*(src + i) != '\0'); i++)
	{
		*dest = *(src + i);
		dest++;
	}

	// null-terminate the destination string
	*dest = '\0';

	// return the destination string
	return dest - len;
}