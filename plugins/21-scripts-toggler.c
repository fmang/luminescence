#include <luminescence.h>
#include <string.h>

const char *name = "Scripts toggler";

Lumi *lumi;
GtkWidget *scripts_label;

void scripts_set_enabled(gboolean enable){
    g_object_set(webkit_web_view_get_settings(WEBKIT_WEB_VIEW(lumi->web_view)),
        "enable-scripts", enable, "enable-plugins", enable, NULL);
    gtk_widget_set_visible(scripts_label, enable);
}

void toggle_scripts(int argc, char **argv){
    if(argc <= 1) scripts_set_enabled(TRUE);
    else if(strcasecmp(argv[1], "on") == 0) scripts_set_enabled(TRUE);
    else if(strcasecmp(argv[1], "off") == 0) scripts_set_enabled(FALSE);
}

Command commands[] = {
    { "scripts", toggle_scripts, "on / OFF" },
    { 0 } };

void init(){
    scripts_label = gtk_label_new("JS");
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), scripts_label, FALSE, FALSE, 3);
    scripts_set_enabled(FALSE);
}
