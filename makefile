CC = gcc

PARSER = Parser/lexer.c Parser/grammar.c
GENERAL = General/hashtable.c

ALL = $(PARSER) $(GENERAL)
NAME = quark

all:
	$(CC) $(ALL) -o $(NAME)