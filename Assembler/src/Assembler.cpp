#include <stdio.h>
#include <string.h>

#include "Assembler.h"

//-----------------------------------------------------------

void Assembler (FILE* code_in, FILE* code_out)
    {
    int code[100] = {};
    int ip = 0;

    while (1)
        {
        char cmd[30] = {};
        fscanf (code_in, "%s", &cmd);

        if (strcmp (cmd, "push") == 0)
            {
            code [ip++] = PUSH;
            int arg = 0;
            fscanf (code_in, "%d", &arg);
            code [ip++] = arg;
            continue;
            }
        if (strcmp (cmd, "add") == 0)
            {
            code [ip++] = ADD;
            continue;
            }
        if (strcmp (cmd, "sub") == 0)
            {
            code [ip++] = SUB;
            continue;
            }
        if (strcmp (cmd, "mul") == 0)
            {
            code [ip++] = MUL;
            continue;
            }
        if (strcmp (cmd, "div") == 0)
            {
            code [ip++] = DIV;
            continue;
            }
        if (strcmp (cmd, "hlt") == 0)
            {
            code [ip++] = HLT;
            fwrite (code, sizeof (code[0]), sizeof (code) / sizeof (code[0]), code_out);
            break;
            }
        }
    }

//-----------------------------------------------------------