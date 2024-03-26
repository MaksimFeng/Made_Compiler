
#include "defs.h"
#include "data.h"
#include "decl.h"


static int getAST(struct ASTnode *n){
    int leftreg, rightreg;
    if(n->left)
        leftreg = getAST(n->left);
    if(n->right)
        rightreg = getAST(n->right);
    switch(n->op){
        case A_ADD:
            return(cgadd(leftreg, rightreg));
        case A_SUBTRACT:
            return(cgsub(leftreg, rightreg));
        case A_MULTIPLY:
            return(cgmul(leftreg, rightreg));
        case A_DIVIDE:
            return(cgdiv(leftreg, rightreg));
        case A_INTLIT:
            return(cgload(n->intvalue));
        default:
            fprintf(stderr, "Unknown AST operator %d\n", n->op);
            exit(1);
}
}

void generatecode(struct ASTnode *n){
    int reg;
    cgpreamble();
    reg = getAST(n);
    cgprintint(reg);
    cgpostamble();
}