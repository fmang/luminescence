#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

const char *name = "Web Search";

const char *description = "Bind keys to web search engines.";

GtkWidget *web_view;
GtkWidget *search_label;
GtkWidget *search_entry;

typedef struct{
    char key;
    char *name;
    char *url;
} Engine;

Engine *engines = 0;
int engine_count = 0;

const char *search_url = 0;

void add_engine(const char *arg){
    if(!arg) return;
    if(*arg == '\0') return;
    char key = *arg;
    arg++;
    while(*arg == ' ') arg++;
    int comma = 0;
    while(arg[comma] != '\0' && arg[comma] != ',') comma++;
    if(arg[comma] == '\0') return; // bad argument
    engines = (Engine*) realloc(engines, sizeof(Engine) * (engine_count+1));
    Engine *engine = engines + engine_count++;
    engine->key = key;
    engine->name = strndup(arg, comma);
    arg += comma + 1;
    while(*arg == ' ') arg++;
    engine->url = strdup(arg);
}

Option options[] = {
    { "search-engine", add_engine, "e.g. \"f Foo, http://foo/search?q=\"" },
    { 0 }};

void hide(){
    gtk_widget_hide(search_label);
    gtk_widget_hide(search_entry);
    search_url = 0;
    gtk_entry_set_text(GTK_ENTRY(search_entry), "");
}

void setup(Engine *engine){
    search_url = engine->url;
    gtk_label_set_text(GTK_LABEL(search_label), engine->name);
    gtk_widget_show(search_label);
    gtk_widget_show(search_entry);
    gtk_widget_grab_focus(search_entry);
}

void search(){
    const gchar *input = gtk_entry_get_text(GTK_ENTRY(search_entry));
    gchar *uri = (gchar*) malloc(strlen(search_url)+strlen(input)+1);
    strcpy(uri, search_url);
    strcat(uri, input);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(web_view), uri);
    free(uri);
    hide();
}

int key_callback(GdkEventKey *e){
    if(e->keyval == GDK_KEY_Escape){
        hide();
        return FOCUS_RELEASE | EVENT_PROPAGATE;
    }
    else if(search_url)
        return FOCUS_GRAB | EVENT_PROPAGATE;
    int i = 0;
    for(; i<engine_count; i++){
        if(engines[i].key == e->keyval){
            setup(engines + i);
            return FOCUS_GRAB | EVENT_STOP;
        }
    }
    return EVENT_PROPAGATE;
}

void init(Lumi *lumi){
    web_view = lumi->web_view;

    search_label = gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(search_label), 0, 0.5);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), search_label, FALSE, FALSE, 2);

    search_entry = gtk_entry_new();
    gtk_entry_set_has_frame(GTK_ENTRY(search_entry), FALSE);
    g_signal_connect(search_entry, "activate", G_CALLBACK(search), NULL);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), search_entry, TRUE, TRUE, 0);
}
