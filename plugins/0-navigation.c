#include <luminescence.h>

const char *name = "Navigation";

Lumi *lumi;

void go_back(){ webkit_web_view_go_back(WEBKIT_WEB_VIEW(lumi->web_view)); }
void go_forward(){ webkit_web_view_go_forward(WEBKIT_WEB_VIEW(lumi->web_view)); }

Command commands[] = {
    { "back", go_back },
    { "forward", go_forward },
    { 0 } };
