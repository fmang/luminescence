#include <luminescence.h>

const char *name = "Insert mode";

Lumi *lumi;
GtkWidget *insert_label;

void leave(const char *arg){
    gtk_widget_hide(insert_label);
}

void insert_mode(const char *arg){
    gtk_widget_show(insert_label);
    lumi_focus();
}

Command commands[] = {
    { "insert-mode", insert_mode },
    { "leave", leave },
    { 0 } };

void init(){
    insert_label = gtk_label_new("INSERT");
    gtk_box_pack_end(GTK_BOX(lumi->status_bar), insert_label, FALSE, FALSE, 3);
}
