CC = gcc
CFLAGS = -O3
TARGETS = TMsearch DEJsearch VTMsearch

all: $(TARGETS)

TMsearch: TMsearch.o powerfree.o
	$(CC) $(CFLAGS) -o TMsearch TMsearch.o powerfree.o

DEJsearch: DEJsearch.o powerfree.o
	$(CC) $(CFLAGS) -o DEJsearch DEJsearch.o powerfree.o

VTMsearch: VTMsearch.o powerfree.o
	$(CC) $(CFLAGS) -o VTMsearch VTMsearch.o powerfree.o

TMsearch.o: TMsearch.c powerfree.h
	$(CC) $(CFLAGS) -c TMsearch.c

DEJsearch.o: DEJsearch.c powerfree.h
	$(CC) $(CFLAGS) -c DEJsearch.c

VTMsearch.o: VTMsearch.c powerfree.h
	$(CC) $(CFLAGS) -c VTMsearch.c

powerfree.o: powerfree.c powerfree.h
	$(CC) $(CFLAGS) -c powerfree.c

clean:
	rm -f *.o $(TARGETS)
