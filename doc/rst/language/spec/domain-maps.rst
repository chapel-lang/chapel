.. default-domain:: chpl

.. index::
   single: domain maps
   single: mapped; domain maps
   single: layouts
   single: distributions
   single: domain maps; layouts
   single: domain maps; distributions
.. _Chapter-Domain_Maps:

=============
Distributions
=============

A distribution, also called a *domain map*,
specifies the implementation of the domains and arrays that
are *mapped* using it. That is, it defines how domain indices and array
elements are mapped to locales, how they are stored in memory, and how
operations such as accesses, iteration, and slicing are performed. Each
domain and array is mapped using some distribution.

In general, a distribution describes domains and arrays
that are partitioned across multiple locales.
A *layout* is a distribution that describes domains and arrays
that exist on a single locale.

A distribution is represented in the program with an instance of a
*distribution record*. Chapel provides a set of standard distributions.
Users can create distributions as well.

Distributions are presented as follows:

-  distributions for
   :ref:`domain types <Domain_Maps_For_Types>`,
   :ref:`domain values <Domain_Maps_For_Values>`,
   :ref:`arrays <Domain_Maps_For_Arrays>`

-  :ref:`distributions are not retained upon domain assignment <Domain_Maps_Not_Assigned>`

-  :ref:`Standard Layouts and Distributions <layouts_and_distributions>`,
   such as Block and Cyclic

-  specification of user-defined distributions is forthcoming; please
   refer to the
   :ref:`Domain Map Standard Interface technical note <readme-dsi>`

.. index::
   single: domain maps for domain types
   single: domains; domain maps for
   single: dmap value
   single: dmapped clause
   single: domain maps; dmap value
   single: domain maps; dmapped clause
.. _Domain_Maps_For_Types:

Distributions for Domain Types
------------------------------

Each domain type has a distribution associated with it. This distribution is
used to map all domain values of this type
(see :ref:`Domain_Maps_For_Values`).

If a domain type does not have a distribution specified for it explicitly
as described below, a default distribution is provided by the Chapel
implementation. Such a distribution will typically be a layout that maps
the entire domain to the locale on which the current task is executed or
the domain or array variable is declared.

A distribution can be specified explicitly using a ``dmapped`` clause:

.. code-block:: syntax

   mapped-domain-type:
     domain-type 'dmapped' distribution-record

   distribution-record:
     expression

where ``distribution-record`` is an instance of a distribution or layout type.

.. warning::

   In previous versions of Chapel, it was legal to insert a distribution
   initialization-expression on the right side of a ``dmapped`` expression,
   where the ``new`` keyword was essentially implied. For example, the following
   was legal:

      .. code-block:: chapel

         var d = {1..n} dmapped blockDist({1..n});

   but should now be replaced by:

      .. code-block:: chapel

         var d = {1..n} dmapped new blockDist({1..n});

   Omitting the ``new`` keyword is deprecated and will become an error in a
   future release.


.. warning::

   The ``dmapped`` keyword and the ``dmapped`` clause
   are currently unstable and may change in the future.
   Factory functions provided by the desired distribution,
   when available, should be used instead.

..

   *Example*.

   The code 

   .. code-block:: chapel

      use BlockDist;
      var MyBlockDist = new blockDist({1..5,1..6});

   creates an instance of the Block distribution record
   for two-dimensional domains and arrays
   with a bounding box of ``{1..5, 1..6}`` over all of the locales.

   *Example*.

   The code 

   .. code-block:: chapel

      use BlockDist;
      var MyBlockDist = new blockDist({1..5,1..6});
      type MyBlockedDom = domain(2) dmapped MyBlockDist;

   defines the type of two-dimensional rectangular domains
   that are mapped using a Block distribution.

The following syntactic sugar is provided within the ``dmapped`` clause.
If a ``dmapped`` clause starts with the name of a distribution record, it
is considered to be an initialization expression as if preceded by
``new``.

   *Example*.

   The code 

   .. code-block:: chapel

      use BlockDist;
      type BlockDom = domain(2) dmapped blockDist({1..5,1..6});

   is equivalent to 

   .. code-block:: chapel

      use BlockDist;
      type BlockDom = domain(2) dmapped new blockDist({1..5,1..6});

.. index::
   single: domain maps; for domain values
.. _Domain_Maps_For_Values:

Distributions for Domain Values
-------------------------------

A domain value is always mapped using the distribution of that value's
type. The type inferred for a domain literal
(see :ref:`Rectangular_Domain_Values`) has a default distribution.

   *Example*.

   In the following code 

   .. code-block:: chapel

      use BlockDist;
      var MyDomLiteral = {1..2,1..3};
      var MyBlockedDom: domain(2) dmapped blockDist({1..5,1..6}) = MyDomLiteral;

   ``MyDomLiteral`` is a domain literal and so will be mapped using
   a default distribution. MyBlockedDom is given a type explicitly,
   therefore it will be mapped using a Block distribution.

A domain value's distribution can be specified explicitly with a ``dmapped``
clause, in the same way as a domain type's distribution.

.. code-block:: syntax

   mapped-domain-expression:
     domain-expression 'dmapped' distribution-record

..

   *Example*.

   In the following code 

   .. code-block:: chapel

      use BlockDist;
      var MyBlockedDomLiteral1 = {1..2,1..3} dmapped new blockDist({1..5,1..6});
      var MyBlockedDomLiteral2 = {1..2,1..3} dmapped blockDist({1..5,1..6});

   both ``MyBlockedDomLiteral1`` and ``MyBlockedDomLiteral2`` will be
   mapped using a Block distribution.

.. index::
   single: domain maps; for arrays
   single: arrays; domain maps
.. _Domain_Maps_For_Arrays:

Distributions for Arrays
------------------------

Each array is mapped using the distribution of the domain over which the
array was declared.

   *Example*.

   In the code 

   .. code-block:: chapel

      use BlockDist;
      var Dom: domain(2) dmapped blockDist({1..5,1..6}) = {1..5,1..6};
      var MyArray: [Dom] real;

   the distribution used for ``MyArray`` is the Block distribution from
   the type of ``Dom``.

.. index::
   single: domain maps; domain assignment
   single: domains; assignment
   single: assignment; domain
.. _Domain_Maps_Not_Assigned:

Distributions Are Not Retained upon Domain Assignment
-----------------------------------------------------

:ref:`Domain assignment <Domain_Assignment>` transfers only the
index set of the right-hand side expression. The implementation of the
left-hand side domain expression, including its distribution, is
determined by its type and so does not change upon a domain assignment.

   *Example*.

   In the code 

   .. code-block:: chapel

      use BlockDist;
      var Dom1: domain(2) dmapped blockDist({1..5,1..6}) = {1..5,1..6};
      var Dom2: domain(2) = Dom1;

   ``Dom2`` is mapped using a default distribution, despite ``Dom1``
   having a Block distribution.

..

   *Example*.

   In the code 

   .. code-block:: chapel

      use BlockDist;
      var Dom1: domain(2) dmapped blockDist({1..5,1..6}) = {1..5,1..6};
      var Dom2 = Dom1;

   ``Dom2`` is mapped using the same distribution as ``Dom1``. This is
   because the declaration of ``Dom2`` lacks an explicit type specifier
   and so its type is defined to be the type of its initialization
   expression, ``Dom1``. So in this situation the effect is that the
   distribution does transfer upon initialization.

.. _Predefined_Distribution_Operations:

Predefined Operations on Distributions
--------------------------------------

Equality and Inequality
~~~~~~~~~~~~~~~~~~~~~~~

Equality and inequality operators are defined to test if two distributions
are equivalent or not:

   .. code-block:: chapel

     dist1 == dist2
     dist1 != dist2

targetLocales
~~~~~~~~~~~~~

Distributions that describe partitioning across multiple locales
typically define the method ``targetLocales()`` that returns
these locales as an array.
