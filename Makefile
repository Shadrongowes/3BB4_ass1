
# Use with make 
#

CFLAGS=  -c -Wall
CC=gcc

all:asgn1 job

asgn1:asgn1.o log.o
	$(CC)  asgn1.o  log.o -o asgn1
job:job.o log.o
	$(CC) log.o job.o -o job

job.o:job.c
	$(CC) $(CFLAGS) job.c

log.o:log.c
	$(CC) $(CFLAGS) log.c

asgn1.o:asgn1.c
	$(CC) $(CFLAGS) asgn1.c





clean :
	rm -rf *.o asgn1 job
