.. _readme-subquery:

==========================
Querying a Local Subdomain
==========================

While writing a distributed program, one might need to know the index set that
a certain locale owns in a given distributed array. This README describes the
initial support for this functionality.

For code examples using these features, see the
``$CHPL_HOME/examples/primers/distributions.chpl`` primer.

.. contents::

Functions Supported on All Array Types
======================================

hasSingleLocalSubdomain
-----------------------

.. code-block:: chapel

  proc array.hasSingleLocalSubdomain() : bool;

This is a param function which returns a boolean. If true, then the index set
owned by a locale can be represented by a single domain.

============= =======================
Distribution  hasSingleLocalSubdomain
============= =======================
Block         true
Cyclic        true
BlockCyclic   false
Replicated    true
============= =======================

This function always returns ``true`` for non-distributed arrays.

To support this function on a custom distributed array type, write a param
function named 'dsiHasSingleLocalSubdomain'.

localSubdomain
--------------

.. code-block:: chapel

  proc array.localSubdomain() : domain;

This function only operates on arrays whose 'hasSingleLocalSubdomain()' result
is true. Otherwise, a compiler error is thrown.

This function returns a single domain that represents the index set owned
by the current locale. This returned domain cannot be used to modify the index
set owned by the locale.

For non-distributed arrays, this function returns a copy of that array's
domain.

To support this function on a custom distributed array type, write a function
named 'dsiLocalSubdomain'.

localSubdomains
---------------

.. code-block:: chapel

  iter array.localSubdomains() : domain;

This iterator yields the subdomain(s) that represent the index set owned by the
current locale.

If the locale's index set can be represented by a single domain, then the
result of ``localSubdomain`` is yielded.

Currently, this is a serial iterator.

To support this iterator on a custom distributed array type, write an iterator
named 'dsiLocalSubdomains'.

Functions Supported on Distributed Arrays
=========================================

Any functions in this section are only supported on distributed array types.
Calling them on a non-distributed array will result in a compiler error.

targetLocales
-------------

.. code-block:: chapel

  proc array.targetLocales() : [] locale;

This function returns an array of locales that the distribution uses as the
locale grid.

Future Work
===========

While these functions are currently supported on arrays, future work involves
support for domains and domain maps.
