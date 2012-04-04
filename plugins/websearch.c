#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

const char *name = "Web Search";

Lumi *lumi;
GtkWidget *search_label;
GtkWidget *search_entry;

char *search_url = 0;

void setup(int argc, char **argv){
    if(argc < 3) return;
    gtk_label_set_text(GTK_LABEL(search_label), argv[1]);
    if(search_url) free(search_url);
    search_url = strdup(argv[2]);
    gtk_entry_set_text(GTK_ENTRY(search_entry), "");
    gtk_widget_show(search_label);
    gtk_widget_show(search_entry);
    gtk_widget_grab_focus(search_entry);
    lumi_exec("focus", 0);
}

void search(){
    const gchar *input = gtk_entry_get_text(GTK_ENTRY(search_entry));
    gchar *uri = (gchar*) malloc(strlen(search_url)+strlen(input)+1);
    strcpy(uri, search_url);
    strcat(uri, input);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(lumi->web_view), uri);
    free(uri);
    lumi_exec("leave", 0);
}

void hide(){
    gtk_widget_hide(search_label);
    gtk_widget_hide(search_entry);
}

Command commands[] = {
    { "websearch", setup },
    { "leave", hide },
    { 0 } };

void init(){
    search_label = gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(search_label), 0, 0.5);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), search_label, FALSE, FALSE, 2);
    search_entry = gtk_entry_new();
    gtk_entry_set_has_frame(GTK_ENTRY(search_entry), FALSE);
    g_signal_connect(search_entry, "activate", G_CALLBACK(search), NULL);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), search_entry, TRUE, TRUE, 0);
}
