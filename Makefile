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

debug : debugbin debugobj debugbin/$(progname)
	

release: bin obj bin/$(progname)
	

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

obj/%.o : %.c
	$(CC) $(CPPFLAGS) $(RELFLAGS) $(CFLAGS) -c -o $@ $<

debugobj/%.o : %.c
	$(CC) $(CPPFLAGS) $(DBGFLAGS) $(CFLAGS) -c -o $@ $<

