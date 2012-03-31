#include <luminescence.h>

const char *name = "Find";

Lumi *lumi;
GtkWidget *label;
GtkWidget *entry;

void do_find(){
    gtk_widget_show(label);
    gtk_widget_show(entry);
    gtk_widget_grab_focus(entry);
    lumi_focus();
    webkit_web_view_search_text(WEBKIT_WEB_VIEW(lumi->web_view), gtk_entry_get_text(GTK_ENTRY(entry)), FALSE, TRUE, TRUE);
}

void find(const char *txt){
    if(txt) gtk_entry_set_text(GTK_ENTRY(entry), txt);
    else if(!gtk_widget_get_visible(entry))
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    do_find();
}

void leave(){
    gtk_widget_hide(label);
    gtk_widget_hide(entry);
}

Command commands[] = {
    { "find", find },
    { "leave", leave },
    { 0 } };

void init(){
    label = gtk_label_new("Find");
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), label, FALSE, FALSE, 2);
    entry = gtk_entry_new();
    gtk_entry_set_has_frame(GTK_ENTRY(entry), FALSE);
    g_signal_connect(entry, "activate", G_CALLBACK(do_find), NULL);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), entry, TRUE, TRUE, 0);
}
