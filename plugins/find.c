#include <luminescence.h>

const char *name = "Find";

const char *description = "Press / to search for text.";

GtkWidget *web_view;
GtkWidget *label;
GtkWidget *entry;

void find(){
    webkit_web_view_search_text(WEBKIT_WEB_VIEW(web_view), gtk_entry_get_text(GTK_ENTRY(entry)), FALSE, TRUE, TRUE);
}

int key_callback(GdkEventKey *e){
    if(e->keyval == GDK_KEY_Escape){
        gtk_widget_hide(label);
        gtk_widget_hide(entry);
        return FOCUS_RELEASE | EVENT_PROPAGATE;
    }
    else if(gtk_widget_get_visible(entry))
        return FOCUS_GRAB | EVENT_PROPAGATE;
    else if(e->keyval == GDK_KEY_slash){
        gtk_entry_set_text(GTK_ENTRY(entry), "");
        gtk_widget_show(label);
        gtk_widget_show(entry);
        gtk_widget_grab_focus(entry);
        return FOCUS_GRAB | EVENT_STOP;
    }
    return EVENT_PROPAGATE;
}

void init(Lumi *lumi){
    web_view = lumi->web_view;
    label = gtk_label_new("Find");
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), label, FALSE, FALSE, 2);
    entry = gtk_entry_new();
    gtk_entry_set_has_frame(GTK_ENTRY(entry), FALSE);
    g_signal_connect(entry, "activate", G_CALLBACK(find), NULL);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), entry, TRUE, TRUE, 0);
}
