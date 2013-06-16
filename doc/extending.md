% Writing plugins for [Luminescence]
% Frédéric Mangano
% April 23, 2012

[Luminescence]: index.html

Note: this guide is written for Luminescence 1.0.1.

Naming conventions
------------------

Since the plugins are loaded one after another, and some of them add widgets in the status bar, the order they're loaded defines the order of the widgets in the status bar.

A simple solution is to prefix each plugin with a number; and to keep some harmony, here are the conventions I used:

* When a plugin doesn't have GUI components, it can be loaded anytime. To identify them, they're prefixed by `0-`.

* Some plugins, like the address bar, can display a field to ask something to the user. Since they fit best in the leftmost part of the status bar, they're prefixed by `1-`. As two plugins shouldn't display their field at the same time, the order in which these plugins are loaded is irrelevant.

* The `2*-` family consist in plugins that display the active mode or some "flags" (e.g. whether scripts are enabled or not). They're usually at the left of the status bar, unless a `1-` plugin takes the control. This time, the order `2*-` plugins are loaded *is* relevant, so the second digit is here to order them.

* The `3*-` plugins are supposed to be right-aligned. Currently only the uri-viewer plugin belongs to this category. That could easily have been a simple placeholder.

* Finally, the `4*-` plugins are similar to the `2*-` plugins, except that they're displayed in the right part of the status bar.

Don't take these too seriously though. These will help if you want to integrate your plugins with the default ones. Otherwise you're free to use whatever names you want.

User Agent plugin
-----------------

See [0-user-agent.c](0-user-agent.c.html) ([plain](0-user-agent.c)).

### Header file

    #include <luminescence.h>

This is the only header for Luminescence. It includes Gtk and WebKit too, so this single #include is often sufficient.

### Meta

*Important: in the following sections, the names aren't given as examples; the variables must be called this way otherwise Luminescence won't be able to find them.*

    const char *name = "User Agent";

This is the name displayed in the help (`luminescence -h`). It's completely optional.

You can also add a `description` the same way. (But please don't when it's obvious.)

### Lumi pointer

    Lumi *lumi;

This pointer is set by Luminescence *before* any function is called (no need to check it).
Sometimes it's not even useful, so don't declare it systematically.

### Commands

    void set_user_agent(int argc, char **argv){ ...

This is the form of command callbacks. The ressemblance with `main`'s prototype is intended.
Note that, like `main`'s `argv`, `argv[0]` is the name of the called command.

Also, though I omitted the `const` keyword, it is *not* recommended to modify the arguments (but maybe a creative mind can do that to "filter" the arguments). They mustn't be freed either.

    Command commands[] = {
        { "user-agent", set_user_agent },
        { 0 } };

Once your callbacks are defined, you can "register" the commands by adding them in the `commands` array.
The `Command` struct currently has 3 fields: the name of the command, the callback, and an optional `help` string that will be displayed in the help.

The `{ 0 }` terminator marks the end of the array. Make *sure* you don't forget it!

Insert Mode plugin
------------------

See [22-insert.c](22-insert.c.html) ([plain](22-insert.c)).

### See above

If you're wondering why the callbacks are `void`, it's because `gcc` doesn't generate warnings when casting the pointer.
Maybe I should look into that someday.

### Initialization

    void init()

This function is called at the startup of Luminescence. You'll usually use it to create the widgets and add them to the status bar (since the `lumi` pointer is already initialized).

### The `focus` and `leave` commands

These two commands are specific to the `bind` plugin. See [its documentation](bind.html#focus) for details.

### Command execution

    void lumi_exec(char*, ...);
    void lumi_execv(int argc, char **argv);

These should be too hard to understand: `lumi_execv` takes a list of arguments and run the `argv[0]` command.
Luminescence will relay these two parameters to the plugins that have implemented it.

`lumi_exec` is provided for convenience. The last "sentinel" argument must be null.

    lumi_exec("focus", 0);

Technical details
-----------------

### Delayed command executions

I can hear you ask:

> Okay, but what if I call a command from `init()`, before the other plugins are initialized?

Easy: Luminescence stores the parameters and actually calls the command once every plugin is initialized.

### The wildcard command

The `*` command is quite special. When a plugin registers that command, the corresponding callback will always be called. (This is where `argv[0]` is useful.)

The `alias` plugin uses it as you cannot register commands dynamically.
That could also be used to make a `history` plugin. (I don't plan on implementing one though.)

More information
----------------

I invite you to read the source files of the plugins (you can also read Luminescence's, it's not even 300 lines long).

There are also [other documents](index.html#documentation).

If you still have questions, feel free to email me at <fmang@mg0.fr>.
