# ugly Makefile

CC		= gcc
FLGS	= -g -Wall -W 
SQLFLG	= `mysql_config --cflags --libs`
BIN		= bin
FILES	= scrub.c util.c driver.c

# compile the util
compile:
	${CC} ${FLGS} ${FILES} -o ${BIN} ${SQLFLG}

# check binary for memory leaks
mem_check:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck ./${BIN}
# format source files using clang-format
format:
	find . -regex '.*\.\(c\|cu\|cuh\|cpp\|hpp\|h\|cxx\)' -exec clang-format-9 -style=file -i {} \;

# generate Doxygen documentation locally
docs:
	cd ../ && doxygen

# TODO: This may result in your terminal session being broken
SQL_copy:
	mysqldump -u aba275 -p --no-data TEST_DB | mysql -u aba275 -p TEST_DB2
	mysqldump -u aba275 -p --no-create-info TEST_DB | mysql -u aba275 -p TEST_DB2

# remove binary
clean:
	rm ${BIN}

