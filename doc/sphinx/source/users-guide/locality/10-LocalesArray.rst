:title: Users Guide: Locale Type and Variables

.. default-domain:: chpl

Locale Type and Variables
=========================

The Chapel language has a locale type and a number of built-in
variables that permit a programmer to refer to, and reason about, the
locales on which the program is running.


The *locale* Type
-----------------

Locales are represented within Chapel programs using a built-in type
named ``locale``.  The locale type supports a fixed number of unique
values equal to the number of locales on which the program is
executing, each value corresponding to one of the compute nodes.  

Locale values support methods that permit the programmer to make
queries about the target architecture's capabilities, such as the
maximum amount of task parallelism supported or the amount of physical
memory available on the locale.  They also support queries such as the
locale's unique ID or name.  For details on this interface, refer to
:chpl:class:`ChapelLocale.locale`.

As a simple, example, all locales support an ``id`` method that
reports their unique 0-based ID.  Thus, the following program
demonstrates the property that Chapel programs begin execution on
locale 0:

.. literalinclude:: ../../../../../test/release/examples/guide/locality/09-here-id.chpl
   :language: chapel

Running it generates:

.. literalinclude:: ../../../../../test/release/examples/guide/locality/09-here-id.good
   :language: text

*numLocales*
------------

The variable *numLocales* is a built-in integer constant indicating
the number of locales on which the program is running.  For example,
when run on four locales, the following program:

.. literalinclude:: ../../../../../test/release/examples/guide/locality/10-numLocales.chpl
   :language: chapel

would generate:

.. literalinclude:: ../../../../../test/release/examples/guide/locality/10-numLocales.good
   :language: text


The *Locales* Array
-------------------

Chapel programs also have a built-in array, *Locales* whose
*numLocales* elements are the set of distinct locale values on which
the program is running.  This array is defined over a built-in domain,
*LocaleSpace*.  The domain and array are 1-dimensional and use dense,
0-based indexing.  Thus, a locale's ID is the same as its index in the
*LocaleSpace* domain and *Locales* array.  The following program
demonstrates this:

.. literalinclude:: ../../../../../test/release/examples/guide/locality/12-localesDomArr.chpl
   :language: chapel

Running on four locales, its output is:

.. literalinclude:: ../../../../../test/release/examples/guide/locality/12-localesDomArr.good
   :language: text




'here'
------
