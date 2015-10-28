:title: Users Guide: Built-in Locale Variables

Built-in Locale Variables
=========================

'numLocales'
------------

All Chapel programs have a handful of built-in variables that permit
the programmer to symbolically refer to the locales on which the
program is running.  The first of these is the 'numLocales' variable,
an integer indicating the number of locales specified by the user on
the command-line.  For example, given the 4-locale execution from the
previous page, the following program:

.. code-include::../code/10-numLocales.chpl
  :lexer: chapel

would generate:

.. code-include::../code/10-numLocales.good


The 'Locales' Array
-------------------

The next two variables are a domain 'LocaleSpace' and array '


'here'
------
