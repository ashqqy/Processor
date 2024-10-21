#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Processor.h"
#include "Stack.h"
#include "Common.h"

//-----------------------------------------------------------

#define SPUDUMP(SPU, stack_dump) SPUDump (SPU, stack_dump, __FILE__, __LINE__, __func__);

//-----------------------------------------------------------

RUNTIME_ERRORS Processor (FILE* machine_code)
    {
    assert (machine_code != NULL);

    SPU_t SPU = {};
    SPUInit (&SPU);

    int code[100] = {}; // TODO read full file
    fread (code, sizeof (code[0]), sizeof (code) / sizeof (code[0]), machine_code);

    int ip = 0;

    while (code[ip] != 0)
        {
        switch (code[ip])
            {
            case PUSH:  
                        StackPush (&SPU.stack, code[++ip]); // TODO check errors
                        ++ip; break; 
            case PUSHR: 
                        StackPush (&SPU.stack, (SPU.registers)[code[++ip]]); 
                        ++ip; break;
            case ADD:   
                        StackPush (&SPU.stack, StackPop (&SPU.stack) + StackPop (&SPU.stack)); 
                        ++ip; break;
            case SUB: 
                        {
                        int a = StackPop (&SPU.stack);
                        StackPush (&SPU.stack, StackPop (&SPU.stack) - a); 
                        ++ip; break;
                        }
            case MUL:   
                        StackPush (&SPU.stack, StackPop (&SPU.stack) * StackPop (&SPU.stack)); 
                        ++ip; break;
            case DIV: 
                        {
                        int a = StackPop (&SPU.stack);
                        assert (a != 0);
                        StackPush (&SPU.stack, StackPop (&SPU.stack) / a);
                        ++ip; break;
                        }
            case IN:
                        {
                        int a = 0; scanf ("%d", &a);
                        StackPush (&SPU.stack, a); 
                        ++ip; break;
                        }
            case OUT:   
                        printf ("%d", StackPop (&SPU.stack)); 
                        ++ip; break;
            case POP:   
                        (SPU.registers)[code[++ip]] = StackPop (&SPU.stack); 
                        ++ip; break;
            case DUMP:
                        SPUDUMP (&SPU, 1);
                        ++ip; break;
            case JMP:   
                        if (code[ip + 1] < 0)
                            {
                            ErrorOutput (INVALID_LABEL, "", code[ip + 1]);
                            return INVALID_LABEL;
                            }
                        ip = code[++ip]; 
                              break;
            case HLT:   
                        ++ip; break;
            }
        }

    SPUDestroy (&SPU);
    return RUN_OK;
    }

//-----------------------------------------------------------

void SPUInit (SPU_t* SPU)
    {
    assert (SPU != NULL);

    SPU->stack = {};
    StackInit (&SPU->stack);

    memset (&SPU->registers, 0, N_REGS * sizeof (int));
    }

//-----------------------------------------------------------

void SPUDestroy (SPU_t* SPU)
    {
    assert (SPU != NULL);

    StackDestroy (&SPU->stack);

    memset (&SPU->registers, 0, N_REGS * sizeof (int));
    }

//-----------------------------------------------------------

// FIXME можно сделать только один дамп
void SPUDump (SPU_t* SPU, bool stack_dump, const char* file, int line, const char* func)
    {
    /// Если файл для дампа есть или его можно создать, то вывод туда, иначе в консоль
    FILE* dump_file = fopen ("./SPU/DumpFile.txt", "w");
    if (dump_file == NULL)
        dump_file = stderr;

    assert (SPU != NULL);

    fprintf (dump_file, "SPU[0x%p] at %s:%d (%s)\n", SPU, file, line, func);
    fprintf (dump_file, "    {\n");

    assert (SPU->registers != NULL);

    fprintf (dump_file, "    registers[0x%p]\n", SPU->registers);
    fprintf (dump_file, "        {\n");
    for (int i = 0; i < N_REGS; i++)
        {
        switch (i)
            {
            case ZR: fprintf (dump_file, "        ZR = [%d]\n", SPU->registers[i]); break;
            case AX: fprintf (dump_file, "        AX = [%d]\n", SPU->registers[i]); break;
            case BX: fprintf (dump_file, "        BX = [%d]\n", SPU->registers[i]); break;
            case CX: fprintf (dump_file, "        CX = [%d]\n", SPU->registers[i]); break;
            default: fprintf (dump_file, "        reg(?) = [%d]\n", SPU->registers[i]); break;
            }
        }
    fprintf (dump_file, "        }\n");
    fprintf (dump_file, "    }\n");

    if (stack_dump == 1)
        {  
        assert (&(SPU->stack) != NULL);
        StackDump (&(SPU->stack), dump_file, __FILE__, __LINE__, __func__);
        }
    }

//-----------------------------------------------------------