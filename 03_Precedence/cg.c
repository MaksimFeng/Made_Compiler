#include "defs.h"
#include "data.h"
#include "decl.h"


static int freereg[4];
static char *reglist[4] = {"%r8", "%r9", "%r10", "%r11"};

void freeall_registers(void)
{
    freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}
// Allocate a free register. Return the number of
// the register. Die if no available registers.
static int alloc_register(void){
    for (int i = 0; i<4; i++){
        if(freereg[i]){
            freereg[i] = 0;
            return(i);
        }
    }
    fprintf(stderr, "Out of registers!\n");
    exit(1);
}

// Return a register to the list of available registers.
// Check to see if it's not already there.
static void free_register(int reg){
    if(freereg[reg] !=0){
        fprintf(stderr, "Error trying to free register %d\n", reg);
        exit(1);
    }
    freereg[reg] = 1;
}


// Print out the assembly preamble
void cgpreamble()
{
  freeall_registers();
  fputs(
	"\tglobal\tmain\n"
	"\textern\tprintf\n"
	"\tsection\t.text\n"
	"LC0:\tdb\t\"%d\",10,0\n"
	"printint:\n"
	"\tpush\trbp\n"
	"\tmov\trbp, rsp\n"
	"\tsub\trsp, 16\n"
	"\tmov\t[rbp-4], edi\n"
	"\tmov\teax, [rbp-4]\n"
	"\tmov\tesi, eax\n"
	"\tlea	rdi, [rel LC0]\n"
	"\tmov	eax, 0\n"
	"\tcall	printf\n"
	"\tnop\n"
	"\tleave\n"
	"\tret\n"
	"\n"
	"main:\n"
	"\tpush\trbp\n"
	"\tmov	rbp, rsp\n",
  Outfile);
}

// Print out the assembly postamble
void cgpostamble()
{
  fputs(
	"\tmov	eax, 0\n"
	"\tpop	rbp\n"
	"\tret\n",
  Outfile);
}


int cgload(int value){
    int r = alloc_register();

    fprintf(Outfile, "\tmovq $%d, %s\n", value, reglist[r]);
    return(r);
}

int cgadd(int r1, int r2){
    fprintf(Outfile, "\taddq %s, %s\n", reglist[r1], reglist[r2]);
    free_register(r1);
    return(r2);
}

int cgmul(int r1, int r2){
    fprintf(Outfile, "\timulq %s, %s\n", reglist[r1], reglist[r2]);
    free_register(r1);
    return(r2);
}

int cgsub(int r1, int r2){
    fprintf(Outfile, "\tsubq %s, %s\n", reglist[r1], reglist[r2]);
    free_register(r1);
    return(r2);
}

int cgdiv(int r1, int r2){
    fprintf(Outfile, "\tmovq %s, %%rax\n", reglist[r1]);
    fprintf(Outfile, "\tcqo\n");
    fprintf(Outfile, "\tidivq %s\n", reglist[r2]);
    fprintf(Outfile, "\tmovq %%rax, %s\n", reglist[r1]);
    free_register(r2);
    return(r1);
}

void cgprintint(int r){
    fprintf(Outfile, "\tmovq %s, %%rdi\n", reglist[r]);
    fprintf(Outfile, "\tcall printint\n");
    free_register(r);
}