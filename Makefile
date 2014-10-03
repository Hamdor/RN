# Makefile for RNA1
# RN WiSe 2014
# marian.triebe  (at) haw-hamburg.de
# katja.kirstein (at) haw-hamburg.de
LDFLAGS=-lpthread -lopencv_highgui -lopencv_core -lopencv_imgproc
CFLAGS=-Wall -g
INCLUDE_PATH=include
OUTPUTNAME=rna1
SRC_FILES=src/*

all:
	g++ -I ${INCLUDE_PATH} ${CFLAGS} ${SRC_FILES} ${LDFLAGS} -o ${OUTPUTNAME}

clean:
	rm -f ${OUTPUTNAME}
