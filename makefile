CC = gcc
CFLAGS = -O3
TARGET1 = TMsearch
TARGET2 = DEJsearch

all: $(TARGET1) $(TARGET2)

$(TARGET1): TMsearch.o powerfree.o
	$(CC) $(CFLAGS) -o $(TARGET1) TMsearch.o powerfree.o

$(TARGET2): DEJsearch.o powerfree.o
	$(CC) $(CFLAGS) -o $(TARGET2) DEJsearch.o powerfree.o

TMsearch.o: TMsearch.c powerfree.h
	$(CC) $(CFLAGS) -c TMsearch.c

DEJsearch.o: DEJsearch.c powerfree.h
	$(CC) $(CFLAGS) -c DEJsearch.c

powerfree.o: powerfree.c powerfree.h
	$(CC) $(CFLAGS) -c powerfree.c

clean:
	rm -f *.o $(TARGET1) $(TARGET2)
