#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "stack_token.h"

/*
 * This is the board for the game. Each boxe of the board is a stack of tokens (integers).
 * It is a square board.
 * Its length, for instance n, is set initially, the board is composed of n² boxes
 * @see: stack_token.h 
 */

/*
 * construction of position
 * @args    : x, y
 * @requires: \nothing
 * @assigns : \nothing
 * @ensures : returns a position set according to x and y
 */
position init_position(int x, int y)
{
    position p;
    p.x = x;
    p.y = y;
    return p;
}

/*
 * construction of an empty board with a specific size
 * @args    : board_size
 * @requires: board_size >= 0
 * @assigns : \nothing
 * @ensures : returns an empty board of size board_size
 */
board create_board(int board_size)
{
    int i, j;
    /* initializes a board with a size board_size */
    board res;
    res.size = board_size;
    /* allocates the memory space for each box of the 2D table thanks to two loops for */
    res.tab = (stack_token **)malloc(board_size * sizeof(stack_token *));
    /* @loop invariant : i is the index of an element of res.tab,
        board_size is the number of lines in the board*/
    for (i = 0; i < board_size; i += 1)
    {
        res.tab[i] = (stack_token *)malloc(board_size * sizeof(stack_token));
        /* @loop invariant : j is the index of an element of res.tab[i],
            board_size is the number of boxes in a line of the board*/
        for (j = 0; j < board_size; j += 1)
            /* intializes the box to an empty stack_token */
            res.tab[i][j] = empty_stack_token();
    }
    /* returns the empty board initialized */
    return res;
}
/*
 * belonging test
 * @args    : p, b
 * @requires: p and b must be set
 * @assigns : \nothing
 * @ensures : returns 1 if p is a position of a box in b, 0 otherwise
 */
int is_placable(position p, board b)
{
    /* tests if p is a box of the board */
    return (p.y <= b.size && p.x <= b.size && 0 < p.y && 0 < p.x);
}

/*
 * fullness test
 * @args    : b
 * @requires: b is valid
 * @assigns : \nothing
 * @ensures : returns 1 if the board is full (all the boxes have at least one element),
 *            0 otherwise
 */
int is_full(board b)
{
    int i, j;
    /* @loop invariant : i is the index of an element of b.tab,
        b.size is the number of lines in the board*/
    for (i = 0; i < b.size; i += 1)
    {
        /* @loop invariant : j is the index of an element of res.tab[i],
            b.size is the number of boxes in a line of the board*/
        for (j = 0; j < b.size; j += 1)
        {
            /* if a box is empty then the board is not full */
            if (is_empty_stack_token(b.tab[i][j]))
                return 0;
        }
    }
    /* none of the boxes are empty */
    return 1;
}

/*
 * vacuity test
 * @args    : b
 * @requires: b is valid
 * @assigns : \nothing
 * @ensures : returns 1 if the board is empty (all the boxes are empty stacks),
 *            0 otherwise
 * this function is not used during the game but may be useful for another use of the board
 */
int is_empty_board(board b)
{
    int i, j;
    /* @loop invariant : i is the index of an element of b.tab,
        b.size is the number of lines in the board*/
    for (i = 0; i < b.size; i += 1)
    {
        /* @loop invariant : j is the index of an element of res.tab[i],
            b.size is the number of boxes in a line of the board*/
        for (j = 0; j < b.size; j += 1)
        {
            /* if a box is not empty then the board is not empty */
            if (!is_empty_stack_token(b.tab[i][j]))
                return 0;
        }
    }
    /* all the boxes are empty */
    return 1;
}

/*
 * puts a token on the board
 * @args    : v, p, *b
 * @requires: p is a position on *b,
 *            b is pointing on a valid board
 * @assigns : *b
 * @ensures : adds v to the head of the stack of the box at p on *b
 */
void add_token(token_value v, position p, board *b)
{
    /* offset of 1 to place p at the right box of b.tab */
    push(v, &b->tab[p.y - 1][p.x - 1]);
}

/*
 * removes the first element of a board box 
 * @args    : p, *b
 * @requires: p is a position on *b,
 *            b is pointing on a valid stack 
 * @assigns : *b
 * @ensures : removes the head of the stack of the box at p on *b
 */
void remove_token(position p, board *b)
{
    /* offset of 1 to place p at the right box of b.tab */
    pop(&b->tab[p.y - 1][p.x - 1]);
}

/*
 * counter of points
 * @args    : different_tokens[], size, b
 * @requires: different_tokens contains all different values of tokens on b,
 *            size is number of different tokens (size of different_tokens[]),
 *            b is a valid board 
 * @assigns : \nothing
 * @ensures : counts the number of times a token is on the top of a stack on b 
 *            returns a table containing the number of occurrences with respect to the index of different_tokens[]
 */
int *most_present_elem(token_value different_tokens[], int size, board b)
{
    int i, j, k;
    /* creates a table of int with the same size as diffretent_tokens and with all its boxes equal to 0 */
    int *count = calloc(size, sizeof(int));
    /* @loop invariant : i is the index of an element of b.tab,
        b.size is the number of lines in the board*/
    for (i = 0; i < b.size; i += 1)
    {
        /* @loop invariant : j is the index of an element of res.tab[i],
            b.size is the number of boxes in a line of the board*/
        for (j = 0; j < b.size; j += 1)
        {
            /* @loop invariant : k is the index of an element of count[] and different_tokens[],
            size is the number of boxes in count[] and different_tokens[]*/
            for (k = 0; k < size; k += 1)
            {
                /* if the head of the stack is this token, incriments count[k]*/
                if (get_head(b.tab[i][j]) == different_tokens[k])
                    count[k] += 1;
            }
        }
    }
    /* returns count containing the number of occurrences with respect to the index of different_tokens[] */
    return count;
}

/*
 * gives the token_value of the stack's head at a certain position on the board
 * @args    : b, p
 * @requires: b is a valid board,
 *            p is a position on b 
 * @assigns : \nothing
 * @ensures : returns the token_value on the top of the stack at p on b
 */
token_value on_top(board b, position p)
{
    /* offset of 1 to place p at the right box of b.tab */
    return get_head(b.tab[p.y - 1][p.x - 1]);
}

/*
 * prints the board
 * @args    : b
 * @requires: b is a valid board 
 * @assigns : \nothing
 * @ensures : prints the board b with this pattern :
 *            -- instance for b.size = 4 --
 *               |1 2 3 4 
 *              -+---------
 *              1|        
 *              2|        
 *              3|        
 *              4| 
 *            the tokens shown is those at the head of stacks       
 */
void print_board(board b)
{
    int i, j;
    printf("\n |");
    /* prints the index of the columns starting with 1 and ending with b.size
       @loop invariant : i+1 is a column number of the board */
    for (i = 0; i < b.size; i += 1)
        printf("%d ", i + 1);
    /* prints the separation line */
    printf("\n-+-");
    for (i = 0; i < b.size; i += 1)
        printf("--");
    printf("\n");
    /* @loop invariant : i is the index of an element of b.tab,
        b.size is the number of lines in the board*/
    for (i = 0; i < b.size; i += 1)
    {
        /* prints the index of the lines starting with 1 and ending with b.size */
        printf("%d|", i + 1);
        /* @loop invariant : j is the index of an element of res.tab[i],
            b.size is the number of boxes in a line of the board */
        for (j = 0; j < b.size; j += 1)
        {
            /*
             * if the stack b.tab[i][j] is not empty prints its head, else prints a blank
             * get_head tests if the stack is empty so we do not re-use is_empty_stack
             * (if the stack is empty get_head returns -1)
             */
            token_value head = get_head(b.tab[i][j]);
            if (head != -1)
                printf("%d ", head);
            else
                printf("%c ", ' ');
        }
        printf("\n");
    }
    printf("\n");
}

/*
 * freeing the memory space of the board
 * @args    : *b
 * @requires: b is pointing on a valid board
 * @assigns : *b
 * @ensures : frees the memory space allocated for the board
 */
void free_board(board *b)
{
    int i, j;
    /* @loop invariant : i is the index of an element of b.tab,
        b->size is the number of lines in the board*/
    for (i = 0; i < b->size; i += 1)
    {
        /* @loop invariant : j is the index of an element of res.tab[i],
            b->size is the number of boxes in a line of the board*/
        for (j = 0; j < b->size; j += 1)
        {
            /* frees the memory space alocated for the box (stack_token) */
            free_stack_token(&b->tab[i][j]);
        }
        /* frees the memory space alocated for the 2D table (boxes) */
        free(b->tab[i]);
    }
    /* frees the memory space alocated for the 2D table (lines) */
    free(b->tab);
}