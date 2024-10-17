#ifndef ASSEMBLER
#define ASSEMBLER

#include <stdio.h>

//-----------------------------------------------------------

enum SPU_COMMANDS
    {
    PUSH = 1,
    ADD =  2,
    SUB =  3,
    MUL =  4,
    DIV =  5,
    HLT =  -1
    };

void Assembler (FILE* code_in, FILE* code_out);

//-----------------------------------------------------------

#endif //ASSEMBLER