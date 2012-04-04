#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

Lumi *lumi;

typedef struct {
    char *command;
    int argc;
    char **argv;
} Alias;

Alias *aliases = 0;
int alias_count = 0;

void run_on(int argc, char **argv){
    if(argc < 3) return;
    aliases = (Alias*) realloc(aliases, sizeof(Alias) * (alias_count+1));
    Alias *alias = aliases + alias_count++;
    alias->command = strdup(argv[1]);
    alias->argc = argc-2;
    alias->argv = (char**) malloc(sizeof(char*) * (argc-2));
    int i = 0;
    for(; i<argc-2; i++)
        alias->argv[i] = strdup(argv[i+2]);
}

void lookup_and_run(int argc, char **argv){
    int i = 0;
    for(; i<alias_count; i++){
        if(strcmp(argv[0], aliases[i].command) == 0)
            lumi_execv(aliases[i].argc, aliases[i].argv);
    }
}

Command commands[] = {
    { "on", run_on },
    { "*", lookup_and_run },
    { 0 } };
