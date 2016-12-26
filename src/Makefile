CC=musl-gcc
#CC=gcc
CFLAGS=-Wall -Wextra -pedantic -static -std=c99
EXEC=r2k
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

$(EXEC):
	$(CC) $(EXEC).c $(CFLAGS) -O3 -o $(EXEC)

clean:
	@rm -f $(EXEC) *.o
