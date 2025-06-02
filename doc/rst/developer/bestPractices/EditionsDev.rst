.. _best-practices-editions:

========
Editions
========

The implementation of editions is currently in progress.  Here are some
supported ways to tie a change to a particular edition.

----------------------
The @edition Attribute
----------------------

For breaking changes that can exist entirely in module code, the ``@edition``
attribute can be used.

The ``@edition`` attribute requires one or both of the following arguments:

- first

  - To specify the earliest edition that will include this symbol.  When not
    specified, will be the earliest supported edition.

- last

  - To specify the final edition that will include this symbol.  When not
    specified, will be ``pre-edition``.

When using the attribute, the old behavior should be marked with
``@edition(last="<most recent edition name>")``, while the new behavior should
be marked with ``@edition(first="pre-edition")``.  This will ensure that the
old behavior remains available with previous editions, while the new behavior
becomes available in the ``pre-edition``.  E.g.,

.. literalinclude:: ../../../../test/edition/basics/attributeCheck.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE

.. note::

   Though ``default`` is a supported value for the ``--edition`` compilation
   flag, it is not supported for either the ``first`` or ``last`` argument of
   the edition attribute.  These arguments must be concrete, specific editions.

