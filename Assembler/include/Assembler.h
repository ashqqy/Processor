#ifndef ASSEMBLER
#define ASSEMBLER

#include <stdio.h>
#include "common.h"

//-----------------------------------------------------------

const int LABLE_NAME_LEN = 30;

struct label
    {
    char label_name[LABLE_NAME_LEN];
    int label_number;
    };

const int N_LABELS = 10;

const int FIRST_COMPILATION  = 1;
const int SECOND_COMPILATION = 2;

//-----------------------------------------------------------

COMPILATION_ERRORS Assembler (FILE* code_file_in, FILE* code_file_out, label* labels_array, int compilation_number);
void LabelsInit (label* labels_array);
void LabelsDestroy (label* labels_array);
void LabelsDump (label* labels_array);

//-----------------------------------------------------------

#endif //ASSEMBLER