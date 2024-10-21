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
                {
                machine_code[ip++] = machine_cmd;
                fscanf (code_file_in, "%d", &machine_code[ip++]);
                break;
                }

            case PUSHR: case POP:
                {
                machine_code[ip++] = machine_cmd;
                fscanf (code_file_in, "%s", &cmd);
                if (strcmp (cmd, "AX") == 0) machine_code[ip++] = AX;
                if (strcmp (cmd, "BX") == 0) machine_code[ip++] = BX;
                if (strcmp (cmd, "CX") == 0) machine_code[ip++] = CX;
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
                                ErrorOutput (SYNTAX_ERROR, cmd);
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

            case ADD: case SUB: case MUL: case DIV: case IN: case OUT: case DUMP: case HLT: 
                machine_code[ip++] = machine_cmd; 
                break;

            case ERRCMD: 
                if (n_readed != -1) ErrorOutput (SYNTAX_ERROR, cmd);
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