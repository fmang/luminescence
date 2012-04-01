#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

Lumi *lumi;

void **bindings = 0;
int binding_count;

void run(char *line, int is_keys){
    char *lf = strchr(line, '\n');
    if(lf) *lf = '\0';
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
    if(is_keys){
        bindings = (void**) realloc(bindings, sizeof(void*) * (binding_count+1));
        bindings[binding_count++] = lumi_bind(key, command, *cur == '\0' ? 0 : strdup(cur));
    }
    else{
        lumi_exec(command, *cur == '\0' ? 0 : cur);
        free(command);
    }
}

void interpret(const char *file, bool is_bind){
    FILE *f = fopen(file, "r");
    if(!f) return;
    char *line = 0;
    size_t n = 0;
    while(getline(&line, &n, f) != -1)
        run(line, is_bind);
    free(line);
    fclose(f);
}

void exec(const char *c){
    char *line = strdup(c);
    run(line, 0);
    free(line);
}

void read_file(const char *f){ interpret(f, 0); }
void read_keys(const char *f){ interpret(f, 1); }

void unbind_keys(){
    int i = 0;
    for(; i<binding_count; i++)
        lumi_unbind(bindings[i]);
    free(bindings);
    bindings = 0;
    binding_count = 0;
}

Command commands[] = {
    { "exec", exec },
    { "run", read_file },
    { "key-bindings", read_keys },
    { "clear-bindings", unbind_keys },
    { 0 } };

void init(){
    read_file("config");
}
