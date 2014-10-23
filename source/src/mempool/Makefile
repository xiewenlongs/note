
CC=gcc

CFLAGS=

OBJS = test.o palloc.o

DEST = exe



$DEST: $(OBJS)
	$(CC) $(OBJS) -o $(DEST)


palloc.o: palloc.h core.h


clean:
	-rm $(OBJS) $(DEST)
