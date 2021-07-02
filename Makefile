CFLAGS:=-O3
LIBDIR=/usr/lib64
HEADERDIR=/usr/include

all: fbgl link

fbgl: src/fbgl.c
	$(CC) src/fbgl.c -c -o work/fbgl.o $(CFLAGS)

OBJECTS=work/fbgl.o
link: $(OBJECTS)
	$(AR) rcs build/libfbgl.a $(OBJECTS)

install: build/libfbgl.a src/fbgl.h
	@cp build/libfbgl.a $(LIBDIR)
	@cp src/fbgl.h $(HEADERDIR)
