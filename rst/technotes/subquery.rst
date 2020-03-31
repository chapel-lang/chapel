.. _readme-subquery:

==========================
Querying a Local Subdomain
==========================

While writing a distributed program, one might need to know the index set that
a certain locale owns in a given distributed array, domain, or distribution.
This README describes the initial support for this functionality.

For code examples using these features, see the
:ref:`distributions.chpl <primers-distributions>` primer.

.. contents::

Functions Supported on Array and Domain Types
=============================================

hasSingleLocalSubdomain
-----------------------

.. code-block:: chapel

  proc array.hasSingleLocalSubdomain() : bool;
  proc domain.hasSingleLocalSubdomain() : bool;

This is a param function which returns a boolean. If true, then the index set
owned by a locale can be represented by a single domain. For example:

============= =========================
Distribution  hasSingleLocalSubdomain()
============= =========================
Block         true
Cyclic        true
BlockCyclic   false
Replicated    true
============= =========================

This function always returns ``true`` for non-distributed arrays and domains.

To support this function on a custom domain map, write a param function named
``dsiHasSingleLocalSubdomain``.

localSubdomain
--------------

.. code-block:: chapel

  proc array.localSubdomain(loc: locale = here) : domain;
  proc domain.localSubdomain(loc: locale = here) : domain;

This function only operates on arrays and domains whose
``hasSingleLocalSubdomain()`` result is ``true``. Otherwise, a compile-time
error is issued.

This function returns a single domain that represents the index set owned by
the given locale, which defaults to ``here``. This returned domain cannot be
used to modify the index set owned by the locale.

To support this function on a custom domain map, write a function named
``dsiLocalSubdomain``. For example:

.. code-block:: chapel

  proc BlockArr.dsiLocalSubdomain(loc: locale) : domain;

localSubdomains
---------------

.. code-block:: chapel

  iter array.localSubdomains(loc: locale = here) : domain;
  iter domain.localSubdomains(loc: locale = here) : domain;

This iterator yields the subdomain(s) that represent the index set owned by the
given locale, which defaults to ``here``

If the locale's index set can be represented by a single domain, then the
result of ``localSubdomain`` is yielded.

Currently, this is a serial iterator.

To support this iterator on a custom domain map, write an iterator named
``dsiLocalSubdomains``. For example:

.. code-block:: chapel

  iter BlockCyclicArr.dsiLocalSubdomains(loc: locale) : domain;

Functions Supported on Arrays, Domains, and Distributions
=========================================================

targetLocales
-------------

.. code-block:: chapel

  proc array.targetLocales() : [] locale;
  proc domain.targetLocales() : [] locale;
  proc distribution.targetLocales() : [] locale;


This function returns an array of locales that the distribution uses as the
locale grid.

