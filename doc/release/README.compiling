===================================
Compiling Using the Chapel Compiler
===================================

If you have set up your environment as described in the top-level
README file and built the compiler as described in README.building,
you should be able to see the Chapel compiler's help message using:

     chpl --help

This displays the compiler's command-line flags sorted by category.
For each flag, the short one-letter form is given (if one exists),
followed by the long form of the flag.  Next, the pattern for any
arguments to the flag are given.  And finally a brief description of
the flag is provided.  A flag may have a corresponding environment
variable, setting which is equivalent to passing the flag.  These
variables are listed by the --help-env option.  The flags' current
settings are listed by --help-settings.

More detailed information about the compiler and its command-line
flags is also available on its man page, which you can read using:

     man chpl


-----------------
Most Useful Flags
-----------------

We note some of the most useful flags here:

     -o <filename> : specify the filename of the generated executable,
                     otherwise ./a.out is used by default

     --no-checks : turns off runtime semantic checks like bounds
                   checking and nil class instance dereferencing

     -O : turns on optimization of the generated C code

     --fast : turns on --no-checks, -O, and enables many other optimizations

     --savec <dir> : saves the generated C code in the specified
                     directory

     -g : support debugging of the generated C code

     --ccflags <flags> : specify flags that should be used when invoking
     --ldflags <flags> :   the back-end C compiler and linker

     -s <name>[=<expression>] : set a config with the given expression
                                as its default value (config params must
                                be set to values that are known at compile
                                time)

     --print-passes : print the compiler passes as they execute

     --print-commands : print the system commands that the compiler executes

     --print-code-size : prints some code size statistics about the
                         number of lexical tokens per line, as well as
                         the number of code, comment, and blank lines

     --version : print the Chapel compiler version number

     --help : print a brief overview of the command-line options

     --help-env : lists the environment variables for each command-line flag

     --help-settings : lists the current setting of each command-line flag
