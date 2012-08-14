#include "luminescence.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>

Lumi lumi;

/************************************************/
/* Commands                                     */
/************************************************/

Command **commands = 0;
int command_count = 0;

void run_command(int argc, char **argv){
    if(argc < 1) return;
    if(strcmp(argv[0], "*") == 0) return;
    int i = 0;
    for(; i<command_count; i++){
        if(strcmp(commands[i]->name, argv[0]) == 0 || strcmp(commands[i]->name, "*") == 0)
            commands[i]->exec(argc, argv);
    }
}

int ready = 0;
int *delayed_argc = 0;
char ***delayed_argv = 0;
int delayed_count = 0;

void run_command_delayed(int argc, char **argv){
    if(ready){
        run_command(argc, argv);
        return;
    }
    delayed_argc = (int*) realloc(delayed_argc, sizeof(int) * (delayed_count+1));
    delayed_argc[delayed_count] = argc;
    delayed_argv = (char***) realloc(delayed_argv, sizeof(char**) * (delayed_count+1));
    char **args = delayed_argv[delayed_count] = malloc(sizeof(char*) * argc);
    int i = 0;
    for(; i<argc; i++)
        args[i] = strdup(argv[i]);
    delayed_count++;
}

void run_free(int argc, char ***argv){
    run_command(argc, *argv);
    int i = 0;
    for(; i<argc; i++)
        free((*argv)[i]);
    free(*argv);
    *argv = 0;
}

void run_delayed_commands(){
    ready = 1;
    int i = 0;
    for(; i<delayed_count; i++)
        run_free(delayed_argc[i], delayed_argv+i);
    free(delayed_argc); delayed_argc = 0;
    free(delayed_argv); delayed_argv = 0;
    delayed_count = 0;
}

void run_command_va(const char *cmd, ...){
    int argc = 1;
    char **argv = (char**) malloc(sizeof(char*));
    argv[0] = strdup(cmd);
    char *arg;
    va_list vl;
    va_start(vl, cmd);
    while((arg = va_arg(vl, char*))){
        argv = (char**) realloc(argv, sizeof(char*) * (argc+1));
        argv[argc++] = arg;
    }
    va_end(vl);
    run_command_delayed(argc, argv);
    free(argv);
}

/************************************************/
/* Plugins                                      */
/************************************************/

typedef struct {
    void *handle;
    char *filename;
    const char *name;
    const char *description;
    void (*init)();
    Command *commands;
} Plugin;

Plugin *plugins = 0;
int plugin_count = 0;

void load_plugin(const char *filename){
    char path[256];
    strcpy(path, "plugins/");
    strncat(path, filename, 247);
    void *handle = dlopen(path, RTLD_LAZY);
    if(!handle) return;

    plugins = (Plugin*) realloc(plugins, sizeof(Plugin) * (plugin_count+1));
    Plugin *plugin = plugins + plugin_count++;
    plugin->handle = handle;
    plugin->filename = strdup(filename);
    const char **name = dlsym(handle, "name");
    plugin->name = name ? *name : 0;
    const char **description = dlsym(handle, "description");
    plugin->description = description ? *description : 0;
    plugin->init = dlsym(handle, "init");
    Lumi **plugin_lumi = dlsym(handle, "lumi");
    if(plugin_lumi) *plugin_lumi = &lumi;

    // Commands
    plugin->commands = dlsym(handle, "commands");
    if(plugin->commands){
        Command *c = plugin->commands;
        for(; c->name; c++){
            commands = (Command**) realloc(commands, sizeof(Command*) * (command_count+1));
            commands[command_count++] = c;
        }
    }
}

int is_visible(const struct dirent *entry){
    return entry->d_name[0] != '.';
}

void load_plugins(){
    struct dirent **entries;
    int entry_count = scandir("plugins", &entries, is_visible, alphasort);
    if(entry_count < 0) return;
    int i = 0;
    for(; i<entry_count; i++){
        load_plugin(entries[i]->d_name);
        free(entries[i]);
    }
    free(entries);
}

/************************************************/
/* Command Line                                 */
/************************************************/

void parse_arguments(int argc, char **argv){
    int i, eq;
    int _argc = 0;
    char **_argv = 0;
    for(i=1; i<argc; i++){
        if(argv[i][0] == '\0') continue;
        if(argv[i][0] == '-' && argv[i][1] == '-'){
            if(_argv) run_free(_argc, &_argv);
            for(eq=0; argv[i][2+eq] != '=' && argv[i][2+eq] != '\0'; eq++);
            if(argv[i][2+eq] == '='){
                _argc = 2;
                _argv = (char**) malloc(sizeof(char*) * 2);
                _argv[0] = strndup(argv[i]+2, eq);
                _argv[1] = strdup(argv[i]+3+eq);
            }
            else{
                _argc = 1;
                _argv = (char**) malloc(sizeof(char*));
                _argv[0] = strdup(argv[i]+2);
            }
        }
        else if(_argv){
            _argv = (char**) realloc(_argv, sizeof(char*) * (_argc+1));
            _argv[_argc++] = strdup(argv[i]);
        }
    }
    if(_argv) run_free(_argc, &_argv);
}

void print_help(){
    puts("Luminescence version 1.0");
    puts("Usage: luminescence --COMMAND ARG1 ARG2 ...\n");
    if(!plugin_count){
        puts("No plugins.");
        return;
    }
    int i, j, pad;
    Command *c;
    int longest = 0;
    for(i=0; i<command_count; i++){
        if(strlen(commands[i]->name) > longest)
            longest = strlen(commands[i]->name);
    }
    for(i=0; i<plugin_count; i++){
        fputs("* ", stdout);
        if(plugins[i].name)
            printf("%s (%s)\n", plugins[i].name, plugins[i].filename);
        else
            puts(plugins[i].filename);
        if(plugins[i].description)
            printf("  %s\n", plugins[i].description);
        c = plugins[i].commands;
        if(!c) continue;
        puts("  Commands:");
        for(; c->name; c++){
            fputs("    ", stdout);
            fputs(c->name, stdout);
            if(c->help){
                pad = longest - strlen(c->name) + 3;
                for(j=0; j<pad; j++)
                    putchar(' ');
                fputs(c->help, stdout);
            }
            putchar('\n');
        }
    }
}

/************************************************/
/* Main                                         */
/************************************************/

int main(int argc, char **argv){
    char *lumi_dir = strdup(getenv("HOME"));
    lumi_dir = realloc(lumi_dir, strlen(lumi_dir) + 15);
    strcat(lumi_dir, "/.luminescence");
    chdir(lumi_dir);

    lumi.execv = run_command_delayed;
    lumi.execl = run_command_va;
    load_plugins();

    // Help
    if(argc == 2){
        if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
            print_help();
            return 0;
        }
    }

    g_thread_init(NULL);
    gdk_threads_init();
    gdk_threads_enter();
    gtk_init(&argc, &argv);

    // Window
    lumi.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_has_resize_grip(GTK_WINDOW(lumi.window), FALSE);
    g_signal_connect(lumi.window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    // Layout
    GtkWidget *layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(lumi.window), layout);
    gtk_widget_show(layout);

    // Web view
    lumi.web_view = webkit_web_view_new();
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), lumi.web_view);
    gtk_box_pack_start(GTK_BOX(layout), scrolled_window, TRUE, TRUE, 0);
    gtk_widget_show_all(scrolled_window);

    // Status bar
    lumi.status_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(layout), lumi.status_bar, FALSE, FALSE, 0);
    gtk_widget_show(lumi.status_bar);

    // Plugins
    int i;
    for(i=0; i<plugin_count; i++)
        if(plugins[i].init) (*plugins[i].init)();
    run_delayed_commands();
    parse_arguments(argc, argv);

    // Exec
    gtk_widget_show(lumi.window);
    gtk_main();
    gdk_threads_leave();

    return EXIT_SUCCESS;
}
