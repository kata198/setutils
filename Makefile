
CC?=gcc
INCLUDE_FLAGS=`pkg-config glib-2.0 --cflags`
USECFLAGS=${INCLUDE_FLAGS} -Wall -O2 -fPIC ${CFLAGS}
LDFLAGS=-lglib-2.0 -O1 -fPIC

EXECUTABLES=set_intersection \
  set_union \
  set_difference

LIB=libsetutils.so.1.0.0

all: ${EXECUTABLES} ${LIB}

install: ${EXECUTABLES}
	./install.sh ${DESTDIR}

recompile: clean all

${LIB}:
	gcc -shared set.o ${LDFLAGS} -o ${LIB}

set_intersection: set_intersection.o set.o
	gcc set_intersection.o set.o ${LDFLAGS} -o set_intersection

set_union: set_union.o set.o
	gcc set_union.o set.o ${LDFLAGS} -o set_union

set_difference: set_difference.o set.o
	gcc set_difference.o set.o ${LDFLAGS} -o set_difference

clean:
	rm -f ${EXECUTABLES} ${LIB} *.o

set.o: set.c
	gcc set.c -c -o set.o ${USECFLAGS}

main.o: main.c
	gcc main.c -c -o main.o ${USECFLAGS}

set_intersection.o: set_intersection.c
	gcc set_intersection.c -c -o set_intersection.o ${USECFLAGS}

set_union.o: set_union.c
	gcc set_union.c -c -o set_union.o ${USECFLAGS}

set_difference.o: set_difference.c
	gcc set_difference.c -c -o set_difference.o ${USECFLAGS}
