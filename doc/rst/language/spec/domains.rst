.. _Chapter-Domains:

Domains
=======

A *domain* is a first-class representation of an index set. Domains are
used to specify iteration spaces, to define the size and shape of arrays
(:ref:`Chapter-Arrays`), and to specify aggregate operations like
slicing. A domain can specify a single- or multi-dimensional rectangular
iteration space or represent a set of indices of a given type. Domains
can also represent a subset of another domain’s index set, using either
a dense or sparse representation. A domain’s indices may potentially be
distributed across multiple locales as described
in :ref:`Chapter-Domain_Maps`, thus supporting global-view data
structures.

In the next subsection, we introduce the key characteristics of domains.
In :ref:`Base_Domain_Types_and_Values`, we discuss the types and
values that can be associated with a base domain.
In :ref:`Simple_Subdomain_Types_and_Values`, we discuss the
types and values of simple subdomains that can be created from those
base domains. In :ref:`Sparse_Subdomain_Types_and_Values`, we
discuss the types and values of sparse subdomains. The remaining
sections describe the important manipulations that can be performed with
domains, as well as the predefined operators and functions defined for
domains.

Domain Overview
---------------

There are three *kinds* of domain, distinguished by their subset
dependencies: *base domains*, *subdomains* and *sparse subdomains*. A
base domain describes an index set spanning one or more dimensions. A
subdomain creates an index set that is a subset of the indices in a base
domain or another subdomain. Sparse subdomains are subdomains which can
represent sparse index subsets efficiently. Simple subdomains are
subdomains that are not sparse. These relationships can be represented
as follows:



.. code-block:: syntax

   domain-type:
     base-domain-type
     simple-subdomain-type
     sparse-subdomain-type

Domains can be further classified according to whether they are
*regular* or *irregular*. A regular domain represents a rectangular
iteration space and can have a compact representation whose size is
independent of the number of indices. Rectangular domains, with the
exception of sparse subdomains, are regular.

An irregular domain can store an arbitrary set of indices of an
arbitrary but homogeneous index type. Irregular domains typically
require space proportional to the number of indices being represented.
All *associative* domain types and their subdomains (including sparse
subdomains) are irregular. Sparse subdomains of regular domains are also
irregular.

An index set can be either *ordered* or *unordered* depending on whether
its members have a well-defined order relationship. All regular domains
are ordered. All associative domains are unordered.

The type of a domain describes how a domain is represented and the
operations that can be performed upon it, while its value is the set of
indices it represents. In addition to storing a value, each domain
variable has an identity that distinguishes it from other domains that
may have the same type and value. This identity is used to define the
domain’s relationship with subdomains, index
types (:ref:`Index_Types`), and
arrays (:ref:`Association_of_Arrays_to_Domains`).

The runtime representation of a domain is controlled by its domain map.
Domain maps are presented in :ref:`Chapter-Domain_Maps`.

.. _Base_Domain_Types_and_Values:

Base Domain Types and Values
----------------------------

Base domain types can be classified as regular or irregular. Dense and
strided rectangular domains are regular domains. Irregular base domain
types include all of the associative domain types.



.. code-block:: syntax

   base-domain-type:
     rectangular-domain-type
     associative-domain-type

These base domain types are discussed in turn in the following
subsections.

Rectangular Domains
~~~~~~~~~~~~~~~~~~~

Rectangular domains describe multidimensional rectangular index sets.
They are characterized by a tensor product of ranges and represent
indices that are tuples of an integral type. Because their index sets
can be represented using ranges, regular domain values typically require
only :math:`O(1)` space.

Rectangular Domain Types
^^^^^^^^^^^^^^^^^^^^^^^^

Rectangular domain types are parameterized by three things:

-  ``rank`` a positive ``int`` value indicating the number of dimensions
   that the domain represents;

-  ``idxType`` a type member representing the index type for each
   dimension; and

-  ``stridable`` a ``bool`` parameter indicating whether any of the
   domain’s dimensions will be characterized by a strided range.

If ``rank`` is :math:`1`, the index type represented by a rectangular
domain is ``idxType``. Otherwise, the index type is the homogeneous
tuple type ``rank*idxType``. If unspecified, ``idxType`` defaults to
``int`` and ``stridable`` defaults to ``false``.

   *Open issue*.

   We may represent a rectangular domain’s index type as rank*idxType
   even if rank is 1. This would eliminate a lot of code currently used
   to support the special (rank == 1) case.

The syntax of a rectangular domain type is summarized as follows:


.. code-block:: syntax

   rectangular-domain-type:
     `domain' ( named-expression-list )

where ``named-expression-list`` permits the values of ``rank``,
``idxType``, and ``stridable`` to be specified using standard type
signature.

   *Example (typeFunctionDomain.chpl)*.

   The following declarations both create an uninitialized rectangular
   domain with three dimensions, with ``int`` indices: 

   .. code-block:: chapel

      var D1 : domain(rank=3, idxType=int, stridable=false);
      var D2 : domain(3);

   

   .. BLOCK-test-chapelpost

      writeln(D1);
      writeln(D2);

   

   .. BLOCK-test-chapeloutput

      {1..0, 1..0, 1..0}
      {1..0, 1..0, 1..0}

.. _Rectangular_Domain_Values:

Rectangular Domain Values
^^^^^^^^^^^^^^^^^^^^^^^^^

Each dimension of a rectangular domain is a range of type
``range(idxType, BoundedRangeType.bounded, stridable)``. The index set
for a rank 1 domain is the set of indices described by its singleton
range. The index set for a rank \ :math:`n` domain is the set of all
``n*idxType`` tuples described by the tensor product of its ranges. When
expanded (as by an iterator), rectangular domain indices are ordered
according to the lexicographic order of their values. That is, the index
with the highest rank is listed first and changes most slowly. [3]_

   .. note::

      *Future*

      Domains defined using unbounded ranges may be supported.

Literal rectangular domain values are represented by a comma-separated
list of range expressions of matching ``idxType`` enclosed in curly
braces:



.. code-block:: syntax

   rectangular-domain-literal:
     { range-expression-list }

   range-expression-list:
     range-expression
     range-expression, range-expression-list

The type of a rectangular domain literal is defined as follows:

-  ``rank`` = the number of range expressions in the literal;

-  ``idxType`` = the type of the range expressions;

-  ``stridable`` = ``true`` if any of the range expressions are
   stridable, otherwise ``false``.

If the index types in the ranges differ and all of them can be promoted
to the same type, then that type is used as the ``idxType``. Otherwise,
the domain literal is invalid.

   *Example*.

   The expression ``{1..5, 1..5}`` defines a rectangular domain with
   type ``domain(rank=2,`` ``idxType=int,`` ``stridable=false)``. It is
   a :math:`5 \times 5` domain with the indices:

   .. math:: (1, 1), (1, 2), \ldots, (1, 5), (2, 1), \ldots (5, 5).

A domain expression may contain bounds which are evaluated at runtime.

   *Example*.

   In the code 

   .. code-block:: chapel

      var D: domain(2) = {1..n, 1..n};

   ``D`` is defined as a two-dimensional, nonstridable rectangular
   domain with an index type of ``2*int`` and is initialized to contain
   the set of indices :math:`(i,j)` for all :math:`i` and :math:`j` such
   that :math:`i \in {1, 2, \ldots, n}` and
   :math:`j \in {1, 2, \ldots, n}`.

The default value of a domain type is the ``rank`` default range values
for type:

   ``range(idxType, BoundedRangeType.bounded, stridable)``

..

   *Example (rectangularDomain.chpl)*.

   The following creates a two-dimensional rectangular domain and then
   uses this to declare an array. The array indices are iterated over
   using the domain’s ``dim()`` method, and each element is filled with
   some value. Then the array is printed out.

   Thus, the code 

   .. code-block:: chapel

      var D : domain(2) = {1..2, 1..7};
      var A : [D] int;
      for i in D.dim(0) do
        for j in D.dim(1) do
          A[i,j] = 7 * i**2 + j;
      writeln(A);

   produces 

   .. code-block:: printoutput

      8 9 10 11 12 13 14
      29 30 31 32 33 34 35

Associative Domains
~~~~~~~~~~~~~~~~~~~

Associative domains represent an arbitrary set of indices of a given
type and can be used to describe sets or to create dictionary-style
arrays (hash tables). The type of indices of an associative domain, or
its ``idxType``, can be any primitive type except ``void`` or any class
type.

.. _Associative_Domain_Types:

Associative Domain Types
^^^^^^^^^^^^^^^^^^^^^^^^

An associative domain type is parameterized by ``idxType``, the type of
the indices that it stores. The syntax is as follows:



.. code-block:: syntax

   associative-domain-type:
     `domain' ( associative-index-type )

   associative-index-type:
     type-expression

The ``associative-index-type`` determines the ``idxType`` of the
associative domain type.

When an associative domain is used as the index set of an array, the
relation between the indices and the array elements can be thought of as
a map between the values of the index set and the elements stored in the
array.

.. _Associative_Domain_Values:

Associative Domain Values
^^^^^^^^^^^^^^^^^^^^^^^^^

An associative domain’s value is simply the set of all index values that
the domain describes. The iteration order over the indices of an
associative domain is undefined.

Specification of an associative domain literal value follows a similar
syntax as rectangular domain literal values. What differentiates the two
are the types of expressions specified in the comma separated list. Use
of values of a type other than ranges will result in the construction of
an associative domain.



.. code-block:: syntax

   associative-domain-literal:
      { associative-expression-list }

   associative-expression-list:
      non-range-expression
      non-range-expression, associative-expression-list

   non-range-expression:
      expression

It is required that the types of the values used in constructing an
associative domain literal value be of the same type. If the types of
the indices does not match a compiler error will be issued.

   .. note::

      *Future*
      
      Due to implementation of ``==`` over arrays it is currently not possible
      to use arrays as indices within an associative domain.

..

   *Open issue*.

   Assignment of an associative domain literal results in a warning
   message being printed alerting the user that whole-domain assignment
   has been serialized. This results from the resize operation over
   associative arrays not being parsafe.

   *Example (associativeDomain.chpl)*.

   The following example illustrates construction of an associative
   domain containing string indices "bar" and "foo". Note that due to
   internal hashing of indices the order in which the values of the
   associative domain are iterated is not the same as their
   specification order.

   This code 

   .. code-block:: chapel

      var D : domain(string) = {"bar", "foo"};
      writeln(D);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   produces the output 

   .. code-block:: printoutput

      {foo, bar}

If uninitialized, the default value of an associative domain is the
empty index set.

Indices can be added to or removed from an associative domain as
described in :ref:`Adding_and_Removing_Domain_Indices`.

.. _Simple_Subdomain_Types_and_Values:

Simple Subdomain Types and Values
---------------------------------

A subdomain is a domain whose indices are guaranteed to be a subset of
those described by another domain known as its *parent domain*. A
subdomain has the same type as its parent domain, and by default it
inherits the domain map of its parent domain. All domain types support
subdomains.

Simple subdomains are subdomains which are not sparse. Sparse subdomains
are discussed in the following section
(:ref:`Sparse_Subdomain_Types_and_Values`). A simple subdomain
inherits its representation (regular or irregular) from its base domain
(or base subdomain). A sparse subdomain is always irregular, even if its
base domain is regular.

In all other respects, the two kinds of subdomain behave identically. In
this specification, “subdomain” refers to both simple and sparse
subdomains, unless it is specifically distinguished as one or the other.

   *Rationale*.

   Subdomains are provided in Chapel for a number of reasons: to
   facilitate the ability of the compiler or a reader to reason about
   the inter-relationship of distinct domain variables; to support the
   author’s ability to omit redundant domain mapping specifications; to
   support the compiler’s ability to reason about the relative alignment
   of multiple domains; and to improve the compiler’s ability to prove
   away bounds checks for array accesses.

.. _Simple_Subdomain_Types:

Simple Subdomain Types
~~~~~~~~~~~~~~~~~~~~~~

A simple subdomain type is specified using the following syntax:


.. code-block:: syntax

   simple-subdomain-type:
     `subdomain' ( domain-expression )

This declares that ``domain-expression`` is the parent domain of this
subdomain type. A simple subdomain specifies a subdomain with the same
underlying representation as its base domain.

   *Open issue*.

   An open semantic issue for subdomains is when a subdomain’s subset
   property should be re-verified once its parent domain is reassigned
   and whether this should be done aggressively or lazily.

Simple Subdomain Values
~~~~~~~~~~~~~~~~~~~~~~~

The value of a simple subdomain is the set of all index values that the
subdomain describes.

The default value of a simple subdomain type is the same as the default
value of its parent’s type (:ref:`Rectangular_Domain_Values`,
:ref:`Associative_Domain_Values`).

A simple subdomain variable can be initialized or assigned to with a
tuple of values of the parent’s ``idxType``. Indices can also be added
to or removed from a simple subdomain as described in
:ref:`Adding_and_Removing_Domain_Indices`. It is an error to
attempt to add an index to a subdomain that is not also a member of the
parent domain.

.. _Sparse_Subdomain_Types_and_Values:

Sparse Subdomain Types and Values
---------------------------------



.. code-block:: syntax

   sparse-subdomain-type:
     `sparse' `subdomain'[OPT] ( domain-expression )

This declaration creates a sparse subdomain. *Sparse subdomains* are
irregular domains that describe an arbitrary subset of a domain, even if
the parent domain is a regular domain. Sparse subdomains are useful in
Chapel for defining *sparse arrays* in which a single element value
(usually “zero”) occurs frequently enough that it is worthwhile to avoid
storing it redundantly. The set difference between a sparse subdomain’s
index set and that of parent domain is the set of indices for which the
sparse array will store this replicated value.
See :ref:`Sparse_Arrays` for details about sparse arrays.

Sparse Subdomain Types
~~~~~~~~~~~~~~~~~~~~~~

Each root domain type has a unique corresponding sparse subdomain type.
Sparse subdomains whose parent domains are also sparse subdomains share
the same type.

.. _Sparse_Domain_Values:

Sparse Subdomain Values
~~~~~~~~~~~~~~~~~~~~~~~

A sparse subdomain’s value is simply the set of all index values that
the domain describes. If the parent domain defines an iteration order
over its indices, the sparse subdomain inherits that order.

There is no literal syntax for a sparse subdomain. However, a variable
of a sparse subdomain type can be initialized using a tuple of values of
the parent domain’s index type.

The default value for a sparse subdomain value is the empty set.

   *Example*.

   The following code declares a two-dimensional dense domain ``D``,
   followed by a two dimensional sparse subdomain of ``D`` named
   ``SpsD``. Since ``SpsD`` is uninitialized, it will initially describe
   an empty set of indices from ``D``. 

   .. code-block:: chapel

      const D: domain(2) = {1..n, 1..n};
      var SpsD: sparse subdomain(D);

.. _Index_Types:

Domain Index Types
------------------

Each domain value has a corresponding compiler-provided *index type*
which can be used to represent values belonging to that domain’s index
set. Index types are described using the following syntax:



.. code-block:: syntax

   index-type:
     `index' ( domain-expression )

A variable with a given index type is constrained to take on only values
available within the domain on which it is defined. This restriction
allows the compiler to prove away the bound checking that code safety
considerations might otherwise require. Due to the subset relationship
between a base domain and its subdomains, a variable of an index type
defined with respect to a subdomain is also necessarily a valid index
into the base domain.

Since an index types are known to be legal for a given domain, it may
also afford the opportunity to represent that index using an optimized
format that doesn’t simply store the index variable’s value. This fact
could be used to support accelerated access to arrays declared over that
domain. For example, iteration over an index type could be implemented
using memory pointers and strides, rather than explicitly calculating
the offset of each index within the domain.

These potential optimizations may make it less expensive to index into
arrays using index type variables of their domains or subdomains.

In addition, since an index type is associated with a specific domain or
subdomain, it carries more semantic weight than a generic index. For
example, one could iterate over a rectangular domain with integer bounds
using an ``int(n)`` as the index variable. However, it would be more
precise to use a variable of the domain’s index type.

   *Open issue*.

   An open issue for index types is what the semantics should be for an
   index type value that is live across a modification to its domain’s
   index set—particularly one that shrinks the index set. Our hypothesis
   is that most stored indices will either have short lifespans or
   belong to constant or monotonically growing domains. But these
   semantics need to be defined nevertheless.

.. _Iteration_over_Domains:

Iteration Over Domains
----------------------

All domains support iteration via standard ``for``, ``forall``, and
``coforall`` loops. These loops iterate over all of the indices that the
domain describes. If the domain defines an iteration order of its
indices, then the indices are visited in that order.

The type of the iterator variable for an iteration over a domain named
``D`` is that domain’s index type, ``index(D)``.

.. _Domain_Arguments:

Domains as Arguments
--------------------

This section describes the semantics of passing domains as arguments to
functions.

Formal Arguments of Domain Type
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When a domain value is passed to a formal argument of compatible domain
type by default intent, it is passed by reference in order to preserve
the domain’s identity.

.. _Domain_Promotion_of_Scalar_Functions:

Domain Promotion of Scalar Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Domain values may be passed to a scalar function argument whose type
matches the domain’s index type. This results in a promotion of the
scalar function as defined in :ref:`Promotion`.

   *Example*.

   Given a function ``foo()`` that accepts real floating point values
   and an associative domain ``D`` of type ``domain(real)``, ``foo`` can
   be called with ``D`` as its actual argument which will result in the
   function being invoked for each value in the index set of ``D``.

..

   *Example*.

   Given an array ``A`` with element type ``int`` declared over a
   one-dimensional domain ``D`` with ``idxType`` ``int``, the array
   elements can be assigned their corresponding index values by writing:
   

   .. code-block:: chapel

      A = D;

   This is equivalent to: 

   .. code-block:: chapel

      forall (a,i) in zip(A,D) do
        a = i;

Domain Operations
-----------------

Chapel supplies predefined operators and functions that can be used to
manipulate domains. Unless otherwise noted, these operations are
applicable to a domain of any type, whether a base domain or a
subdomain.

.. _Domain_Assignment:

Domain Assignment
~~~~~~~~~~~~~~~~~

All domain types support domain assignment.



.. code-block:: syntax

   domain-expression:
     domain-literal
     domain-name
     domain-assignment-expression
     domain-striding-expression
     domain-alignment-expression
     domain-slice-expression

   domain-literal:
     rectangular-domain-literal
     associative-domain-literal

   domain-assignment-expression:
     domain-name = domain-expression

   domain-name:
     identifier

Domain assignment is by value and causes the target domain variable to
take on the index set of the right-hand side expression. In practice,
the right-hand side expression is often another domain value; a tuple of
ranges (for regular domains); or a tuple of indices or a loop that
enumerates indices (for irregular domains). If the domain variable being
assigned was used to declare arrays, these arrays are reallocated as
discussed in :ref:`Association_of_Arrays_to_Domains`.

It is an error to assign a stridable domain to an unstridable domain
without an explicit conversion.

   *Example*.

   The following three assignments show ways of assigning indices to a
   sparse domain, ``SpsD``. The first assigns the domain two index
   values, ``(1,1)`` and ``(n,n)``. The second assigns the domain all of
   the indices along the diagonal from
   ``(1,1)``\ :math:`\ldots`\ ``(n,n)``. The third invokes an iterator
   that is written to ``yield`` indices read from a file named
   “inds.dat”. Each of these assignments has the effect of replacing the
   previous index set with a completely new set of values. 

   .. code-block:: chapel

      SpsD = ((1,1), (n,n));
      SpsD = [i in 1..n] (i,i);
      SpsD = readIndicesFromFile("inds.dat");

.. _Domain_Striding:

Domain Striding
~~~~~~~~~~~~~~~

The ``by`` operator can be applied to a rectangular domain value in
order to create a strided rectangular domain value. The right-hand
operand to the ``by`` operator can either be an integral value or an
integral tuple whose size matches the domain’s rank.



.. code-block:: syntax

   domain-striding-expression:
     domain-expression `by' expression

The type of the resulting domain is the same as the original domain but
with ``stridable`` set to true. In the case of an integer stride value,
the value of the resulting domain is computed by applying the integer
value to each range in the value using the ``by`` operator. In the case
of a tuple stride value, the resulting domain’s value is computed by
applying each tuple component to the corresponding range using the
``by`` operator.

.. _Domain_Alignment:

Domain Alignment
~~~~~~~~~~~~~~~~

The ``align`` operator can be applied to a rectangular domain value in
order to change the alignment of a rectangular domain value. The
right-hand operand to the ``align`` operator can either be an integral
value or an integral tuple whose size matches the domain’s rank.



.. code-block:: syntax

   domain-alignment-expression:
     domain-expression `align' expression

The type of the resulting domain is the same as the original domain but
with ``stridable`` set to true. In the case of an integer alignment
value, the value of the resulting domain is computed by applying the
integer value to each range in the value using the ``align`` operator.
In the case of a tuple alignment value, the resulting domain’s value is
computed by applying each tuple component to the corresponding range
using the ``align`` operator.

.. _Domain_Slicing:

Domain Slicing
~~~~~~~~~~~~~~

Slicing is the application of an index set to a domain. It can be
written using either parentheses or square brackets. The index set can
be defined with either a domain or a list of ranges.



.. code-block:: syntax

   domain-slice-expression:
     domain-expression [ slicing-index-set ]
     domain-expression ( slicing-index-set )

   slicing-index-set:
     domain-expression
     range-expression-list

The result of slicing, or a *slice*, is a new domain value that
represents the intersection of the index set of the domain being sliced
and the index set being applied. The type and domain map of the slice
match the domain being sliced.

Slicing can also be performed on an array, resulting in aliasing a
subset of the array’s elements (:ref:`Array_Slicing`).

Domain-based Slicing
^^^^^^^^^^^^^^^^^^^^

If the brackets or parentheses contain a domain value, its index set is
applied for slicing.

   *Open issue*.

   Can we say that it is an alias in the case of sparse/associative?

.. _Range_Based_Slicing:

Range-based Slicing
^^^^^^^^^^^^^^^^^^^

When slicing rectangular domains or arrays, the brackets or parentheses
can contain a list of ``rank`` ranges. These ranges can either be
bounded or unbounded. When unbounded, they inherit their bounds from the
domain or array being sliced. The Cartesian product of the ranges’ index
sets is applied for slicing.

   *Example*.

   The following code declares a two dimensional rectangular domain
   ``D``, and then a number of subdomains of ``D`` by slicing into ``D``
   using bounded and unbounded ranges. The ``InnerD`` domain describes
   the inner indices of D, ``Col2OfD`` describes the 2nd column of
   ``D``, and ``AllButLastRow`` describes all of ``D`` except for the
   last row.

   

   .. code-block:: chapel

      const D: domain(2) = {1..n, 1..n},
            InnerD = D[2..n-1, 2..n-1],
            Col2OfD = D[.., 2..2],
            AllButLastRow = D[..n-1, ..];

.. _Rank_Change_Slicing:

Rank-Change Slicing
^^^^^^^^^^^^^^^^^^^

For multidimensional rectangular domains and arrays, substituting
integral values for one or more of the ranges in a range-based slice
will result in a domain or array of lower rank.

The result of a rank-change slice on an array is an alias to a subset of
the array’s elements as described
in :ref:`Rectangular_Array_Slicing`.

The result of rank-change slice on a domain is a subdomain of the domain
being sliced. The resulting subdomain’s type will be the same as the
original domain, but with a ``rank`` equal to the number of dimensions
that were sliced by ranges rather than integers.

.. _Count_Operator_Domains:

Count Operator
~~~~~~~~~~~~~~

The ``#`` operator can be applied to dense rectangular domains with a
tuple argument whose size matches the rank of the domain (or optionally
an integer in the case of a 1D domain). The operator is equivalent to
applying the ``#`` operator to the component ranges of the domain and
then using them to slice the domain as in
Section \ `21.8.4.2 <#Range_Based_Slicing>`__.

.. _Adding_and_Removing_Domain_Indices:

Adding and Removing Domain Indices
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

All irregular domain types support the ability to incrementally add and
remove indices from their index sets. This can either be done using
``add(i:idxType)`` and ``remove(i:idxType)`` methods on a domain
variable or by using the ``+=`` and ``-=`` assignment operators. It is
legal to add the same index to an irregular domain’s index set twice,
but illegal to remove an index that does not belong to the domain’s
index set.

   *Open issue*.

   These remove semantics seem dangerous in a parallel context; maybe
   add flags to both the method versions of the call that say whether
   they should balk or not? Or add exceptions...

As with normal domain assignments, arrays declared in terms of a domain
being modified in this way will be reallocated as discussed
in :ref:`Association_of_Arrays_to_Domains`.

Predefined Methods on Domains
-----------------------------

This section gives a brief description of the library functions provided
for Domains. These are categorized by the type of domain to which they
apply: all, regular or irregular. Within each subsection, entries are
listed in alphabetical order.

Methods on All Domain Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The methods in this subsection can be applied to any domain.



.. function:: proc domain.clear()

   Resets this domain’s index set to the empty set.

*Example (clearAssociativeDomain)*.

   This function provides a way to produce an empty associative domain.

   When run, the code 

   .. code-block:: chapel

      enum Counter { one, two, three };
      var D : domain ( Counter ) = {Counter.one, Counter.two};
      writeln("D has ", D.size, " indices.");
      D.clear();
      writeln("D has ", D.size, " indices.");

   prints out 

   .. code-block:: printoutput

      D has 2 indices.
      D has 0 indices.



.. function:: proc domain.dist : dmap

	 Returns the domain map that implements this domain

*Example (getDomainMap)*.

   In the code 

   .. code-block:: chapel

      use BlockDist;
      proc foo(d : domain) where isSubtype(d.dist.type, Block) {
        writeln("Block-distributed domain");
      }
      proc foo(d : domain) {
        writeln("Unknown distribution");
      }
      var D = {1..10} dmapped Block({1..10});
      foo(D);

   ``dist`` is used in a where-clause to determine the type of the
   argument’s distribution. The output is: 

   .. code-block:: printoutput

      Block-distributed domain



.. function:: proc domain.idxType type

   Returns the domain type’s ``idxType``.



.. function:: proc domain.indexOrder(i: index(domain)): idxType

   If ``i`` is a member of the domain, returns the ordinal value of ``i``
   using a total ordering of the domain’s indices using 0-based indexing.
   Otherwise, it returns ``(-1):idxType``. For rectangular domains, this
   ordering will be based on a row-major ordering of the indices; for other
   domains, the ordering may be implementation-defined and unstable as
   indices are added and removed from the domain.



.. function:: proc isIrregularDom(d: domain) param

   Returns a param ``true`` if the given domain is irregular, false
   otherwise.



.. function:: proc isRectangularDom(d: domain) param

   Returns a param ``true`` if the given domain is rectangular, false
   otherwise.



.. function:: proc isSparseDom(d: domain) param

   Returns a param ``true`` if the given domain is sparse, false otherwise.



.. function:: proc domain.member(i)

      Returns true if the given index ``i`` is a member of this domain’s index
      set, and false otherwise.

*Open issue*.

   We would like to call the type of i above idxType, but it’s not true
   for rectangular domains. That observation provides some motivation to
   normalize the behavior.




Methods on Regular Domains
~~~~~~~~~~~~~~~~~~~~~~~~~~

The methods described in this subsection can be applied to regular
domains only.



.. function:: proc domain.dim(d: int): range

   Returns the range of indices described by dimension ``d`` of the domain,
   where ``d`` is a value from ``0`` to ``rank-1``.

*Example*.

The code:

.. code-block:: chapel

   for i in D.dim(0) do
      for j in D.dim(1) do
         writeln(A(i,j));

iterates over the indices of a dense 2D domain ``D`` using two
nested loops, one per dimension.




.. function:: proc domain.dims(): rank*range

   Returns a tuple of ranges describing the dimensions of the domain.



.. code-block:: chapel

   proc domain.expand(off: integral): domain
   proc domain.expand(off: rank*integral): domain

Returns a new domain that is the current domain expanded in dimension
``d`` if ``off`` or ``off(d)`` is positive or contracted in dimension
``d`` if ``off`` or ``off(d)`` is negative.



.. code-block:: chapel

   proc domain.exterior(off: integral): domain
   proc domain.exterior(off: rank*integral): domain

Returns a new domain that is the exterior portion of the current domain
with ``off`` or ``off(d)`` indices for each dimension ``d``. If ``off``
or ``off(d)`` is negative, compute the exterior from the low bound of
the dimension; if positive, compute the exterior from the high bound.



.. code-block:: chapel

   proc domain.high: index(domain)

Returns the high index of the domain as a value of the domain’s index
type.



.. code-block:: chapel

   proc domain.interior(off: integral): domain
   proc domain.interior(off: rank*integral): domain

Returns a new domain that is the interior portion of the current domain
with ``off`` or ``off(d)`` indices for each dimension ``d``. If ``off``
or ``off(d)`` is negative, compute the interior from the low bound of
the dimension; if positive, compute the interior from the high bound.



.. function:: proc domain.low: index(domain)

   Returns the low index of the domain as a value of the domain’s index
   type.



.. function:: proc domain.rank param : int

   Returns the rank of the domain.



.. function:: proc domain.size: capType

   Returns the number of indices in the domain as a value of the capacity
   type.



.. function:: proc domain.stridable param : bool

   Returns whether or not the domain is stridable.



.. code-block:: chapel

   proc domain.stride: int(numBits(idxType)) where rank == 1
   proc domain.stride: rank*int(numBits(idxType))

Returns the stride of the domain as the domain’s stride type (for 1D
domains) or a tuple of the domain’s stride type (for multidimensional
domains).



.. code-block:: chapel

   proc domain.translate(off: integral): domain
   proc domain.translate(off: rank*integral): domain

Returns a new domain that is the current domain translated by ``off`` or
``off(d)`` for each dimension ``d``.

Methods on Irregular Domains
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following methods are available only on irregular domain types.



.. code-block:: chapel

   proc +(d: domain, i: index(d))
   proc +(i, d: domain) where i: index(d)

Adds the given index to the given domain. If the given index is already
a member of that domain, it is ignored.



.. function:: proc +(d1: domain, d2: domain)

   Merges the index sets of the two domain arguments.



.. function:: proc -(d: domain, i: index(d))

   Removes the given index from the given domain. It is an error if the
   domain does not contain the given index.



.. function:: proc -(d1: domain, d2: domain)

   Removes the indices in domain ``d2`` from those in ``d1``. It is an
   error if ``d2`` contains indices which are not also in ``d1``.



.. function:: proc requestCapacity(s: int)

   Resizes the domain internal storage to hold at least ``s`` indices.

.. [3]
   This is also known as row-major ordering.
