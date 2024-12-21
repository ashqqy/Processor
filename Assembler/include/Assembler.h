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

const int N_LABELS = 20;

enum compilation_number_t
    {
    FIRST_COMPILATION  = 1,
    SECOND_COMPILATION = 2
    };

const int MACHINE_CODE_SIZE = 1000;

//-----------------------------------------------------------

compilation_error_t Assembler (FILE* code_file_in, FILE* code_file_out, label* labels_array, compilation_number_t compilation_number);
void FromTextToMachineCode (char* text_cmd, spu_command_t* machine_cmd);

void LabelsInit (label* labels_array);
void LabelsDestroy (label* labels_array);
void LabelsDump (label* labels_array);

char* SearchConst (char* str, int str_len);
int SearchReg (char* str);

compilation_error_t FillArgType (char* push_arg, int* arg_type);
void FormateArg (char push_arg_unformated[], char push_arg_formated[]);

compilation_error_t PushPopCase (int* machine_code, int* ip, char* arg);
compilation_error_t JumpCase (char* text_cmd, int* machine_code, int* ip, label* labels_array, int compilation_number);

//-----------------------------------------------------------

#endif //ASSEMBLER