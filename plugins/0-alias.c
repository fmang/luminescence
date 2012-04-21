#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

const char *name = "Alias";

Lumi *lumi;

typedef struct {
    char *command;
    int argc;
    char **argv;
    int pass;
} Alias;

Alias *aliases = 0;
int alias_count = 0;

void add_alias(int argc, char **argv, int pass){
    if(argc < 3) return;
    aliases = (Alias*) realloc(aliases, sizeof(Alias) * (alias_count+1));
    Alias *alias = aliases + alias_count++;
    alias->command = strdup(argv[1]);
    alias->argc = argc-2;
    alias->argv = (char**) malloc(sizeof(char*) * (argc-2));
    int i = 0;
    for(; i<argc-2; i++)
        alias->argv[i] = strdup(argv[i+2]);
    alias->pass = pass;
}

void run_on(int argc, char **argv){ add_alias(argc, argv, 0); }
void set_alias(int argc, char **argv){ add_alias(argc, argv, 1); }

void lookup_and_run(int argc, char **argv){
    int i = 0;
    for(; i<alias_count; i++){
        if(strcmp(argv[0], aliases[i].command) == 0){
            Alias *a = aliases+i;
            if(a->pass){
                int _argc = a->argc + argc-1;
                char **_argv = malloc(sizeof(char*) * _argc + 1);
                memcpy(_argv, a->argv, sizeof(char*) * a->argc);
                memcpy(_argv+a->argc, argv+1, sizeof(char*) * argc-1);
                lumi_execv(_argc, _argv);
                free(_argv);
            }
            else
                lumi_execv(aliases[i].argc, aliases[i].argv);
        }
    }
}

Command commands[] = {
    { "on", run_on },
    { "alias", set_alias },
    { "*", lookup_and_run },
    { 0 } };
