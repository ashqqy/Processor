#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "Common.h"

//----------------------------------------------------------------

/// @brief Функция создаёт массив с текстом на основе данных из файла
/// @param[in] file_input указатель на файл
/// @param[out] text указатель на начало массива с текстом
/// @return size_file_input - количество прочитанных символов (размер входного файла)

size_t TextCreator (FILE* file_input, char** text)
    {
    assert (file_input != NULL);
    
    /// Находим размер файла
    size_t size_file_input = FileSizeFinder (file_input);

    /// Выделяем память для текста
    *text = (char*) calloc (size_file_input + 1, sizeof(char));
    assert (*text != NULL);

    /// Переносим файл в массив
    size_t counter = fread (*text, sizeof(char), size_file_input, file_input);
    assert (size_file_input == counter);

    /// Возвращаем количество прочитанных символов (размер входного файла)
    return size_file_input;
    }

//----------------------------------------------------------------

size_t FileSizeFinder (FILE* file_input)
    {
    fseek (file_input, 0L, SEEK_END); //лучше fstat
    size_t size_file_input = ftell(file_input);
    fseek(file_input, 0L, SEEK_SET);

    return size_file_input;
    }

//----------------------------------------------------------------

void ArrayDump (int* array, size_t size_array)
    {
    for (size_t j = 0; j < size_array; j++)
        printf ("[%d]", array[j]);
    }

//----------------------------------------------------------------

void ErrorOutput (int error_type, const char* error_message, int error_arg)
    {
    switch (error_type)
        {
        case SYNTAX_ERROR:          
                                    printf ("%sSyntax Error:%s '%s%s%s'\n", 
                                            COLOR_RED, COLOR_DEFAULT, COLOR_MAGENTA, 
                                            error_message, COLOR_DEFAULT);
                                    break;
                             
        case NONEXISTENT_REGISTER:  
                                    printf ("%sUsing a Nonexistent Register:%s '%s%s%s'\n", 
                                            COLOR_RED, COLOR_DEFAULT, COLOR_MAGENTA,
                                            error_message, COLOR_DEFAULT);  
                                    break;      
        case MISSING_CONSTANT_ARGUMENT:         
                                    printf ("%sMissing Constant Argument:%s '%s%s%s'\n", 
                                            COLOR_RED, COLOR_DEFAULT, COLOR_MAGENTA, 
                                            error_message, COLOR_DEFAULT);  
                                    break;   
        case TOO_MANY_LABELS:       
                                    printf ("%sToo Many Labels Used:%s '%s%s%s'\n", 
                                            COLOR_RED, COLOR_DEFAULT, COLOR_MAGENTA, 
                                            error_message, COLOR_DEFAULT);  
                                    break; 
        case UNINDEFINED_COMMAND:
                                    printf ("%sUnindefined Command:%s '%s%d%s'\n", 
                                            COLOR_RED, COLOR_DEFAULT, COLOR_MAGENTA,
                                            error_arg, COLOR_DEFAULT);
                                    break;  

        case INVALID_LABEL:         
                                    printf ("%sInvalid Label:%s '%s%d%s'\n", 
                                            COLOR_RED, COLOR_DEFAULT, COLOR_MAGENTA,
                                            error_arg, COLOR_DEFAULT);
                                    break;  
        default: 
                                    printf ("%sUnidentified error:%s '%s%s%s'\n", 
                                            COLOR_RED, COLOR_DEFAULT, COLOR_MAGENTA, 
                                            error_message, COLOR_DEFAULT);  
                                    break;  
        }                   
    }

//----------------------------------------------------------------