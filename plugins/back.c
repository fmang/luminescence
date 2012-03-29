#include <luminescence.h>

const char *name = "Back";

const char *description = "Press Backspace to go back one page.";

GtkWidget *web_view;

int key_callback(GdkEventKey *e){
    if(e->keyval == GDK_KEY_BackSpace){
        webkit_web_view_go_back(WEBKIT_WEB_VIEW(web_view));
        return EVENT_STOP;
    }
    return EVENT_PROPAGATE;
}

void init(Lumi *lumi){
    web_view = lumi->web_view;
}
