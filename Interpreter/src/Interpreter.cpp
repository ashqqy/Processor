#include <string.h>
#include <assert.h>

#include "Interpreter.h"
#include "Stack.h"

//-----------------------------------------------------------

void RunInterpreter ()
    {
    Stack_t stack = {};
    StackInit (&stack);

    while (1)
        {
        char cmd[30] = "";
        scanf ("%s", &cmd);

        if (strcmp (cmd, "push") == 0)
            {
            int arg = 0;
            scanf ("%d", &arg);
            StackPush (&stack, arg);
            continue;
            }
        
        if (strcmp (cmd, "out") == 0)
            {
            // FIXME не использовать переменные, переписать Pop
            int arg = 0;
            StackPop (&stack, &arg);
            printf ("%d\n", arg);
            continue;
            }

        if (strcmp (cmd, "add") == 0)
            {
            // FIXME не использовать переменные, переписать Pop
            int a = 0;
            int b = 0;
            StackPop (&stack, &a);
            StackPop (&stack, &b);
            StackPush (&stack, a + b);
            continue;
            }
        
        if (strcmp (cmd, "sub") == 0)
            {
            int a = 0;
            int b = 0;
            StackPop (&stack, &a);
            StackPop (&stack, &b);
            StackPush (&stack, b - a);
            continue;
            }

        if (strcmp (cmd, "mul") == 0)
            {
            // FIXME не использовать переменные, переписать Pop
            int a = 0;
            int b = 0;
            StackPop (&stack, &a);
            StackPop (&stack, &b);
            StackPush (&stack, a * b);
            continue;
            }

        
        if (strcmp (cmd, "div") == 0)
            {
            int a = 0;
            int b = 0;
            StackPop (&stack, &a);
            StackPop (&stack, &b);
            assert (b != 0);
            StackPush (&stack, b / a);
            continue;
            }

        if (strcmp (cmd, "hlt") == 0)
            break;
        
        printf ("Syntax Error: '%s'\n", cmd);
        }
    }

//-----------------------------------------------------------