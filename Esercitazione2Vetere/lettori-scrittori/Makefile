CC = g++
CFLAGS = -O3 -Wall -pthread -std=c++11
LFLAGS = -pthread

all : lett-scritt

lett-scritt: applicazione.o monitor.o
	${CC} -o lett-scritt applicazione.o monitor.o ${LFLAGS}

applicazione.o: applicazione.cpp monitor.h
	${CC} ${CFLAGS} -c applicazione.cpp

monitor.o: monitor.cpp monitor.h
	${CC} ${CFLAGS} -c monitor.cpp

clean :
	rm -f *.o *~ lett-scritt


