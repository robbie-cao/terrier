all: target
FORCE: ;
.PHONY: FORCE

program = terrier

target: $(program)

CC = gcc

VPATH = .
IPATH = .

CFLAGS  = -O2 --std=c99 -Wall -Wextra

DEBUG = 1

# DEBUG option, set before make, eg 'DEBUG=1 make'
ifeq ($(DEBUG),1)
	CFLAGS+=-DDEBUG=1
endif

LDFLAGS = -L /usr/lib/x86_64-linux-gnu/ -L /usr/local/lib

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

terrier: main.o hex.o
	$(CC) $^  $(LDFLAGS) -o $@

clean:
	rm -f *.o $(program)
