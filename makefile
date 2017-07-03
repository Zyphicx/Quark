CC = gcc

PARSER = Parser/lexer.c Parser/grammar.c Parser/parser.c
GENERAL = General/hashtable.c General/comperror.c

ALL = $(PARSER) $(GENERAL)
NAME = quark

all:
	$(CC) $(ALL) -o $(NAME)
