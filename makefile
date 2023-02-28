CC = gcc
CFLAGS = -O3
TARGET = TMsearch

$(TARGET): TMsearch.o powerfree.o
	$(CC) $(CFLAGS) -o $(TARGET) TMsearch.o powerfree.o

TMsearch.o: TMsearch.c powerfree.h
	$(CC) $(CFLAGS) -c TMsearch.c

powerfree.o: powerfree.c powerfree.h
	$(CC) $(CFLAGS) -c powerfree.c

clean:
	rm -f *.o $(TARGET)
