#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *web_view;

GtkWidget *search_label;
GtkWidget *search_entry;

const char *search_url = 0;

void hide(){
    gtk_widget_hide(search_label);
    gtk_widget_hide(search_entry);
    search_url = 0;
    gtk_entry_set_text(GTK_ENTRY(search_entry), "");
}

void setup(const char *name, const char *url){
    search_url = url;
    gtk_label_set_text(GTK_LABEL(search_label), name);
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
#define ENGINE(K, E, U) case (K): setup((E),(U)); return FOCUS_GRAB | EVENT_STOP;
    switch(e->keyval){
        ENGINE(GDK_KEY_g, "Google", "http://google.com/search?hl=en&q=")
        ENGINE(GDK_KEY_G, "Lucky?", "http://google.com/search?hl=en&btnI=&q=")
        ENGINE(GDK_KEY_w, "Wikipedia", "http://en.wikipedia.org/wiki/")
        ENGINE(GDK_KEY_W, "Wiktionary", "http://en.wiktionary.org/wiki/")
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
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), search_entry, FALSE, FALSE, 0);
}
