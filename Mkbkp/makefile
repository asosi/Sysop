CC=gcc
SOURCE=mkbkp.c
OBJECT=${SOURCE:.c=.o}
EXECUTABLE=mkbkp

all: ${EXECUTABLE} cleanob

mkbkp: ${OBJECT}
	@${CC} ${OBJECT} -o ${EXECUTABLE}

mkbkp.o: ${SOURCE}
	@${CC} -c ${SOURCE}

cleanob:
	@rm ${OBJECT}
	@rm -rf ${SOURCE}~

clean:
	@rm -rf *o ${EXECUTABLE}
