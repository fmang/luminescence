#include <luminescence.h>

const char *name = "Page Cache";

Lumi *lumi;

void enable_page_cache(){
    g_object_set(webkit_web_view_get_settings(
        WEBKIT_WEB_VIEW(lumi->web_view)),
        "enable-page-cache", TRUE, NULL);
}

Command commands[] = {
    { "page-cache", enable_page_cache },
    { 0 } };
