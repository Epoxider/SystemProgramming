CPP = gcc
FLAGS = -Wall -std=c99 -pedantic -g

EXEC = stackuser
OBJS = stack.o stackuser.o

default:${EXEC}

clean:
	rm -f ${EXEC}
	rm -f *.o

run: ${EXEC}
	./${EXEC}

${EXEC}:${OBJS}
	${CPP} ${FLAGS} -o ${EXEC} ${OBJS}

.c.o:
	${CPP} ${FLAGS} -c $<

stack.o: stack.c stack.h
stackuser.o: stackuser.c stack.h