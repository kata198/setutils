
CC?=gcc
INCLUDE_FLAGS=`pkg-config glib-2.0 --cflags`
USECFLAGS=${INCLUDE_FLAGS} -Wall -fPIC -O3 -std=gnu99 -pedantic ${CFLAGS}
USELDFLAGS=-lglib-2.0 -Wl,-O1 -fPIC -s ${LDFLAGS}

EXECUTABLES=set_intersection \
  iset_intersection \
  set_union \
  iset_union \
  set_difference \
  iset_difference

LIB=libsetutils.so.1.0.0

LIB_OBJECTS=set.o str_set.o int_set.o


all: ${EXECUTABLES} ${LIB}

install: ${EXECUTABLES}
	./install.sh ${DESTDIR}

recompile: clean all

${LIB}: ${LIB_OBJECTS}
	gcc -shared ${LIB_OBJECTS} ${USELDFLAGS} -o ${LIB}

set_intersection: set_intersection.o ${LIB_OBJECTS}
	gcc set_intersection.o ${LIB_OBJECTS} ${USELDFLAGS} -o set_intersection

iset_intersection: iset_intersection.o ${LIB_OBJECTS}
	gcc iset_intersection.o ${LIB_OBJECTS} ${USELDFLAGS} -o iset_intersection

set_union: set_union.o ${LIB_OBJECTS}
	gcc set_union.o ${LIB_OBJECTS} ${USELDFLAGS} -o set_union

iset_union: iset_union.o ${LIB_OBJECTS}
	gcc iset_union.o ${LIB_OBJECTS} ${USELDFLAGS} -o iset_union

set_difference: set_difference.o set.o str_set.o
	gcc set_difference.o ${LIB_OBJECTS} ${USELDFLAGS} -o set_difference

iset_difference: iset_difference.o set.o int_set.o
	gcc iset_difference.o ${LIB_OBJECTS} ${USELDFLAGS} -o iset_difference

clean:
	rm -f ${EXECUTABLES} ${LIB} *.o

set.o: set.c set.h
	gcc set.c -c -o set.o ${USECFLAGS}

str_set.o: str_set.c set.h
	gcc str_set.c -c -o str_set.o ${USECFLAGS}

int_set.o: int_set.c set.h
	gcc int_set.c -c -o int_set.o ${USECFLAGS}


main.o: main.c
	gcc main.c -c -o main.o ${USECFLAGS}

set_intersection.o: set_intersection.c
	gcc set_intersection.c -c -o set_intersection.o ${USECFLAGS}

iset_intersection.o: iset_intersection.c
	gcc iset_intersection.c -c -o iset_intersection.o ${USECFLAGS}

set_union.o: set_union.c
	gcc set_union.c -c -o set_union.o ${USECFLAGS}

iset_union.o: iset_union.c
	gcc iset_union.c -c -o iset_union.o ${USECFLAGS}

set_difference.o: set_difference.c
	gcc set_difference.c -c -o set_difference.o ${USECFLAGS}

iset_difference.o: iset_difference.c
	gcc iset_difference.c -c -o iset_difference.o ${USECFLAGS}
