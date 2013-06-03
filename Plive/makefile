CC=gcc
CUFLAGS=-lncurses
SOURCE=plive.c
OBJECT=${SOURCE:.c=.o}
EXECUTABLE=plive

all: ${EXECUTABLE} cleanob

plive: ${OBJECT}
	@${CC} ${OBJECT} -o ${EXECUTABLE} ${CUFLAGS}

plive.o: ${SOURCE}
	@${CC} -c ${SOURCE}

cleanob:
	@rm ${OBJECT}
	@rm -rf ${SOURCE}~

clean:
	@rm -rf *o ${EXECUTABLE}
