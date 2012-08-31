#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

const char *name = "Key bindings";

Lumi *lumi;

int focused = 0;
void focus(int argc, char **argv){ focused = 1; }
void leave(int argc, char **argv){ focused = 0; }

struct Binding {
    guint key;
    guint modifiers;
    int argc;
    char **argv;
    struct Binding *next;
};

struct Binding *bindings = 0;
struct Binding *last_binding = 0;

void add_binding(guint mods, guint key, int argc, char **argv){
    struct Binding *b = malloc(sizeof(struct Binding));
    b->key = key;
    b->modifiers = mods;
    b->argc = argc;
    b->argv = (char**) malloc(sizeof(char*) * argc);
    int i = 0;
    for(; i<argc; i++)
        b->argv[i] = strdup(argv[i]);
    b->next = 0;
    if(!bindings)
        bindings = last_binding = b;
    else{
        last_binding->next = b;
        last_binding = b;
    }
}

void bind_key(int argc, char **argv){
    if(argc < 3) return;
    if(argv[1][0] == '^'){
        if(argv[1][1] != '\0')
            add_binding(GDK_CONTROL_MASK, argv[1][1], argc-2, argv+2);
    }
    else if(argv[1][0] != '\0')
        add_binding(0, argv[1][0], argc-2, argv+2);
}

#define MOD_MASK ~(GDK_SHIFT_MASK|GDK_MOD2_MASK)

bool on_key_press(GtkWidget *widget, GdkEventKey *event){
    if(event->keyval == GDK_KEY_Escape){
        lumi_exec("leave", 0);
        return FALSE;
    }
    if(focused) return FALSE;
    struct Binding *b = bindings;
    for(; b; b=b->next){
        if(b->key == event->keyval && (b->modifiers&MOD_MASK) == (event->state&MOD_MASK))
            lumi_execv(b->argc, b->argv);
    }
    return TRUE;
}

Command commands[] = {
    { "bind", bind_key },
    { "focus", focus },
    { "leave", leave },
    { 0 } };

void init(){
    gtk_widget_add_events(lumi->window, GDK_KEY_PRESS_MASK);
    g_signal_connect(lumi->window, "key-press-event", G_CALLBACK(on_key_press), NULL);
}
