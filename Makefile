# Makefile for RTSmini

all:
	gcc -o rtsmini main.c utils.c -Wall -lncurses

clean:
	@rm -f rtsmini

