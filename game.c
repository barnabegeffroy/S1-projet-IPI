#include <stdlib.h>
#include <stdio.h>
#include "stack_token.h"
#include "stack_position.h"
#include "board.h"

/*
 * game.c is the file containing the main function. 
 * It is where the game takes place, so it uses a board, stacks and stacks of positions
 * @see: board.h, stack.h, stack_position.h
 */

/*
 * asks the player where he or she wants to play
 * @args    : b
 * @requires: b is a valid board
 * @assigns : \nothing
 * @ensures : returns the position the player enters if he or she valids it,
 *            otherwise it returns the invalid position (-1, -1)
 */
position ask_player(board b)
{
    /* intializes the returned value */
    int y, x;
    position res;
    char check;
    char buf[256];
    /*
     * asks the player for the coordinate he or she wants to play
     * for instance if the player enters "2 3"
     * res will be set to (2, 3)
     * the function goes on if res is a position of a box of b,
     * otherwise it will ask the player to enter an other position until this one is valid
     */
    do
    {
        printf("Quelle case choisissez-vous ?\n");
        fgets(buf, 256, stdin);
        sscanf(buf, "%d %d", &y, &x);
        res = init_position(x, y);
        /* @loop end : the loop ends when the position entered matches with a box of the board */
    } while (!is_placable(res, b)); /* res is a set position */
    /* prints all the elements of the stack at res in b
       there is an offset of 1 to place p at the right box of b.tab */
    print_stack_token(b.tab[res.y - 1][res.x - 1]);
    /*
     * asks the player to confirm its choice, if the answer begins with an 'o' that means yes
     * if the answer begins with an 'n' that means no
     * the player must enters an answer until this one begins with an 'o' or an 'n'
     */
    do
    {
        printf("\nVoulez-vous mettre le jeton ici ?\n");
        fgets(buf, 256, stdin);
        /* @loop end : the loop ends when the value entered begins with an 'o' or an 'n' */
    } while (buf[0] != 'o' && buf[0] != 'n');
    sscanf(buf, "%c", &check);
    /* if the players do not confirms his or her choice, res is set to (-1, -1) */
    if (check == 'n')
        res = init_position(-1, -1);
    return res;
}

/*
 * places a token t
 * @args    : t, p, *b
 * @requires: b is pointing on a valid board
 *            p is a position of the board
 *            t is the token to place
 * @assigns : *b
 * @ensures : places token t on *b at the position p :
 *            stacking it on top of any tokens already present.
 *            If the two tokens at the top of the stack are equal to t, the box is activated:
 *            1. Both tokens are removed from the stack.
 *            2. Tokens t are added to the four adjacent boxes, provided they have not already been activated on that player's turn.
 *                Tokens added on the adjacent squares can activate those boxes, if they also had a token t on top. 
 *                Tokens are then added to their adjacent boxes that have not already been activated this turn, and so on.
 */
void place_token(token_value t, position p, board *b)
{
    /* intializes a counter which represents the activation level */
    int count = 0;
    /* 
     * intializes an empty board to note activated boxes
     * to note them, we will add the value of the counter to the box,
     * if we try to place a token on a position, is_activated will tell if this box is activated 
     * and if it is, what is its activation level thanks to the counter
     * The first activation level is the position chosen by the player, the second one is the positions actived thanks to the first activated position and so on.
    */
    board is_activated = create_board(b->size);
    /* intializes an empty stack of positions (i.e. stack_position) 
       it represents the position for the next activation level, so initially just p */
    stack_position next_act_lvl = empty_stack_position();
    push_position(p, &next_act_lvl);
    /* @loop variant : the length of next_act_lvl */
    while (!is_empty_stack_position(next_act_lvl))
    /* @loop end : the loop ends when there is no more activation level and so when next_act_lvl is empty 
        we cannot have an element of next_act_lvl at two different activation levels,
        so in the worst case, all the boxes has been treated and next_act_lvl will be empty */
    {
        /* initializes a stack_position which will be the positions to place during this loop */
        stack_position cur_act_lvl;
        cur_act_lvl = next_act_lvl;
        /* sets next_act_lvl to an empty stack_position, it will contain the positions to place for the next loop (i.e. next activation level), if any (i.e. activated boxes) */
        next_act_lvl = empty_stack_position();
        /* @loop variant : the length of cur_act_lvl */
        while (!is_empty_stack_position(cur_act_lvl))
        /* @loop end : the loop ends when all the positions of cur_act_lvl has been treated and so when cur_act_lvl is empty 
        as we pop an element at the beginning of each loop and do not add any other element, cur_act_lvl will become empty at a time */
        {
            /* pops the head of cur_act_lvl and places its returned value in p */
            p = pop_position(&cur_act_lvl);
            /* if the token on the top of box chosen is different, tries to place it */
            if (on_top(*b, p) != t)
            {
                /* places it if the position is not activated or have been activated during this activation level */
                token_value act = on_top(is_activated, p);
                if (act == -1 || act == count)
                    add_token(t, p, b);
            }
            else
            {
                /* the box has the same token on the top of the box at p so the box is activated and the token removed */
                remove_token(p, b);
                /* adds the activation level (count) to box p of the board of activated boxes */
                add_token(count, p, &is_activated);
                /* initializes the position of the adjacent boxes where we want to place tokens */
                position up = p, down = p, right = p, left = p;
                up.y += 1;
                down.y -= 1;
                right.x += 1;
                left.x -= 1;
                /* initalizes a table to work on the adjacent boxes equally */
                position to_add[4] = {up, right, left, down};
                int i;
                /* @loop invariant : i is the index of an element of to_add*/
                for (i = 0; i < 4; i += 1)
                {
                    /* if the position is on the board and its not yet activated, 
                    the positions is pushed in next_act_lvl and will be placed in the next activation level */
                    if (is_placable(to_add[i], *b) && on_top(is_activated, to_add[i]) == -1)
                        push_position(to_add[i], &next_act_lvl);
                }
            }
        }
        /* increments the counter and if next_act_lvl is not empty, runs the next activation level */
        count += 1;
        /* we do not have to free the memory space of cur_act_lvl as this one is an empty stack_position at the end of the loop */
        // free_stack_position(&cur_act_lvl);
    }
    /* we do not have to free the memory space of next_act_lvl as this one is an empty stack_position at the end of the loop */
    // free_stack_position(&next_act_lvl);
    /* frees the memory space of the board is_activated */
    free_board(&is_activated);
}

/*
 * starts a game with nb_players and a size board equal to board_size
 * @args    : nb_players, board_size
 * @requires: nb_players >= 0, board_size >= 0
 * @assigns : \nothing
 * @ensures : plays out the game with nb_players and a size board is equal to board_size
 */
void play(int nb_players, int board_size)
{
    /* creates the board of the game */
    board b = create_board(board_size);
    int i;
    /* creates a table where each box represents a player token
       in this game, player 1 has a token '1' set in players[0] and so on */
    int players[nb_players];
    /* @loop invariant : i is the index of the number of a player in the game */
    for (i = 0; i < nb_players; i += 1)
        players[i] = i + 1;
    while (!is_full(b))
    /* @loop end : the loop is ending when the board does not contain any empty box
    we cannot assure that is loop will ending, if players do not try to fill the board */
    {
        /* @loop invariant : i is the index of the table players */
        for (i = 0; i < nb_players; i += 1)
        {
            /* if the board is not full, asks the player to place a token
            this test is for the case of a player filling the board while the loop 'for' is not ended */
            if (!is_full(b))
            {
                position p;
                do
                {
                    /* prints the game (player who has to play and the board) */
                    printf("\nTour du joueur %d.\n", players[i]);
                    print_board(b);
                    /* asks the player for the position he or she wants to play until this one is on the board */
                    p = ask_player(b);
                    /* the player can refuse the position he or she choose in ask_player, then p is (-1, -1) */
                    /* @loop end : the loop ends when the player confirms a valid position on the board */
                } while (!is_placable(p, b)); /* @loop invariant : p is a set position */
                /* places the token on the chosen position */
                place_token(players[i], p, &b);
            }
        }
    }
    /* prints the final board */
    print_board(b);
    /* 
     * gets the score table and prints it
     * the winner is the one with the most tokens at the top of box on the board 
     */
    int *score = most_present_elem(players, nb_players, b);
    printf("\nVoici le score obtenu par chaque joueur :\n");
    /* @loop invariant : i is the index of players[] and score[] */
    for (i = 0; i < nb_players; i += 1)
        printf("\nLe joueur %i : %d", players[i], score[i]);
    printf("\n");
    /* b and score are no more useful, so we free their memory space */
    free_board(&b);
    free(score);
}

int main()
{
    int nb_players, board_size;
    char buf[256];
    /* asks for a positive number of players */
    do
    {
        printf("Entrer le nombre de joueurs\n");
        fgets(buf, 256, stdin);
        sscanf(buf, "%d", &nb_players);
        /* @loop end : the loop ends when the value entered is positive integer */
    } while (nb_players < 0);
    /* asks for a positive size of the board*/
    do
    {
        printf("Entrer la taille du plateau\n");
        fgets(buf, 256, stdin);
        sscanf(buf, "%d", &board_size);
        /* @loop end : the loop ends when the value entered is positive integer */
    } while (board_size < 0);
    /* execute the game */
    play(nb_players, board_size);
    return 0;
}