#ifndef _BOARD_H
#define _BOARD_H
#include "stack_token.h"

/*
 * This is the board for the game. Each boxe of the board is a stack of tokens (integers).
 * It is a square board.
 * Its length, for instance n, is set initially, the board is composed of n² boxes.
 * @see: stack_token.h 
 */

/* 
 * Concrete type of the position in the board
 * @args: x, its x-coordinate
 *        y, its y-coordinate
 * the position must be well differentiated from an index of a 2D table, 
 * it starts at (1, 1) in a position for a box on the board, 
 * while for a 2D table it starts at (0, 0).
 */
typedef struct _position
{
    int x;
    int y;
} position;

/*
 * construction of position
 * @args    : x, y
 * @requires: \nothing
 * @assigns : \nothing
 * @ensures : returns a position set according to x and y
 */
position init_position(int x, int y);

/* 
 * Concrete type of the board
 * @args:   tab, a 2D table of stacks of tokens (integers)
 *          size, the board length (number of boxes in a line)
 */
typedef struct board_base
{
    stack_token **tab;
    int size;
} board;

/*
 * construction of an empty board with a specific size
 * @args    : board_size
 * @requires: board_size >= 0
 * @assigns : \nothing
 * @ensures : returns an empty board of size board_size
 */
board create_board(int board_size);

/*
 * belonging test
 * @args    : p, b
 * @requires: p and b must be set
 * @assigns : \nothing
 * @ensures : returns 1 if p is a position of a box in b, 0 otherwise
 */
int is_placable(position p, board b);

/*
 * fullness test
 * @args    : b
 * @requires: b is valid
 * @assigns : \nothing
 * @ensures : returns 1 if the board is full (all the boxes have at least one element),
 *            0 otherwise
 */
int is_full(board b);

/*
 * vacuity test
 * @args    : b
 * @requires: b is valid
 * @assigns : \nothing
 * @ensures : returns 1 if the board is empty (all the boxes are empty stacks),
 *            0 otherwise
 * this function is not used during the game but may be useful for another use of the board
 */
int is_empty_board(board b);

/*
 * puts a token on the board
 * @args    : v, p, *b
 * @requires: p is a position on *b,
 *            b is pointing on a valid board
 * @assigns : *b
 * @ensures : adds v to the head of the stack of the box at p on *b
 */
void add_token(token_value v, position p, board *b);

/*
 * removes the first element of a board box 
 * @args    : p, *b
 * @requires: p is a position on *b,
 *            b is pointing on a valid board 
 * @assigns : *b
 * @ensures : removes the head of the stack of the box at p on *b
 */
void remove_token(position p, board *b);

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
int *most_present_elem(token_value different_tokens[], int size, board b);

/*
 * gives the token_value of the stack's head at a certain position on the board
 * @args    : b, p
 * @requires: b is a valid board,
 *            p is a position on b 
 * @assigns : \nothing
 * @ensures : returns the token_value on the top of the stack at p on b
 */
token_value on_top(board b, position p);

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
void print_board(board b);

/*
 * freeing the memory space of the board
 * @args    : *b
 * @requires: b is pointing on a valid board
 * @assigns : *b
 * @ensures : frees the memory space allocated for the board
 */
void free_board(board *b);

#endif