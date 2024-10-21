#ifndef PROCESSOR
#define PROCESSOR

#include <stdio.h>

#include "Stack.h"
#include "Common.h"

//-----------------------------------------------------------

const int N_REGS = 4;

struct SPU_t
    {
    Stack_t stack;
    int registers[N_REGS];
    };

//-----------------------------------------------------------

RUNTIME_ERRORS Processor (FILE* machine_code);
void SPUInit (SPU_t* SPU);
void SPUDestroy (SPU_t* SPU);
void SPUDump (SPU_t* SPU, bool stack_dump, const char* file, int line, const char* func);

//-----------------------------------------------------------

#endif // PROCESSOR