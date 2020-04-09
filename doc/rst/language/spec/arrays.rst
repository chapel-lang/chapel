.. _Chapter-Arrays:

Arrays
======

An *array* is a map from a domain’s indices to a collection of variables
of homogeneous type. Since Chapel domains support a rich variety of
index sets, Chapel arrays are also richer than the traditional linear or
rectilinear array types in conventional languages. Like domains, arrays
may be distributed across multiple locales without explicitly
partitioning them using Chapel’s Domain
Maps (:ref:`Chapter-Domain_Maps`).

.. _Array_Types:

Array Types
-----------

An array type is specified by the identity of the domain that it is
declared over and the element type of the array. Array types are given
by the following syntax:



.. code-block:: syntax

   array-type:
     [ domain-expression ] type-expression

The ``domain-expression`` must specify a domain that the array can be
declared over. If the ``domain-expression`` is a domain literal, the
curly braces around the literal may be omitted.

   *Example (decls.chpl)*.

   In the code 

   .. code-block:: chapel

      const D: domain(2) = {1..10, 1..10};
      var A: [D] real;

   

   .. BLOCK-test-chapelpost

      writeln(D);
      writeln(A);

   

   .. BLOCK-test-chapeloutput

      {1..10, 1..10}
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0

   ``A`` is declared to be an arithmetic array over rectangular domain
   ``D`` with elements of type ``real``. As a result, it represents a
   2-dimensional :math:`10 \times 10` real floating point variables
   indexed using the indices
   :math:`(1, 1), (1, 2), \ldots, (1, 10), (2, 1), \ldots, (10, 10)`.

An array’s element type can be referred to using the member symbol
``eltType``.

   *Example (eltType.chpl)*.

   In the following example, ``x`` is declared to be of type ``real``
   since that is the element type of array ``A``. 

   .. BLOCK-test-chapelpre

      const D: domain(2) = {1..10, 1..10};

   

   .. code-block:: chapel

      var A: [D] real;
      var x: A.eltType;

   

   .. BLOCK-test-chapelpost

      writeln(x.type:string);
      writeln(A.eltType:string);

   

   .. BLOCK-test-chapeloutput

      real(64)
      real(64)

.. _Array_Values:

Array Values
------------

An array’s value is the collection of its elements’ values. Assignments
between array variables are performed by value as described
in :ref:`Array_Assignment`. Chapel semantics are defined so that
the compiler will never need to insert temporary arrays of the same size
as a user array variable.

Array literal values can be either rectangular or associative,
corresponding to the underlying domain which defines its indices.



.. code-block:: syntax

   array-literal:
     rectangular-array-literal
     associative-array-literal

.. _Rectangular_Array_Literals:

Rectangular Array Literals
~~~~~~~~~~~~~~~~~~~~~~~~~~

Rectangular array literals are specified by enclosing a comma-separated
list of expressions representing values in square brackets. A 0-based
domain will automatically be generated for the given array literal. The
type of the array’s values will be the type of the first element listed.
A trailing comma is allowed.



.. code-block:: syntax

   rectangular-array-literal:
     [ expression-list ]
     [ expression-list , ]

..

   *Example (adecl-literal.chpl)*.

   The following example declares a 5 element rectangular array literal
   containing strings, then subsequently prints each string element to
   the console. 

   .. code-block:: chapel

      var A = ["1", "2", "3", "4", "5"];

      for i in 0..4 do
        writeln(A[i]);

   

   .. BLOCK-test-chapeloutput

      1
      2
      3
      4
      5

   *Future*.

   Provide syntax which allows users to specify the domain for a
   rectangular array literal.

..

   *Future*.

   Determine the type of a rectangular array literal based on the most
   promoted type, rather than the first element’s type.

   *Example (decl-with-anon-domain.chpl)*.

   The following example declares a 2-element array ``A`` containing
   3-element arrays of real numbers. ``A`` is initialized using array
   literals. 

   .. code-block:: chapel

      var A: [1..2] [1..3] real = [[1.1, 1.2, 1.3], [2.1, 2.2, 2.3]];

   

   .. BLOCK-test-chapelpost

      writeln(A.domain);

   

   .. BLOCK-test-chapeloutput

      {1..2}

..

   *Open issue*.

   We would like to differentiate syntactically between array literals
   for an array of arrays and a multi-dimensional array.

An rectangular array’s default value is for each array element to be
initialized to the default value of the element type.

.. _Associative_Array_Literals:

Associative Array Literals
~~~~~~~~~~~~~~~~~~~~~~~~~~

Associative array values are specified by enclosing a comma separated
list of index-to-value bindings within square brackets. It is expected
that the indices in the listing match in type and, likewise, the types
of values in the listing also match. A trailing comma is allowed.



.. code-block:: syntax

   associative-array-literal:
     [ associative-expr-list ]
     [ associative-expr-list , ]

   associative-expr-list:
     index-expr => value-expr
     index-expr => value-expr, associative-expr-list

   index-expr:
     expression

   value-expr:
     expression

..

   *Open issue*.

   Currently it is not possible to use other associative domains as
   values within an associative array literal.

   *Example (adecl-assocLiteral.chpl)*.

   The following example declares a 5 element associative array literal
   which maps integers to their corresponding string representation. The
   indices and their corresponding values are then printed. 

   .. code-block:: chapel

      var A = [1 => "one", 10 => "ten", 3 => "three", 16 => "sixteen"];

      for da in zip (A.domain, A) do
        writeln(da);

   

   .. BLOCK-test-chapelprediff

      #!/usr/bin/env sh
      testname=$1
      outfile=$2
      sort $outfile > $outfile.2
      mv $outfile.2 $outfile

   

   .. BLOCK-test-chapeloutput

      (1, one)
      (10, ten)
      (16, sixteen)
      (3, three)

.. _Array_Runtime_Representation:

Runtime Representation of Array Values
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The runtime representation of an array in memory is controlled by its
domain’s domain map. Through this mechanism, users can reason about and
control the runtime representation of an array’s elements. See
 :ref:`Chapter-Domain_Maps` for more details.

.. _Array_Indexing:

Array Indexing
--------------

Arrays can be indexed using index values from the domain over which they
are declared. Array indexing is expressed using either parentheses or
square brackets. This results in a reference to the element that
corresponds to the index value.

   *Example (array-indexing.chpl)*.

   Given: 

   .. code-block:: chapel

      var A: [1..10] real;

   the first two elements of A can be assigned the value 1.2 and 3.4
   respectively using the assignment: 

   .. code-block:: chapel

      A(1) = 1.2;
      A[2] = 3.4;

   

   .. BLOCK-test-chapelpost

      writeln(A.domain);
      writeln(A);

   

   .. BLOCK-test-chapeloutput

      {1..10}
      1.2 3.4 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0

Except for associative arrays, if an array is indexed using an index
that is not part of its domain’s index set, the reference is considered
out-of-bounds and a runtime error will occur, halting the program.

.. _Rectangular_Array_Indexing:

Rectangular Array Indexing
~~~~~~~~~~~~~~~~~~~~~~~~~~

Since the indices for multidimensional rectangular domains are tuples,
for convenience, rectangular arrays can be indexed using the list of
integer values that make up the tuple index. This is semantically
equivalent to creating a tuple value out of the integer values and using
that tuple value to index the array. For symmetry, 1-dimensional
rectangular arrays can be accessed using 1-tuple indices even though
their index type is an integral value. This is semantically equivalent
to de-tupling the integral value from the 1-tuple and using it to index
the array.

   *Example (array-indexing-2.chpl)*.

   Given: 

   .. code-block:: chapel

      var A: [1..5, 1..5] real;
      var ij: 2*int = (1, 1);

   the elements of array A can be indexed using any of the following
   idioms: 

   .. code-block:: chapel

      A(ij) = 1.1;
      A((1, 2)) = 1.2;
      A(1, 3) = 1.3;
      A[ij] = -1.1;
      A[(1, 4)] = 1.4;
      A[1, 5] = 1.5;

   

   .. BLOCK-test-chapelpost

      writeln(ij);
      writeln(A);

   

   .. BLOCK-test-chapeloutput

      (1, 1)
      -1.1 1.2 1.3 1.4 1.5
      0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0
      0.0 0.0 0.0 0.0 0.0

..

   *Example (index-using-var-arg-tuple.chpl)*.

   The code 

   .. code-block:: chapel

      proc f(A: [], is...)
        return A(is);

   

   .. BLOCK-test-chapelpost

      var B: [1..5] int;
      [i in 1..5] B(i) = i;
      var C: [1..5,1..5] int;
      [(i,j) in {1..5,1..5}] C(i,j) = i+i*j;
      writeln(f(B, 3));
      writeln(f(C, 3, 3));

   

   .. BLOCK-test-chapeloutput

      3
      12

   defines a function that takes an array as the first argument and a
   variable-length argument list. It then indexes into the array using
   the tuple that captures the actual arguments. This function works
   even for one-dimensional arrays because one-dimensional arrays can be
   indexed into by 1-tuples.

.. _Associative_Array_Indexing:

Associative Array Indexing
~~~~~~~~~~~~~~~~~~~~~~~~~~

Indices can be added to associative arrays through the array’s domain.

   *Example (assoc-add-index.chpl)*.

   Given: 

   .. code-block:: chapel

      var D : domain(string);
      var A : [D] int;

   the array A initially contains no elements. We can change that by
   adding indices to the domain D: 

   .. code-block:: chapel

      D.add("a");
      D.add("b");

   The array A can now be indexed with indices "a" and "b":

   

   .. code-block:: chapel

      A["a"] = 1;
      A["b"] = 2;
      var x = A["a"];

.. _Iteration_over_Arrays:

Iteration over Arrays
---------------------

All arrays support iteration via standard ``for``, ``forall`` and
``coforall`` loops. These loops iterate over all of the array elements
as described by its domain. A loop of the form:



.. code-block:: chapel

   [for|forall|coforall] a in A do
     ...a...

is semantically equivalent to:



.. code-block:: chapel

   [for|forall|coforall] i in A.domain do
     ...A[i]...

The iterator variable for an array iteration is a reference to the array
element type.

.. _Array_Assignment:

Array Assignment
----------------

Array assignment is by value. Arrays can be assigned arrays, ranges,
domains, iterators, or tuples as long as the two expressions are
compatible in terms of number of dimensions and shape.

   *Example (assign.chpl)*.

   If ``A`` is an array variable and ``B`` is an expression of array,
   range, domain, or tuple type, or an iterator, then the assignment
   

   .. BLOCK-test-chapelpre

      var A: [1..3] int;
      var B: [1..3] int;
      A = -1;
      B = 1;

   

   .. BLOCK-test-chapelnoprint

      writeln(A);
      writeln(B);

   

   .. code-block:: chapel

      A = B;

   

   .. BLOCK-test-chapelnoprint

      writeln(A);
      writeln(B);
      A = -2;
      B = 2;
      writeln(A);
      writeln(B);

   is equivalent to 

   .. code-block:: chapel

      [(a,b) in zip(A,B)] a = b;

   

   .. BLOCK-test-chapelpost

      writeln(A);
      writeln(B);

   

   .. BLOCK-test-chapeloutput

      -1 -1 -1
      1 1 1
      1 1 1
      1 1 1
      -2 -2 -2
      2 2 2
      2 2 2
      2 2 2

   If the zipper iteration is illegal, then the assignment is illegal.
   This means, for example, that a range cannot be assigned to a
   multidimensional rectangular array because the two expressions don’t
   match in shape and can’t be zipped together. Notice that the
   assignment is implemented using parallelism when possible, and
   serially otherwise.

Arrays can be assigned tuples of values of their element type if the
tuple contains the same number of elements as the array. For
multidimensional arrays, the tuple must be a nested tuple such that the
nesting depth is equal to the rank of the array and the shape of this
nested tuple must match the shape of the array. The values are assigned
element-wise.

Arrays can also be assigned single values of their element type. In this
case, each element in the array is assigned this value.

   *Example (assign-2.chpl)*.

   If ``e`` is an expression of the element type of the array or a type
   that can be implicitly converted to the element type of the array,
   then the assignment 

   .. BLOCK-test-chapelpre

      var A: [1..4] uint;
      writeln(A);
      var e: uint = 77;

   

   .. code-block:: chapel

      A = e;

   

   .. BLOCK-test-chapelnoprint

      writeln(A);
      e = 33;

   is equivalent to 

   .. code-block:: chapel

      forall a in A do
        a = e;

   

   .. BLOCK-test-chapelpost

      writeln(A);

   

   .. BLOCK-test-chapeloutput

      0 0 0 0
      77 77 77 77
      33 33 33 33

.. _Array_Slicing:

Array Slicing
-------------

An array can be sliced using a domain that has the same type as the
domain over which it was declared. The result of an array slice is an
alias to the subset of the array elements from the original array
corresponding to the slicing domain’s index set.

   *Example (slicing.chpl)*.

   Given the definitions 

   .. BLOCK-test-chapelpre

      config const n = 2;

   

   .. code-block:: chapel

      var OuterD: domain(2) = {0..n+1, 0..n+1};
      var InnerD: domain(2) = {1..n, 1..n};
      var A, B: [OuterD] real;

   

   .. BLOCK-test-chapelnoprint

      writeln(OuterD);
      writeln(InnerD);
      B = 1;

   the assignment given by 

   .. code-block:: chapel

      A[InnerD] = B[InnerD];

   

   .. BLOCK-test-chapelpost

      writeln(A);
      writeln(B);

   

   .. BLOCK-test-chapeloutput

      {0..3, 0..3}
      {1..2, 1..2}
      0.0 0.0 0.0 0.0
      0.0 1.0 1.0 0.0
      0.0 1.0 1.0 0.0
      0.0 0.0 0.0 0.0
      1.0 1.0 1.0 1.0
      1.0 1.0 1.0 1.0
      1.0 1.0 1.0 1.0
      1.0 1.0 1.0 1.0

   assigns the elements in the interior of ``B`` to the elements in the
   interior of ``A``.

.. _Rectangular_Array_Slicing:

Rectangular Array Slicing
~~~~~~~~~~~~~~~~~~~~~~~~~

A rectangular array can be sliced by any rectangular domain that is a
subdomain of the array’s defining domain. If the subdomain relationship
is not met, an out-of-bounds error will occur. The result is a subarray
whose indices are those of the slicing domain and whose elements are an
alias of the original array’s.

Rectangular arrays also support slicing by ranges directly. If each
dimension is indexed by a range, this is equivalent to slicing the array
by the rectangular domain defined by those ranges. These range-based
slices may also be expressed using partially unbounded or completely
unbounded ranges. This is equivalent to slicing the array’s defining
domain by the specified ranges to create a subdomain as described
in :ref:`Array_Slicing` and then using that subdomain to slice
the array.

.. _Rectangular_Array_Slicing_With_Rank_Change:

Rectangular Array Slicing with a Rank Change
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For multidimensional rectangular arrays, slicing with a rank change is
supported by substituting integral values within a dimension’s range for
an actual range. The resulting array will have a rank less than the
rectangular array’s rank and equal to the number of ranges that are
passed in to take the slice.

   *Example (array-decl.chpl)*.

   Given an array 

   .. BLOCK-test-chapelpre

      config const n = 4;

   

   .. code-block:: chapel

      var A: [1..n, 1..n] int;

   

   .. BLOCK-test-chapelpost

      writeln(A);

   

   .. BLOCK-test-chapeloutput

      0 0 0 0
      0 0 0 0
      0 0 0 0
      0 0 0 0

   the slice ``A[1..n, 1]`` is a one-dimensional array whose elements
   are the first column of ``A``.

.. _Count_Operator_Arrays:

Count Operator
--------------

The ``#`` operator can be applied to dense rectangular arrays with a
tuple argument whose size matches the rank of the array (or optionally
an integer in the case of a 1D array). The operator is equivalent to
applying the ``#`` operator to the array’s domain and using the result
to slice the array as described in
Section \ `22.6.1 <#Rectangular_Array_Slicing>`__.

.. _Array_Arguments_To_Functions:

Array Arguments to Functions
----------------------------

By default, arrays are passed to function by ``ref`` or ``const ref``
depending on whether or not the formal argument is modified. The ``in``,
``inout``, and ``out`` intent can create copies of arrays.

When a formal argument has array type, the element type of the array can
be omitted and/or the domain of the array can be queried or omitted. In
such cases, the argument is generic and is discussed
in :ref:`Formal_Arguments_of_Generic_Array_Types`.

If a formal array argument specifies a domain as part of its type
signature, the domain of the actual argument must represent the same
index set. If the formal array’s domain was declared using an explicit
domain map, the actual array’s domain must use an equivalent domain map.

.. _Array_Promotion_of_Scalar_Functions:

Array Promotion of Scalar Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Arrays may be passed to a scalar function argument whose type matches
the array’s element type. This results in a promotion of the scalar
function as defined in :ref:`Promotion`.

   *Example (whole-array-ops.chpl)*.

   Whole array operations is a special case of array promotion of scalar
   functions. In the code 

   .. BLOCK-test-chapelpre

      var A, B, C: [1..3] real;
      A = -1;
      B = 2;
      C = 3;

   

   .. code-block:: chapel

      A = B + C;

   

   .. BLOCK-test-chapelpost

      writeln(A);

   

   .. BLOCK-test-chapeloutput

      5.0 5.0 5.0

   if ``A``, ``B``, and ``C`` are arrays, this code assigns each element
   in ``A`` the element-wise sum of the elements in ``B`` and ``C``.

.. _Returning_Arrays_from_Functions:

Returning Arrays from Functions
-------------------------------

Arrays return by value by default. The ``ref`` and ``const ref`` return
intents can be used to return a reference to an array.

Similarly to array arguments, the element type and/or domain of an array
return type can be omitted.

.. _Sparse_Arrays:

Sparse Arrays
-------------

Sparse arrays in Chapel are those whose domain is sparse. A sparse array
differs from other array types in that it stores a single value
corresponding to multiple indices. This value is commonly referred to as
the *zero value*, but we refer to it as the *implicitly replicated
value* or *IRV* since it can take on any value of the array’s element
type in practice including non-zero numeric values, a class reference, a
record or tuple value, etc.

An array declared over a sparse domain can be indexed using any of the
indices in the sparse domain’s parent domain. If it is read using an
index that is not part of the sparse domain’s index set, the IRV value
is returned. Otherwise, the array element corresponding to the index is
returned.

Sparse arrays can only be written at locations corresponding to indices
in their domain’s index set. In general, writing to other locations
corresponding to the IRV value will result in a runtime error.

By default a sparse array’s IRV is defined as the default value for the
array’s element type. The IRV can be set to any value of the array’s
element type by assigning to a pseudo-field named ``IRV`` in the array.

   *Example (sparse-error.chpl)*.

   The following code example declares a sparse array, ``SpsA`` using
   the sparse domain ``SpsD`` (For this example, assume that
   ``n``\ :math:`>`\ 1). Line 2 assigns two indices to ``SpsD``\ ’s
   index set and then lines 3–4 store the values 1.1 and 9.9 to the
   corresponding values of ``SpsA``. The IRV of ``SpsA`` will initially
   be 0.0 since its element type is ``real``. However, the fifth line
   sets the IRV to be the value 5.5, causing ``SpsA`` to represent the
   value 1.1 in its low corner, 9.9 in its high corner, and 5.5
   everywhere else. The final statement is an error since it attempts to
   assign to ``SpsA`` at an index not described by its domain, ``SpsD``.

   .. BLOCK-test-chapelpre

      config const n = 5;
      const D = {1..n, 1..n};

   .. code-block:: chapel

      var SpsD: sparse subdomain(D);
      var SpsA: [SpsD] real;
      SpsD = ((1,1), (n,n));
      SpsA(1,1) = 1.1;
      SpsA(n,n) = 9.9;
      SpsA.IRV = 5.5;
      SpsA(1,n) = 0.0;  // ERROR!

   .. BLOCK-test-chapeloutput

      sparse-error.chpl:9: error: halt reached - attempting to assign a 'zero' value in a sparse array: (1, 5)

.. _Association_of_Arrays_to_Domains:

Association of Arrays to Domains
--------------------------------

When an array is declared, it is linked during execution to the domain
identity over which it was declared. This linkage is invariant for the
array’s lifetime and cannot be changed.

When indices are added or removed from a domain, the change impacts the
arrays declared over this particular domain. In the case of adding an
index, an element is added to the array and initialized to the IRV for
sparse arrays, and to the default value for the element type for dense
arrays. In the case of removing an index, the element in the array is
removed.

When a domain is reassigned a new value, its arrays are also impacted.
Values that correspond to indices in the intersection of the old and new
domain are preserved in the arrays. Values that could only be indexed by
the old domain are lost. Values that can only be indexed by the new
domain have elements added to the new array, initialized to the IRV for
sparse arrays, and to the element type’s default value for other array
types.

For performance reasons, there is an expectation that a method will be
added to domains to allow non-preserving assignment, *i.e.*, all values
in the arrays associated with the assigned domain will be lost. Today
this can be achieved by assigning the array’s domain an empty index set
(causing all array elements to be deallocated) and then re-assigning the
new index set to the domain.

An array’s domain can only be modified directly, via the domain’s name
or an alias created by passing it to a function via default intent. In
particular, the domain may not be modified via the array’s ``.domain``
method, nor by using the domain query syntax on a function’s formal
array
argument (:ref:`Formal_Arguments_of_Generic_Array_Types`).

   *Rationale*.

   When multiple arrays are declared using a single domain, modifying
   the domain affects all of the arrays. Allowing an array’s domain to
   be queried and then modified suggests that the change should only
   affect that array. By requiring the domain to be modified directly,
   the user is encouraged to think in terms of the domain distinctly
   from a particular array.

   In addition, this choice has the beneficial effect that arrays
   declared via an anonymous domain have a constant domain. Constant
   domains are considered a common case and have potential compilation
   benefits such as eliminating bounds checks. Therefore making this
   convenient syntax support a common, optimizable case seems prudent.

.. _Predefined_Functions_and_Methods_on_Arrays:

Predefined Functions and Methods on Arrays
------------------------------------------

There is an expectation that this list of predefined methods will grow.



.. code-block:: chapel

   proc Array.eltType type

Returns the element type of the array.



.. code-block:: chapel

   proc Array.rank param

Returns the rank of the array.



.. code-block:: chapel

   proc Array.domain: this.domain

Returns the domain of the given array. This domain is constant, implying
that the domain cannot be resized by assigning to its domain field, only
by modifying the domain directly.



.. code-block:: chapel

   proc reshape(A: Array, D: Domain): Array

Returns a copy of the array containing the same values but in the shape
of the new domain. The number of indices in the domain must equal the
number of elements in the array. The elements of the array are copied
into the new array using the default iteration orders over both arrays.



.. code-block:: chapel

   proc Array.size: this.domain.idxType

Returns the number of elements in the array.
