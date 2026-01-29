CC = gcc
CFLAGS = -Wall -pedantic
EXEC = interpreter

all: $(EXEC)

$(EXEC): main.o interpreter.o functions.o memory.o
		$(CC) $(CFLAGS) main.o interpreter.o functions.o memory.o -o $(EXEC) 

main.o: interpreter.h main.c
		$(CC) $(CFLAGS) -c main.c

interpreter.o: functions.h interpreter.h interpreter.c
		$(CC) $(CFLAGS) -c interpreter.c

functions.o: memory.h functions.h functions.c
		$(CC) $(CFLAGS) -c functions.c

memory.o: memory.h memory.c
		$(CC) $(CFLAGS) -c memory.c

clean:
		rm -f memory.o functions.o interpreter.o main.o

allclean: $(EXEC) clean

run: $(EXEC)
		./$(EXEC)
