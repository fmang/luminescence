#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

const char *name = "URI Loader";

const char *description = "Loads an URI when started.";

GtkWidget *web_view = 0;

char *uri = 0;

void set_uri(const char *new_uri){
    if(web_view)
        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(web_view), new_uri);
    else{
        if(uri) free(uri);
        uri = strdup(new_uri);
    }
}

void new_window(WebKitWebView *view){
    webkit_web_view_stop_loading(view);
    const char *uri = webkit_web_view_get_uri(view);
    if(fork() == 0)
        execlp("luminescence", "luminescence", "--uri", uri, NULL);
    else
        gtk_widget_destroy(GTK_WIDGET(view));
}

WebKitWebView* create_web_view(){
    WebKitWebView* new_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    g_signal_connect(new_view, "notify::uri", G_CALLBACK(new_window), NULL);
    return new_view;
}

Option options[] = {
    { "uri", set_uri },
    { 0 }};

void init(Lumi *lumi){
    web_view = lumi->web_view;
    g_signal_connect(web_view, "create-web-view", G_CALLBACK(create_web_view), NULL);
    if(uri){
        set_uri(uri);
        free(uri);
    }
}
