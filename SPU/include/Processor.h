#ifndef PROCESSOR
#define PROCESSOR

#include <stdio.h>

#include "Stack.h"
#include "Common.h"

//-----------------------------------------------------------

const int N_REGS = 4;
const int RAM_SIZE = 100;

struct SPU_t
    {
    Stack_t stack;
    int registers[N_REGS];
    int RAM[RAM_SIZE];
    int** code;
    int ip;
    };

const int YES = 1;
const int NO  = 0;

//-----------------------------------------------------------

runtime_error_t Processor (FILE* machine_code);
void SPUInit (SPU_t* SPU, int** code);
void SPUDestroy (SPU_t* SPU);
void SPUDump (SPU_t* SPU, bool stack_dump, const char* file, int line, const char* func);
int* GetArg (SPU_t* SPU);
bool JumpOrNo (int jump, Stack_t* stack);

//-----------------------------------------------------------

#endif // PROCESSOR