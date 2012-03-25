#include <luminescence.h>

GtkWidget *web_view;

void set_user_agent(const char *agent){
    g_object_set(webkit_web_view_get_settings(WEBKIT_WEB_VIEW(web_view)), "user-agent", agent, NULL);
}

Option options[] = {
    { "user-agent", REQUIRED_ARGUMENT, set_user_agent },
    { 0 } };

void init(Lumi *lumi){
    web_view = lumi->web_view;
}
