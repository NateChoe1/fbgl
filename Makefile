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
	@cp build/libfbgl.a $(LIBDIR)/libfbgl.a
	@cp src/fbgl.h $(HEADERDIR)/fbgl.h

uninstall: $(LIBDIR)/libfbgl.a $(HEADERDIR)/fbgl.h
	@rm $(LIBDIR)/libfbgl.a
	@rm $(HEADERDIR)/fbgl.h

clean: build/libfbgl.a
	@rm build/libfbgl.a
