.. _readme-editions:

========
Editions
========

Chapel 2.5 introduces the concept of editions as a way of enabling continued
language evolution.

An edition is a version of the language.  It is accessible using a new
compilation flag, ``--edition``.  When the ``--edition`` flag is not used, the
default edition will be relied upon.

The following table contains the currently maintained editions, with a brief
description:

+-----------------+------------------------------------------------------------+
| Edition Name    | Description                                                |
+=================+============================================================+
| ``2.0``         | The language as it stood with the 2.0 release.  Currently  |
|                 | the default edition.                                       |
+-----------------+------------------------------------------------------------+
| ``default``     | The default edition.  Currently the same as the ``2.0``    |
|                 | edition, but may change over time.                         |
|                 |                                                            |
|                 | See :ref:`default-edition`.                                |
+-----------------+------------------------------------------------------------+
| ``preview``     | A collection of changes intended for future editions.      |
|                 |                                                            |
|                 | See :ref:`preview-edition` and :ref:`preview-changes`.     |
+-----------------+------------------------------------------------------------+

When a change is required that would break existing user codes, the change will
be guarded by an edition.  Users interested in updating to use this new behavior
can compile their code with ``--edition=<edition with the change>``, while the
majority of users can continue to rely on the old behavior by default.

The 2.5 release introduced two editions, ``2.0`` and ``pre-edition``.  In 2.6,
``pre-edition`` was renamed to ``preview``.  ``2.0`` is the default edition and
will remain the default for the foreseeable future.  ``preview`` is a gathering
point for breaking changes until there is a sufficient quantity of them to
justify a new edition.

.. _default-edition:

-------------------
The Default Edition
-------------------

``2.0`` is the default edition.

When no ``--edition`` flag is used at compilation, the default edition will be
relied upon.  It can also be specified via ``--edition=default``, or by
explicitly listing the default edition's unique name (e.g., ``--edition=2.0``).

Relying upon ``--edition=default`` or leaving the edition unspecified can be
beneficial.  Users should note, however, that when the default edition changes,
this will result in behavior changes across releases.

The default edition will not change frequently.  No future edition will
immediately become the default upon its creation.  There will always be at least
one release cycle between an edition's creation and becoming the default
edition, if not more.

.. note::

   We anticipate adding a compilation flag to aid in transitioning between
   editions.  The design on that flag has not been finalized, but any ideas
   along those lines are welcome, see
   https://github.com/chapel-lang/chapel/issues/27336 for the most recent
   discussion on that subject.


.. _preview-edition:

-------------------
The Preview Edition
-------------------

The ``preview`` edition is a gathering point for breaking changes until there is
a sufficient quantity of them to justify a new edition.  There will always be a
``preview`` edition, though users should avoid relying on it for production code
as breaking changes will be introduced to it without warning.

Changes that have been elevated from the ``preview`` edition into an official
edition will still be present in the ``preview`` edition going forward, until
such time as a conflicting change is made.

------------
Old Editions
------------

Editions that are no longer the default will eventually stop being supported.
However, no edition will be dropped immediately upon being replaced by another
default edition, there will always be at least one release cycle between an
edition being considered old and that edition having its support removed.

.. TODO: list old editions and their final release here, in a chart

.. _edition-changes:

-------
Changes
-------

This section is intended as a living list of breaking changes and which edition
they were introduced in.

.. _preview-changes:

++++++++++++++++++++++++++++++
Changes in the Preview Edition
++++++++++++++++++++++++++++++


The following are a list of breaking changes currently accessible by compiling
with ``--edition=preview``.  When a new edition is created, some of these
changes may be included in that edition as well, while others may continue to
remain in the preview until they are deemed sufficiently complete.

- The ``reshape`` function has been modified to support aliasing.  This enables
  viewing an existing array's elements using a different shape for the array.
  This behavior is potentially breaking for ``ref = reshape(A, D);`` cases.
  The behavior when storing into a ``var`` or ``const`` is unchanged.

  Other changes to this function include:

  - the ability to reshape to a 1D inferred-size array (like
    ``reshape(A, 1..)``)
  - the ability to specify the shape using a series of ranges rather than a
    domain (e.g., ``reshape(A, 1..n, 1..n)``)
  - the (default-on) check to make sure dimensions don’t get split or combined
    in surprising (potentially buggy) ways

  See the note on :chpl:proc:`~ChapelArray.reshape` for more information.

- The ``domain.sorted`` iterator method has been removed. It was only
  implemented for associative domains (as that was the only type of domain that
  could be sorted) and all other ``sort`` / ``sorted`` methods have been
  removed. The intended replacement is to import ``Sort`` and call
  :chpl:proc:`Sort.sorted(domain)<Sort.sorted>`.

  The temporary config ``noSortedWarnings`` has also been removed, as its only
  purpose was to suppress warnings about the ``domain.sorted`` method.

- Casts and printing of complex numbers involving ``nan`` have been adjusted to
  reflect the values of individual components instead of making the entire value
  be ``nan``.
