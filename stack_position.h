#ifndef _STACK_POSITION_H
#define _STACK_POSITION_H
#include "board.h"

/*
 * @see: board.h
 * stack_position allows to stack positions in order to place them on the board sequentially
 */

/* Abstract type of the stack of positions */
typedef struct stack_position_base *stack_position;

/*
 * construction of the empty stack_position
 * @args    : \nothing
 * @requires: \nothing
 * @assigns : \nothing
 * @ensures : returns the empty stack_position
 */
stack_position empty_stack_position();

/*
 * vacuity test
 * @args    : s
 * @requires: s is valid
 * @assigns : \nothing
 * @ensures : returns 1 if the stack_position is empty, 0 otherwise
 */
int is_empty_stack_position(stack_position s);

/*
 * pushes a position into the stack_position
 * @args    : p, *s
 * @requires: s is pointing on a valid stack_position
 *            p is a valid position 
 * @assigns : *s
 * @ensures : adds p to the head of *s
 */
void push_position(position p, stack_position *s);

/*
 * removes the first element of the stack_position
 * @args    : *s
 * @requires: s is pointing on a valid stack_position 
 * @assigns : *s
 * @ensures : removes the head of *s and returns its value
 */
position pop_position(stack_position *s);

/*
 * freeing the memory space of the stack_position
 * @args    : *s
 * @requires: s is pointing on a valid stack_token 
 * @assigns : *s
 * @ensures : frees the memory space allocated for all the elements of *s
 */
void free_stack_position(stack_position *s);

#endif