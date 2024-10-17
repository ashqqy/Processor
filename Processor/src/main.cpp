#include <stdio.h>

#include "Processor.h"

//-----------------------------------------------------------

int main ()
    {
    FILE* machine_code = fopen ("./MachineCode.asm", "rb");

    Processor (machine_code);

    fclose (machine_code);
    }

//-----------------------------------------------------------