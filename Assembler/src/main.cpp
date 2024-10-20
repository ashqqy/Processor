#include <stdio.h>
#include <assert.h>

#include "Assembler.h"

//-----------------------------------------------------------

int main () // TODO args parsing
    {
    FILE* code_file_in = fopen ("./Code.asm", "rb");
    if (code_file_in == NULL)
        {
        printf ("File opening error");
        exit;
        }

    FILE* code_file_out = fopen ("./MachineCode.bin", "wb");
    if (code_file_in == NULL)
        {
        printf ("File opening/creating error");
        exit;
        }

    Assembler (code_file_in, code_file_out); // FIXME parse errors

    if (fclose (code_file_in) != NULL)
        printf ("File closing error");
    if (fclose (code_file_out) != NULL)
        printf ("File closing error");
    }

//-----------------------------------------------------------