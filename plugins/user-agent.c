#include <luminescence.h>

const char *name = "User Agent";

Lumi *lumi;

void set_user_agent(const char *agent){
    g_object_set(webkit_web_view_get_settings(WEBKIT_WEB_VIEW(lumi->web_view)), "user-agent", agent, NULL);
}

Command commands[] = {
    { "user-agent", set_user_agent },
    { 0 } };
