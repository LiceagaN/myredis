CC = gcc
CFLAGS = -Wall -g
OBJS = main.o server.o ../03-resp-parser/resp_parser.o ../04-db/db.o

myredis: $(OBJS)
	$(CC) $(CFLAGS) -o myredis $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

server.o: server.c server.h ../03-resp-parser/resp_parser.h ../04-db/db.h
	$(CC) $(CFLAGS) -c server.c -o server.o

../03-resp-parser/resp_parser.o:
	$(CC) $(CFLAGS) -c ../03-resp-parser/resp_parser.c -o ../03-resp-parser/resp_parser.o

../04-db/db.o:
	$(CC) $(CFLAGS) -c ../04-db/db.c -o ../04-db/db.o

clean:
	rm -f *.o *.out server.o main.o myredis ../03-resp-parser/*.o ../04-db/*.o
