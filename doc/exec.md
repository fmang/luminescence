% [Luminescence]'s shell
% Frédéric Mangano
% April 23, 2012

[luminescence]: index.html

Introduction
------------

This document describes the `exec` plugin included in Luminescence 1.0.1 (which haven't changed since 1.0).
You can find that version of the file in the [Git][exec-git].

[exec-git]: http://git.mg0.fr/luminescence/tree/plugins/0-exec.c?id=2b40fbc2c8e3238b308dfad05733bee5b9af10f4).

By default, Luminescence doesn't have any interpreter, only a `lumi_exec` function (actually it's a macro) callable from C.

Playing around
--------------

You'd better install the plugin first. Once you've found the source file, run:

    luminescence-install 0-exec.c

You'd better install the `echo` plugin too if you want to try the examples.

Now that the plugins are install, run:

    luminescence --shell

Don't wait for a prompt (it doesn't display any) and type:

    echo "Hello world!"

Syntax
------

It's like (really) barebone bash:

    # This is a comment
    echo arg1 arg2 # This is a comment too

    echo "arg1, still arg1" arg2
    echo "\"Yo dawg, I heard you like quotes\"" \# this is NOT a comment

Also, the redundant spaces between arguments are ignored (like in bash).

And that's it!

Commands
--------

### The `shell` command

Run it and it will read `stdin`.

Nothing special, really.

### The `exec` command

This command interprets inline scripts. Have some examples:

    luminescence --exec "echo hello" "echo world"
    luminescence --exec "$(cat script-file)"

### The `run` command

The better way to execute a script file is to use the `run` command.

    luminesce --run ~/script

The paths are relative to Luminescence's directory (usually `~/.luminescence`).
That also means the working directory is ignored. In case of doubts, you'd better use absolute paths.

By default, the `exec` plugin interprets the `config` file on startup.
