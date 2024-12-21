#include <stdio.h>
#include <assert.h>

#include "Common.h"
#include "Assembler.h"

//-----------------------------------------------------------

int main (int argc, const char** argv)
    {
    FILE* code_file_in = fopen (argv[argc - 1], "rb");
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

    compilation_error_t compilation_error = COMPILATION_OK;
    compilation_error = Assembler (code_file_in, code_file_out, labels_array, FIRST_COMPILATION);
    if (compilation_error != COMPILATION_OK)
        return compilation_error;
    compilation_error = Assembler (code_file_in, code_file_out, labels_array, SECOND_COMPILATION);
    if (compilation_error != COMPILATION_OK)
        return compilation_error;

    LabelsDestroy (labels_array);

    if (fclose (code_file_in) != 0)
        {
        printf ("File closing error");
        return FILE_CLOSING_ERROR;
        }
    if (fclose (code_file_out) != 0)
        {
        printf ("File closing error");
        return FILE_CLOSING_ERROR;
        }
    }

//-----------------------------------------------------------


