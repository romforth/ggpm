progname = ggpm
CC = gcc
CPPFLAGS =
CFLAGS =
DBGFLAGS = -O0 -g
RELFLAGS = -O2
D = debugobj/
R = obj/
DBGOBJS = $(D)ggpm.o $(D)usage.o
RELOBJS = $(R)ggpm.o $(R)usage.o

all : release
	

love: notwar release
	

notwar:
	echo "\\nNot war?\\n"

debug : debugbin debugobj debugbin/$(progname) debugbin/compact debugbin/ggpm2c
	

release: bin obj bin/$(progname) bin/compact bin/ggpm2c
	

clean:
	rm -f $(R)*.o bin/$(progname)

debugclean:
	rm -f $(D)*.o debugbin/$(progname)

bin:
	mkdir -p bin

obj:
	mkdir -p obj

debugbin:
	mkdir -p debugbin

debugobj:
	mkdir -p debugobj

debugbin/$(progname) : $(DBGOBJS)
	$(CC)  $(LDFLAGS) -o debugbin/$(progname) $(DBGOBJS)

bin/$(progname) : $(RELOBJS)
	$(CC)  $(LDFLAGS) -o bin/$(progname) $(RELOBJS)

debugbin/compact : debugobj/compact.o debugobj/store.o
	$(CC)  $(LDFLAGS) $(CFLAGS) -o debugbin/compact debugobj/compact.o debugobj/store.o

bin/compact : obj/compact.o obj/store.o
	$(CC)  $(LDFLAGS) $(CFLAGS) -o bin/compact obj/compact.o obj/store.o

debugbin/ggpm2c : debugobj/ggpm2c.o debugobj/ggpm_compacted.o debugobj/store.o
	$(CC)  $(LDFLAGS) $(CFLAGS) -o debugbin/ggpm2c debugobj/ggpm2c.o debugobj/ggpm_compacted.o debugobj/store.o

bin/ggpm2c : obj/ggpm2c.o obj/ggpm_compacted.o obj/store.o
	$(CC)  $(LDFLAGS) $(CFLAGS) -o bin/ggpm2c obj/ggpm2c.o obj/ggpm_compacted.o obj/store.o


obj/ggpm_compacted.c : ggpm.c bin/compact
	bin/compact ggpm.c obj/ggpm_compacted.c

debugobj/ggpm_compacted.c : ggpm.c debugbin/compact
	debugbin/compact ggpm.c debugobj/ggpm_compacted.c

obj/ggpm_compacted.o : obj/ggpm_compacted.c
	$(CC)  $(LDFLAGS) $(RELFLAGS) $(CFLAGS) -c -o obj/ggpm_compacted.o  obj/ggpm_compacted.c

debugobj/ggpm_compacted.o : debugobj/ggpm_compacted.c
	$(CC)  $(LDFLAGS) $(DBGFLAGS) $(CFLAGS) -c -o debugobj/ggpm_compacted.o  debugobj/ggpm_compacted.c


obj/compact.o : compact.c store.h
	$(CC) $(CPPFLAGS) $(RELFLAGS) $(CFLAGS) -c -o obj/compact.o compact.c
debugobj/compact.o : compact.c store.h
	$(CC) $(CPPFLAGS) $(DBGFLAGS) $(CFLAGS) -c -o debugobj/compact.o compact.c

obj/ggpm.o : ggpm.c usage.h
	$(CC) $(CPPFLAGS) $(RELFLAGS) $(CFLAGS) -c -o obj/ggpm.o ggpm.c
debugobj/ggpm.o : ggpm.c usage.h
	$(CC) $(CPPFLAGS) $(DBGFLAGS) $(CFLAGS) -c -o debugobj/ggpm.o ggpm.c

obj/ggpm2c.o : ggpm2c.c store.h version.h
	$(CC) $(CPPFLAGS) $(RELFLAGS) $(CFLAGS) -c -o obj/ggpm2c.o ggpm2c.c
debugobj/ggpm2c.o : ggpm2c.c store.h version.h
	$(CC) $(CPPFLAGS) $(DBGFLAGS) $(CFLAGS) -c -o debugobj/ggpm2c.o ggpm2c.c

obj/store.o : store.c store.h
	$(CC) $(CPPFLAGS) $(RELFLAGS) $(CFLAGS) -c -o obj/store.o store.c
debugobj/store.o : store.c version.h
	$(CC) $(CPPFLAGS) $(DBGFLAGS) $(CFLAGS) -c -o debugobj/store.o store.c

obj/usage.o : usage.c usage.h version.h
	$(CC) $(CPPFLAGS) $(RELFLAGS) $(CFLAGS) -c -o obj/usage.o usage.c
debugobj/usage.o : usage.c usage.h version.h
	$(CC) $(CPPFLAGS) $(DBGFLAGS) $(CFLAGS) -c -o debugobj/usage.o usage.c
