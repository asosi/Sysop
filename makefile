CC=gcc
CUFLAGS=-lncurses
SOURCE1=plive.c
SOURCE2=mkbkp.c
SOURCE3=equals.c
EXECUTABLE1=plive
EXECUTABLE2=mkbkp
EXECUTABLE3=equals
LOG=makelog.c

all: ${EXECUTABLE1} ${EXECUTABLE2} ${EXECUTABLE3}

plive: plive.o
	${CC} plive.o -o ${EXECUTABLE1} ${LOG} ${CUFLAGS}

plive.o: ${SOURCE1}
	${CC} -c plive.c

mkbkp: mkbkp.o
	${CC} mkbkp.o -o ${EXECUTABLE2} ${LOG}

mkbkp.o: ${SOURCE2}
	${CC} -c mkbkp.c

equals: equals.o
	${CC} equals.o -o ${EXECUTABLE3} ${LOG}

equals.o: ${SOURCE3}
	${CC} -c equals.c

clean:
	@rm -rf *o ${EXECUTABLE}