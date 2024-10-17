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
            printf ("%d\n", StackPop (&stack));
            continue;
            }

        if (strcmp (cmd, "add") == 0)
            {
            StackPush (&stack, StackPop (&stack) + StackPop (&stack));
            continue;
            }
        
        if (strcmp (cmd, "sub") == 0)
            {
            int a = StackPop (&stack);
            int b = StackPop (&stack);
            StackPush (&stack, b - a);
            continue;
            }

        if (strcmp (cmd, "mul") == 0)
            {
            StackPush (&stack, StackPop (&stack) * StackPop (&stack));
            continue;
            }

        
        if (strcmp (cmd, "div") == 0)
            {
            int a = StackPop (&stack);
            int b = StackPop (&stack);
            assert (a != 0);
            StackPush (&stack, b / a);
            continue;
            }

        if (strcmp (cmd, "hlt") == 0)
            break;
        
        printf ("Syntax Error: '%s'\n", cmd);
        }
    }

//-----------------------------------------------------------