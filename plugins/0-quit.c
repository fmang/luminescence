#include <luminescence.h>
#include <stdlib.h>

void quit(){
    exit(EXIT_SUCCESS);
}

Command commands[] = {
    { "quit", quit },
    { 0 } };
