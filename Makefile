# Makefile for RTSmini

all:
	gcc -g -o rtsmini main.c utils.c -Wall -lncurses

clean:
	@rm -f rtsmini

