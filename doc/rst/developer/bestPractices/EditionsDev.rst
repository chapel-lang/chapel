.. _best-practices-editions:

========
Editions
========

--------------------
New Editions Process
--------------------

When creating new editions, be sure to:

- Add the new edition name to the array of editions in the compiler just
  **before** ``preview``.
- Update all desired changes that are currently associated with the ``preview``
  edition to be linked with this new edition name.

  - Update :ref:`edition-changes` to list those changes under this new release
    name instead of under :ref:`preview-changes`.

- When https://github.com/chapel-lang/chapel/issues/27336 gets implemented, be
  sure to update it as well.

-----------------------------------
Associating Changes With An Edition
-----------------------------------

The implementation of editions is currently in progress.  Here are some
supported ways to tie a change to a particular edition.

.. _edition-attribute:

++++++++++++++++++++++
The @edition Attribute
++++++++++++++++++++++

For breaking changes that can exist entirely in module code, the ``@edition``
attribute can be used.

The ``@edition`` attribute requires one or both of the following arguments:

- first

  - To specify the earliest edition that will include this symbol.  When not
    specified, will be the earliest supported edition.

- last

  - To specify the final edition that will include this symbol.  When not
    specified, will be the ``preview`` edition.

When using the attribute, the old behavior should be marked with
``@edition(last="<most recent edition name>")``, while the new behavior should
be marked with ``@edition(first="preview")``.  This will ensure that the
old behavior remains available with previous editions, while the new behavior
becomes available in the ``preview`` edition.  E.g.,

.. literalinclude:: ../../../../test/edition/basics/attributeCheck.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE

.. note::

   Though ``default`` is a supported value for the ``--edition`` compilation
   flag, it is not supported for either the ``first`` or ``last`` argument of
   the edition attribute.  These arguments must be concrete, specific editions,
   to avoid potential implementation bugs.

Generally, specifying both ``first`` and ``last`` is most helpful if a feature
has been changed in multiple editions.  It is hoped that such occurrences will
be rare.

++++++++++++++++
Compiler Changes
++++++++++++++++

When making a change that should be supported by the compiler itself, one should
be careful to indicate the range of editions to which the change is applicable,
rather than specifying a single edition.  There is a helper function to use to
validate that the compiled edition is within a known first and last edition,
which can be used to determine if the change should apply.  There is also a
helper function to validate that the edition range specified itself is valid.

The ``Symbol`` AST node in the production compiler will return the default
values listed in :ref:`edition-attribute` if they are not explicitly provided.
This will be useful when extending attribute support to more than just function
resolution, but for specific compiler-level breaking changes the range will
likely have to be explicitly specified.

All known editions are stored in an array in the compiler in order, and the last
edition in the array is always the ``preview`` edition.

+++++++++++++++
Runtime Changes
+++++++++++++++

Since users will specify the edition to use via a compilation flag, it's likely
better to avoid edition-specific computations in the runtime.  That doesn't
preclude the possibility of needing certain implementation decisions to happen
at runtime, but in general it's probably better to have done the computation of
which implementation to use based on the compiled edition during compilation.

As a concrete example, changing the printing behavior of nan in complex numbers
requires modifying our runtime I/O code
(https://github.com/chapel-lang/chapel/pull/27011).  The best way to handle this
is probably adding an argument to ``qio_channel_print_complex`` to indicate
which version of the implementation to use, and then using the attribute
solution in module code to switch between how that argument is set.  Something
like:

.. code-block:: chapel

   @edition(first="preview")
   proc chpl_print_complex(...) {
     return qio_channel_print_complex(false, _channel_internal, re, im,
                                      numBytes(re.type),
                                      /* edition applicable */ true);
   }

   @edition(last="2.0")
   proc chpl_print_complex(...) {
     return qio_channel_print_complex(false, _channel_internal, re, im,
                                      numBytes(re.type),
                                      /* edition applicable */ false);
   }

This will make use of the existing edition computations in the compiler (saving
execution time performance), and allow the runtime code to use a descriptive
name for the argument that reflects the behavior being changed.  Alternatively,
a separate overload could be added to the runtime to handle the change.
