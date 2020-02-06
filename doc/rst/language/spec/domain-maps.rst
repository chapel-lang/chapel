.. _Chapter-Domain_Maps:

Domain Maps
===========

A domain map specifies the implementation of the domains and arrays that
are *mapped* using it. That is, it defines how domain indices and array
elements are mapped to locales, how they are stored in memory, and how
operations such as accesses, iteration, and slicing are performed. Each
domain and array is mapped using some domain map.

A domain map is either a *layout* or a *distribution*. A layout
describes domains and arrays that exist on a single locale, whereas a
distribution describes domains and arrays that are partitioned across
multiple locales.

A domain map is represented in the program with an instance of a *domain
map class*. Chapel provides a set of standard domain map classes. Users
can create domain map classes as well.

Domain maps are presented as follows:

-  domain maps for domain types :ref:`Domain_Maps_For_Types`,
   domain values :ref:`Domain_Maps_For_Values`, and arrays
   :ref:`Domain_Maps_For_Arrays`

-  domain maps are not retained upon domain assignment
   :ref:`Domain_Maps_Not_Assigned`

-  | standard layouts and distributions, such as Block and Cyclic, are
     documented under *Standard Layouts and Distributions* in Chapel's online
     documentation here:
   | https://chapel-lang.org/docs/modules/layoutdist.html

-  | specification of user-defined domain maps is forthcoming; please
     refer to the *Domain Map Standard Interface* page under *Technical
     Notes* in Chapel's online documentation here:
   | https://chapel-lang.org/docs/technotes/dsi.html

.. _Domain_Maps_For_Types:

Domain Maps for Domain Types
----------------------------

Each domain type has a domain map associated with it. This domain map is
used to map all domain values of this type
(:ref:`Domain_Maps_For_Values`).

If a domain type does not have a domain map specified for it explicitly
as described below, a default domain map is provided by the Chapel
implementation. Such a domain map will typically be a layout that maps
the entire domain to the locale on which the domain value is created or
the domain or array variable is declared.

A domain map can be specified explicitly by providing a *dmap value* in
a ``dmapped`` clause:



.. code-block:: syntax

   mapped-domain-type:
     domain-type `dmapped' dmap-value

   dmap-value:
     expression

A dmap value consists of an instance of a domain map class wrapped in an
instance of the predefined record ``dmap``. The domain map class is
chosen and instantiated by the user. ``dmap`` behaves like a generic
record with a single generic field, which holds the domain map instance.

   *Example*.

   The code 

   .. code-block:: chapel

      use BlockDist;
      var MyBlockDist: dmap(Block(rank=2));

   | declares a variable capable of storing dmap values for a
     two-dimensional Block distribution. The Block distribution is
     described in more detail in the standard library documentation.
     See:
   | https://chapel-lang.org/docs/modules/dists/BlockDist.html

..

   *Example*.

   The code 

   .. code-block:: chapel

      use BlockDist;
      var MyBlockDist: dmap(Block(rank=2)) = new dmap(new Block({1..5,1..6}));

   creates a dmap value wrapping a two-dimensional Block distribution
   with a bounding box of ``{1..5, 1..6}`` over all of the locales.

   *Example*.

   The code 

   .. code-block:: chapel

      use BlockDist;
      var MyBlockDist = new dmap(new Block({1..5,1..6}));
      type MyBlockedDom = domain(2) dmapped MyBlockDist;

   defines a two-dimensional rectangular domain type that is mapped
   using a Block distribution.

The following syntactic sugar is provided within the ``dmapped`` clause.
If a ``dmapped`` clause starts with the name of a domain map class, it
is considered to be an initialization expression as if preceded by
``new``. The resulting domain map instance is wrapped in a newly-created
instance of ``dmap`` implicitly.

   *Example*.

   The code 

   .. code-block:: chapel

      use BlockDist;
      type BlockDom = domain(2) dmapped Block({1..5,1..6});

   is equivalent to 

   .. code-block:: chapel

      use BlockDist;
      type BlockDom = domain(2) dmapped new dmap(new Block({1..5,1..6}));

.. _Domain_Maps_For_Values:

Domain Maps for Domain Values
-----------------------------

A domain value is always mapped using the domain map of that value’s
type. The type inferred for a domain literal
(:ref:`Rectangular_Domain_Values`) has a default domain map.

   *Example*.

   In the following code 

   .. code-block:: chapel

      use BlockDist;
      var MyDomLiteral = {1..2,1..3};
      var MyBlockedDom: domain(2) dmapped Block({1..5,1..6}) = MyDomLiteral;

   ``MyDomLiteral`` is given the inferred type of the domain literal and
   so will be mapped using a default map. MyBlockedDom is given a type
   explicitly, in accordance to which it will be mapped using a Block
   distribution.

A domain value’s map can be changed explicitly with a ``dmapped``
clause, in the same way as a domain type’s map.



.. code-block:: syntax

   mapped-domain-expression:
     domain-expression `dmapped' dmap-value

..

   *Example*.

   In the following code 

   .. code-block:: chapel

      use BlockDist;
      var MyBlockedDomLiteral1 = {1..2,1..3} dmapped new dmap(new Block({1..5,1..6}));
      var MyBlockedDomLiteral2 = {1..2,1..3} dmapped Block({1..5,1..6});

   both ``MyBlockedDomLiteral1`` and ``MyBlockedDomLiteral2`` will be
   mapped using a Block distribution.

.. _Domain_Maps_For_Arrays:

Domain Maps for Arrays
----------------------

Each array is mapped using the domain map of the domain over which the
array was declared.

   *Example*.

   In the code 

   .. code-block:: chapel

      use BlockDist;
      var Dom: domain(2) dmapped Block({1..5,1..6}) = {1..5,1..6};
      var MyArray: [Dom] real;

   the domain map used for ``MyArray`` is the Block distribution from
   the type of ``Dom``.

.. _Domain_Maps_Not_Assigned:

Domain Maps Are Not Retained upon Domain Assignment
---------------------------------------------------

Domain assignment (:ref:`Domain_Assignment`) transfers only the
index set of the right-hand side expression. The implementation of the
left-hand side domain expression, including its domain map, is
determined by its type and so does not change upon a domain assignment.

   *Example*.

   In the code 

   .. code-block:: chapel

      use BlockDist;
      var Dom1: domain(2) dmapped Block({1..5,1..6}) = {1..5,1..6};
      var Dom2: domain(2) = Dom1;

   ``Dom2`` is mapped using the default distribution, despite ``Dom1``
   having a Block distribution.

..

   *Example*.

   In the code 

   .. code-block:: chapel

      use BlockDist;
      var Dom1: domain(2) dmapped Block({1..5,1..6}) = {1..5,1..6};
      var Dom2 = Dom1;

   ``Dom2`` is mapped using the same distribution as ``Dom1``. This is
   because the declaration of ``Dom2`` lacks an explicit type specifier
   and so its type is defined to be the type of its initialization
   expression, ``Dom1``. So in this situation the effect is that the
   domain map does transfer upon an initializing assignment.
