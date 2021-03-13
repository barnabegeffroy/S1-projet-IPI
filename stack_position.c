#include <stdlib.h>
#include <stdio.h>
#include "stack_position.h"
#include "board.h"

/*
 * @see: board.h
 * stack_position allows to stack positions in order to place them on the board sequentially
 */

/* 
 * Concrete type of a stack of positions with linked list
 * @args: val, the element value : the position
 *        *next, the following element
 */
struct stack_position_base
{
    position val;
    struct stack_position_base *next;
};

/*
 * construction of the empty stack_position
 * @args    : \nothing
 * @requires: \nothing
 * @assigns : \nothing
 * @ensures : returns the empty stack_position
 */
stack_position empty_stack_position()
{
    return NULL;
}

/*
 * vacuity test
 * @args    : s
 * @requires: s is valid
 * @assigns : \nothing
 * @ensures : returns 1 if the stack_position is empty, 0 otherwise
 */
int is_empty_stack_position(stack_position s)
{
    /* tests if s is equal to NULL */
    return (s == NULL);
}

/*
 * pushes a position into the stack_position
 * @args    : p, *s
 * @requires: s is pointing on a valid stack_position
 *            p is a valid position 
 * @assigns : *s
 * @ensures : adds p to the head of *s
 */
void push_position(position p, stack_position *s)
{
    /* allocates a memory space for the new stack_position element*/
    stack_position new_stack_position;
    new_stack_position = malloc(sizeof(struct stack_position_base));
    /* initalizes its position and its next (the whole stack_position *s)  */
    new_stack_position->val = p;
    new_stack_position->next = *s;
    /* *s begins at the new head */
    *s = new_stack_position;
}

/*
 * removes the first element of the stack_position
 * @args    : *s
 * @requires: s is pointing on a valid stack_position 
 * @assigns : *s
 * @ensures : removes the head of *s and returns its value
 */
position pop_position(stack_position *s)
{
    /* we save the position and the stack_position that will be removed */
    position res;
    stack_position tmp;
    res = (*s)->val;
    tmp = *s;
    /* *s begins one element later */
    *s = (*s)->next;
    /* free of the former head memory space*/
    free(tmp);
    /* returns the position of the element removed */
    return res;
}

/*
 * freeing the memory space of the stack_position
 * @args    : *s
 * @requires: s is pointing on a valid stack_token 
 * @assigns : *s
 * @ensures : frees the memory space allocated for all the elements of *s
 */
void free_stack_position(stack_position *s)
{
    stack_position tmp;
    /* @loop variant : the length of *s */
    while (*s != NULL)
    {
        tmp = *s;
        *s = (*s)->next;
        /* frees the current element of the stack_position */
        free(tmp);
        /* @loop end : as *s is a valid, the last element is pointing to NULL, 
    so after printing the last element, *s is set to NULL and the loop ends */
    }
}