#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

void (*exec)(const char*, const char*);
void* (*bind_key)(guint, guint, const char*, const char*);

void run(const char *line, int is_keys){
    const char *cur = 0;
    size_t count;
    char *command;
    guint key;
    for(cur = line; *cur == ' '; cur++); // skip spaces
    if(*cur == '\0') return; // null
    if(*cur == '\n' || *cur == '#') return; // comment or empty
    if(is_keys){
        if(cur[1] != ' ') return; // only one-letter keys are allowed
        key = *cur;
        cur++;
        while(*cur == ' ') cur++; // skip spaces
    }
    if(*cur == '\0') return;
    for(count = 0; cur[count] != '\0' && cur[count] != ' '; count++);
    command = strndup(cur, count);
    cur += count;
    while(*cur == ' ') cur++; // skip spaces
    if(is_keys)
        (*bind_key)(0, key, command, *cur == '\0' ? 0 : strdup(cur));
    else{
        (*exec)(command, *cur == '\0' ? 0 : cur);
        free(command);
    }
}

void interpret(const char *file, bool is_bind){
    FILE *f = fopen(file, "r");
    char *line = 0, *lf;
    size_t n = 0;
    if(!f) return;
    while(getline(&line, &n, f) != -1){
        lf = strchr(line, '\n');
        if(lf) *lf = '\0';
        run(line, is_bind);
    }
    free(line);
    fclose(f);
}

void read_file(const char *f){ interpret(f, 0); }
void read_keys(const char *f){ interpret(f, 1); }

Command commands[] = {
    { "run", read_file },
    { "keys", read_keys },
    { 0 } };

void init(Lumi *lumi){
    exec = lumi->exec;
    bind_key = lumi->bind;
    read_file("config");
    read_keys("keys");
}
