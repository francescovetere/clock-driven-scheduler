CC = g++
CFLAGS = -O3 -Wall -pthread -std=c++11
LFLAGS = -pthread

OUT = simple_wrong simple_solved

all : $(OUT)

simple_%: simple_%.o
	$(CC) -o $@ $< $(LFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f *.o *~ $(OUT)



