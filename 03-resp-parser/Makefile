CC = gcc
CFLAGS = -Wall -g
OBJS = server.o resp_parser.o main.o

all: redis

redis: $(OBJS)
	$(CC) $(CFLAGS) -o redis $(OBJS)

server.o: server.c server.h resp_parser.h
	$(CC) $(CFLAGS) -c server.c

resp_parser.o: resp_parser.c resp_parser.h
	$(CC) $(CFLAGS) -c resp_parser.c

main.o: main.c server.h resp_parser.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o redis