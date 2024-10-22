#ifndef ASSEMBLER
#define ASSEMBLER

#include <stdio.h>
#include "common.h"

//-----------------------------------------------------------

const int LABLE_NAME_LEN = 30;
const int ARG_LEN        = 30;

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
char* SearchConst (char* str, int str_len);
int SearchReg (char* str);

COMPILATION_ERRORS FillArgType (char* push_arg, int* arg_type);
void FormateArg (char push_arg_unformated[], char push_arg_formated[], int push_arg_len);
COMPILATION_ERRORS PushPopCase (int* machine_code, int* ip, char* arg);

//-----------------------------------------------------------

#endif //ASSEMBLER