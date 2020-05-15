CC = g++
CFLAGS = -O3 -Wall -pthread -std=c++11
LFLAGS = -Lrt -pthread -lrt_pthread

OUT = rt/librt_pthread.a hello hello-rt timing

all : $(OUT)

hello: hello.o barrier.o
	$(CC) -o $@ $^ $(LFLAGS)
	
hello-rt: hello-rt.o barrier.o
	$(CC) -o $@ $^ $(LFLAGS)

timing: timing.o
	$(CC) -o $@ $^ $(LFLAGS)

hello.o: hello.cpp barrier.h
	$(CC) $(CFLAGS) -c hello.cpp

hello-rt.o: hello-rt.cpp barrier.h
	$(CC) $(CFLAGS) -c hello-rt.cpp

timing.o: timing.cpp
	$(CC) $(CFLAGS) -c timing.cpp

barrier.o: barrier.cpp barrier.h
	$(CC) $(CFLAGS) -c barrier.cpp

rt/librt_pthread.a:
	cd rt; make

clean:
	rm -f *.o *~ $(OUT)
	cd rt; make clean



