#include <luminescence.h>

const char *name = "Scroll";

const char *description = "Scroll using the hjkl keys (left, bottom, top, right, respectively).";

GtkWidget *web_view;

int key_callback(GdkEventKey *e){
    GtkAdjustment *adjustment = 0;
    gdouble factor = 1;
    switch(e->keyval){
        case GDK_KEY_k: factor = -1;
        case GDK_KEY_j: adjustment = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(web_view));
            break;
        case GDK_KEY_h: factor = -1;
        case GDK_KEY_l: adjustment = gtk_scrollable_get_hadjustment(GTK_SCROLLABLE(web_view));
            break;
    }
    if(adjustment){
        gtk_adjustment_set_value(adjustment,
            gtk_adjustment_get_value(adjustment) + factor*gtk_adjustment_get_step_increment(adjustment));
        return EVENT_STOP;
    }
    return EVENT_PROPAGATE;
}

void init(Lumi *lumi){
    web_view = lumi->web_view;
}
