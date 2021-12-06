.. default-domain:: chpl
.. _readme-sets:

==========================
Associative Set Operations
==========================

This README describes some initial support for set operations on associative
domains. It is expected that the features described here will be
included in a future version of the language specification.

.. contents::

Associative Domain Set Operations
=================================

The following set functions/operations are supported by associative domains.
In the following code examples, ``A``, ``B``, and ``C`` are associative domains.
For every operator, an op= variant exists. For these op= functions, LHS op= RHS,
the LHS argument is modified in-place using the same rules as the normal
operator function.

isSubset
--------
.. code-block:: chapel

  proc isSubset(super : domain) : bool;

  A.isSubset(B);

This statement will return true if ``A`` is a subset of ``B``. ``A`` is a subset of
``B`` if every index in ``A`` is also present in ``B``.

isSuper
-------
.. code-block:: chapel

  proc isSuper(sub : domain) : bool;

  A.isSuper(B);

This statement will return true if ``B`` is a subset of ``A``.

Union
-----
.. code-block:: chapel

  C = A + B;
  C = A | B;

``C`` will be a new associative domain which contains every index in ``A`` and
every index in ``B``.

The op= variants are::

  A |= B;
  A += B;

Difference
----------
.. code-block:: chapel

  C = A - B;

``C`` will contain the indices in ``A`` that are not also in ``B``.

The op= variant is::

  A -= B;

Any indices in both ``A`` and ``B`` will be removed from ``A``.

Intersection
------------
.. code-block:: chapel

  C = A & B;

``C`` will contain the indices in ``A`` that are also in ``B``.

The op= variant is::

  A &= B;

Symmetric Difference
--------------------
.. code-block:: chapel

  C = A ^ B;

``C`` will contain the indices that are unique to ``A`` and unique to ``B``. Another
way of writing this statement is::

  C = (A + B) - (A & B)

The op= variant is::

  A ^= B;
