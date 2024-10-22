#include <stdio.h>
#include <assert.h>

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
    fseek (file_input, 0L, SEEK_END); //лучше fstat
    long size_file_input = ftell(file_input);
    fseek(file_input, 0L, SEEK_SET);

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

void ArrayDump (char* array, size_t size_array)
    {
    for (int j = 0; j < size_array; j++)
        printf ("%c", array[j]);
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

        case INVALID_LABEL:         
                                    printf ("%sInvalid Label:%s '%s%d%s'\n", 
                                            COLOR_RED, COLOR_DEFAULT, COLOR_MAGENTA,
                                            error_arg, COLOR_DEFAULT);
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
        }                   
    }

//----------------------------------------------------------------