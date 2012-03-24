#include "luminescence.h"
#include <stdlib.h>
#include <dlfcn.h>

Lumi lumi;

KeyCallback **key_callbacks;
KeyCallback *key_grabber = 0;
int key_callbacks_size = 0;
int key_callback_count = 0;

bool on_key_press(GtkWidget *widget, GdkEventKey *event){
    int i = 0, code;
    for(; i<key_callback_count; i++){
        code = (*(key_grabber ? key_grabber : key_callbacks[i]))(event);
        if(code & FOCUS_GRAB){
            key_grabber = key_callbacks[i];
            return code & EVENT_STOP ? TRUE : FALSE;
        }
        else if(key_grabber){
            key_grabber = 0;
            i--;
        }
        if(code & EVENT_STOP)
            return TRUE;
    }
    switch(event->keyval){
        case GDK_KEY_r: webkit_web_view_reload(WEBKIT_WEB_VIEW(lumi.web_view)); break;
        case GDK_KEY_R: webkit_web_view_reload_bypass_cache(WEBKIT_WEB_VIEW(lumi.web_view)); break;
    }
    // Unknown key, forget about it.
    return TRUE;
}

void load_plugin(const char *path){
    if(key_callback_count >= key_callbacks_size){
        key_callbacks_size += 8;
        key_callbacks = (KeyCallback**) realloc(key_callbacks, key_callbacks_size*sizeof(KeyCallback*));
    }
    void *plugin = dlopen(path, RTLD_LAZY);
    if(!plugin) return;
    void (*plugin_init)(Lumi*) = dlsym(plugin, "init");
    if(plugin_init)
        (*plugin_init)(&lumi);
    KeyCallback* plugin_key_callback = dlsym(plugin, "key_callback");
    if(plugin_key_callback)
        key_callbacks[key_callback_count++] = plugin_key_callback;
}

int main(int argc, char **argv){
    gtk_init(&argc, &argv);

    // Window

    lumi.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_add_events(lumi.window, GDK_KEY_PRESS_MASK);
    gtk_window_set_has_resize_grip(GTK_WINDOW(lumi.window), FALSE);
    g_signal_connect(lumi.window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(lumi.window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    GtkWidget *layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show(layout);

    // Web view

    lumi.web_view = webkit_web_view_new();

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), lumi.web_view);
    gtk_box_pack_start(GTK_BOX(layout), scrolled_window, TRUE, TRUE, 0);
    gtk_widget_show_all(scrolled_window);

    // Status

    lumi.status_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(layout), lumi.status_bar, FALSE, FALSE, 0);
    gtk_widget_show(lumi.status_bar);

    load_plugin("plugins/address-bar.so");
    load_plugin("plugins/scripts-toggler.so");
    load_plugin("plugins/insert.so");

    // Show

    gtk_container_add(GTK_CONTAINER(lumi.window), layout);
    gtk_widget_show(lumi.window);
    gtk_main();

    return 0;
}
