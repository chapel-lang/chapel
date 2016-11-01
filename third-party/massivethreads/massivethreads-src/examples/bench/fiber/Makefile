CC=gcc
CFLAGS=-Wall -O3
TARGETS=jmp ctx mix

all: $(TARGETS)

jmp: main.c jmp.c fiber.h
	$(CC) -o $@ jmp.c main.c $(CFLAGS)

ctx: main.c ctx.c fiber.h
	$(CC) -o $@ ctx.c main.c $(CFLAGS)

mix: main.c mix.c fiber.h
	$(CC) -o $@ mix.c main.c $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGETS)
