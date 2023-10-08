CC = gcc
CFLAGS = -g -Wall
TARGET = compfs
OBJ = P4.o scanner.o parser.o generator.o testTree.o statSem.o

%.o: %.c
        $(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJ)
        $(CC) $(CFLAGS) -o $@ $^

P4.o: P4.c node.h parser.h testTree.h generator.h
scanner.o: scanner.c character.h token.h testTree.h scanner.h
parser.o: parser.c token.h node.h scanner.h testTree.h parser.h
generator.o: generator.c token.h node.h statSem.h testTree.h generator.h
tree.o: testTree.c token.h node.h testTree.h
statSem.o: statSem.c statSem.h

clean:
        rm -f $(TARGET) $(OBJ)
