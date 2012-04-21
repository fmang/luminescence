#include <luminescence.h>

const char *name = "Echo";
const char *description = "bash-like echo command.";

void echo(int argc, char **argv){
    int i = 1;
    for(; i<argc; i++){
        if(i>1) putchar(' ');
        fputs(argv[i], stdout);
    }
    putchar('\n');
}

Command commands[] = {
    { "echo", echo },
    { 0 } };
