#include <luminescence.h>
#include <string.h>

const char *name = "Page Cache";

Lumi *lumi;

void enable_page_cache(int argc, char **argv){
    gboolean enable;
    if(argc <= 1) enable = TRUE;
    else if(strcasecmp("on", argv[1]) == 0) enable = TRUE;
    else if(strcasecmp("off", argv[1]) == 0) enable = FALSE;
    else return;
    g_object_set(webkit_web_view_get_settings(
        WEBKIT_WEB_VIEW(lumi->web_view)),
        "enable-page-cache", enable, NULL);
}

Command commands[] = {
    { "page-cache", enable_page_cache },
    { 0 } };
