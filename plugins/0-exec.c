#include <luminescence.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

Lumi *lumi;

void push(int *argc, char ***argv, char *arg, int *cur){
    arg[*cur] = '\0';
    *cur = 0;
    *argv = (char**) realloc(*argv, sizeof(char*) * (*argc+1));
    (*argv)[(*argc)++] = strdup(arg);
}

void exec(const char *c){
    int argc, cur, i;
    char **argv = 0, *arg = (char*) malloc(strlen(c)+1);
    char quote, escape;
    while(1){
        while(*c == '\n' || *c == ' ') c++;
        if(*c == '\0') break;
        argc = cur = 0;
        quote = escape = 0;
        for(; *c != '\0' && *c != '\n'; c++){
            if(escape){ escape = 0; arg[cur++] = *c; }
            else if(*c == '\\') escape = *c;
            else if(*c == quote){ quote = 0; push(&argc, &argv, arg, &cur); }
            else if(quote) arg[cur++] = *c;
            else if(*c == '#'){ while(*c != '\0' && *c != '\n') c++; }
            else if(*c == '"') quote = *c;
            else if(*c == ' ' && cur) push(&argc, &argv, arg, &cur);
            else if(*c != ' ') arg[cur++] = *c;
        }
        if(cur) push(&argc, &argv, arg, &cur);
        if(!argc) continue;
        lumi_execv(argc, argv);
        for(i=0; i<argc; i++)
            free(argv[i]);
    }
    free(arg);
    if(argv) free(argv);
}

// Wrappers

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

void* shell_f(void *p){
    char *line = 0;
    size_t n = 0;
    while(getline(&line, &n, stdin) != -1){
        gdk_threads_enter();
        exec(line);
        gdk_threads_leave();
    }
    free(line);
    return 0;
}

// Commands

void shell(){
    static int started = 0;
    if(started) return;
    started = 1;
    pthread_t thread;
    pthread_create(&thread, 0, shell_f, 0);
}

Command commands[] = {
    { "exec", exec_many },
    { "run", exec_files },
    { "shell", shell },
    { 0 } };

void init(){
    exec_file("config");
}
