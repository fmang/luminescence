#!/bin/bash
gcc -g -Wall -o luminescence luminescence.c `pkg-config --cflags --libs gtk+-3.0`
