SHELL = /bin/sh

CC = gcc
CFLAGS = -I.
OBJS =  main.o json.o

CJson:${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@  ${LIBS}

lib:
	gcc -shared  json.o  -o CjsonLib.so  

clean:
	rm -rf bin/ obj/


R:clean
Rd:CFLAGS += -O3
R:CJson

Rd:clean
Rd:CFLAGS += -DDebug -g
Rd:CJson

install:
	cp CjsonLib.so /usr/local/lib/libCjson.so
	cp json.h /usr/local/include/Cjson.h
	ldconfig