#include <luminescence.h>

const char *name = "Page Cache";

const char *description = "Enables WebKit's Page Cache.";

gboolean page_cache = TRUE;
GtkWidget *web_view = 0;

void page_cache_set_enabled(gboolean enable){
    page_cache = enable;
    if(!web_view) return;
    g_object_set(webkit_web_view_get_settings(
        WEBKIT_WEB_VIEW(web_view)),
        "enable-page-cache", enable, NULL);
}

void disable_page_cache(){ page_cache_set_enabled(FALSE); }

Option options[] = {
    { "no-page-cache", NO_ARGUMENT, disable_page_cache },
    { 0 } };

void init(Lumi *lumi){
    web_view = lumi->web_view;
    page_cache_set_enabled(page_cache);
}
