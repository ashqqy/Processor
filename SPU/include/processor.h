#ifndef PROCESSOR
#define PROCESSOR

#include <stdio.h>
#include <stdbool.h>

#include "stack.h"
#include "common.h"

//-----------------------------------------------------------

#define N_REGS 5
#define RAM_SIZE 300

typedef struct 
{
    stack_t stack;
    stack_t stack_for_func;
    int registers[N_REGS];
    int RAM[RAM_SIZE];
    int** code;
    int ip;
} SPU_t;

static const int YES = 1;
static const int NO  = 0;

typedef enum 
{
    REGISTER_BIT = 1,
    CONSTANT_BIT = 2,
    MEMORY_BIT   = 4
} arg_bits_t;

//-----------------------------------------------------------

runtime_error_t Processor (FILE* machine_code);
void SPUInit (SPU_t* SPU, int** code);
void SPUDestroy (SPU_t* SPU);
void SPUDump (SPU_t* SPU, bool stack_dump, const char* file, int line, const char* func);
int* GetArg (SPU_t* SPU);
bool JumpOrNo (int jump, stack_t* stack);

//-----------------------------------------------------------

#endif // PROCESSOR
