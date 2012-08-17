DESTDIR=/usr/local
CFLAGS=-g -Wall
LIBDL=$(shell [ -e /lib/libdl.so ] && echo '-ldl' )

all: luminescence

luminescence: luminescence.c luminescence.h
	gcc -o luminescence luminescence.c $(CFLAGS) $(LIBDL) `pkg-config --cflags --libs gtk+-3.0 webkitgtk-3.0 gthread-2.0`

clean:
	rm -f luminescence

install: luminescence
	mkdir -p $(DESTDIR)/{bin,include,src/luminescence/plugins}
	install -m 755 luminescence luminescence-install $(DESTDIR)/bin
	install -m 644 luminescence.h $(DESTDIR)/include
	install -m 644 plugins/*.c $(DESTDIR)/src/luminescence/plugins
