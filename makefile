CC = gcc
CFLAGS = -O3
TARGETS = TMsearch DEJsearch VTMsearch LEsearch VTMnonuniform

all: $(TARGETS)

TMsearch: TMsearch.o powerfree.o
	$(CC) $(CFLAGS) -o TMsearch TMsearch.o powerfree.o

DEJsearch: DEJsearch.o powerfree.o
	$(CC) $(CFLAGS) -o DEJsearch DEJsearch.o powerfree.o

VTMsearch: VTMsearch.o powerfree.o
	$(CC) $(CFLAGS) -o VTMsearch VTMsearch.o powerfree.o

LEsearch: LEsearch.o powerfree.o
	$(CC) $(CFLAGS) -o LEsearch LEsearch.o powerfree.o

VTMnonuniform: VTMnonuniform.o powerfree.o
	$(CC) $(CFLAGS) -o VTMnonuniform VTMnonuniform.o powerfree.o

TMsearch.o: TMsearch.c powerfree.h
	$(CC) $(CFLAGS) -c TMsearch.c

DEJsearch.o: DEJsearch.c powerfree.h
	$(CC) $(CFLAGS) -c DEJsearch.c

VTMsearch.o: VTMsearch.c powerfree.h
	$(CC) $(CFLAGS) -c VTMsearch.c

LEsearch.o: LEsearch.c powerfree.h
	$(CC) $(CFLAGS) -c LEsearch.c

VTMnonuniform.o: VTMnonuniform.c powerfree.h
	$(CC) $(CFLAGS) -c VTMnonuniform.c

powerfree.o: powerfree.c powerfree.h
	$(CC) $(CFLAGS) -c powerfree.c

clean:
	rm -f *.o $(TARGETS)
