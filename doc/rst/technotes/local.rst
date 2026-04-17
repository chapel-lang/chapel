.. index::
    single: local
    pair: keywords; local

.. _readme-local:

===================
The 'local' keyword
===================

The 'local' Statement
=====================

This README describes the ``local`` statement in the Chapel language.
Its definition and implementation is an area of ongoing work and it
should be used with caution.

Overview
--------

The ``local`` construct in Chapel performs runtime checks for any
communication within the construct. If communication occurs, an error
is reported. The checks are performed in the code within the lexical
scope of the construct, as well as in all function calls performed by
that code, directly or indirectly, explicitly or implicitly. The
checks can be disabled with the ``--no-local-checks`` flag, which is implied
by the ``--no-checks`` and ``--fast`` flags.

Communication occurs in the following cases:

* remote memory (i.e. data not located on the current locale)
  is referenced (read from or assigned to), or

* an ``on`` statement attempts to execute on a remote locale.

The ``local`` construct is useful to establish that certain code is
communication free. This may be desired, for example, when tuning
the performance of a program, as communication usually slows down
execution.

The ``local`` construct does not necessarily indicate the cause of
communication when present. See the :chpl:mod:`CommDiagnostics` module for ways to
diagnose communication.



Syntax
------

The ``local`` construct is a statement. It consists of the ``local`` keyword
followed by a do statement or block:

.. code-block:: none

    local-statement:
        "local" [condition] do statement
        "local" [condition] block-statement


Examples
--------

Here is an example of a ``local`` statement:

.. literalinclude:: ../../../test/technotes/doc-examples/LocalStatementExamples.chpl
   :language: chapel
   :start-after: START_EXAMPLE_0
   :end-before: STOP_EXAMPLE_0
   :dedent:

The inner statement is often a block, commonly referred to as a
"local block":

.. literalinclude:: ../../../test/technotes/doc-examples/LocalStatementExamples.chpl
   :language: chapel
   :start-after: START_EXAMPLE_1
   :end-before: STOP_EXAMPLE_1
   :dedent:

In the above examples, the Chapel implementation checks whether ``x``,
as well as all memory referenced during the calls of ``A.this(5)``
(an implicit call for ``A(5)``), ``initializeMyData()``, and ``compute()``,
are located on the current locale. Otherwise an error is reported.
Analogously, if ``on`` statement(s) are executed during these calls
that attempt to execute on a different locale, an error is reported.

Conditional `local`
-------------------

The ``local`` statement behavior can be controlled via the optional
conditional expression.

.. literalinclude:: ../../../test/technotes/doc-examples/LocalStatementExamples.chpl
   :language: chapel
   :start-after: START_EXAMPLE_2
   :end-before: STOP_EXAMPLE_2
   :dedent:

The above example will be localized only if ``data`` resides in the
current locale. The code for the above example is identical to:

.. literalinclude:: ../../../test/technotes/doc-examples/LocalStatementExamples.chpl
   :language: chapel
   :start-after: START_EXAMPLE_3
   :end-before: STOP_EXAMPLE_3
   :dedent:

This implies that local statements in outer dynamic/static scopes will
override the inner ones. i.e. if ``data.locale == here`` evaluates to
``true``, localized bodies of ``initializeMyData`` and ``compute`` will be
used whether they have any ``local`` statement, or not. (This includes
``local false`` blocks).

The ``local do on`` statement
-----------------------------

The ``local do on`` construct behaves similarly to a normal ``on``
statement, but it will halt if the ``on`` statement migrates
execution away from the current locale.

It is otherwise unrelated to ``local`` statements or ``local`` blocks,
and it has no impact on what communication is or is not allowed within
the ``on`` block itself.

For example, the following program would produce a runtime error when
the ``on`` statement is hit if executed with any number of locales
greater than ``1``:

.. literalinclude:: ../../../test/technotes/doc-examples/LocalStatementHalt.chpl
   :language: chapel
   :start-after: START_EXAMPLE_0
   :end-before: STOP_EXAMPLE_0
   :dedent:

Output when run with ``-nl 2``:

.. literalinclude:: ../../../test/technotes/doc-examples/LocalStatementHalt.good

Enabling and disabling runtime checks
-------------------------------------

Runtime checks will be inserted when the ``--local-checks`` flag is enabled.
The flag ``--local-checks`` is enabled by default and can be disabled with
``--no-local-checks``, ``--no-checks``, or ``--fast``.
