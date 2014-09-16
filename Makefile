CC=gcc
CFLAGS= -g   -Wall -Llib
LIBS=-lreadline -lncurses -lmylib

PROGS=library mydash
OBJECTS=Parser.o JobManager.o error.o

all: $(PROGS) dox

library:
	cd libsrc; make install

mydash: mydash.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $< $(LIBS)

dox:
	echo "Generating documentation using doxygen..."
	doxygen doxygen-config > doxygen.log
	echo "Use konqueror docs/html/index.html to see docs (or any other browser)"


clean:
	cd libsrc; make installclean
	/bin/rm -f *.o $(PROGS) a.out core  *.log
	/bin/rm -fr docs
	/bin/rm -f *.uncrustify
