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

- When `#27336 <TransitionFlag>`_ gets implemented, be sure to
  update it as well.

-----------------------
What Goes in an Edition
-----------------------

When making a change to the language and/or standard library, it's important
to consider how that change fits into an edition. Editions define the state of
the language at a point in time, so breaking changes to stable code should be
associated with an edition.

It is important to define what constitutes a "breaking change" and what belongs
in an edition versus what can just be changed or added.
We consider a feature to be stable if it is not marked unstable (either directly
or indirectly by being included in a module that is marked unstable).

* **Breaking Changes**

   * altering the behavior of a stable feature

   * removal of a stable feature

* **Non-Breaking Changes**

   * altering the behavior of an unstable feature

   * deprecation or removal of an unstable feature

   * addition of a new feature

.. note::

   On a case by case basis, some changes that would normally be considered
   non-breaking may be treated as breaking changes. For example, if an
   unstable feature is widely used in practice, changing its behavior may be
   considered a breaking change.

The following provides several scenarios of language evolution. In
this, we do not distinguish between language features and standard library
features.

* **Changing the behavior of a stable feature** (`Breaking Change`)

    A stable feature should only have its behavior change in the ``preview``
    edition. The ``preview`` edition collects changes that will be included in
    the next edition.

    .. note::

        An unimplemented feature is the flag described in
        `#27336 <TransitionFlag>`_, which will warn users
        in a given edition about code that will need to be migrated in the next
        edition.

* **Removal of a stable feature** (`Breaking Change`)

    A stable feature should only be removed in the ``preview`` edition.
    The act of marking a stable feature as removed in a future edition
    enables deprecation warnings for the feature in the current edition
    (when requested).

    .. note::

        An unimplemented feature is the flag described in
        `#27336 <TransitionFlag>`_, which will warn users
        in a given edition about code that will need to be migrated in the next
        edition.

* **Changing the behavior of an unstable feature** (`Non-Breaking Change`)

    Unstable features are not considered part of an edition, and so their
    behavior can be changed at any time without being associated with an
    edition. However, the first Chapel compiler release to change the behavior
    of an unstable feature should warn users that the feature has changed in
    some way. Later Chapel compiler releases may remove the warning.

* **Deprecation (and subsequent removal) of an unstable feature** (`Non-Breaking Change`)

    Unstable features are not considered part of an edition, and so they can be
    deprecated at any time without being associated with an edition. However,
    the first Chapel compiler release to deprecate or remove an unstable
    feature should warn users that the feature has been deprecated or removed.
    Later Chapel compiler releases may remove the warning and the feature.
    Unstable features should not be removed without warning.

    When deprecating features, please refer to the guidelines in
    :ref:`best-practices-deprecation`.

* **Addition of a new feature** (`Non-Breaking Change`)

    New features may be added at any time without being associated with an
    edition, as long as they are added as unstable features. The new feature
    can be added as an unstable feature to any Chapel compiler release. This
    makes the feature available to users who wish to use it, without affecting
    the edition. When we feel the new feature is ready to be stabilized, it can
    be marked as stable in a new edition by being added to the ``preview``
    edition. Then, when a new edition is created from the ``preview`` edition,
    the feature will be stable in that edition. Adding a new feature to the
    ``preview`` edition does not require removing the unstable version of the
    feature; both versions can coexist until the edition containing the
    unstable version is no longer supported.

    Users who make use of the new feature will get an unstable warning if
    ``--warn-unstable`` is enabled (or by default when compiling with a
    non-default and non-preview edition), unless they are using an edition of
    the language that includes the feature as stable.

In summary, if a user is using only stable features of a given language
edition, two different versions of the compiler that support that edition
should have the same behavior (barring bugs).
Breaking changes to stable features should be associated with a
edition. Unstable features may be changed or removed at any time without being
associated with an edition. New features should be added as unstable features or
directly added to the ``preview`` edition as stable features.


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

.. _TransitionFlag: https://github.com/chapel-lang/chapel/issues/27336
