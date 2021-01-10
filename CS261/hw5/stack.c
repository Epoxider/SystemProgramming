/*
 * stack.c
 *
 * Implementation of a simple, dynamically allocated stack of ints
 * 
 * This implementation uses a basic singly linked list to manage values.
 *
 */

#include <stdio.h>
#include <stdlib.h>     
#include "stack.h"


/* Inserting at the head of a list */
int stack_push(struct stack_entry **top, int val)  {    
    struct stack_entry *newbie;

    /* we always need to allocate memory, so start there */
    newbie = (struct stack_entry *)malloc(sizeof(struct stack_entry));
    if(!newbie) {
        return 0;
    }
    
    // set value we're storing in the new entry
    newbie->value = val;

    // put this node at the start of the list
    newbie->next = *top;

    *top = newbie;
    return 1;
}


/* 
 * pop could just return what it pops, but then we have trouble telling
 * if the stack was empty. This way, we always know if the stack had 
 * something to pop after we finish the call
 */
int stack_pop(struct stack_entry **top, int *val) {
    struct stack_entry *next;

    if(!(*top)) {
        // no values in stack
        return 0;
    } 

    // prepare the value for return
    *val = (*top)->value;

    // remove top element and free it
    next = (*top)->next;

    // BUG FOUND
    // Need to free(*top);
    free(*top);

    *top = next;
    return 1;
}

/* just runs through the entire list freeing everything */
void stack_delete(struct stack_entry **top) {
    struct stack_entry *next;  

    while( *top != 0 ) {     
        next = (*top)->next;
        free(*top);            
        *top = next;         
    }
}
