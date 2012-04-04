#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

Lumi *lumi;

void push(int *argc, char ***argv, char *arg, int *cur){
    arg[*cur] = '\0';
    *cur = 0;
    *argv = (char**) realloc(*argv, sizeof(char*) * (*argc+1));
    (*argv)[(*argc)++] = strdup(arg);
}

void exec(const char *c){
    int argc = 0;
    char **argv = 0;
    char *arg = (char*) malloc(strlen(c)+1);
    int cur = 0;
    char quote = 0, escape = 0;
    for(; *c != '\0' && *c != '\n'; c++){
        if(escape){ escape = 0; arg[cur++] = *c; }
        else if(*c == '\\') escape = *c;
        else if(*c == quote){ quote = 0; push(&argc, &argv, arg, &cur); }
        else if(quote) arg[cur++] = *c;
        else if(*c == '#') break;
        else if(*c == '"') quote = *c;
        else if(*c == ' ' && cur) push(&argc, &argv, arg, &cur);
        else if(*c != ' ') arg[cur++] = *c;
    }
    if(cur) push(&argc, &argv, arg, &cur);
    free(arg);
    if(!argv) return;
    lumi_execv(argc, argv);
    int i = 0;
    for(; i<argc; i++)
        free(argv[i]);
    free(argv);
}

void exec_many(int argc, char **argv){
    int i = 1;
    for(; i<argc; i++)
        exec(argv[i]);
}

void exec_file(const char *file){
    FILE *f = fopen(file, "r");
    if(!f) return;
    char *line = 0;
    size_t n = 0;
    while(getline(&line, &n, f) != -1)
        exec(line);
    free(line);
    fclose(f);
}

void exec_files(int argc, char **argv){
    int i = 1;
    for(; i<argc; i++)
        exec_file(argv[i]);
}

Command commands[] = {
    { "exec", exec_many },
    { "run", exec_files },
    { 0 } };

void init(){
    exec_file("config");
}
