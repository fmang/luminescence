#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

Lumi *lumi;
char *active_uri = 0;

void link_hovered(WebKitWebView *view, gchar *title, gchar *uri){
    if(active_uri) free(active_uri);
    active_uri = uri ? strdup(uri) : 0;
}

void paste(){
    char *txt = gtk_clipboard_wait_for_text(gtk_clipboard_get(GDK_SELECTION_PRIMARY));
    if(!txt) return;
    lumi_exec("uri-edit", txt, 0);
    free(txt);
}

void yank(){
    gtk_clipboard_set_text(gtk_clipboard_get(GDK_SELECTION_PRIMARY),
        active_uri ? active_uri : webkit_web_view_get_uri(WEBKIT_WEB_VIEW(lumi->web_view)), -1);
}

Command commands[] = {
    { "uri-paste", paste },
    { "uri-yank", yank },
    { 0 } };

void init(){
    g_signal_connect(lumi->web_view, "hovering-over-link", G_CALLBACK(link_hovered), NULL);
}
