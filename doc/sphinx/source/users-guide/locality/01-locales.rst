:title: Users Guide: Definition of Locales

Definition of Locales
=====================

In Chapel, a "locale" is a unit of the target architecture that is
useful for reasoning about locality.  Two things within a single
locale are close to one another in the system, and therefore can
interact with each other reasonably cheaply.  When two things are in
distinct locales, they can still interact with one another, simply at
greater expense.  For example, if a task running on a specific locale
can access a variable stored within its own locale more cheaply than
it can access a variable on a distinct ("remote") locale.

For most target architectures, a locale can be equated with a compute
node.  A program that is executing using 16 compute nodes will be
thought of as executing on 16 locales.

Locales have the ability to run tasks and to store variables.  This
means that a locale must contain processors (or have processors on
which it can schedule tasks by proxy) and memory (or have mmeory in
which it can store variables by proxy).

When executing a Chapel program, the number of locales on which the
program should execute can be specified using the "--numLocales" or
"-nl" flags.  For example, the following command-line would launch the
given Chapel program on four locales:

    ./a.out -nl 4

The Chapel compiler supports distinct compilation modes for creating
executables that can only run on a single locale (shared-memory
execution) vs. those that can be run on multiple locales
(distributed-memory execution).  A program compiled in the former mode
need not specify its number of locales since it can only use one.
Moreover, it will generate an error message if a user attempts to run
it on more than one locale.

Locales may themselves contain locales ("sub-locales"), but this
capability is not yet widely deployed so will not be covered in this
draft of the user's guide.
