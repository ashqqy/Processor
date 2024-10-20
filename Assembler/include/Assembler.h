#ifndef ASSEMBLER
#define ASSEMBLER

#include <stdio.h>

//-----------------------------------------------------------

const int LABLE_NAME_LEN = 30;

struct label
    {
    char label_name[LABLE_NAME_LEN];
    int label_number;
    };

const int N_LABELS = 10;

void Assembler (FILE* code_file_in, FILE* code_file_out);

//-----------------------------------------------------------

#endif //ASSEMBLER