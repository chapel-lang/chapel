.. default-domain:: chpl

.. module:: LinearAlgebra
   :synopsis: A high-level interface to linear algebra operations and procedures.

LinearAlgebra
=============
**Usage**

.. code-block:: chapel

   use LinearAlgebra;

**Submodules**

.. toctree::
   :maxdepth: 1
   :glob:

   LinearAlgebra/*



A high-level interface to linear algebra operations and procedures.

Compiling with Linear Algebra
-----------------------------

Programs using the :mod:`LinearAlgebra` module can be built with no additional
dependencies `if` they do not use any procedures that rely on :mod:`BLAS` or
:mod:`LAPACK`. Procedure dependencies are specified in procedure documentation
below.

If a program calls a procedure that depends on :mod:`BLAS` or :mod:`LAPACK`, the
headers and library will need to be available during compilation, typically
through compiler flags and/or environment variables.

Some procedures have implementations both with `and` without dependencies. By
default, the implementation with dependencies will be selected. Users can
explicitly opt out of using the :mod:`BLAS` and :mod:`LAPACK` dependent
implementations by setting the ``blasImpl`` and ``lapackImpl`` flags to ``none``.

**Building programs with no dependencies**

.. code-block:: chpl

  // example1.chpl
  var A = Matrix([0.0, 1.0, 1.0],
                 [1.0, 0.0, 1.0],
                 [1.0, 1.0, 0.0]);
  var I = eye(3,3);
  var B = A + I;

The program above has no dependencies and can therefore be compiled without
the ``BLAS`` or ``LAPACK`` headers and libraries available:

.. code-block:: bash

  chpl example1.chpl

If this program had used a procedure with a dependency such as
:proc:`cholesky` (depends on ``LAPACK``), compilation without ``LAPACK``
headers and libraries available would result in a compilation error.

**Building programs with dependencies**

.. code-block:: chpl

  // example2.chpl
  var A = Matrix([2.0, 1.0],
                 [1.0, 2.0]);
  var (eigenvalues, eigenvectors) = eig(A, right=true);

The program above uses :proc:`eig`, which depends on :mod:`LAPACK`.
Compilation without ``LAPACK`` headers and libraries available would result in
a compilation error.

Following the instructions from the :mod:`LAPACK` module
documentation, the above program could be compiled if ``LAPACK`` is available
on the system and specified with the following compilation flags:

.. code-block:: bash

  # Building with LAPACK dependency
  chpl -I$PATH_TO_LAPACKE_INCLUDE_DIR \
       -L$PATH_TO_LIBGFORTRAN -lgfortran \
       -L$PATH_TO_LAPACK_BINARIES -llapacke -llapack -lrefblas \
       example2.chpl

**Building programs with optional dependencies**

.. code-block:: chpl

  // example3.chpl
  var A = Matrix(3,5);
  A = 2;
  var AA = A.dot(A.T);


The program above uses :proc:`dot`, which has two available implementations:
one that depends on  :mod:`BLAS` and one that is written in Chapel.  The
program will default to using the more performant ``BLAS`` implementation
of matrix-matrix multiplication.

Following the instructions from the
:mod:`BLAS` module documentation, the above program could be compiled if
``BLAS`` is available on the system and specified with the following
compilation flags:

.. code-block:: bash

  # Building with BLAS dependency
  chpl -I$PATH_TO_CBLAS_DIR \
       -L$PATH_TO_BLAS_LIBS -lblas \
       example3.chpl

.. note::

  Users can set environment variables like ``LDFLAGS`` for ``-L`` arguments and
  ``CFLAGS`` for ``-I`` arguments, to avoid throwing these flags every time.

  Additionally, the required linker flags (``-l``) may vary depending on the
  ``BLAS`` and ``LAPACK`` implementations being used.


To opt out of using the ``BLAS`` implementation, users can add the ``--set
blasImpl=none`` flag, so that ``BLAS`` is no longer a dependency:

.. code-block:: bash

  # Building with BLAS dependency explicitly disabled
  chpl --set blasImpl=none example3.chpl

Similarly, users can opt out of of ``LAPACK`` implementations with the ``--set
lapackImpl=none`` flag. Setting both flags to ``none`` will always choose the
Chapel implementation when available, and will emit a compiler error
when no native implementation is available:

.. code-block:: bash

  # Building with all dependencies explicitly disabled
  chpl --set lapackImpl=none --set blasImpl=none example3.chpl

See the documentation of :mod:`BLAS` or :mod:`LAPACK` for
more details on these flags.

.. _LinearAlgebraInterface:

Linear Algebra Interface
------------------------

**Matrix and Vector representation**

Matrices and vectors are represented as Chapel arrays, which means the
convenience constructors provided in this module are not necessary to use this
module's functions. This also means that matrices (2D arrays) and vectors (1D
arrays) will work with any other Chapel library that works with arrays.

.. note::

  This documentation uses the terms `matrix` to refer to `2D arrays`, and
  `vector` to refer to `1D arrays`.

**Domain offsets**

All functions that return arrays will inherit their domains from the input
array if possible.  Otherwise they will return arrays with 1-based indices.

**Row vs Column vectors**

Row and column vectors are both represented as 1D arrays and are
indistinguishable in Chapel. In the :proc:`dot` function, matrix-vector
multiplication assumes a column vector, vector-matrix multiplication assumes a
row vector, vector-vector multiplication is always treated as an inner-product,
as the function name implies.
An outer product can be computed with the :proc:`outer` function.

**Domain maps**

All of the functions in this module only support
``DefaultRectangular`` arrays (the default domain map), unless explicitly
specified in the function's documentation. Other domain maps
are supported through submodules, such ``LinearAlgebra.Sparse`` for the
``CS`` layout.


.. class:: LinearAlgebraError : Error

   Base ``Error`` type for ``LinearAlgebra`` errors. 


   .. attribute:: var info: string

      Stores message to be emitted upon uncaught throw 

.. function:: proc Vector(length, type eltType = real)

   Return a vector (1D array) over domain ``{1..length}``

.. function:: proc Vector(space: range, type eltType = real)

   Return a vector (1D array) over domain ``{space}`` 

.. function:: proc Vector(Dom: domain(1), type eltType = real)

   Return a vector (1D array) over domain ``Dom`` 

.. function:: proc Vector(A: [?Dom] ?Atype, type eltType = Atype)

   Return a vector (1D array) with domain and values of ``A`` 

.. function:: proc Vector(x: ?t, Scalars ...?n, type eltType)

   Return a vector (1D array), given 2 or more numeric values
   
   If `type` is omitted, it will be inferred from the first argument

.. function:: proc Matrix(rows, type eltType = real)

   Return a square matrix (2D array) over domain ``{1..rows, 1..rows}``

.. function:: proc Matrix(rows, cols, type eltType = real)

   Return a matrix (2D array) over domain ``{1..rows, 1..cols}``

.. function:: proc Matrix(space: range, type eltType = real)

   Return a square matrix (2D array) over domain ``{space, space}`` 

.. function:: proc Matrix(rowSpace: range, colSpace: range, type eltType = real)

   Return a matrix (2D array) over domain ``{rowSpace, colSpace}`` 

.. function:: proc Matrix(Dom: domain, type eltType = real)

   Return a matrix (2D array) over domain ``Dom`` 

.. function:: proc Matrix(A: [?Dom] ?Atype, type eltType = Atype)

   Return a matrix (2D array) with domain and values of ``A``.
   
   ``A`` can be sparse (CS) or dense.

.. function:: proc Matrix(const Arrays: ?t ...?n, type eltType)

   
   Return a matrix (2D array), given 2 or more vectors, such that the vectors
   form the rows of the matrix. In other words, the vectors are
   concatenated such that the ``ith`` vector corresponds to the matrix slice:
   ``A[i, ..]``
   
   If `type` is omitted, it will be inferred from the first array.
   
   For example:
   
   .. code-block:: chapel
   
       var A = Matrix([1, 2, 3],
                      [4, 5, 6],
                      [7, 8, 9]);
       /* Produces the 3x3 matrix of integers:
            1 2 3
            4 5 6
            7 8 9
        */
   

.. function:: proc eye(m: integral, type eltType = real)

   Return a square identity matrix over domain ``{1..m, 1..m}`` 

.. function:: proc eye(m: integral, n: integral, type eltType = real)

   Return an identity matrix over domain ``{1..m, 1..n}`` 

.. function:: proc eye(Dom: domain(2), type eltType = real)

   Return an identity matrix over domain ``Dom`` 

.. function:: proc setDiag(ref X: [?D] ?eltType, in k: int = 0, val: eltType = 0)

   Sets the value of a diagonal in a matrix. If the matrix is sparse,
   indices on the diagonal will be added to its domain
   
   ``k > 0``, represents an upper diagonal starting
   from the ``k``th column, ``k == 0`` represents the main
   diagonal, ``k < 0`` represents a lower diagonal starting
   from the ``-k``th row. ``k`` is 0-indexed.

.. function:: proc transpose(A: [?Dom] ?eltType)

   Transpose vector, matrix, or domain.
   
   .. note::
   
      Since row vectors and columns vectors are indistinguishable, passing
      a vector to this function will return that vector unchanged
   

.. method:: proc _array.T

   Transpose vector or matrix 

.. method:: proc _array.plus(A: [?Adom] ?eltType)

   Element-wise addition. Same as ``A + B``. 

.. method:: proc _array.minus(A: [?Adom] ?eltType)

   Element-wise subtraction. Same as ``A - B``. 

.. method:: proc _array.times(A: [?Adom])

   Element-wise multiplication. Same as ``A * B``. 

.. method:: proc _array.elementDiv(A: [?Adom])

   Element-wise division. Same as ``A / B``. 

.. function:: proc dot(A: [?Adom] ?eltType, B: [?Bdom] eltType)

   
   Generic matrix multiplication, ``A`` and ``B`` can be a matrix, vector, or
   scalar.
   
   .. note::
   
     When ``A`` is a vector and ``B`` is a matrix, this function implicitly
     computes ``dot(transpose(A), B)``, which may not be as efficient as
     passing ``A`` and ``B`` in the reverse order.
   
   .. note::
   
     Dense matrix-matrix and matrix-vector multiplication will utilize the
     :mod:`BLAS` module for improved performance, if available. Compile with
     ``--set blasImpl=none`` to opt out of the :mod:`BLAS` implementation.

.. method:: proc _array.dot(A: [])

   Compute the dot-product
   
   .. note::
   
     Dense matrix-matrix and matrix-vector multiplication will utilize the
     :mod:`BLAS` module for improved performance, if available. Compile with
     ``--set blasImpl=none`` to opt out of the :mod:`BLAS` implementation.
   

.. function:: proc inner(const ref A: [?Adom] ?eltType, const ref B: [?Bdom])

   Inner product of 2 vectors. 

.. function:: proc outer(A: [?Adom] ?eltType, B: [?Bdom] eltType)

   Outer product of 2 vectors. 

.. function:: proc inv(ref A: [?Adom] ?eltType, overwrite = false)

   
   Returns the inverse of ``A`` square matrix A.
   
   
     .. note::
   
       This procedure depends on the :mod:`LAPACK` module, and will generate a
       compiler error if ``lapackImpl`` is ``none``.

.. function:: proc matPow(A: [], b)

   
   Return the matrix ``A`` to the ``bth`` power, where ``b`` is a positive
   integral type.
   
   .. note::
   
     ``matPow`` will utilize the :mod:`BLAS` module for improved performance, if
     available. Compile with ``--set blasImpl=none`` to opt out of the
     :mod:`BLAS` implementation.

.. function:: proc cross(A: [?Adom] ?eltType, B: [?Bdom] eltType)

   Return cross-product of 3-element vectors ``A`` and ``B`` with domain of
   ``A``. 

.. function:: proc diag(A: [?Adom] ?eltType, k = 0)

   
   Return a Vector containing the diagonal elements of ``A`` if the argument ``A`` is of rank 2.
   Return a diagonal Matrix whose diagonal contains elements of ``A`` if argument ``A`` is of rank 1.
   

.. function:: proc tril(A: [?D] ?eltType, k = 0)

   
   Return lower triangular part of matrix, above the diagonal + ``k``,
   where ``k = 0`` includes the diagonal, and ``k = -1`` does *not* include
   the diagonal. For example:
   
   .. code-block:: chapel
   
    var A = Matrix(4, 4, eltType=int);
    A = 1;
   
    tril(A);
    /* Returns:
   
        1    0    0    0
        1    1    0    0
        1    1    1    0
        1    1    1    1
    */
   
    tril(A, 1);
    /* Returns:
   
        1    1    0    0
        1    1    1    0
        1    1    1    1
        1    1    1    1
    */
   
    tril(A, -1);
    /* Returns:
   
        0    0    0    0
        1    0    0    0
        1    1    0    0
        1    1    1    0
    */
   

.. function:: proc triu(A: [?D] ?eltType, k = 0)

   
   Return upper triangular part of matrix, above the diagonal + ``k``,
   where ``k = 0`` includes the diagonal, and ``k = 1`` does *not* include
   the diagonal. For example:
   
   .. code-block:: chapel
   
      var A = Matrix(4, 4, eltType=int);
      A = 1;
   
      triu(A);
      /* Returns:
   
          1    1    1    1
          0    1    1    1
          0    0    1    1
          0    0    0    1
      */
   
      triu(A, 1);
      /* Returns:
   
          0    1    1    1
          0    0    1    1
          0    0    0    1
          0    0    0    0
      */
   
      triu(A, -1);
      /* Returns:
   
          1    1    1    1
          1    1    1    1
          0    1    1    1
          0    0    1    1
      */
   

.. function:: proc isDiag(A: [?D] ?eltType)

   Return `true` if matrix is diagonal. 

.. function:: proc isHermitian(A: [?D])

   Return `true` if matrix is Hermitian 

.. function:: proc isSymmetric(A: [?D])

   Return `true` if matrix is symmetric 

.. function:: proc isTril(A: [?D] ?eltType, k = 0): bool

   
   Return `true` if matrix is lower triangular below the diagonal + ``k``,
   where ``k = 0`` does *not* include the diagonal, and ``k = 1`` includes the
   diagonal
   

.. function:: proc isTriu(A: [?D] ?eltType, k = 0): bool

   Return `true` if matrix is upper triangular above the diagonal + ``k``,
   where ``k = 0`` does *not* include the diagonal, and ``k = -1`` includes the
   diagonal
   

.. function:: proc isSquare(A: [?D])

   Return `true` if matrix is square 

.. function:: proc trace(A: [?D] ?eltType)

   Return the trace (sum of diagonal elements) of ``A`` 

.. function:: proc lu(A: [?Adom] ?eltType)

   
   Compute an LU factorization of square matrix `A` 
   using partial pivoting, such that `A = P * L * U` where P
   is a permutation matrix. Return a tuple of size 2 `(LU, ipiv)`.
   
   `L` and `U` are stored in the same matrix `LU` where 
   the unit diagonal elements of L are not stored.
   
   `ipiv` contains the pivot indices such that row i of `A` 
   was interchanged with row `ipiv(i)`.
   

.. function:: proc det(A: [?Adom] ?eltType)

   Return the determinant of a square matrix.
   
   .. note::
   
     This procedure performs LU factorization to compute the 
     determinant. In certain cases, e.g. having a lower/upper
     triangular matrix, it is more desirable to compute the 
     determinant manually.

.. function:: proc solve_tril(const ref L: [?Ldom] ?eltType, const ref b: [?bdom] eltType, unit_diag = true)

   Return the solution ``x`` to the linear system `` L * x = b `` 
   where ``L`` is a lower triangular matrix. Setting `unit_diag` to true
   will assume the diagonal elements as `1` and will not be referenced 
   within this procedure.

.. function:: proc solve_triu(const ref U: [?Udom] ?eltType, const ref b: [?bdom] eltType)

   Return the solution ``x`` to the linear system `` U * x = b `` 
   where ``U`` is an upper triangular matrix.

.. function:: proc solve(A: [?Adom] ?eltType, b: [?bdom] eltType)

   Return the solution ``x`` to the linear system ``A * x = b``.

.. function:: proc cholesky(A: [] ?t, lower = true)

   Perform a Cholesky factorization on matrix ``A``.  ``A`` must be square.
   Argument ``lower`` indicates whether to return the lower or upper
   triangular factor.  Matrix ``A`` is not modified.  Returns an array with
   the same shape as argument ``A`` with the lower or upper triangular
   Cholesky factorization of ``A``.
   
    .. note::
   
      This procedure depends on the :mod:`LAPACK` module, and will generate a
      compiler error if ``lapackImpl`` is ``none``.
   

.. function:: proc eigvals(A: [] ?t)

   Find the eigenvalues of matrix ``A``. ``A`` must be square.
   
   .. note::
   
     This procedure depends on the :mod:`LAPACK` module, and will generate a
     compiler error if ``lapackImpl`` is ``none``.
   

.. function:: proc eig(A: [] ?t, param left = false, param right = false)

   Find the eigenvalues and eigenvectors of matrix ``A``. ``A`` must be square.
   
   * If ``left`` is ``true`` then the "left" eigenvectors are computed. The
     return value is a tuple with two elements:
     ``(eigenvalues, leftEigenvectors)``
   
   * If ``right`` is ``true`` then the "right" eigenvectors are computed. The
     return value is a tuple with two elements:
     ``(eigenvalues, rightEigenvectors)``
   
   * If ``left`` and ``right`` are both ``true`` then both eigenvectors are
     computed.  The return value is a tuple with three elements:
     ``(eigenvalues, leftEigenvectors, rightEigenvectors)``
   
   * If ``left`` and ``right`` are both ``false`` only the eigenvalues are
     computed, and returned as a single array.
   
    .. note::
   
      This procedure depends on the :mod:`LAPACK` module, and will generate a
      compiler error if ``lapackImpl`` is ``none``.
   
   

.. function:: proc svd(A: [?Adom] ?t) throws

   
   Singular Value Decomposition.
   
   Factorizes the `m x n` matrix ``A`` such that:
   
   
   .. math::
   
     \mathbf{A} = \textbf{U} \cdot \Sigma \cdot \mathbf{V^H}
   
   where
   
     - :math:`\mathbf{U}` is an `m x m` unitary matrix,
     - :math:`\Sigma` is a diagonal `m x n` matrix,
     - :math:`\mathbf{V}` is an `n x n` unitary matrix, and
       :math:`\mathbf{V^H}` is the Hermitian transpose.
   
   This procedure returns a tuple of ``(U, s, Vh)``, where ``s`` is a vector
   containing the diagonal elements of :math:`\Sigma`, known as the
   singular values.
   
   For example:
   
   .. code-block:: chapel
   
     var A = Matrix([3, 2,  2],
                    [2, 3, -2],
                    eltType=real);
     var (U, s, Vh) = svd(A);
   
   ``LinearAlgebraError`` will be thrown if the SVD computation does not
   converge or an illegal argument, such as a matrix containing a ``NAN`` value,
   is given.
   
   .. note::
   
    A temporary copy of ``A`` will be created within this computation.
   
   .. note::
   
     This procedure depends on the :mod:`LAPACK` module, and will generate a
     compiler error if ``lapackImpl`` is ``none``.

.. function:: proc jacobi(A: [?Adom] ?eltType, ref X: [?Xdom] eltType, b: [Xdom] eltType, tol = 0.0001, maxiter = 1000)

   
   Compute the approximate solution to ``A * x = b`` using the Jacobi method.
   iteration will stop when ``maxiter`` is reached or error is smaller than
   ``tol``, whichever comes first. Return the number of iterations performed.
   
   .. note::
     ``X`` is passed as a reference, meaning the initial solution guess can be
     stored in ``X`` before calling the procedure, and the approximate solution 
     will be stored in the same array.
     
     Dense and CSR arrays are supported.

.. function:: proc kron(A: [?ADom] ?eltType, B: [?BDom] eltType)

   Return the Kronecker Product of matrix ``A`` and matrix ``B``.
   If the size of A is ``x * y`` and of B is ``a * b`` then size of the resulting
   matrix will be ``(x * a) * (y * b)`` 

