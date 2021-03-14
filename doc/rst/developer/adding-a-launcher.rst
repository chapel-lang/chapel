Adding a New Launcher
=====================

An Overview
-----------

Minimum files modified/added in writing a new launcher:

- Implementation
    * ``runtime/src/launch/<name>/launch-<name>.c``
- Boiler plate build code
    * ``runtime/src/launch/<name>/Makefile``
    * ``runtime/src/launch/<name>/Makefile.include``
    * ``runtime/src/launch/<name>/Makefile.share``
- Documentation
    * ``doc/rst/usingchapel/launcher.rst``
    * ``doc/rst/usingchapel/multilocale.rst``

There may be other files that require modification depending on the situation.
For example, ``util/chplenv/chpl_launcher.py`` would require modification if
your launcher should be the default under certain conditions, such as the
platform.

Interface
---------

A launcher has 3 entry points that are accessed from ``main_launcher.c``:

.. code-block:: C

    /* Launches the program */
    int chpl_launch(int argc, char* argv[], int32_t numLocales) {
        ...
    }


    /*
       Parses the current argument ``argv[argNum]`` and returns the number of
       argv elements were consumed with argument found. Returns 0 if argument
       is not recognized
    */
    int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                               int32_t lineno, int32_t filename) {
        ...
    }


    /* Prints help output of particular launcher */
    void chpl_launch_print_help(void) {
        ...
    }


See ``$CHPL_HOME/runtime/src/launch/dummy`` for a minimal example of a
launcher. When writing a new launcher, it is advised to copy the ``dummy``
directory's contents, replacing the ``dummy`` name with your new launcher name.
