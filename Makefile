SHELL = /bin/sh

CC = gcc
CFLAGS = -I.
OBJS =  main.o json.o

CJson:${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@  ${LIBS}

clean:
	rm -rf bin/ obj/


R:clean
Rd:CFLAGS += -O3
R:CJson

Rd:clean
Rd:CFLAGS += -DDebug -g
Rd:CJson
