:title: Users Guide: Locale Type and Variables

Locale Type and Variables
=========================

The Locale Type
---------------

Locales are represented within Chapel programs using a built-in type
named *locale*.  The locale type supports a fixed number of unique
values equal to the number of locales on which the program is
executing, each value corresponding to one of the compute nodes.  The
locale values support methods that permit the programmer to make
queries about the target architecture's capabilities, such as the
maximum amount of task parallelism supported or the amount of physical
memory.

.. TODO: Fix the following hyperlink.  Ideally, would want it to point directly to the 'class locale'

For details on this interface, refer to :ref:ChapelLocale:class:locale:.


Built-in Locale Variables
=========================

Chapel supports a number of built-in variables that permit a
programmer to refer abstractly to the locales on which the program is
running.  These are as follows:

*numLocales*
------------

The variable *numLocales* is an integer indicating the number of
locales on which the program is running.  For example, given the
4-locale execution shown in :ref:multilocale:, the following program:

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
0-based indexing.


'here'
------
