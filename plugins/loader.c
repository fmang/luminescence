#include <luminescence.h>

const char *name = "URI Loader";

Lumi *lumi;

void set_uri(int argc, char **argv){
    if(argc > 1)
        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(lumi->web_view), argv[1]);
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
    WebKitWebView* view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    g_signal_connect(view, "notify::uri", G_CALLBACK(new_window), NULL);
    return view;
}

Command commands[] = {
    { "uri", set_uri },
    { 0 } };

void init(){
    g_signal_connect(lumi->web_view, "create-web-view", G_CALLBACK(create_web_view), NULL);
}
