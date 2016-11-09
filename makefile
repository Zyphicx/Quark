CC = gcc

PARSER = Parser/lexer.c Parser/token_list.c

ALL = $(PARSER)
NAME = quark

all:
	$(CC) $(ALL) -o $(NAME)