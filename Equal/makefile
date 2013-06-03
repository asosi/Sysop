CC=gcc
SOURCE=equals.c
OBJECT=${SOURCE:.c=.o}
EXECUTABLE=equals

all: ${EXECUTABLE} cleanob

equals: ${OBJECT}
	@${CC} ${OBJECT} -o ${EXECUTABLE}

equals.o: ${SOURCE}
	@${CC} -c ${SOURCE}

cleanob:
	@rm ${OBJECT}
	@rm -rf ${SOURCE}~

clean:
	@rm -rf *o ${EXECUTABLE}
