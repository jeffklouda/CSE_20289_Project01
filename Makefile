CC=		gcc
CFLAGS=		-g -gdwarf-2 -Wall -std=gnu99
LD=		gcc
LDFLAGS=	-L.
AR=		ar
ARFLAGS=	rcs
TARGETS=	search
OBJS=		execute.o filter.o main.o search.o utilities.o

all:		$(TARGETS)

search: $(OBJS)
	@echo Linking search...
	@$(LD) $(LDFLAGS) -o search $(OBJS)

execute.o:	execute.c
	@echo compiling execute.o
	@$(CC) $(CFLAGS) -c -o execute.o execute.c

filter.o:	filter.c
	@echo compiling filter.o
	@$(CC) $(CFLAGS) -c -o filter.o filter.c

main.o:	main.c
	@echo compiling main.o
	@$(CC) $(CFLAGS) -c -o main.o main.c

search.o:	search.c
	@echo compiling search.o
	@$(CC) $(CFLAGS) -c -o search.o search.c

utilities.o:	utilities.c
	@echo compiling utilities.o
	@$(CC) $(CFLAGS) -c -o utilities.o utilities.c

test:		search test_search.sh
	@echo Testing $<...
	@./test_search.sh

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) *.o *.log *.input

.PHONY:		all test benchmark clean

# TODO: Add rules for search and object files
