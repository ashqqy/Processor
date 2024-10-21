#include <stdio.h>
#include <assert.h>

#include "Common.h"
#include "Assembler.h"

//-----------------------------------------------------------

int main () // TODO args parsing
    {
    FILE* code_file_in = fopen ("./Code.asm", "rb");
    if (code_file_in == NULL)
        {
        printf ("File opening error");
        return FILE_OPENING_ERROR;
        }

    FILE* code_file_out = fopen ("./MachineCode.bin", "wb");
    if (code_file_in == NULL)
        {
        printf ("File opening/creating error");
        return FILE_OPENING_ERROR;
        }

    label labels_array[N_LABELS] = {};
    LabelsInit (labels_array);

    Assembler (code_file_in, code_file_out, labels_array, FIRST_COMPILATION); // FIXME parse errors
    Assembler (code_file_in, code_file_out, labels_array, SECOND_COMPILATION);

    LabelsDestroy (labels_array);

    if (fclose (code_file_in) != NULL)
        {
        printf ("File closing error");
        return FILE_CLOSING_ERROR;
        }
    if (fclose (code_file_out) != NULL)
        {
        printf ("File closing error");
        return FILE_CLOSING_ERROR;
        }
    }

//-----------------------------------------------------------


