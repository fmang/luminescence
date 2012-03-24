#include <luminescence.h>
#include <stdlib.h>

void init(Lumi *lumi){
    FILE *f = fopen("user-agent", "r");
    if(!f) return;
    char *line = 0;
    size_t n = 0;
    size_t len = getline(&line, &n, f);
    line[len-1] = '\0'; // drop the newline
    g_object_set(webkit_web_view_get_settings(WEBKIT_WEB_VIEW(lumi->web_view)), "user-agent", line, NULL);
    free(line);
    fclose(f);
}
