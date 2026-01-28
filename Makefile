memory.o: memory.c
		$(CC) -c memory.c

functions.o: memory.h functions.c
		$(CC) -c functions.c

interpret.o: functions.h interpret.c
		$(CC) -c interpret.c

main.o: interpret.h main.c
		$(CC) -c main.c

all: main.o interpret.o functions.o memory.o
		$(CC) main.o interpret.o functions.o memory.o -o exec

allclean: all
		rm memory.o functions.o interpret.o main.o

run: allclean
		./exec
