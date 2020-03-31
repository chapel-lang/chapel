.. default-domain:: chpl

.. module:: Sparse
   :synopsis: Linear Algebra Sparse Submodule

Sparse
======
**Usage**

.. code-block:: chapel

   use LinearAlgebra.Sparse;

Linear Algebra Sparse Submodule

A high-level interface to linear algebra operations and procedures for sparse
matrices (2D arrays).

Sparse matrices are represented as 2D arrays domain-mapped to a sparse *layout*.
All sparse operations support the CSR layout (``LayoutCS.CS(compressRows=true)``)
and some operations support COO layout (default sparse array layout).

See the :ref:`Sparse Primer <primers-sparse>` for more information about working
with sparse domains and arrays in Chapel.

Sparse Linear Algebra Interface
-------------------------------

``LinearAlgebra.Sparse`` follows the same conventions and interface choices
as the parent module, ``LinearAlgebra``, with few exceptions. These
exceptions are detailed below.


**Sparse Domains**

In Chapel, changes to the index set of a sparse array must be made
directly on the sparse domain. When working with sparse arrays that will
require index modification, it is necessary to maintain access to their sparse
domains as well. As a result of this, the sparse linear algebra interface
provides helper functions for creating both sparse domains and sparse arrays.

A common usage of this interface might look like this:

.. code-block:: chapel

  // Create an empty 3x3 CSR domain
  var D = CSRDomain(3,3);

  // Create a CSR matrix over this domain
  var A = CSRMatrix(D, int);
  // The above is equivalent to:
  // var A: [D] int;

  // Add indices to the sparse domain along the diagonal
  D += (1,1);
  D += (2,2);
  D += (3,3);

  // Set all nonzero indices to the value of 1
  A = 1;

  // A is now a 3x3 sparse identity matrix
  writeln(A);


.. note::
  This is an early prototype package submodule. As a result, interfaces may
  change over the next release.


.. function:: proc CSRDomain(rows)

   Return an empty CSR domain over parent domain:
   ``{1..rows, 1..rows}``
   

.. function:: proc CSRDomain(rows, cols)

   Return an empty CSR domain  over parent domain: ``{1..rows, 1..cols}``

.. function:: proc CSRDomain(space: range)

   Return an empty CSR domain over parent domain: ``{space, space}`` 

.. function:: proc CSRDomain(rowSpace: range, colSpace: range)

   Return an empty CSR domain over parent domain: ``{rowSpace, colSpace}`` 

.. function:: proc CSRDomain(Dom: domain)

   Return a CSR domain based on domain: ``Dom``
   
   If ``Dom`` is dense, it will be interpreted as the parent domain, and the
   domain returned will be empty.
   
   If ``Dom`` is sparse (CSR), the domain returned will contain the same
   nonzeros as ``Dom``
   

.. function:: proc CSRMatrix(Dom: domain, type eltType = real)

   Return a CSR matrix over domain: ``Dom``
   
   If ``Dom`` is dense, it will be interpreted as the parent domain, and the
   matrix returned will be empty.
   
   If ``Dom`` is sparse (CSR), the matrix returned will contain the same
   nonzeros as ``Dom``
   

.. function:: proc CSRMatrix(A: [?Dom] ?Atype, type eltType = Atype)

   Return a CSR matrix with domain and values of ``A``
   
   If ``A`` is dense, only the indices holding nonzero elements are added
   to the sparse matrix returned.
   
   If ``A`` is sparse (CSR), the returned sparse matrix will be a copy of ``A``
   casted to ``eltType``
   

.. function:: proc CSRMatrix(shape: 2*(int), data: [?nnzDom] ?eltType, indices: [nnzDom], indptr: [?indDom])

   Return a CSR matrix constructed from internal representation:
   
   - ``shape``: bounding box dimensions
   - ``data``: non-zero element values
   - ``indices``: non-zero row pointers
   - ``indptr``: index pointers
   
   

.. function:: proc CSRDomain(shape: 2*(int), indices: [?nnzDom], indptr: [?indDom])

   Return a CSR domain constructed from internal representation 

.. function:: proc dot(A: [?Adom] ?eltType, B: [?Bdom] eltType)

   
   Generic matrix multiplication, ``A`` and ``B`` can be a scalar, dense
   vector, or sparse matrix.
   
   .. note::
   
     When ``A`` is a vector and ``B`` is a matrix, this function implicitly
     computes ``dot(transpose(A), B)``, which may not be as efficient as
     passing ``A`` and ``B`` in the reverse order.
   
   

.. method:: proc _array.dot(A: [])

   Compute the dot-product 

.. method:: proc _array.dot(a)

   Compute the dot-product 

.. function:: proc transpose(D: domain)

   Transpose CSR domain 

.. function:: proc transpose(A: [?Adom] ?eltType)

   Transpose CSR matrix 

.. method:: proc _array.T

   Transpose CSR matrix 

.. method:: proc _array.plus(A: [?Adom] ?eltType)

   Element-wise addition, supports CSR and COO. 

.. method:: proc _array.minus(A: [?Adom] ?eltType)

   Element-wise subtraction, supports CSR and COO.  

.. method:: proc _array.times(A)

   Element-wise multiplication, supports CSR and COO.  

.. method:: proc _array.elementDiv(A)

   Element-wise division, supports CSR and COO.  

.. function:: proc eye(Dom: domain, type eltType = real)

   Return an identity matrix over sparse domain ``Dom`` 

.. function:: proc isDiag(A: [?D] ?eltType)

   Return ``true`` if sparse matrix is diagonal. Supports CSR and COO arrays. 

.. function:: proc isHermitian(A: [?D])

   Return ``true`` if matrix is Hermitian. Supports CSR and COO arrays. 

.. function:: proc isSymmetric(A: [?D])

   Return ``true`` if sparse matrix is symmetric. Supports CSR and COO arrays. 

