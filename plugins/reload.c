#include "../luminescence.h"

GtkWidget *web_view;

int key_callback(GdkEventKey *e){
    switch(e->keyval){
        case GDK_KEY_r:
            webkit_web_view_reload(WEBKIT_WEB_VIEW(web_view));
            return EVENT_STOP;
        case GDK_KEY_R:
            webkit_web_view_reload_bypass_cache(WEBKIT_WEB_VIEW(web_view));
            return EVENT_STOP;
    }
    return EVENT_PROPAGATE;
}

void init(Lumi *lumi){
    web_view = lumi->web_view;
}
