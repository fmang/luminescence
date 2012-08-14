#include <luminescence.h>
#include <stdlib.h>

const char *name = "Proxy";

const char *description = "Configure a proxy. "
    "Use the http_proxy environment variable by default.";

Lumi *lumi;

void set_proxy(const char *proxy){
    SoupURI *uri = proxy ? soup_uri_new(proxy) : 0;
    g_object_set(webkit_get_default_session(), SOUP_SESSION_PROXY_URI, uri, NULL);
    if(uri) soup_uri_free(uri);
}

void proxy_command(int argc, char **argv){
    set_proxy(argc>=2 ? argv[1] : 0);
}

Command commands[] = {
    { "proxy", proxy_command, "e.g. http://example.foo:8080" },
    { 0 } };

void init(){
    set_proxy(getenv("http_proxy"));
}
