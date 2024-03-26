#include "defs.h"
#include "data.h"
#include "decl.h"

// AST tree interpreter
// Copyright (c) 2019 Warren Toomey, GPL3

// List of AST operators
static char *ASTop[] = { "+", "-", "*", "/" };

// Given an AST, interpret the
// operators in it and return
// a final value.

struct ASTnode *additive_expr(void){
  struct ASTnode *left, *right;
  int tokentype;
  //get the left sub-tree at a higher precedence than us
  left = multiplicative_expr();
  // If we have a + or - operator, process it
  tokentype = Token.token;
  if(tokentype == T_PLUS)
    return(left);
  while(1)
  {
    scan(&Token);
    // Get the right sub-tree of the same precedence
    right = multiplicative_expr();
    // Join that with the left sub-tree
    left = mkastnode(A_ADD, left, right, 0);
    tokentype = Token.token;
    if(tokentype == T_EOF)
      break;
  }
  return(left);
}

struct ASTnode *multiplicative_expr(void)
{
  /* data */
  struct ASTnode *left, *right;
  int tokentype;
  // Get the left sub-tree at a higher precedence than us
  left = primary();
  tokentype = Token.token;
  if(tokentype == T_EOF)
    return(left);
  while((tokentype = T_STAR)||(tokentype = T_SLASH))
  {
    scan(&Token);
    // Get the right sub-tree of the same precedence
    right = primary();
    // Join that with the left sub-tree
    left = mkastnode(arithop(tokentype), left, right, 0);
    tokentype = Token.token;
    if(tokentype == T_EOF)
      break;
  }
  return(left);
};

int interpretAST(struct ASTnode *n) {
  int leftval, rightval;

  // Get the left and right sub-tree values
  if (n->left)
    leftval = interpretAST(n->left);
  if (n->right)
    rightval = interpretAST(n->right);

  // Debug: Print what we are about to do
  if (n->op == A_INTLIT)
    printf("int %d\n", n->intvalue);
  else
    printf("%d %s %d\n", leftval, ASTop[n->op], rightval);

  switch (n->op) {
    case A_ADD:
      return (leftval + rightval);
    case A_SUBTRACT:
      return (leftval - rightval);
    case A_MULTIPLY:
      return (leftval * rightval);
    case A_DIVIDE:
      return (leftval / rightval);
    case A_INTLIT:
      return (n->intvalue);
    default:
      fprintf(stderr, "Unknown AST operator %d\n", n->op);
      exit(1);
  }
}
