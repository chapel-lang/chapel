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

.. code-block:: chapel

    local do
      x = A(5);

The inner statement is often a block, commonly referred to as a
"local block":

.. code-block:: chapel

    local {
      initializeMyData(data);
      compute(data);
    }

In the above examples, the Chapel implementation checks whether ``x``,
as well as all memory referenced during the calls of ``A.this(5)``
(an implicit call for ``A(5)``), initializeMyData(), and compute(),
are located on the current locale. Otherwise an error is reported.
Analogously, if ``on`` statement(s) are executed during these calls
that attempt to execute on a different locale, an error is reported.

Conditional `local`
-------------------

The ``local`` statement behavior can be controlled via the optional
conditional expression.

.. code-block:: chapel

    local data.locale == here {
      initializeMyData(data);
      compute(data);
    }

The above example will be localized only if ``data`` resides in the
current locale. Conditional local statement above compiled identically
as:

.. code-block:: chapel

    if data.locale == here {
      local {
        initializeMyData(data);
        compute(data);
      }
    }
    else {
      initializeMyData(data);
      compute(data);
    }

This implies that local statements in outer dynamic/static scopes will
override the inner ones. i.e. if ``data.locale == here`` evaluates to
``true``, localized bodies of ``initializeMyData`` and ``compute`` will be
used whether they have any ``local`` statement, or not. (This includes
``local false`` blocks).

The 'local do on' Statement
===========================


The ``local do on`` construct in Chapel performs an on-statement on a
sublocale within the current node. For example:

.. code-block:: chapel

  for i in 0..#here.getChildCount() {
    local do on here.getChild(i) {
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
  local do on LastLocale {
    writeln("On remote locale ", LastLocale);
  }

Output::

  > ./local-on-err -nl 2
  local-on-err.chpl:2: error: Local-on is not local

This program begins executing on Locale 0, so when the ``local do on``
attempts to execute on a different node (the last Locale) we see a
runtime error.

The ``local do on`` construct functions similarly to a normal
on-statement in all other ways. Note that it is unrelated to ``local``
statements or ``local`` blocks, and that it has no impact on what
communication is or is not allowed (other than where the on-statement
can execute).

With this information the compiler can reduce overhead associated with wide
pointers and hopefully improve performance.
