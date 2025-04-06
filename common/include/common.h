#ifndef COMMON
#define COMMON

//----------------------------------------------------------------

typedef enum
{
    ERRCMD = 0,
    PUSH   = 1,
    POP    = 2,
    ADD    = 3,
    SUB    = 4,
    MUL    = 5,
    DIV    = 6,
    SQRT   = 7,
    IN     = 8,
    OUT    = 9,
    DUMP   = 10,
    JMP    = 11,
    JA     = 12,
    JAE    = 13, 
    JB     = 14,
    JBE    = 15,
    JE     = 16,
    JNE    = 17,
    CALL   = 18,
    RET    = 19,
    LABEL  = 20,
    DRAW   = 21,
    HLT    = -1
} spu_command_t;

typedef enum
{
    ZR = 0,
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
} reg_t;

typedef enum
{
    FILE_OPENING_ERROR = 2000,
    FILE_CLOSING_ERROR = 2001
} file_error_t;

typedef enum
{
    COMPILATION_OK            = 1000,
    SYNTAX_ERROR              = 1001,
    NONEXISTENT_REGISTER      = 1002,
    MISSING_CONSTANT_ARGUMENT = 1003,
    TOO_MANY_LABELS           = 1004
} compilation_error_t;

typedef enum
{
    RUN_OK              = 1500,
    INVALID_LABEL       = 1501,
    UNINDEFINED_COMMAND = 1502
} runtime_error_t;

//----------------------------------------------------------------

static const char* const COLOR_RED     = "\033[1;31m";
static const char* const COLOR_MAGENTA = "\033[1;35m";
static const char* const COLOR_DEFAULT = "\033[1;0m";

//----------------------------------------------------------------

size_t TextCreator (FILE* file_input, char** text);
size_t FileSizeFinder (FILE* file_input);
size_t MyStrlen (char* str);
void ArrayDump (int* array, size_t size_array);
void ErrorOutput (int error_type, const char* error_message, int error_arg);

//----------------------------------------------------------------

#endif // COMMON
