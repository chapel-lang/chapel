:title: Users Guide: The **locale** Type and Variables

.. default-domain:: chpl

The **locale** Type and Variables
=================================

The Chapel language has a locale type and a number of built-in
variables that permit a programmer to refer to, and reason about, the
locales on which the program is running.


The *locale* Type
-----------------

Locales are represented within Chapel programs using a built-in type
named ``locale``.  The locale type supports a fixed number of unique
values equal to the number of locales on which the program is
executing, each value typically corresponding to one of the compute
nodes on which the program is executing.

Locale values support methods that permit a programmer to make queries
about the target architecture's capabilities, such as the maximum
amount of task parallelism supported or the amount of physical memory
available on the locale.  For details on this interface, refer to
:chpl:mod:`ChapelLocale`.

As a simple example, each locale value supports an ``id`` method that
reports its unique (0-based) ID.  We'll see the use of this method in
subsequent program examples.


*numLocales*
------------

The variable *numLocales* is a built-in integer constant indicating
the number of locales on which the program is running.  For example,
when run on four locales, the following program:

.. literalinclude:: examples/users-guide/locality/numLocalesVar.chpl
  :caption:
  :language: chapel

would generate:

.. literalinclude:: examples/users-guide/locality/numLocalesVar.good
  :language: text


The *Locales* Array
-------------------

Chapel programs also have a built-in *numLocales*-element array,
*Locales*, whose elements are the distinct locale values on which the
program is running.  This array is defined over a built-in domain,
*LocaleSpace*.  Both the domain and array are 1-dimensional and use
dense, 0-based indexing.  

Each locale value's ID corresponds to its index in the *Locales*
array.  This is demonstrated by the following program which first
prints the *LocaleSpace* domain and then iterates over it, showing
that the ID of each value in *Locales* corresponds to its index:

.. literalinclude:: examples/users-guide/locality/localesDomArr.chpl
  :caption:
  :language: chapel

Running on four locales, its output is:

.. literalinclude:: examples/users-guide/locality/localesDomArr.good
  :language: text




*here*
------

The final built-in locale variable that we'll cover in this section is
*here*.  For any given task, this variable resolves to the locale
value on which the task is running.

As an example, the following program demonstrates that Chapel programs
begin their execution on locale 0:


.. literalinclude:: examples/users-guide/locality/here-id.chpl
  :caption:
  :language: chapel

Running it on any number of locales generates:

.. literalinclude:: examples/users-guide/locality/here-id.good
  :language: text
