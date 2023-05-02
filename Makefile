CC		= gcc
FLGS	= -g -Wall -W 
SQLFLG	= `mysql_config --cflags --libs`
BIN		= bin
FILES	= scrub.c driver.c

compile:
	${CC} ${FLGS} ${FILES} -o ${BIN} ${SQLFLG}

mem_check:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck ./${BIN}

clean:
	rm ${BIN}

