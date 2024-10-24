#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "Assembler.h"
#include "Common.h"

//-----------------------------------------------------------

// TODO tokenization (read about standart function) for comments ( ;blabla )

compilation_error_t Assembler (FILE* code_file_in, FILE* code_file_out, label* labels_array, compilation_number_t compilation_number)
    {
    assert (code_file_in  != NULL);
    assert (code_file_out != NULL);
    assert (compilation_number == FIRST_COMPILATION || compilation_number == SECOND_COMPILATION);

    if (compilation_number == SECOND_COMPILATION)
        fseek (code_file_in, 0L, 0);

    int machine_code[100] = {}; // TODO Читать в массив и calloc
    int ip = 0;

    int n_readed = 0;
    while (n_readed != EOF)
        {
        char text_cmd[10] = {};
        n_readed = fscanf (code_file_in, "%s", text_cmd);

        spu_command_t machine_cmd = ERRCMD;
        FromTextToMachineCode (text_cmd, &machine_cmd);

        switch (machine_cmd)
            {
            case PUSH: 
            case POP:
                {
                machine_code[ip++] = machine_cmd;

                char arg_unformate[ARG_LEN] = {};
                fscanf (code_file_in, "%[^\n]", arg_unformate);
                char arg[ARG_LEN] = {};
                FormateArg (arg_unformate, arg);

                compilation_error_t ppc_err = PushPopCase (machine_code, &ip, arg);
                if (ppc_err != COMPILATION_OK)
                    return ppc_err;
    
                break;
                }

            case JMP:
            case JA:
            case JAE:
            case JB:
            case JBE:
            case JE:
            case JNE:
                {
                machine_code[ip++] = machine_cmd;
                fscanf (code_file_in, "%s", text_cmd);
                compilation_error_t jc_err = JumpCase (text_cmd, machine_code, &ip, labels_array, compilation_number);
                if (jc_err != COMPILATION_OK)
                    return jc_err;
                break;
                }

            case LABEL:
                {
                for (int i = 0; i < N_LABELS; i++)
                    {
                    if (labels_array[i].label_number == -1)
                        {
                        strcpy (labels_array[i].label_name, text_cmd);
                        labels_array[i].label_number = ip;
                        break;
                        }
                    if (i == N_LABELS - 1)
                        {
                        ErrorOutput (TOO_MANY_LABELS, "There is no place for new labels");
                        return TOO_MANY_LABELS;
                        }
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
            case DRAW:
            case HLT: 
                machine_code[ip++] = machine_cmd; 
                break;

            case ERRCMD: 
                if (n_readed != -1) 
                    {
                    ErrorOutput (SYNTAX_ERROR, text_cmd);
                    return SYNTAX_ERROR;
                    }
                break;

            default:
                ErrorOutput (SYNTAX_ERROR, text_cmd);
                return SYNTAX_ERROR;
            }
        }
        if (compilation_number == SECOND_COMPILATION)
            fwrite (machine_code, sizeof(machine_code[0]), sizeof (machine_code) / sizeof (machine_code[0]), code_file_out);
    
    return COMPILATION_OK;
    }

//-----------------------------------------------------------

void FromTextToMachineCode (char* text_cmd, spu_command_t* machine_cmd)
    {
    if      (strchr (text_cmd, ':')     != 0)  *machine_cmd = LABEL;
    else if (strcmp (text_cmd, "push")  == 0)  *machine_cmd = PUSH;
    else if (strcmp (text_cmd, "pop")   == 0)  *machine_cmd = POP;
    else if (strcmp (text_cmd, "add")   == 0)  *machine_cmd = ADD;
    else if (strcmp (text_cmd, "sub")   == 0)  *machine_cmd = SUB;
    else if (strcmp (text_cmd, "mul")   == 0)  *machine_cmd = MUL;
    else if (strcmp (text_cmd, "div")   == 0)  *machine_cmd = DIV;
    else if (strcmp (text_cmd, "in")    == 0)  *machine_cmd = IN;
    else if (strcmp (text_cmd, "out")   == 0)  *machine_cmd = OUT;
    else if (strcmp (text_cmd, "dump")  == 0)  *machine_cmd = DUMP;
    else if (strcmp (text_cmd, "jmp")   == 0)  *machine_cmd = JMP;
    else if (strcmp (text_cmd, "ja")    == 0)  *machine_cmd = JA;
    else if (strcmp (text_cmd, "jae")   == 0)  *machine_cmd = JAE;
    else if (strcmp (text_cmd, "jb")    == 0)  *machine_cmd = JB;
    else if (strcmp (text_cmd, "jbe")   == 0)  *machine_cmd = JBE;
    else if (strcmp (text_cmd, "je")    == 0)  *machine_cmd = JE;
    else if (strcmp (text_cmd, "jne")   == 0)  *machine_cmd = JNE;
    else if (strcmp (text_cmd, "draw")  == 0)  *machine_cmd = DRAW;
    else if (strcmp (text_cmd, "hlt")   == 0)  *machine_cmd = HLT;
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

compilation_error_t FillArgType (char* arg, int* arg_type)
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

    for (int i = 0; i < str_len; i++)
        {
        char symb = *(str++);
        if (isdigit(symb))
            return --str;
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

void FormateArg (char push_arg_unformated[], char push_arg[])
    {
    assert (push_arg_unformated != NULL);

    char symb = *push_arg_unformated;
    char push_arg_formated[ARG_LEN] = {};
    int format_ptr = 0;

    while (symb == ' ')
        {
        push_arg_unformated += 1;
        symb = *push_arg_unformated;
        }

    for (int i = 0; i < ARG_LEN; i++)
        {
        if (symb != '\r')
            {
            push_arg_formated[format_ptr] = symb;
            format_ptr += 1;
            }
        push_arg_unformated += 1;
        symb = *push_arg_unformated;
        }
    memcpy (push_arg, push_arg_formated, ARG_LEN);
    }

//-----------------------------------------------------------

compilation_error_t PushPopCase (int* machine_code, int* ip, char* arg)
    {
    assert (machine_code != NULL);
    assert (ip           != NULL);
    assert (arg          != NULL);

    int arg_type = 0;

    compilation_error_t fat_err = FillArgType (arg, &arg_type);
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
            {
            sscanf (intptr, "%d", &machine_code[(*ip)++]);
            }
        else
            {
            ErrorOutput (MISSING_CONSTANT_ARGUMENT, arg);
            return MISSING_CONSTANT_ARGUMENT;
            }
        }
    
    return COMPILATION_OK;
    }

//-----------------------------------------------------------

compilation_error_t JumpCase (char* text_cmd, int* machine_code, int* ip, label* labels_array, int compilation_number)
    {
    if (strchr (text_cmd, ':') == NULL)
        {
        if (sscanf (text_cmd, "%d", &machine_code[(*ip)++]) == 0)
            {
            ErrorOutput (SYNTAX_ERROR, text_cmd);
            return SYNTAX_ERROR;
            }
        } 
    else
        for (int i = 0; i < N_LABELS; i++)
            {  
            if (strcmp (labels_array[i].label_name, text_cmd) == 0)
                {
                machine_code[(*ip)++] = labels_array[i].label_number;
                break;
                }
            if (i == N_LABELS - 1)
                {
                if (compilation_number == FIRST_COMPILATION)
                    machine_code[(*ip)++] = -1;
                if (compilation_number == SECOND_COMPILATION)
                    {
                    ErrorOutput (SYNTAX_ERROR, text_cmd);
                    return SYNTAX_ERROR;
                    }
                }
            }
    return COMPILATION_OK;
    }

//-----------------------------------------------------------