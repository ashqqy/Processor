#ifndef COMMON
#define COMMON

//----------------------------------------------------------------

enum SPU_COMMANDS
    {
    ERRCMD = 0,
    PUSH   = 1,
    ADD    = 2,
    SUB    = 3,
    MUL    = 4,
    DIV    = 5,
    IN     = 6,
    OUT    = 7,
    PUSHR  = 8,
    POP    = 9,
    DUMP   = 10,
    LABEL  = 11,
    JMP    = 12,
    HLT    = -1
    };

enum REG
    {
    ZR = 0,
    AX = 1,
    BX = 2,
    CX = 3
    };

//----------------------------------------------------------------

size_t TextCreator (FILE* file_input, char** text);
size_t MyStrlen (char* str);
void ArrayDump (char* array, size_t size_array);

//----------------------------------------------------------------

#endif // COMMON