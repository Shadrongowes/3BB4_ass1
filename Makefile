
# Use with make 
#

CFLAGS=  -c -Wall
CC=gcc

assgn1:assgn1 job

assgn1:assgn1.o log.o
	$(CC)  assgn1.o  log.o -o assgn1
job:job.o log.o
	$(CC) log.o job.o -o job

job.o:job.c
	$(CC) $(CFLAGS) job.c

log.o:log.c
	$(CC) $(CFLAGS) log.c

assgn1.o:assgn1.c
	$(CC) $(CFLAGS) assgn1.c





clean :
	rm -rf *.o assgn1 job
