#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

const char *name = "Address Bar";

Lumi *lumi;
GtkWidget *entry;

void go(){
    const gchar *raw_uri = gtk_entry_get_text(GTK_ENTRY(entry));
    gchar *uri = (gchar*) malloc(strlen(raw_uri)+8);
    uri[0] = '\0';
    if(!strstr(raw_uri, "://"))
        strcat(uri, "http://");
    strcat(uri, raw_uri);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(lumi->web_view), uri);
    free(uri);
    lumi_exec("leave", 0);
}

void leave(){
    gtk_widget_hide(entry);
}

void edit(const char *uri){
    gtk_entry_set_text(GTK_ENTRY(entry), uri ? uri : "");
    gtk_widget_show(entry);
    gtk_widget_grab_focus(entry);
    lumi_exec("focus", 0);
}

void prompt(int argc, char **argv){
    edit(argc>1 ? argv[1] : webkit_web_view_get_uri(WEBKIT_WEB_VIEW(lumi->web_view)));
}

void paste(){
    char *txt = gtk_clipboard_wait_for_text(gtk_clipboard_get(GDK_SELECTION_PRIMARY));
    edit(txt);
    if(txt) g_free(txt);
}

Command commands[] = {
    { "uri-edit", prompt },
    { "uri-paste", paste },
    { "leave", leave } };

void init(){
    entry = gtk_entry_new();
    gtk_entry_set_has_frame(GTK_ENTRY(entry), FALSE);
    g_signal_connect(entry, "activate", G_CALLBACK(go), NULL);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), entry, TRUE, TRUE, 0);
}
