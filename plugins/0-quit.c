#include <luminescence.h>
#include <stdlib.h>

void quit(){
    exit(0);
}

Command commands[] = {
    { "quit", quit },
    { 0 } };
