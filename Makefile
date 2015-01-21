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
	

debug : debugbin/$(progname)
	

release: bin/$(progname)
	

clean:
	rm -f $(R)*.o bin/$(progname)

debugclean:
	rm -f $(D)*.o debugbin/$(progname)

debugbin/$(progname) : $(DBGOBJS)
	$(CC)  $(LDFLAGS) -o debugbin/$(progname) $(DBGOBJS)

bin/$(progname) : $(RELOBJS)
	$(CC)  $(LDFLAGS) -o bin/$(progname) $(RELOBJS)

obj/%.o : %.c
	$(CC) $(CPPFLAGS) $(RELFLAGS) $(CFLAGS) -c -o $@ $<

debugobj/%.o : %.c
	$(CC) $(CPPFLAGS) $(DBGFLAGS) $(CFLAGS) -c -o $@ $<

