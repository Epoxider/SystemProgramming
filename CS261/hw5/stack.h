/*
 * stack.h
 *
 * Interface for a simple, dynamically allocated stack of integers
 * 
 * Users of this data structure are expected to keep a pointer to the
 * entry currently at the top of the stack, and pass it into stack functions.
 * 
 * A null top pointer is taken to mean the stack is empty,
 *
 */

#ifndef STACK_H
#define STACK_H

struct stack_entry {
    int value;
    struct stack_entry * next;     
};  

/* 
 * Push given value onto indicated stack
 * 
 * Input:
 * 		**top: pointer to top pointer held by calling code.
 * 			   head pointer must point to a struct stackEnty or NULL.
 * 
 * Return: 1 on success, or 0 if memory was not allcated (no change on 0).
 * NOTE: Will modify top pointer in caller
 */
int stack_push(struct stack_entry **top, int val);

/*
 * Pop value off stack, returning it via passed pointer
 *  
 * Input:
 * 		**top: pointer to top pointer held by calling code.
 * 				   head pointer must point to a struct stackEnty or NULL.
 * 		*val: pointer to variable in which to store popped value
 * 
 * Return: 1 on success, or 0 if there was nothing to pop
 * NOTE: Will modify top pointer in caller
 */
int stack_pop(struct stack_entry **top, int *val);

/*
 * Delete stack, leaving it ready to use again
 * 
 * Input:
 * 		**top: pointer to top pointer held by calling code.
 * 				   head pointer must point to a struct stackEnty or NULL.
 * Return: None
 * NOTE: Will modify top pointer in caller
 */
void stack_delete(struct stack_entry **top);

#endif
