CC=g++
CFLAGS=-Os -Wall -W -Wwrite-strings
LDFLAGS=
O_TARGET=hull
SRCS=hull.cpp
OBJS=hull.o

all:	$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(O_TARGET) $(OBJS)
	strip $(O_TARGET)

$(SRCS):
	$(CC) $(CFLAGS) -o $@ $*.c

clean:
	@rm *.o
