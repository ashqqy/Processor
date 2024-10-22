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

enum FILE_ERRORS
    {
    FILE_OPENING_ERROR = 2000,
    FILE_CLOSING_ERROR = 2001
    };

enum COMPILATION_ERRORS
    {
    COMPILATION_OK            = 1000,
    SYNTAX_ERROR              = 1001,
    NONEXISTENT_REGISTER      = 1002,
    MISSING_CONSTANT_ARGUMENT = 1003
    };

enum RUNTIME_ERRORS
    {
    RUN_OK        = 1500,
    INVALID_LABEL = 1501
    };

//----------------------------------------------------------------

const char* const COLOR_RED     = "\033[1;31m";
const char* const COLOR_MAGENTA = "\033[1;35m";
const char* const COLOR_DEFAULT = "\033[1;0m";

//----------------------------------------------------------------

size_t TextCreator (FILE* file_input, char** text);
size_t MyStrlen (char* str);
void ArrayDump (char* array, size_t size_array);
void ErrorOutput (int error_type, const char* error_message, int error_arg = 0);

//----------------------------------------------------------------

#endif // COMMON