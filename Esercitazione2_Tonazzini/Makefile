CC = g++
CFLAGS = -O3 -Wall -pthread -std=c++11
LFLAGS = -Lrt -pthread -lrt_pthread

all : timing timing.o clean

timing: timing.o
	$(CC) -o $@ $^ $(LFLAGS)

timing.o: timing.cpp
	$(CC) $(CFLAGS) -c timing.cpp

rt/librt_pthread.a:
	cd rt; make

clean:
	rm -f *.o



