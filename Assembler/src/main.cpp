#include <stdio.h>
#include <assert.h>

#include "Assembler.h"

//-----------------------------------------------------------

int main ()
    {
    FILE* code_in = fopen ("./Code.asm", "rb");
    assert (code_in != NULL);
    FILE* code_out = fopen ("./MachineCode.asm", "wb");
    assert (code_out != NULL);

    Assembler (code_in, code_out);

    fclose (code_in);
    fclose (code_out);
    }

//-----------------------------------------------------------