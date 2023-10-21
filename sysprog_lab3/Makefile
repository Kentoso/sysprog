CC=gcc

CFLAGS=-Wall

LDFLAGS=-lpcre2-8

OBJS=main.o token_table.o

TARGET=lexer

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

main.o: main.c token_table.h token_type.h
	$(CC) $(CFLAGS) -c main.c

table.o: token_table.c token_table.h token_type.h
	$(CC) $(CFLAGS) -c token_table.c

clean:
	rm -f $(OBJS) $(TARGET)
