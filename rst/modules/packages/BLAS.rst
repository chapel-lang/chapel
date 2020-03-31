.. default-domain:: chpl

.. module:: BLAS
   :synopsis: Support for Basic Linear Algebra Subprograms (BLAS) kernel routines.

BLAS
====
**Usage**

.. code-block:: chapel

   use BLAS;

**Submodules**

.. toctree::
   :maxdepth: 1
   :glob:

   BLAS/*



Support for Basic Linear Algebra Subprograms (BLAS) kernel routines.

BLAS (Basic Linear Algebra Subprograms) are the de facto standard routines for
low-level matrix, vector, and scalar operations. While `netlib
<https://www.netlib.org/blas/#_documentation>`_ provides the official
reference version of BLAS, this documentation refers to the
`MKL BLAS <https://software.intel.com/en-us/mkl-developer-reference-fortran>`_
documentation, due to interface similarities.

This module is intended to work with non-distributed dense rectangular
(``DefaultRectangular``) arrays.


Compiling with BLAS
-------------------

In order to compile a Chapel program that uses this module, the
BLAS and C_BLAS (C wrappers to BLAS) libraries must be installed on the system.
The paths to both the ``cblas.h`` header file and BLAS library
must be passed to the ``-I`` and ``-L`` compiler arguments. The library name,
typically ``blas``, must be passed to the ``-l`` argument as well.

The compilation command should look something like this:

.. code-block:: sh

    chpl -I$PATH_TO_CBLAS_DIR \
         -L$PATH_TO_BLAS_LIBS -lblas source.chpl

BLAS Implementations:
  There is a wide range of
  `BLAS implementations <https://en.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms#Implementations>`_
  available.
  This module was built and tested with `netlib's C_BLAS
  <https://www.netlib.org/blas/#_cblas>`_, but many other implementations are
  compatible as well.
  Using a version of BLAS optimized for the user's system will yield the best
  performance.

  There are a few known caveats with some popular implementations:

  * **ATLAS**

    * Compilation (linking) requires the additional flag: ``-latlas``

  * **MKL**

    * Compilation requires the additional flag in order to use the MKL header
      instead: ``--set blasImpl=mkl``

  * **OpenBLAS**

    * Compilation will generate warnings about incompatible pointer types,
      which may be ignored.
      These warnings are due to the header files of OpenBLAS differing from the
      reference C_BLAS prototypes for complex arguments by using ``float*`` and
      ``double*`` pointers, instead of ``void*`` pointers.

  * **Cray LibSci**

    * On Cray systems with the ``cray-libsci`` module loaded, no compiler flags
      should be necessary when compiling programs that use BLAS. This is
      typically loaded by default, but can be manually loaded with ``module
      load cray-libsci`` as well.  Chapel programs compiled on Crays utilize
      the ``cc`` wrapper as the backend compiler, which implicitly links
      against the libsci library. Therefore, no additional steps are required
      of the user.


Chapel BLAS API
---------------

This module provides higher-level wrappers around the BLAS functions. These
provide reasonable default values for many less commonly used arguments and
determine the appropriate functions to call, based on the array element types,
as well as the array dimensions. The other functionality is identical to the
corresponding BLAS functions.

The names of these routines are identical to the corresponding BLAS functions,
except that the type prefix is dropped. For instance, ``gemm`` is the
wrapper for the ``[sdcz]gemm`` routines.

The native BLAS interface can still be accessed by calling routines from the
:mod:`C_BLAS` submodule.

The ``ldA`` argument is omitted from the Chapel BLAS API. Chapel determines the
dimensions of the matrices from the arrays that are passed in, even when one is
passing in a sub-array such that the array elements are not contiguously stored
in memory.

.. warning::

  The ``CHPL_LOCALE_MODEL=numa`` configuration is currently not supported by
  this module.

.. MKL Documentation References

.. _GEMM:   https://software.intel.com/en-us/mkl-developer-reference-fortran-gemm
.. _HEMM:   https://software.intel.com/en-us/mkl-developer-reference-fortran-hemm
.. _HERK:   https://software.intel.com/en-us/mkl-developer-reference-fortran-herk
.. _HER2K:  https://software.intel.com/en-us/mkl-developer-reference-fortran-her2k
.. _SYMM:   https://software.intel.com/en-us/mkl-developer-reference-fortran-symm
.. _SYRK:   https://software.intel.com/en-us/mkl-developer-reference-fortran-syrk
.. _SYR2K:  https://software.intel.com/en-us/mkl-developer-reference-fortran-syr2k
.. _TRMM:   https://software.intel.com/en-us/mkl-developer-reference-fortran-trmm
.. _TRSM:   https://software.intel.com/en-us/mkl-developer-reference-fortran-trsm
.. _GBMV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-gbmv
.. _GEMV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-gemv
.. _GER:    https://software.intel.com/en-us/mkl-developer-reference-fortran-ger
.. _GERC:   https://software.intel.com/en-us/mkl-developer-reference-fortran-gerc
.. _GERU:   https://software.intel.com/en-us/mkl-developer-reference-fortran-geru
.. _HBMV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-hbmv
.. _HEMV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-hemv
.. _HER:    https://software.intel.com/en-us/mkl-developer-reference-fortran-her
.. _HER2:   https://software.intel.com/en-us/mkl-developer-reference-fortran-her2
.. _HPMV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-hpmv
.. _HPR:    https://software.intel.com/en-us/mkl-developer-reference-fortran-hpr
.. _HPR2:   https://software.intel.com/en-us/mkl-developer-reference-fortran-hpr2
.. _SBMV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-sbmv
.. _SPMV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-spmv
.. _SPR:    https://software.intel.com/en-us/mkl-developer-reference-fortran-spr
.. _SPR2:   https://software.intel.com/en-us/mkl-developer-reference-fortran-spr2
.. _SYMV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-symv
.. _SYR:    https://software.intel.com/en-us/mkl-developer-reference-fortran-syr
.. _SYR2:   https://software.intel.com/en-us/mkl-developer-reference-fortran-syr2
.. _TBMV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-tbmv
.. _TBSV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-tbsv
.. _TPMV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-tpmv
.. _TPSV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-tpsv
.. _TRMV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-trmv
.. _TRSV:   https://software.intel.com/en-us/mkl-developer-reference-fortran-trsv
.. _ASUM:   https://software.intel.com/en-us/mkl-developer-reference-fortran-asum
.. _AXPY:   https://software.intel.com/en-us/mkl-developer-reference-fortran-axpy
.. _COPY:   https://software.intel.com/en-us/mkl-developer-reference-fortran-copy
.. _DOT:    https://software.intel.com/en-us/mkl-developer-reference-fortran-dot
.. _SDOT:   https://software.intel.com/en-us/mkl-developer-reference-fortran-sdot
.. _DOTC:   https://software.intel.com/en-us/mkl-developer-reference-fortran-dotc
.. _DOTU:   https://software.intel.com/en-us/mkl-developer-reference-fortran-dotu
.. _NRM2:   https://software.intel.com/en-us/mkl-developer-reference-fortran-nrm2
.. _ROT:    https://software.intel.com/en-us/mkl-developer-reference-fortran-rot
.. _ROTG:   https://software.intel.com/en-us/mkl-developer-reference-fortran-rotg
.. _ROTM:   https://software.intel.com/en-us/mkl-developer-reference-fortran-rotm
.. _ROTMG:  https://software.intel.com/en-us/mkl-developer-reference-fortran-rotmg
.. _SCAL:   https://software.intel.com/en-us/mkl-developer-reference-fortran-scal
.. _SWAP:   https://software.intel.com/en-us/mkl-developer-reference-fortran-swap
.. _AMAX:   https://software.intel.com/en-us/mkl-developer-reference-fortran-i-amax
.. _AMIN:   https://software.intel.com/en-us/mkl-developer-reference-fortran-amin
.. _CABS1:  https://software.intel.com/en-us/mkl-developer-reference-fortran-cabs1

.. BLAS Module TODO:
  - Support banded/packed matrix routines


.. enum:: enum BlasImpl { blas, mkl, off }

   Available BLAS implementations for ``blasImpl`` 


.. data:: config param blasImpl = BlasImpl.blas

   Specifies which header filename to include, based on the BLAS
   implementation.
   
   Most BLAS implementations rely on ``cblas.h``, which is used when
   ``blasImpl = blas``, the default setting.
   
    - ``blas`` includes ``cblas.h`` (default)
    - ``mkl`` includes ``mkl_cblas.h``
    - ``off`` includes nothing
   
   

.. data:: config param blasHeader = ""

   Manually specifies the header filename to include. This flag overrides
   the header determined by ``blasImpl``.
   
   This flag should only be necessary if using an ``BLAS`` implementation
   with a unique header name that is not supported by ``blasImpl``.
   However, no guarantees can be made about this module working with untested
   implementations.
   

.. function:: proc isBLASType(type t) param: bool

   Return `true` if type is supported by BLAS 

.. enum:: enum Order { Row = 101: c_int, Col }

   Define row or column order 


.. enum:: enum Op { N = 111: c_int, T, H }

   Operation of matrix : none, transpose, or adjoint 


.. enum:: enum Uplo { Upper = 121: c_int, Lower }

   Storage for symmetric matrices 


.. enum:: enum Diag { NonUnit = 131: c_int, Unit }

   Assume a unit or non-unit diagonal 


.. enum:: enum Side { Left = 141: c_int, Right }

   Operate on the left or right side 


.. function:: proc gemm(A: [?Adom], B: [?Bdom], C: [?Cdom], alpha, beta, opA: Op = Op.N, opB: Op = Op.N, order: Order = Order.Row)

   
   Wrapper for the `GEMM`_ routines::
   
     C := alpha * op(A) * op(B) + beta * C
   
   

.. function:: proc symm(A: [?Adom], B: [?Bdom], C: [?Cdom], alpha, beta, uplo: Uplo = Uplo.Upper, side: Side = Side.Left, order: Order = Order.Row)

   
   Wrapper for the `SYMM`_ routines::
   
     C := alpha * A * B + beta * C
   
   or::
   
     C := alpha * B * A + beta * C
   
   where ``A`` is a symmetric matrix.
   

.. function:: proc hemm(A: [?Adom], B: [?Bdom], C: [?Cdom], alpha, beta, uplo: Uplo = Uplo.Upper, side: Side = Side.Left, order: Order = Order.Row)

   
   Wrapper for the `HEMM`_ routines::
   
     C := alpha * A * B + beta * C
   
   or::
   
     C := alpha * B * A + beta * C
   
   where ``A`` is an hermitian matrix.
   

.. function:: proc syrk(A: [?Adom], C: [?Cdom], alpha, beta, uplo: Uplo = Uplo.Upper, trans: Op = Op.N, order: Order = Order.Row)

   
   Wrapper for the `SYRK`_ routines::
   
     C := alpha * A * A**T + beta * C
   
   or::
   
     C := alpha * A**T * A + beta * C
   
   
   where ``C`` is a symmetric matrix, and ``A**T`` is the transpose of ``A``.
   

.. function:: proc herk(A: [?Adom], C: [?Cdom], alpha, beta, uplo: Uplo = Uplo.Upper, trans: Op = Op.N, order: Order = Order.Row)

   
   Wrapper for the `HERK`_ routines::
   
     C := alpha * A * A**H + beta * C
   
   or::
   
     C := alpha * A**H * A + beta * C
   
   where ``C`` is an hermitian matrix, and ``A**H`` is the conjugate transpose
   of ``A``.
   

.. function:: proc syr2k(A: [?Adom], B: [?Bdom], C: [?Cdom], alpha, beta, uplo: Uplo = Uplo.Upper, trans: Op = Op.N, order: Order = Order.Row)

   
   Wrapper for the `SYR2K`_ routines::
   
     C := alpha * A * B**T + alpha * B * A**T +  beta * C
   
   or::
   
     C := alpha * A**T * B + alpha * B**T * A +  beta * C
   
   where ``C`` is a symmetric matrix, and ``A**T`` and ``B**T`` are the
   transpose of ``A`` and ``B`` , respectively.
   

.. function:: proc her2k(A: [?Adom], B: [?Bdom], C: [?Cdom], alpha, beta, uplo: Uplo = Uplo.Upper, trans: Op = Op.N, order: Order = Order.Row)

   
   Wrapper for the `HER2K`_ routines::
   
     C := alpha * A * B**H + conjg(alpha) * B * A**H +  beta * C
   
   or::
   
     C := alpha * A**H * B + conjg(alpha) * B**H * A +  beta * C
   
   where ``C`` is an hermitian matrix, ``B**H`` is the conjugate transpose of
   ``B`` , ``conjg(alpha)`` is the complex conjugate of ``alpha``.
   

.. function:: proc trmm(A: [?Adom] ?eltType, B: [?Bdom] eltType, alpha, uplo: Uplo = Uplo.Upper, trans: Op = Op.N, side: Side = Side.Left, diag: Diag = Diag.NonUnit, order: Order = Order.Row) throws

   
   Wrapper for the `TRMM`_ routines::
   
     B := alpha * op(A) * B
   
   or::
   
     B := alpha * B * op(A)
   
   where ``A`` is a triangular matrix.
   
   :throws IllegalArgumentError: When `B` is a non-square array.
   

.. function:: proc trsm(A: [?Adom], B: [?Bdom], alpha, uplo: Uplo = Uplo.Upper, trans: Op = Op.N, side: Side = Side.Left, diag: Diag = Diag.NonUnit, order: Order = Order.Row) throws

   
   Wrapper for the `TRSM`_ routines::
   
     op(A) * X = alpha * B
   
   or::
   
     X * op(A) = alpha * B
   
   where ``A`` is a triangular matrix.
   
   :throws IllegalArgumentError: When `B` is a non-square array.
   

.. function:: proc gemv(A: [?Adom] ?eltType, x: [?xdom] eltType, y: [?ydom] eltType, alpha, beta, opA: Op = Op.N, order: Order = Order.Row, incx: c_int = 1, incy: c_int = 1)

   
   Wrapper for the `GEMV`_ routines::
   
     y := alpha*op(A)*x + beta*y,
   
   where ``A`` is an ``m``x``n`` matrix.
   

.. function:: proc ger(A: [?Adom] ?eltType, X: [?Xdom] eltType, Y: [?Ydom] eltType, alpha, order: Order = Order.Row, incx: c_int = 1, incy: c_int = 1)

   
   Wrapper for `GER`_ routines::
   
     A := alpha*x*y'+ A
   
   

.. function:: proc gerc(A: [?Adom] ?eltType, X: [?Xdom] eltType, Y: [?Ydom] eltType, ref alpha: eltType, order: Order = Order.Row, incx: c_int = 1, incy: c_int = 1)

   
   Wrapper for `GERC`_ routines::
   
     A := alpha*x*conjg(y') + A
   
   

.. function:: proc geru(A: [?Adom] ?eltType, X: [?Xdom] eltType, Y: [?Ydom] eltType, ref alpha: eltType, order: Order = Order.Row, incx: c_int = 1, incy: c_int = 1)

   
   Wrapper for the `GERU`_ routines::
   
     A := alpha*x*y' + A
   
   

.. function:: proc hemv(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType, ref alpha: eltType, ref beta: eltType, order: Order = Order.Row, uplo: Uplo = Uplo.Upper, incx: c_int = 1, incy: c_int = 1) throws

   
   Wrapper for the `HEMV`_ routines::
   
     y := alpha*A*x + beta*y
     
   :throws IllegalArgumentError: When `A` is a non-square array.
   

.. function:: proc her(A: [?Adom] ?eltType, X: [?vDom] eltType, alpha, order: Order = Order.Row, uplo: Uplo = Uplo.Upper, incx: c_int = 1) throws

   
   Wrapper for the `HER`_ routines::
   
     A := alpha*x*conjg(x') + A
     
   :throws IllegalArgumentError: When `A` is a non-square array.
   

.. function:: proc her2(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType, ref alpha: eltType, order: Order = Order.Row, uplo: Uplo = Uplo.Upper, incx: c_int = 1, incy: c_int = 1) throws

   
   Wrapper for `HER2`_ routines::
   
     A := alpha *x*conjg(y') + conjg(alpha)*y *conjg(x') + A
     
   :throws IllegalArgumentError: When `A` is a non-square array.
   

.. function:: proc symv(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType, alpha, beta, order: Order = Order.Row, uplo: Uplo = Uplo.Upper, incx: c_int = 1, incy: c_int = 1) throws

   
   Wrapper for the `SYMV`_ routines::
   
     y := alpha*A*x + beta*y
     
   :throws IllegalArgumentError: When `A` is a non-square array.
   

.. function:: proc syr(A: [?Adom] ?eltType, X: [?vDom] eltType, alpha, order: Order = Order.Row, uplo: Uplo = Uplo.Upper, incx: c_int = 1) throws

   
   Wrapper for `SYR`_ routines::
   
     A := alpha*x*x' + A
     
   :throws IllegalArgumentError: When `A` is a non-square array.
   

.. function:: proc syr2(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType, alpha, order: Order = Order.Row, uplo: Uplo = Uplo.Upper, incx: c_int = 1, incy: c_int = 1) throws

   
   Wrapper for `SYR2`_ routines::
   
     A := alpha*x*y'+ alpha*y*x' + A
   
   :throws IllegalArgumentError: When `A` is a non-square array.
   

.. function:: proc trmv(A: [?Adom] ?eltType, X: [?vDom] eltType, trans: Op = Op.N, order: Order = Order.Row, uplo: Uplo = Uplo.Upper, diag: Diag = Diag.NonUnit, incx: c_int = 1) throws

   
   Wrapper for `TRMV`_ routines::
   
     x := op(A)*x
   
   :throws IllegalArgumentError: When `A` is a non-square array.
   

.. function:: proc trsv(A: [?Adom] ?eltType, B: [?vDom] eltType, trans: Op = Op.N, order: Order = Order.Row, uplo: Uplo = Uplo.Upper, diag: Diag = Diag.NonUnit, incx: c_int = 1) throws

   
   Wrapper for the `TRSV`_ routines::
   
     A*op(x) = b
   
   :throws IllegalArgumentError: When `A` is a non-square array.
   

.. function:: proc rotg(ref a: ?eltType, ref b: eltType, ref c: eltType, ref s: eltType)

   
   Wrapper for the `ROTG`_ routines
   
   Construct `Givens plane rotation <https://en.wikipedia.org/wiki/Givens_rotation>`_
   of point ``p`` defined by Cartesian coordinates ``(a, b)``::
   
     | c  s |   |a|   |r|
     |      | . | | = | |
     |-s  c |   |b|   |0|
   
   **Input:**
   
   - ``a``: x-coordinate of point ``p``
   - ``b``: y-coordinate of point ``p``
   
   **Output:**
   
   - ``a``: stores length vector (``r``) of inputs ``(a, b)``
   - ``b``: stores ``z`` parameter that is defined below
   - ``c``: stores value of ``c`` element defined above
   - ``s``: stores value of ``s`` element defined above
   
   The ``z`` parameter (stored in ``b``) is defined such that::
   
     if |a| > |b| then z = s;
     else if c != 0 then z = 1/c;
     else z = 1
   
   

.. function:: proc rotmg(ref d1: ?eltType, ref d2: eltType, ref b1: eltType, b2: eltType, P: [] eltType) throws

   
   Wrapper for the `ROTMG`_ routines
   
   Generate Givens rotation of points::
   
      |b1|     |b1*sqrt(d1)|
      |  | = H.|           |
      |0 |     |b2*sqrt(d2)|
   
   **Input:**
   
   - ``d1``: Scaling factor for ``b1`` (x-axis)
   - ``d2``: Scaling factor for ``b2`` (y-axis)
   - ``b1``: x-coordinate of input vector
   - ``b2``: y-coordinate of input vector
   
   **Output:**
   
   - ``d1``: Provides the first element the diagonal matrix
   - ``d2``: Provides the second element the diagonal matrix
   - ``b1``: Provides the ``b1`` rotated (rotated x coordination) of the vector
   - ``P``: Parameter array of 5 elements, detailed below::
   
   
                              |P[1] P[3]|
      if P[0] == -1 then  H = |         |
                              |P[2] P[4]|
   
                              |1.0  P[3]|
      if P[0] ==  0 then  H = |         |
                              |P[2]  1.0|
   
                              |P[1]  1.0|
      if P[0] == 1 then   H = |         |
                              |-1.0 P[4]|
   
                              |1   0|
      if P[0] == -2 then  H = |     |
                              |0   1|
   
   
   :throws IllegalArgumentError: When `P` does not consist of exactly five
     elements.
   

.. function:: proc rot(X: [?D] ?eltType, Y: [D] eltType, c: eltType, s: eltType, incY: c_int = 1, incX: c_int = 1)

   
   Wrapper for the `ROT`_ routines
   
   Replaces the value elements of two vectors ``X`` and ``Y`` using the equation::
   
     X[i] = c*X[i] + s*X[i]
     Y[i] = c*Y[i] - s*X[i]
   
   **Input:**
   
   - ``X``: Input vector
   - ``Y``: Input vector
   - ``c``: Scalar parameter
   - ``s``: Scalar parameter
   - ``incY``: Defines the increment for the vector ``Y``
   - ``incX``: Defines the increment for the vector ``X``
   
   **Output:**
   
   - ``X``: Vector with updated elements
   - ``Y``: Vector with updated elements
   
   
   

.. function:: proc rotm(X: [?D] ?eltType, Y: [D] eltType, P: [] eltType, incY: c_int = 1, incX: c_int = 1) throws

   
   Wrapper for the `ROTM`_ routines
   
   Executes the modified `Givens rotations <https://en.wikipedia.org/wiki/Givens_rotation>`_ with element wise  substitution::
   
      |X[i]|     |X[i]|
      |    | = H.|    |
      |Y[i]|     |Y[i]|
   
   **Input:**
   
   - ``X``: Input vector
   - ``Y``: Input vector
   - ``incY``: Defines the increment for the vector ``Y``
   - ``incX``: Defines the increment for the vector ``X``
   - ``P``: Parameter array of 5 elements, detailed below::
   
   
                              |P[1] P[3]|
      if P[0] == -1 then  H = |         |
                              |P[2] P[4]|
   
                              |1.0  P[3]|
      if P[0] ==  0 then  H = |         |
                              |P[2]  1.0|
   
                              |P[1]  1.0|
      if P[0] == 1 then   H = |         |
                              |-1.0 P[4]|
   
                              |1   0|
      if P[0] == -2 then  H = |     |
                              |0   1|
   
   **Output:**
   
   - ``X``: Vector with updated elements
   - ``Y``: Vector with updated elements
   
   :throws IllegalArgumentError: When `P` does not consist of exactly five
     elements.
   

.. function:: proc swap(X: [?D] ?eltType, Y: [D] eltType, incY: c_int = 1, incX: c_int = 1)

   
   Wrapper for the `SWAP`_ routines
   
   Exchanges elements of two vectors.
   
   **Input:**
   
   - ``X``: Input vector
   - ``Y``: Input vector
   - ``incY``: Defines the increment for the vector ``Y``
   - ``incX``: Defines the increment for the vector ``X``
   
   **Output:**
   
   - ``X``: Contains input ``Y`` elements
   - ``Y``: Contains input ``X`` elements
   
   

.. function:: proc scal(X: [?D] ?eltType, ref alpha: eltType, incX: c_int = 1)

   
   Wrapper for the `SCAL`_ routines
   
   Calculates the product of a vector ``X`` with scalar alpha::
   
     X' = alpha*X
   
   **Input:**
   
   - ``X``: Input vector
   - ``alpha``: Scalar parameter
   
   **Output:**
   
   - ``X``: Vector updated by the equation: ``X[i] = alpha*X[i]``
   
   

.. function:: proc copy(X: [?D] ?eltType, Y: [D] eltType, incY: c_int = 1, incX: c_int = 1)

   
   Wrapper for the `COPY`_ routines
   
   Copies one vector (``X`` the source) to another (``Y`` the destination)::
   
     Y = X
   
   **Input:**
   
   - ``X``: Input vector
   - ``Y``: Input vector
   - ``incX``: Defines the increment for the vector ``X``
   - ``incY``: Defines the increment for the vector ``Y``
   
   **Output:**
   
   ``Y``: Contains the values copied from ``X`` vector
   
   

.. function:: proc axpy(X: [?D] ?eltType, Y: [D] eltType, ref alpha: eltType, incY: c_int = 1, incX: c_int = 1)

   
   Wrapper for the `AXPY`_ routines
   
   Computes the vector-scalar product of apha and ``X`` and adds the result to
   ``Y``::
   
     Y = alpha*X + Y
   
   **Input:**
   
   - ``X``: Input vector
   - ``alpha``: Scalar parameter
   - ``incX``: Defines the increment for the vector ``X``
   - ``incY``: Defines the increment for the vector ``Y``
   
   **Output:**
   
   - ``Y``: Vector updated by the equation: ``Y[i] = alpha*X[i]+Y[i]``
   
   

.. function:: proc dot(X: [?xD] ?eltType, Y: [?yD] eltType, incY: c_int = 1, incX: c_int = 1): eltType

   
   Wrapper for `DOT`_ routines
   
   Returns  the dot product of two vectors::
   
      X*Y
   
   **Input:**
   
   - ``X``: Input vector
   - ``Y``: Input vector
   - ``incX``: Defines the increment for the vector ``X``
   - ``incY``: Defines the increment for the vector ``Y``
   
   :returns: Scalar value of dot product
   

.. function:: proc dotu(X: [?D] ?eltType, Y: [D] eltType, incY: c_int = 1, incX: c_int = 1)

   
     Wrapper for  `DOTU`_ routines (``DOTU_SUB``)
   
     Obtains the dot product of two complex vectors::
   
        X*Y
   
     **Input:**
   
     - ``X``: Input vector
     - ``Y``: Input vector
     - ``incX``: Defines the increment for the vector ``X``
     - ``incY``: Defines the increment for the vector ``Y``
   
   :returns: The complex dot product
   
   
   

.. function:: proc dotc(X: [?D] ?eltType, Y: [D] eltType, incY: c_int = 1, incX: c_int = 1)

   
   Wrapper for `DOTC`_ routines (``DOTC_SUB``)
   
   Obtains the dot product of conjugated X vector with Y vector::
   
      conj(X)*Y
   
   **Input:**
   
   - ``X``: Conjugated input vector
   - ``Y``: Input vector
   - ``incX``: Defines the increment for the vector ``X``
   - ``incY``: Defines the increment for the vector ``Y``
   
   :returns: The complex dot product
   
   
   

.. function:: proc dsdot(X: [?D] real(32), Y: [D] real(32), incY: c_int = 1, incX: c_int = 1): real(64)

   
   Wrapper for `SDOT`_ routines (``DSDOT`` variant)
   
   Returns the dot product of two ``real(32)`` vectors as a ``real(64)``,
   using ``real(64)`` precision internally::
   
      X*Y
   
   **Input:**
   
   - ``X``: Input vector
   - ``Y``: Input vector
   - ``incX``: Defines the increment for the vector ``X``
   - ``incY``: Defines the increment for the vector ``Y``
   
   :returns: Scalar value of dot product
   
   
   

.. function:: proc sdsdot(X: [?D] real(32), Y: [D] real(32), incY: c_int = 1, incX: c_int = 1): real(32)

   
   Wrapper for `SDOT`_ routines (``SDSDOT`` variant)
   
   Returns the dot product of two ``real(32)`` vectors as a ``real(32)``,
   using ``real(64)`` precision internally::
   
      X*Y
   
   **Input:**
   
   - ``X``: Input vector
   - ``Y``: Input vector
   - ``incX``: Defines the increment for the vector ``X``
   - ``incY``: Defines the increment for the vector ``Y``
   
   :returns: Scalar value of dot product
   
   
   

.. function:: proc nrm2(X: [?D] ?eltType, incX: c_int = 1)

   
   Wrapper for `NRM2`_ routines
   
   Returns the  Euclidean norm of vector ``X``::
   
     ||X||
   
   **Input:**
   
   - ``X``: Input vector
   - ``incX``: Defines the increment for the vector ``X``
   
   :returns: The 2-norm of ``X`` vector
   
   

.. function:: proc asum(X: [?D] ?eltType, incX: c_int = 1)

   
   Wrapper for the `ASUM`_ routines
   
   Returns the sum of the magnitude values of X elements::
   
     |Re X[1]| + |Im X[1]| + |Re  X[2]| + |Im  X[2]|+ ... + |Re  X[N]| + |Im X[N]|.
   
   **Input:**
   
   - ``X``: Input vector
   - ``incX``: Defines the increment for the vector ``X``
   
   :returns: The 1-norm of ``X`` vector
   
   

.. function:: proc amax(X: [?D] ?eltType, incX: c_int = 1)

   
   Wrapper for `AMAX`_ routines
   
   Returns the index of element in the vector with maximum absolute value.
   
   **Input:**
   
   - ``X``: Input vector
   - ``incX``: Defines the increment for the vector ``X``
   
   :returns: The index of maximum absolute value
   
   

