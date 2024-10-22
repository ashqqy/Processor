#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Assembler.h"
#include "Common.h"

//-----------------------------------------------------------

// TODO tokenization (read about standart function) for comments ( ;blabla )

COMPILATION_ERRORS Assembler (FILE* code_file_in, FILE* code_file_out, label* labels_array, int compilation_number)
    {
    assert (code_file_in  != NULL);
    assert (code_file_out != NULL);
    assert (compilation_number == FIRST_COMPILATION || compilation_number == SECOND_COMPILATION);

    if (compilation_number == SECOND_COMPILATION)
        fseek (code_file_in, 0L, 0);

    int machine_code[100] = {}; // TODO Читать в массив и calloc
    int ip = 0;

    int n_readed = 0;
    while (n_readed != -1) // EOF = -1
        {
        char cmd[10] = {};
        n_readed = fscanf (code_file_in, "%s", &cmd);

        SPU_COMMANDS machine_cmd = ERRCMD;
             if (strchr (cmd, ':')     != 0)  machine_cmd = LABEL;
        else if (strcmp (cmd, "push")  == 0)  machine_cmd = PUSH;
        else if (strcmp (cmd, "add")   == 0)  machine_cmd = ADD;
        else if (strcmp (cmd, "sub")   == 0)  machine_cmd = SUB;
        else if (strcmp (cmd, "mul")   == 0)  machine_cmd = MUL;
        else if (strcmp (cmd, "div")   == 0)  machine_cmd = DIV;
        else if (strcmp (cmd, "in")    == 0)  machine_cmd = IN;
        else if (strcmp (cmd, "out")   == 0)  machine_cmd = OUT;
        else if (strcmp (cmd, "pushr") == 0)  machine_cmd = PUSHR;
        else if (strcmp (cmd, "pop")   == 0)  machine_cmd = POP;
        else if (strcmp (cmd, "dump")  == 0)  machine_cmd = DUMP;
        else if (strcmp (cmd, "jmp")   == 0)  machine_cmd = JMP;
        else if (strcmp (cmd, "hlt")   == 0)  machine_cmd = HLT;

        switch (machine_cmd)
            {
            case PUSH: 
            case POP:
                {
                machine_code[ip++] = machine_cmd;

                char arg_unformate[ARG_LEN] = {};
                fscanf (code_file_in, "%[^\n]", &arg_unformate);
                char arg[ARG_LEN] = {};
                FormateArg (arg_unformate, arg, ARG_LEN);

                COMPILATION_ERRORS ppc_err = PushPopCase (machine_code, &ip, arg);
                if (ppc_err != COMPILATION_OK)
                    return ppc_err;
    
                break;
                }

            case JMP: // TODO в функцию и дописать остальные джампы
                {
                machine_code[ip++] = machine_cmd;
                fscanf (code_file_in, "%s", &cmd);
                if (strchr (cmd, ':') == NULL)
                    sscanf (cmd, "%d", &machine_code[ip++]);   
                else
                    for (int i = 0; i < N_LABELS; i++)
                        {  
                        if (strcmp (labels_array[i].label_name, cmd) == 0)
                            {
                            machine_code[ip++] = labels_array[i].label_number;
                            break;
                            }
                        if (i == N_LABELS - 1)
                            {
                            if (compilation_number == FIRST_COMPILATION)
                                machine_code[ip++] = -1;
                            if (compilation_number == SECOND_COMPILATION)
                                {
                                ErrorOutput (SYNTAX_ERROR, cmd);
                                return SYNTAX_ERROR;
                                }
                            }
                        }
                break;
                }

            case LABEL:
                {
                for (int i = 0; i < N_LABELS; i++)
                    if (labels_array[i].label_number == -1)
                        {
                        strcpy (labels_array[i].label_name, cmd);
                        labels_array[i].label_number = ip;
                        break;
                        }
                break;
                }

            case ADD: 
            case SUB: 
            case MUL: 
            case DIV: 
            case IN: 
            case OUT: 
            case DUMP: 
            case HLT: 
                machine_code[ip++] = machine_cmd; 
                break;

            case ERRCMD: 
                if (n_readed != -1) 
                    {
                    ErrorOutput (SYNTAX_ERROR, cmd);
                    return SYNTAX_ERROR;
                    }
                break;
            }
        }
        if (compilation_number == SECOND_COMPILATION)
            fwrite (machine_code, sizeof(machine_code[0]), sizeof (machine_code) / sizeof (machine_code[0]), code_file_out);
    
    return COMPILATION_OK;
    }

//-----------------------------------------------------------

void LabelsInit (label* labels_array)
    {
    assert (labels_array != NULL);

    for (int i = 0; i < N_LABELS; i++)
        labels_array[i].label_number = -1;
    }

void LabelsDestroy (label* labels_array)
    {
    assert (labels_array != NULL);

    for (int i = 0; i < N_LABELS; i++)
        labels_array[i].label_number = -1;
    }

void LabelsDump (label* labels_array)
    {
    assert (labels_array != NULL);

    printf ("labels[%p]\n", labels_array);
    printf ("    {\n");
    for (int i = 0; i < N_LABELS; i++)
        {
        if (labels_array[i].label_number != -1)
            printf ("    label %s %d \n", labels_array[i].label_name, labels_array[i].label_number);
        else
            printf ("    Empty label\n");
        }
    printf ("    }\n");
    }

//-----------------------------------------------------------

COMPILATION_ERRORS FillArgType (char* arg, int* arg_type)
    {
    assert (arg      != NULL);
    assert (arg_type != NULL);

    char* open_bracket = strchr (arg, '[');
    char* plus_ptr     = strchr (arg, '+');
    char* reg_ptr      = strchr (arg, 'X');

    if (open_bracket != NULL)
            {
            char* close_bracket = strchr (arg, ']');
            if (close_bracket == NULL)
                {
                ErrorOutput (SYNTAX_ERROR, "expected: ']'");
                return SYNTAX_ERROR;
                }
            *arg_type |= 4;
            }
    if (plus_ptr != NULL)
            *arg_type |= 3;
    else if (reg_ptr != NULL)
            *arg_type |= 1;
    else    
            *arg_type |= 2;

    return COMPILATION_OK;
    }

//-----------------------------------------------------------

char* SearchConst (char* str, int str_len)
    {
    assert (str != NULL);

    char symb = *str;
    for (int i = 0; i < str_len; i++)
        {
        if (48 <= symb && symb <= 57)
            return str;
        str += 1;
        symb = *str;
        }
    return NULL;
    }

int SearchReg (char* str)
    {
    assert (str != NULL);

         if (strstr (str, "AX") != NULL) return AX;
    else if (strstr (str, "BX") != NULL) return BX;
    else if (strstr (str, "CX") != NULL) return CX;

    return NONEXISTENT_REGISTER;
    }

//-----------------------------------------------------------

void FormateArg (char push_arg_unformated[], char push_arg[], int push_arg_len)
    {
    assert (push_arg_unformated != NULL);

    char symb = *push_arg_unformated;
    char push_arg_formated[push_arg_len] = {};
    int format_ptr = 0;

    while (symb == 32)
        {
        push_arg_unformated += 1;
        symb = *push_arg_unformated;
        }

    for (int i = 0; i < push_arg_len; i++)
        {
        if (symb != 13)
            {
            push_arg_formated[format_ptr] = symb;
            format_ptr += 1;
            }
        push_arg_unformated += 1;
        symb = *push_arg_unformated;
        }
    memcpy (push_arg, push_arg_formated, push_arg_len);
    }

//-----------------------------------------------------------

COMPILATION_ERRORS PushPopCase (int* machine_code, int* ip, char* arg)
    {
    int arg_type = 0;

    COMPILATION_ERRORS fat_err = FillArgType (arg, &arg_type);
    if (fat_err != COMPILATION_OK)
        return fat_err;
    machine_code[(*ip)++] = arg_type;
    if (arg_type & 1)
        {
        int reg = SearchReg (arg);
        if (reg == NONEXISTENT_REGISTER)
            {
            ErrorOutput (NONEXISTENT_REGISTER, arg);
            return NONEXISTENT_REGISTER;
            }
        machine_code[(*ip)++] = reg;
        }
    
    if (arg_type & 2)
        {
        char* intptr = SearchConst (arg, ARG_LEN);
        if (intptr != NULL)
            sscanf (intptr, "%d", &machine_code[(*ip)++]);
        else    
            {
            ErrorOutput (MISSING_CONSTANT_ARGUMENT, arg);
            return MISSING_CONSTANT_ARGUMENT;
            }
        }
    
    return COMPILATION_OK;
    }

//-----------------------------------------------------------