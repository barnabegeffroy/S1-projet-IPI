#include <stdlib.h>
#include <stdio.h>
#include "stack_token.h"

/*
 * The stack_token is the element of the game that represents the boxes of the board by stacks of tokens (integers). 
 * The tokens of the game are the elements of the stack.
 */

/* 
 * Concrete type of a stack of tokens (integers) with linked list
 * @args: val, the value of the token
 *        *next, the following element
 */
struct stack_token_base
{
    token_value val;
    struct stack_token_base *next;
};

/*
 * construction of the empty stack_token
 * @args    : \nothing
 * @requires: \nothing
 * @assigns : \nothing
 * @ensures : returns the empty stack of tokens
 */
stack_token empty_stack_token()
{
    return NULL;
}

/*
 * vacuity test
 * @args    : s
 * @requires: s is valid
 * @assigns : \nothing
 * @ensures : returns 1 if the stack_token is empty, 0 otherwise
 */
int is_empty_stack_token(stack_token s)
{
    /* tests if s is equal to NULL */
    return (s == NULL);
}

/*
 * pushes an element into the stack_token
 * @args    : v, *s
 * @requires: s is pointing on a valid stack_token
 * @assigns : *s
 * @ensures : adds v to the head of *s
 */
void push(token_value v, stack_token *s)
{
    /* allocates a memory space for the new stack_token element*/
    stack_token new_stack_token;
    new_stack_token = malloc(sizeof(struct stack_token_base));
    /* initalizes its token_value and its next (the whole stack_token *s)  */
    new_stack_token->val = v;
    new_stack_token->next = *s;
    /* *s begins at the new head */
    *s = new_stack_token;
}

/*
 * removes the first element of the stack_token
 * @args    : *s
 * @requires: s is pointing on a valid stack_token 
 * @assigns : *s
 * @ensures : removes the head of *s and returns its token_value
 */
token_value pop(stack_token *s)
{
    /* saves the token_value and the stack_token that will be removed */
    token_value res;
    stack_token tmp;
    res = (*s)->val;
    tmp = *s;
    /* *s begins one element later */
    *s = (*s)->next;
    /* free of the former head memory space */
    free(tmp);
    /* returns the token_value of the element removed */
    return res;
}

/*
 * gives the token_value of the stack_token's head
 * @args    : s
 * @requires: s is a valid stack_token 
 * @assigns : \nothing
 * @ensures : returns the token_value of s's head if the stack_token is not empty,
 *            -1 otherwise
 */
token_value get_head(stack_token s)
{
    if (!is_empty_stack_token(s))
        return s->val;
    return -1;
}

/*
 * prints the stack_token
 * @args    : s
 * @requires: s is a valid stack_token 
 * @assigns : \nothing
 * @ensures : prints the stack_token s
 */
void print_stack_token(stack_token s)
{
    printf("\n");
    /* @loop variant : the length of s */
    while (s != NULL)
    {
        printf(" %i\n", s->val);
        s = s->next;
        /* @loop end : as s is a valid, the last element is pointing to NULL, 
    so after printing the last element, s is set to NULL and the loop ends */
    }
    printf("---\n");
}

/*
 * freeing the memory space of the stack_token
 * @args    : *s
 * @requires: s is pointing on a valid stack_token 
 * @assigns : *s
 * @ensures : frees the memory space allocated for all the elements of *s
 */
void free_stack_token(stack_token *s)
{
    stack_token tmp;
    /* @loop variant : the length of *s */
    while (*s != NULL)
    {
        tmp = *s;
        *s = (*s)->next;
        /* frees the current element of the stack_token */
        free(tmp);
        /* @loop end : as *s is a valid, the last element is pointing to NULL, 
    so after printing the last element, *s is set to NULL and the loop ends */
    }
}