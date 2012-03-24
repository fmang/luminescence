#include <luminescence.h>

GtkWidget *insert_label;

int key_callback(GdkEventKey *e){
    if(e->keyval == GDK_KEY_Escape){
        gtk_widget_hide(insert_label);
        return FOCUS_RELEASE | EVENT_PROPAGATE;
    }
    else if(gtk_widget_get_visible(insert_label))
        return FOCUS_GRAB | EVENT_PROPAGATE; // propagate to WebKit
    else if(e->keyval == GDK_KEY_i){
        gtk_widget_show(insert_label);
        return FOCUS_GRAB | EVENT_STOP;
    }
    return EVENT_PROPAGATE;
}

void init(Lumi *lumi){
    insert_label = gtk_label_new("INSERT");
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), insert_label, FALSE, FALSE, 3);
}
