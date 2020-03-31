.. default-domain:: chpl

.. module:: ChapelArray
   :synopsis: Operations on Domains and Arrays.

Domain and Array Operations
===========================

=================================================
Distribution, Domain and Array Equality operators
=================================================

Equality operators are defined to test if two distributions
are equivalent or not:

.. code-block:: chapel

  dist1 == dist2
  dist1 != dist2

Or to test if two domains are equivalent or not:

.. code-block:: chapel

  dom1 == dom2
  dom1 != dom2

Arrays are promoted, so the result of the equality operators is
an array of booleans.  To get a single result use the ``equals``
method instead.

.. code-block:: chapel

  arr1 == arr2 // compare each element resulting in an array of booleans
  arr1 != arr2 // compare each element resulting in an array of booleans
  arr1.equals(arr2) // compare entire arrays resulting in a single boolean

========================================
Miscellaneous Domain and Array Operators
========================================

The domain count operator ``#``
-------------------------------

The ``#`` operator can be applied to dense rectangular domains
with a tuple argument whose size matches the rank of the domain
(or optionally an integer in the case of a 1D domain). The operator
is equivalent to applying the ``#`` operator to the component
ranges of the domain and then using them to slice the domain.

The array count operator ``#``
------------------------------
The ``#`` operator can be applied to dense rectangular arrays
with a tuple argument whose size matches the rank of the array
(or optionally an integer in the case of a 1D array). The operator
is equivalent to applying the ``#`` operator to the array's domain
and using the result to slice the array.

The array swap operator ``<=>``
-------------------------------
The ``<=>`` operator can be used to swap the contents of two arrays
with the same shape.

================================================
Set Operations on Associative Domains and Arrays
================================================

Associative domains and arrays support a number of operators for
set manipulations.  The supported set operators are:

  =======  ====================
  \+ , \|  Union
  &        Intersection
  \-       Difference
  ^        Symmetric Difference
  =======  ====================

Consider the following code where ``A`` and ``B`` are associative arrays:

.. code-block:: chapel

  var C = A op B;

The result ``C`` is a new associative array backed by a new associative
domain. The domains of ``A`` and ``B`` are not modified by ``op``.

There are also ``op=`` variants that store the result into the first operand.

Consider the following code where ``A`` and ``B`` are associative arrays:

.. code-block:: chapel

  A op= B;

``A`` must not share its domain with another array, otherwise the program
will halt with an error message.

For the ``+=`` and ``|=`` operators, the value from ``B`` will overwrite
the existing value in ``A`` when indices overlap.

==================================================
Parallel Safety with respect to Arrays and Domains
==================================================

Users must take care when applying operations to arrays and domains
concurrently from distinct tasks.  For instance, if one task is
modifying the index set of a domain while another task is operating
on either the domain itself or an array declared over that domain,
this represents a race and could have arbitrary consequences
including incorrect results and program crashes.  While making
domains and arrays safe with respect to such concurrent operations
would be appealing, Chapel's current position is that such safety
guarantees would be prohibitively expensive.

Chapel arrays do support concurrent reads, writes, iterations, and
operations as long as their domains are not being modified
simultaneously.  Such operations are subject to Chapel's memory
consistency model like any other memory accesses.  Similarly, tasks
may make concurrent queries and iterations on a domain as long as
another task is not simultaneously modifying the domain's index
set.

By default, associative domains permit multiple tasks
to modify their index sets concurrently.  This adds some amount of
overhead to these operations.  If the user knows that all such
modifications will be done serially or in a parallel-safe context,
the overheads can be avoided by setting ``parSafe`` to ``false`` in
the domain's type declaration.  For example, the following
declaration creates an associative domain of strings where the
implementation will do nothing to ensure that simultaneous
modifications to the domain are parallel-safe:

  .. code-block:: chapel

    var D: domain(string, parSafe=false);

As with any other domain type, it is not safe to access an
associative array while its domain is changing, regardless of
whether ``parSafe`` is set to ``true`` or ``false``.

===========================================
Functions and Methods on Arrays and Domains
===========================================



.. function:: proc isRectangularDom(d: domain) param

   Return true if the argument ``d`` is a rectangular domain.
   Otherwise return false.  

.. function:: proc isRectangularArr(a: []) param

   Return true if the argument ``a`` is an array with a rectangular
   domain.  Otherwise return false. 

.. function:: proc isIrregularDom(d: domain) param

   Return true if ``d`` is an irregular domain; e.g. is not rectangular.
   Otherwise return false. 

.. function:: proc isIrregularArr(a: []) param

   Return true if ``a`` is an array with an irregular domain; e.g. not
   rectangular. Otherwise return false. 

.. function:: proc isAssociativeDom(d: domain) param

   Return true if ``d`` is an associative domain. Otherwise return false. 

.. function:: proc isAssociativeArr(a: []) param

   Return true if ``a`` is an array with an associative domain. Otherwise
   return false. 

.. function:: proc isSparseDom(d: domain) param

   Return true if ``d`` is a sparse domain. Otherwise return false. 

.. function:: proc isSparseArr(a: []) param

   Return true if ``a`` is an array with a sparse domain. Otherwise
   return false. 

.. type:: domain

   The domain type 


   .. method:: proc init(_pid: int, _instance, _unowned: bool)

   .. method:: proc init(value)

   .. method:: proc init(d: _distribution, param rank: int, type idxType = int, param stridable: bool = false)

   .. method:: proc init(d: _distribution, param rank: int, type idxType = int, param stridable: bool = false, ranges: rank*(range(idxType, BoundedRangeType.bounded, stridable)))

   .. method:: proc init(d: _distribution, type idxType, param parSafe: bool = true)

   .. method:: proc init(d: _distribution, dom: domain)

   .. method:: proc init=(const ref other: domain)

   .. method:: proc init=(const ref other: domain)

   .. method:: proc dist

      Return the domain map that implements this domain 

   .. method:: proc rank param

      Return the number of dimensions in this domain 

   .. method:: proc idxType type

      Return the type of the indices of this domain 

   .. method:: proc intIdxType type

      The ``idxType`` as represented by an integer type.  When
      ``idxType`` is an enum type, this evaluates to ``int``.
      Otherwise, it evaluates to ``idxType``. 

   .. method:: proc stridable param

      Return true if this is a stridable domain 

   .. itermethod:: iter these()

      Yield the domain indices 

   .. method:: proc this(i: integral ...rank)

   .. method:: proc dims()

      
      Return a tuple of ranges describing the bounds of a rectangular domain.
      For a sparse domain, return the bounds of the parent domain.
      

   .. method:: proc dim(d: int)

      
      Return a range representing the boundary of this
      domain in a particular dimension.
      

   .. method:: proc shape

      Return a tuple of :proc:`intIdxType` describing the size of each dimension.
      For a sparse domain, return the shape of the parent domain.

   .. method:: proc clear()

   .. method:: proc add(i)

      Add index ``i`` to this domain. This method is also available
      as the ``+=`` operator.
      
      The domain must be irregular.
      

   .. method:: proc makeIndexBuffer(size: int)

      
      Creates an index buffer which can be used for faster index addition.
      
      For example, instead of:
      
        .. code-block:: chapel
      
           var spsDom: sparse subdomain(parentDom);
           for i in someIndexIterator() do
             spsDom += i;
      
      You can use `SparseIndexBuffer` for better performance:
      
        .. code-block:: chapel
      
           var spsDom: sparse subdomain(parentDom);
           var idxBuf = spsDom.makeIndexBuffer(size=N);
           for i in someIndexIterator() do
             idxBuf.add(i);
           idxBuf.commit();
      
      The above snippet will create a buffer of size N indices, and will
      automatically commit indices to the sparse domain as the buffer fills up.
      Indices are also committed when the buffer goes out of scope.
      
        .. note::
      
           The interface and implementation is not stable and may change in the
           future.
      
      :arg size: Size of the buffer in number of indices.
      :type size: int
      

   .. method:: proc bulkAdd(inds: [] _value.rank*(_value.idxType), dataSorted = false, isUnique = false, preserveInds = true, addOn = : nilable locale)

      
      Adds indices in ``inds`` to this domain in bulk.
      
      For sparse domains, an operation equivalent to this method is available
      with the ``+=`` operator, where the right-hand-side is an array. However,
      in that case, default values will be used for the flags ``dataSorted``,
      ``isUnique``, and ``preserveInds``. This method is available because in
      some cases, expensive operations can be avoided by setting those flags.
      To do so, ``bulkAdd`` must be called explicitly (instead of ``+=``).
      
      .. note::
      
        Right now, this method and the corresponding ``+=`` operator are
        only available for sparse domains. In the future, we expect that
        these methods will be available for all irregular domains.
      
      :arg inds: Indices to be added. ``inds`` can be an array of
                 ``rank*idxType`` or an array of ``idxType`` for
                 1-D domains.
      
      :arg dataSorted: ``true`` if data in ``inds`` is sorted.
      :type dataSorted: bool
      
      :arg isUnique: ``true`` if data in ``inds`` has no duplicates.
      :type isUnique: bool
      
      :arg preserveInds: ``true`` if data in ``inds`` needs to be preserved.
      :type preserveInds: bool
      
      :arg addOn: The locale where the indices should be added. Default value
                  is ``nil`` which indicates that locale is unknown or there
                  are more than one.
      :type addOn: locale
      
      :returns: Number of indices added to the domain
      :rtype: int
      

   .. method:: proc remove(i)

      Remove index ``i`` from this domain 

   .. method:: proc requestCapacity(i)

      Request space for a particular number of values in an
      domain.
      
      Currently only applies to associative domains.
      

   .. method:: proc size

      Return the number of indices in this domain 

   .. method:: proc numIndices

      Deprecated - please use :proc:`size`. 

   .. method:: proc low

      Return the lowest index in this domain 

   .. method:: proc high

      Return the highest index in this domain 

   .. method:: proc stride

      Return the stride of the indices in this domain 

   .. method:: proc alignment

      Return the alignment of the indices in this domain 

   .. method:: proc first

      Return the first index in this domain 

   .. method:: proc last

      Return the last index in this domain 

   .. method:: proc alignedLow

      Return the low index in this domain factoring in alignment 

   .. method:: proc alignedHigh

      Return the high index in this domain factoring in alignment 

   .. method:: proc contains(i: _value.idxType ...rank)

      Return true if this domain contains ``i``. Otherwise return false.
      For sparse domains, only indices with a value are considered
      to be contained in the domain.
      

   .. method:: proc member(i: _value.idxType ...rank)

      Deprecated - please use :proc:`contains`. 

   .. method:: proc isSubset(super: domain)

      Return true if this domain is a subset of ``super``. Otherwise
      returns false. 

   .. method:: proc isSuper(sub: domain)

      Return true if this domain is a superset of ``sub``. Otherwise
      returns false. 

   .. method:: proc expand(off: rank*(intIdxType))

      Return a new domain that is the current domain expanded by
      ``off(d)`` in dimension ``d`` if ``off(d)`` is positive or
      contracted by ``off(d)`` in dimension ``d`` if ``off(d)``
      is negative. 

   .. method:: proc expand(off: intIdxType)

      Return a new domain that is the current domain expanded by
      ``off`` in all dimensions if ``off`` is positive or contracted
      by ``off`` in all dimensions if ``off`` is negative. 

   .. method:: proc exterior(off: rank*(intIdxType))

      Return a new domain that is the exterior portion of the
      current domain with ``off(d)`` indices for each dimension ``d``.
      If ``off(d)`` is negative, compute the exterior from the low
      bound of the dimension; if positive, compute the exterior
      from the high bound. 

   .. method:: proc exterior(off: intIdxType)

      Return a new domain that is the exterior portion of the
      current domain with ``off`` indices for each dimension.
      If ``off`` is negative, compute the exterior from the low
      bound of the dimension; if positive, compute the exterior
      from the high bound. 

   .. method:: proc interior(off: rank*(intIdxType))

      Return a new domain that is the interior portion of the
      current domain with ``off(d)`` indices for each dimension
      ``d``. If ``off(d)`` is negative, compute the interior from
      the low bound of the dimension; if positive, compute the
      interior from the high bound. 

   .. method:: proc interior(off: intIdxType)

      Return a new domain that is the interior portion of the
      current domain with ``off`` indices for each dimension.
      If ``off`` is negative, compute the interior from the low
      bound of the dimension; if positive, compute the interior
      from the high bound. 

   .. method:: proc translate(off)

      Return a new domain that is the current domain translated by
      ``off(d)`` in each dimension ``d``. 

   .. method:: proc translate(off)

      Return a new domain that is the current domain translated by
      ``off`` in each dimension. 

   .. method:: proc isEmpty(): bool

      Return true if the domain has no indices 

   .. method:: proc localSlice(r ...rank)

      
      Return a local view of the sub-array (slice) defined by the provided
      range(s), halting if the slice contains elements that are not local.
      
      Indexing into this local view is cheaper, because the indices are known
      to be local.
      

   .. method:: proc localSlice(d: domain)

      
      Return a local view of the sub-array (slice) defined by the provided
      domain, halting if the slice contains elements that are not local.
      
      Indexing into this local view is cheaper, because the indices are known
      to be local.
      

   .. itermethod:: iter sorted(comparator: ?t = chpl_defaultComparator())

      Yield the domain indices in sorted order 

   .. method:: proc safeCast(type t: domain)

      Cast a rectangular domain to another rectangular domain type.
      If the old type is stridable and the new type is not stridable,
      ensure that the stride was 1.
      

   .. method:: proc targetLocales()

      
      Return an array of locales over which this domain has been distributed.
      

   .. method:: proc hasSingleLocalSubdomain() param

      Return true if the local subdomain can be represented as a single
      domain. Otherwise return false. 

   .. method:: proc localSubdomain(loc: locale = here)

      
      Return the subdomain that is local to `loc`.
      
      :arg loc: indicates the locale for which the query should take
                place (defaults to `here`)
      :type loc: locale
      

   .. itermethod:: iter localSubdomains(loc: locale = here)

      
      Yield the subdomains that are local to `loc`.
      
      :arg loc: indicates the locale for which the query should take
                place (defaults to `here`)
      :type loc: locale
      

.. type:: array

   The array type 


   .. method:: proc eltType type

      The type of elements contained in the array 

   .. method:: proc idxType type

      The type of indices used in the array's domain 

   .. method:: proc rank param

      The number of dimensions in the array 

   .. method:: proc indices

      return the array's indices as its domain 

   .. itermethod:: iter these() ref

      Yield the array elements 

   .. method:: proc numElements

      Deprecated - please use :proc:`size`. 

   .. method:: proc size

      Return the number of elements in the array 

   .. method:: proc reindex(newDomain: domain)

      
      Return an array view over a new domain. The new domain must be
      of the same rank and size as the original array's domain.
      
      For example:
      
      .. code-block:: chapel
      
         var A: [1..10] int;
         const D = {6..15};
         ref reA = A.reindex(D);
         reA[6] = 1; // updates A[1]
      

   .. method:: proc reindex(newDims ...)

      
      Return an array view over a new domain defined implicitly
      by one or more `newDims`, which must be ranges. The new domain must be
      of the same rank and size as the original array's domain.
      
      For example:
      
      .. code-block:: chapel
      
         var A: [3..4, 5..6] int;
         ref reA = A.reindex(13..14, 15..16);
         reA[13,15] = 1; // updates A[3,5]
      

   .. method:: proc IRV

      Return the Implicitly Represented Value for sparse arrays 

   .. itermethod:: iter sorted(comparator: ?t = chpl_defaultComparator())

      Yield the array elements in sorted order. 

   .. method:: proc targetLocales()

   .. method:: proc hasSingleLocalSubdomain() param

      Return true if the local subdomain can be represented as a single
      domain. Otherwise return false. 

   .. method:: proc localSubdomain(loc: locale = here)

      
      Return the subdomain that is local to `loc`.
      
      :arg loc: indicates the locale for which the query should take
                place (defaults to `here`)
      :type loc: locale
      

   .. itermethod:: iter localSubdomains(loc: locale = here)

      
      Yield the subdomains that are local to `loc`.
      
      :arg loc: indicates the locale for which the query should take
                place (defaults to `here`)
      :type loc: locale
      

   .. method:: proc isEmpty(): bool

      Return true if the array has no elements 

   .. method:: proc back()

      Return the last element in the array. The array must be a
      rectangular 1-D array.
      

   .. method:: proc front()

      Return the first element in the array. The array must be a
      rectangular 1-D array.
      

   .. method:: proc reverse()

      Reverse the order of the values in the array. 

   .. method:: proc find(val: this.eltType): (bool, index(this.domain))

      Return a tuple containing ``true`` and the index of the first
      instance of ``val`` in the array, or if ``val`` is not found, a
      tuple containing ``false`` and an unspecified value is returned.
      

   .. method:: proc count(val: this.eltType): int

      Return the number of times ``val`` occurs in the array. 

   .. method:: proc shape

      Return a tuple of integers describing the size of each dimension.
      For a sparse array, returns the shape of the parent domain.

.. method:: proc array.equals(that: [] ): bool

   Return true if all this array is the same size and shape
   as argument ``that`` and all elements of this array are
   equal to the corresponding element in ``that``. Otherwise
   return false. 

.. function:: proc isDmapType(type t) param

   Return true if ``t`` is a domain map type. Otherwise return false. 

.. function:: proc isDmapValue(e) param

   Return true if ``e`` is a domain map. Otherwise return false. 

.. function:: proc isDomainType(type t) param

   Return true if ``t`` is a domain type. Otherwise return false. 

.. function:: proc isDomainValue(e) param

   Return true if ``e`` is a domain. Otherwise return false. 

.. function:: proc isArrayType(type t) param

   Return true if ``t`` is an array type. Otherwise return false. 

.. function:: proc isArrayValue(e) param

   Return true if ``e`` is an array. Otherwise return false. 

.. function:: proc reshape(A: [], D: domain)

   Return a copy of the array ``A`` containing the same values but
   in the shape of the domain ``D``. The number of indices in the
   domain must equal the number of elements in the array. The
   elements of ``A`` are copied into the new array using the
   default iteration orders over ``D`` and ``A``.  

