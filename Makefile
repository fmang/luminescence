DESTDIR=/usr/local
CFLAGS=-g -Wall

all: luminescence

luminescence:
	gcc -o luminescence luminescence.c $(CFLAGS) -ldl `pkg-config --cflags --libs gtk+-3.0 webkitgtk-3.0`

clean:
	rm -f luminescence

install: luminescence
	mkdir -p $(DESTDIR)/{bin,include,src/luminescence/plugins}
	install -m 755 luminescence lumi-install -t $(DESTDIR)/bin
	install -m 644 luminescence.h -t $(DESTDIR)/include
	install -m 644 plugins/*.c -t $(DESTDIR)/src/luminescence/plugins
