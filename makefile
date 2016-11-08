CC = gcc

PARSER = Parser/lexer.c

ALL = $(PARSER)
NAME = quark

all:
	$(CC) $(ALL) -o $(NAME)