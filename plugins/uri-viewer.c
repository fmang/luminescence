#include <luminescence.h>

const char *name = "URI viewer";

Lumi *lumi;
GtkWidget *uri_label;

void update_uri(GtkWidget *view){
    if(webkit_web_view_get_load_status(WEBKIT_WEB_VIEW(view)) == WEBKIT_LOAD_COMMITTED)
        gtk_label_set_text(GTK_LABEL(uri_label), webkit_web_view_get_uri(WEBKIT_WEB_VIEW(view)));
}

void link_hovered(WebKitWebView *view, gchar *title, gchar *uri){
    gtk_label_set_text(GTK_LABEL(uri_label), uri ? uri : webkit_web_view_get_uri(view));
}

void yank(){
    gtk_clipboard_set_text(gtk_clipboard_get(GDK_SELECTION_PRIMARY), gtk_label_get_text(GTK_LABEL(uri_label)), -1);
}

void show(){ gtk_widget_show(uri_label); }
void hide(){ gtk_widget_hide(uri_label); }

Command commands[] = {
    { "uri-show", show },
    { "uri-hide", hide },
    { "uri-yank", yank },
    { "leave", show },
    { 0 } };

void init(){
    uri_label = gtk_label_new("");
    gtk_label_set_selectable(GTK_LABEL(uri_label), TRUE);
    gtk_misc_set_alignment(GTK_MISC(uri_label), 0, 0.5);
    gtk_misc_set_padding(GTK_MISC(uri_label), 2, 2);
    g_signal_connect(lumi->web_view, "notify::load-status", G_CALLBACK(update_uri), NULL);
    g_signal_connect(lumi->web_view, "hovering-over-link", G_CALLBACK(link_hovered), NULL);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), uri_label, TRUE, TRUE, 0);
    show();
}
