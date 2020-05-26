CC = g++
CFLAGS = -O3 -Wall -pthread -std=c++11
LFLAGS = -Lrt -pthread -lrt_pthread

all : rt/librt_pthread.a monitor_simple

monitor_simple: monitor_simple.o
	${CC} -o monitor_simple monitor_simple.o ${LFLAGS}
	
monitor_simple.o: monitor_simple.cpp
	${CC} ${CFLAGS} -c monitor_simple.cpp

rt/librt_pthread.a:
	cd rt; make

clean:
	rm -f *.o *~ monitor_simple
	cd rt; make clean


