CC = g++
COMPILE_OPTION = -g -Wall

all : interpreter

clean :
	rm -f *.o interpreter

interpreter : tokenized.o preprocessor.o tables.o evaluate.o main.o
	${CC} -o interpreter tokenized.o preprocessor.o tables.o evaluate.o main.o

tokenized.o : tokenized.cpp tokenized.h tables.h preprocessor.h
	${CC} -c ${COMPILE_OPTION} tokenized.cpp

preprocessor.o : preprocessor.cpp tokenized.o
	${CC} -c ${COMPILE_OPTION} preprocessor.cpp

tables.o : tables.cpp preprocessor.o
	${CC} -c ${COMPILE_OPTION} tables.cpp

evaluate.o : evaluate.cpp tables.o
	${CC} -c ${COMPILE_OPTION} evaluate.cpp

main.o : main.cpp evaluate.o
	${CC} -c ${COMPILE_OPTION} main.cpp
