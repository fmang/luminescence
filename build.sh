#!/bin/bash
gcc -g -Wall -o luminescence luminescence.c -ldl `pkg-config --cflags --libs gtk+-3.0 webkitgtk-3.0`
cd plugins
for f in *.c ; do
    gcc -g -Wall -shared -o "$(basename "$f" .c).so" "$f" -fPIC `pkg-config --cflags --libs gtk+-3.0 webkitgtk-3.0`
done
