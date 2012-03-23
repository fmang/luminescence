#!/bin/bash
gcc -g -Wall -o luminescence luminescence.c -ldl `pkg-config --cflags --libs gtk+-3.0 webkitgtk-3.0`
