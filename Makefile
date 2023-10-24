
DATASTR = datastructures
INCLUDE = includes
PROGRAM = programs

CC = gcc
CFLAGS = -I $(INCLUDE)

OBJS = main.o programs/commands.o $(PROGRAM)/history.o $(PROGRAM)/aliases.o $(PROGRAM)/prompt.o $(PROGRAM)/builtins.o $(PROGRAM)/parser.o programs/shell.o $(DATASTR)/ADTList.o $(DATASTR)/ADTVector.o $(DATASTR)/ADTMap.o
EXEC = main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)