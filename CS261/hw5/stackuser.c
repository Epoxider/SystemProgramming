/*
 * stackuser.c
 *
 * A simple, text-based interface to a stack. Commands are as follows:
 * 
 * u <val>  -- push integer value val onto the stack
 * o        -- pop top value off stack and print it, or print message for empty
 * e        -- empty the stack
 * q        -- quit the program
 *
 * To Compile:
 *      make
 *
 * To Run:
 *      make run
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "stack.h"

int main () {
    // BUG FOUND
    // Must initialize stack to NULL
    struct stack_entry *stack = NULL;
    char cmd;           
    int val;
    
    do {
        do {
            scanf("%c", &cmd);
        } while(isspace(cmd));
        
        switch (cmd) {          
            case 'u': // push a value
                // read value to push
                scanf("%d", &val);

                // push value and check for errors
                if(!stack_push(&stack, val)) {
                    printf("Failed to allocate memory. Exiting...\n");
                    stack_delete(&stack);
                    exit(1);
                }
                break;
            
            case 'o':  // pop a value and print it
                if(stack_pop(&stack, &val)) {
                    printf("Popped %d\n", val);
                }
                else {
                    printf("Nothing to pop\n");
                }
                break;

            case 'e':  // empty the stack
                stack_delete(&stack);
                break;
            
            case 'q':
                break;
                
            default:            
                printf("Oi! That's not a legal command\n");
        }
        
    } while (cmd != 'q');

    // BUG FOUND
    // Need to deallocate the stack
    stack_delete(&stack);
    
    return 0;
}
