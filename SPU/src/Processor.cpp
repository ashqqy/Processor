#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Processor.h"
#include "Stack.h"
#include "Common.h"

//-----------------------------------------------------------

#define SPUDUMP(SPU, stack_dump) SPUDump (SPU, stack_dump, __FILE__, __LINE__, __func__);

//-----------------------------------------------------------

runtime_error_t Processor (FILE* machine_code)
    {
    assert (machine_code != NULL);

    size_t size_machine_code = FileSizeFinder (machine_code);
    size_machine_code /= sizeof (int);

    int* code = (int*) calloc (size_machine_code + 1, sizeof (int));
    code[size_machine_code + 1] = 0;
    fread (code, sizeof (int), size_machine_code, machine_code);

    SPU_t SPU = {};
    SPUInit (&SPU, &code);

    while (code[SPU.ip] != 0)
        {
        switch (code[SPU.ip])
            {
            case PUSH:  
                        {
                        int* push_elem_ptr = GetArg (&SPU);
                        StackPush (&SPU.stack, *push_elem_ptr); // TODO check errors
                        ++(SPU.ip); break; 
                        }
            case POP:   
                        {
                        int* pop_ptr = GetArg (&SPU);
                        *pop_ptr = StackPop (&SPU.stack); 
                        ++(SPU.ip); break;
                        }
            case ADD:   
                        StackPush (&SPU.stack, StackPop (&SPU.stack) + StackPop (&SPU.stack)); 
                        ++(SPU.ip); break;
            case SUB: 
                        {
                        int a = StackPop (&SPU.stack);
                        StackPush (&SPU.stack, StackPop (&SPU.stack) - a); 
                        ++(SPU.ip); break;
                        }
            case MUL:   
                        StackPush (&SPU.stack, StackPop (&SPU.stack) * StackPop (&SPU.stack)); 
                        ++(SPU.ip); break;
            case DIV: 
                        {
                        int a = StackPop (&SPU.stack);
                        assert (a != 0);
                        StackPush (&SPU.stack, StackPop (&SPU.stack) / a);
                        ++(SPU.ip); break;
                        }
            case IN:
                        {
                        int a = 0;
                        scanf ("%d", &a);
                        StackPush (&SPU.stack, a); 
                        ++(SPU.ip); break;
                        }
            case OUT:   
                        printf ("out: %d\n", StackPop (&SPU.stack)); 
                        ++(SPU.ip); break;
            case DUMP:
                        SPUDUMP (&SPU, 1);
                        ++(SPU.ip); break;
            case JMP:   
            case JA:
            case JAE:
            case JB:
            case JBE:
            case JE:
            case JNE:
                        if (code[(SPU.ip) + 1] < 0)
                            {
                            ErrorOutput (INVALID_LABEL, "", code[SPU.ip + 1]);
                            return INVALID_LABEL;
                            }
                        if (JumpOrNo (code[SPU.ip], &SPU.stack) == YES)
                            {
                            SPU.ip++;
                            SPU.ip = code[SPU.ip]; 
                            }
                        else 
                            SPU.ip += 2; 
                        break;
            case DRAW:  
                        {
                        for (int y = 0; y < 10; y++)
                            {
                            for (int x = 0; x < 10; x++)
                                {
                                printf ("[%c]", SPU.RAM[x + 10 * y]);
                                }
                            printf ("\n");
                            }
                        printf ("\n");
                        }
                        ++(SPU.ip); break;
            case HLT:   
                        code[SPU.ip] = 0; 
                        break;
            default:
                        code[SPU.ip] = 0;
                        ErrorOutput (UNINDEFINED_COMMAND, "", code[SPU.ip]);
                        break;
            }
        }
    SPUDestroy (&SPU);
    return RUN_OK;
    }

//-----------------------------------------------------------

void SPUInit (SPU_t* SPU, int** code)
    {
    assert (SPU   != NULL);
    assert (code  != NULL);
    assert (*code != NULL);

    SPU->stack = {};
    StackInit (&SPU->stack);

    SPU->ip = 0;
    SPU->code = code;

    memset (&SPU->registers, 0, N_REGS * sizeof (int));
    memset (&SPU->RAM, 0, RAM_SIZE * sizeof (int));
    }

void SPUDestroy (SPU_t* SPU)
    {
    assert (SPU != NULL);

    StackDestroy (&SPU->stack);

    SPU->ip = 0;

    free (*SPU->code); *SPU->code = NULL;
    SPU->code = NULL;

    memset (&SPU->registers, 0, N_REGS * sizeof (int));
    memset (&SPU->RAM, 0, RAM_SIZE * sizeof (int));
    }

//-----------------------------------------------------------

void SPUDump (SPU_t* SPU, bool stack_dump, const char* file, int line, const char* func)
    {
    /// Если файл для дампа есть или его можно создать, то вывод туда, иначе в консоль
    FILE* dump_file = fopen ("./SPU/DumpFile.txt", "a");
    if (dump_file == NULL)
        dump_file = stderr;

    assert (SPU != NULL);

    fprintf (dump_file, "SPU[0x%p] at %s:%d (%s)\n", SPU, file, line, func);
    fprintf (dump_file, "    {\n");

    fprintf (dump_file, "    ip = [%d]\n", SPU->ip);
    fprintf (dump_file, "    \n");

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

int* GetArg (SPU_t* SPU) // REVIEW Нужна ли проверка для попа
    {
    // int operation = (*SPU->code)[(SPU->ip)];
    int arg_type  = (*SPU->code)[++(SPU->ip)];
    int* arg_value = NULL;

    if (arg_type & 1)
        arg_value = &(SPU->registers[(*SPU->code)[++(SPU->ip)]]);

    if (arg_type & 2)
        {
        if (arg_value != NULL)
            SPU->registers[ZR] = *arg_value + (*SPU->code)[++(SPU->ip)];
        else
            SPU->registers[ZR] = (*SPU->code)[++(SPU->ip)];

        arg_value = &SPU->registers[ZR];
        }

    if (arg_type & 4)
        arg_value = &SPU->RAM[*arg_value];
    
    return arg_value;
    }

//-----------------------------------------------------------

bool JumpOrNo (int jump, Stack_t* stack)
    {
    if (jump == JMP)
        return YES;
    else 
        {
        int a = StackPop (stack);
        int b = StackPop (stack);

        switch (jump)
            {
            case JA:
                if (b > a)
                    return YES;
                break;
            case JAE:
                if (b >= a)
                    return YES;
                break;
            case JB:
                if (b < a)
                    return YES;
                break;
            case JBE:
                if (b <= a)
                    return YES;
                break;
            case JE:
                if (b == a)
                    return YES;
                break;
            case JNE: 
                if (b != a)
                    return YES;
                break;
                
            default: 
                return NO;
            }
        }
    return NO;
    }

//-----------------------------------------------------------