#include <luminescence.h>

GtkWidget *web_view;
GtkWidget *scripts_label;

void scripts_set_enabled(gboolean enable){
    g_object_set(webkit_web_view_get_settings(WEBKIT_WEB_VIEW(web_view)),
        "enable-scripts", enable, "enable-plugins", enable, NULL);
    gtk_widget_set_visible(scripts_label, enable);
}

void enable_scripts(){ scripts_set_enabled(TRUE); }
void disable_scripts(){ scripts_set_enabled(FALSE); }

Option options[] = {
    { "scripts", NO_ARGUMENT, enable_scripts, "Enable scripts by default." },
    { "no-scripts", NO_ARGUMENT, disable_scripts, "Disable scripts (default)." },
    { 0 } };

int key_callback(GdkEventKey *e){
    switch(e->keyval){
        case GDK_KEY_s: enable_scripts();  return EVENT_STOP;
        case GDK_KEY_S: disable_scripts(); return EVENT_STOP;
    }
    return EVENT_PROPAGATE;
}

void init(Lumi *l){
    web_view = l->web_view;
    scripts_label = gtk_label_new("JS");
    gtk_box_pack_start(GTK_BOX(l->status_bar), scripts_label, FALSE, FALSE, 3);
    disable_scripts();
}
