CC=gcc -Wall -Wextra

all:prog

clean:
	rm -f *.o
	rm -f prog

%.o: %.c
	$(CC) -c $<

board.o: stack_token.h board.h

stack_token.o: stack_token.h

stack_position.o: stack_position.h

game.o: stack_token.h stack_position.h board.h

prog: board.o stack_token.o stack_position.o game.o
	$(CC) $^ -o $@
