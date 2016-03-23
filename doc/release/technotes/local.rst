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
communication when present. See the `Module\: CommDiagnostics`_ for ways to
diagnose communication.

.. _Module\: CommDiagnostics:    http://chapel.cray.com/docs/latest/modules/standard/CommDiagnostics.html



Syntax
------

The ``local`` construct is a statement. It consists of the ``local`` keyword
followed by a statement:

.. code-block:: none

    local-statement:
        "local" statement


Examples
--------

Here is an example of a ``local`` statement:

.. code-block:: chapel

    local
      x = A(5);

The inner statement is often a block, commonly referred to as a
"local block":

.. code-block:: chapel

    local {
      initializeMyData();
      compute();
    }

In the above examples, the Chapel implementation checks whether ``x``,
as well as all memory referenced during the calls of ``A.this(5)``
(an implicit call for ``A(5)``), initializeMyData(), and compute(),
are located on the current locale. Otherwise an error is reported.
Analogously, if ``on`` statement(s) are executed during these calls
that attempt to execute on a different locale, an error is reported.


The 'local on' Statement
========================


The ``local on`` construct in Chapel performs an on-statement on a sublocale
within the current node. For example:

.. code-block:: chapel

  for i in 0..#here.getChildCount() {
    local on here.getChild(i) {
      writeln("On sublocale ", here);
    }
  }

When the ``--local-checks`` flag is enabled, a runtime check will be inserted
to confirm that the on-statement is performed within the same node.
``--local-checks`` is enabled by default and can be disabled with
``--no-local-checks``, ``--no-checks``, or ``--fast``.

For example this complete program would produce a runtime error if the number
of locales is greater than one:

.. code-block:: chapel

  var LastLocale = Locales[numLocales-1];
  local on LastLocale {
    writeln("On remote locale ", LastLocale);
  }

Output::

  > ./a.out -nl 2
  local-on-err.chpl:2: error: Local-on is not local

This program begins executing on Locale 0, so when the ``local on`` attempts to
execute on a different node (the last Locale) we see a runtime error.

The ``local on`` construct functions similarly to a normal on-statement in all
other ways. Note that it is unrelated to ``local`` statements or ``local``
blocks, and that it has no impact on what communication is or is not allowed
(other than where the on-statement can execute).

With this information the compiler can reduce overhead associated with wide
pointers and hopefully improve performance.
