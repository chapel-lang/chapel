/*
 * Copyright 2004-2016 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*

Support for Basic Linear Algebra Subprograms (BLAS) kernel routines.

The `netlib documentation <http://www.netlib.org/blas/#_documentation>`_
describes BLAS as the following::

   The BLAS (Basic Linear Algebra Subprograms) are routines that provide
   standard building blocks for performing basic vector and matrix operations.
   The Level 1 BLAS perform scalar, vector and vector-vector operations, the
   Level 2 BLAS perform matrix-vector operations, and the Level 3 BLAS perform
   matrix-matrix operations. Because the BLAS are efficient, portable, and
   widely available, they are commonly used in the development of high quality
   linear algebra software, LAPACK for example.

This module wraps the functionality of level 3 matrix-matrix BLAS routines,
supporting the array element types, ``real(32)`` (single), ``real`` (double),
``complex(32)`` (complex), and ``complex`` (complex double) under a single
interface.


Compiling with BLAS
-------------------

In order to compile a Chapel program that uses this module, the
BLAS and CBLAS (C wrappers to BLAS) libraries must be installed on the system.
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
  This module was built and tested with `netlib's CBLAS
  <http://www.netlib.org/blas/#_cblas>`_, but many other implementations are
  compatible as well.
  Using a version of BLAS optimized for the user's system will yield the best
  performance.

  There are a few known caveats with some popular implementations:

  * **ATLAS**

    * The compilation command above likely requires the additional flag: ``-latlas``

  * **MKL**

    * The BLAS module assumes that the CBLAS functions are defined in the
      header named ``cblas.h``.
      MKL defines these in ``mkl_cblas.h``. This can be worked around by
      creating a symbolic link to the correct header file name:
      ``ln -s mkl_cblas.h cblas.h``

  * **OpenBLAS**

    * The header files that are included with OpenBLAS differ from the reference
      CBLAS prototypes for complex arguments by using ``float*`` and ``double*``
      pointers, instead of ``void*`` pointers.  Using this will likely result in
      warnings about incompatible pointer types. These may be ignored.

Cray Systems:
  No compiler flags should be necessary when compiling BLAS programs on
  Crays. The **PBLAS** implementation is made available through Cray's libsci,
  which comes installed on all Cray systems. This is typically loaded by
  default, but can be manually loaded with ``module load cray-libsci`` as well.
  Chapel programs compiled on Crays utilize the ``cc`` wrapper as the backend
  compiler, which implicitly links against the libsci library. Therefore, no
  additional steps are required of the user.

Chapel Level 3 BLAS API
-----------------------

For convenience, this module provides wrappers around the Level 3
(matrix-matrix) BLAS functions. These determine the appropriate functions to
call, based on the array element types, as well as the array dimensions. The
other functionality is identical to the corresponding BLAS functions.

The names of these routines are identical to the corresponding BLAS functions,
except that the type prefix is dropped. For instance, ``gemm`` is the
wrapper for the ``[sdcz]gemm`` routines.

.. note::
 Chapel determines the dimensions of the matrices from the arrays that are
 passed in. However, if one is passing in a sub-array such that the array
 elements are not contiguously stored in memory, then the user needs to
 pass in the leading dimension (```lda``` etc) to the array, just as they
 would in C. These default to 0, in which case Chapel determines the appropriate
 values based on the array passed in.


*/
module BLAS {

  use C_BLAS;

  use SysCTypes;
  require "cblas.h";


  /* Define row or column order */
  enum Order {Row=101 : c_int, Col};

  /* Operation of matrix : none, transpose, or adjoint */
  enum Op {N=111 : c_int, T, H}; // NoTranspose, Transpose, Adjoint

  /* Storage for symmetric matrices */
  enum Uplo {Upper=121 : c_int, Lower};

  /* Assume a unit or non-unit diagonal */
  enum Diag {NonUnit=131 : c_int, Unit};

  /* Operate on the left or right side */
  enum Side {Left=141 : c_int, Right};

  /* Level 3 BLAS */

  /*
    Wrapper for the `GEMM routines <http://www.netlib.org/lapack/explore-html/db/def/group__complex__blas__level3_gac4e11e8e8a4b0c802d2229b08da341f6.html#gac4e11e8e8a4b0c802d2229b08da341f6>`_

    Performs the matrix-matrix operation::

      C := alpha * op(A) * op(B) + beta * C

  */
  proc gemm(A : [?Adom], B : [?Bdom], C : [?Cdom],
    alpha, beta,
    opA : Op = Op.N, opB : Op = Op.N,
    order : Order = Order.Row,
    ldA : int = 0, ldB : int = 0, ldC : int = 0)
    where (Adom.rank == 2) && (Bdom.rank==2) && (Cdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var m = Cdom.dim(1).size : c_int,
        n = Cdom.dim(2).size : c_int,
        k : c_int;
    if opA > Op.N then k = Adom.dim(1).size : c_int;
                  else k = Adom.dim(2).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, order, ldA),
        _ldB = getLeadingDim(Bdom, order, ldB),
        _ldC = getLeadingDim(Cdom, order, ldC);

    select eltType {
      when real(32) {
        // sgemm
        C_BLAS.cblas_sgemm(order, opA, opB, m, n, k,
          alpha, A, _ldA, B, _ldB, beta, C,_ldC);
      }
      when real(64) {
        // dgemm
        C_BLAS.cblas_dgemm(order, opA, opB, m, n, k,
          alpha, A, _ldA, B, _ldB, beta, C,_ldC);
      }
      when complex(64) {
        // cgemm
        var alpha1 = alpha : complex(64),
            beta1 = beta : complex(64);
        C_BLAS.cblas_cgemm(order, opA, opB, m, n, k,
          alpha1, A, _ldA, B, _ldB, beta1, C,_ldC);
      }
      when complex(128) {
        // zgemm
        var alpha1 = alpha : complex(128),
            beta1 = beta : complex(128);
        C_BLAS.cblas_zgemm(order, opA, opB, m, n, k,
          alpha1, A, _ldA, B, _ldB, beta1, C,_ldC);
      }
      otherwise {
        halt("Unknown type in gemm");
      }
    }

  }

  /*
   Wrapper for the `SYMM routines <http://www.netlib.org/lapack/explore-html/db/def/group__complex__blas__level3_ga2490eea9e962fd69b9902e22aaa3a634.html#ga2490eea9e962fd69b9902e22aaa3a634>`_

    Performs the matrix-matrix operation::

      C := alpha * A * B + beta * C

    or::

      C := alpha * B * A + beta * C

    where ``A`` is a symmetric matrix.
  */
  proc symm(A : [?Adom], B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  side : Side = Side.Left,
    order : Order = Order.Row,
    ldA : int = 0, ldB : int = 0, ldC : int = 0)
    where (Adom.rank == 2) && (Bdom.rank==2) && (Cdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var m = Cdom.dim(1).size : c_int,
        n = Cdom.dim(2).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, order, ldA),
        _ldB = getLeadingDim(Bdom, order, ldB),
        _ldC = getLeadingDim(Cdom, order, ldC);

    select eltType {
      when real(32) {
        // ssymm
        C_BLAS.cblas_ssymm(order, side, uplo, m, n,
          alpha, A, _ldA, B, _ldB, beta, C,_ldC);
      }
      when real(64) {
        // dsymm
        C_BLAS.cblas_dsymm(order, side, uplo, m, n,
          alpha, A, _ldA, B, _ldB, beta, C,_ldC);
      }
      when complex(64) {
        // csymm
        var alpha1 = alpha : complex(64),
            beta1 = beta : complex(64);
        C_BLAS.cblas_csymm(order, side, uplo, m, n,
          alpha1, A, _ldA, B, _ldB, beta1, C,_ldC);
      }
      when complex(128) {
        // zsymm
        var alpha1 = alpha : complex(128),
            beta1 = beta : complex(128);
        C_BLAS.cblas_zsymm(order, side, uplo, m, n,
          alpha1, A, _ldA, B, _ldB, beta1, C,_ldC);
      }
      otherwise {
        halt("Unknown type in symm");
      }
    }

  }

  /*
    Wrapper for the `HEMM routines <http://www.netlib.org/lapack/explore-html/db/def/group__complex__blas__level3_gad2d1853a142397404eae974b6574ece3.html#gad2d1853a142397404eae974b6574ece3>`_

    Performs the matrix-matrix operation::

      C := alpha * A * B + beta * C

    or::

      C := alpha * B * A + beta * C

    where ``A`` is an hermitian matrix.
  */
  proc hemm(A : [?Adom], B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  side : Side = Side.Left,
    order : Order = Order.Row,
    ldA : int = 0, ldB : int = 0, ldC : int = 0)
    where (Adom.rank == 2) && (Bdom.rank==2) && (Cdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var m = Cdom.dim(1).size : c_int,
        n = Cdom.dim(2).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, order, ldA),
        _ldB = getLeadingDim(Bdom, order, ldB),
        _ldC = getLeadingDim(Cdom, order, ldC);

    select eltType {
      when complex(64) {
        // csymm
        var alpha1 = alpha : complex(64),
            beta1 = beta : complex(64);
        C_BLAS.cblas_chemm(order, side, uplo, m, n,
          alpha1, A, _ldA, B, _ldB, beta1, C,_ldC);
      }
      when complex(128) {
        // zsymm
        var alpha1 = alpha : complex(128),
            beta1 = beta : complex(128);
        C_BLAS.cblas_zhemm(order, side, uplo, m, n,
          alpha1, A, _ldA, B, _ldB, beta1, C,_ldC);
      }
      otherwise {
        halt("Unknown type in hemm");
      }
    }

  }

  /*
    Wrapper for the `SYRK routines <http://www.netlib.org/lapack/explore-html/db/def/group__complex__blas__level3_ga1b4f63daf04fdf3061bd25dfec0d3e84.html#ga1b4f63daf04fdf3061bd25dfec0d3e84>`_

    Performs the matrix-matrix operation::

      C := alpha * A * A**T + beta * C

    or::

      C := alpha * A**T * A + beta * C


    where ``C`` is a symmetric matrix, and ``A**T`` is the transpose of ``A``.
  */
  proc syrk(A : [?Adom],  C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    order : Order = Order.Row,
    ldA : int = 0,  ldC : int = 0)
    where (Adom.rank == 2) && (Cdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var n = Cdom.dim(1).size : c_int;
    var k : c_int;
    if trans == Op.N then k = Adom.dim(2).size : c_int;
                     else k = Adom.dim(1).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, order, ldA),
        _ldC = getLeadingDim(Cdom, order, ldC);

    select eltType {
      when real(32) {
        // ssymm
        C_BLAS.cblas_ssyrk(order, uplo, trans, n, k,
          alpha, A, _ldA, beta, C,_ldC);
      }
      when real(64) {
        // dsymm
        C_BLAS.cblas_dsyrk(order, uplo, trans, n, k,
          alpha, A, _ldA, beta, C,_ldC);
      }
      when complex(64) {
        // csymm
        var alpha1 = alpha : complex(64),
            beta1 = beta : complex(64);
        C_BLAS.cblas_csyrk(order, uplo, trans, n, k,
          alpha1, A, _ldA, beta1, C,_ldC);
      }
      when complex(128) {
        // zsymm
        var alpha1 = alpha : complex(128),
            beta1 = beta : complex(128);
        C_BLAS.cblas_zsyrk(order, uplo, trans, n, k,
          alpha1, A, _ldA, beta1, C,_ldC);
      }
      otherwise {
        halt("Unknown type in syrk");
      }
    }

  }

  /*
    Wrapper for the `HERK routines <http://www.netlib.org/lapack/explore-html/db/def/group__complex__blas__level3_gade9f14cf41f0cefea7918d716f3e1c20.html#gade9f14cf41f0cefea7918d716f3e1c20>`_

    Performs the matrix-matrix operation::

      C := alpha * A * A**H + beta * C

    or::

      C := alpha * A**H * A + beta * C

    where ``C`` is an hermitian matrix, and ``A**H`` is the conjugate transpose
    of ``A``.
  */
  proc herk(A : [?Adom],  C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    order : Order = Order.Row,
    ldA : int = 0,  ldC : int = 0)
    where (Adom.rank == 2) && (Cdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var n = Cdom.dim(1).size : c_int;
    var k : c_int;
    if trans == Op.N then k = Adom.dim(2).size : c_int;
                     else k = Adom.dim(1).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, order, ldA),
        _ldC = getLeadingDim(Cdom, order, ldC);

    select eltType {
      when complex(64) {
        // csymm
        var alpha1 = alpha : real(32),
            beta1 = beta : real(32);
        C_BLAS.cblas_cherk(order, uplo, trans, n, k,
          alpha1, A, _ldA, beta1, C,_ldC);
      }
      when complex(128) {
        // zsymm
        var alpha1 = alpha : real(64),
            beta1 = beta : real(64);
        C_BLAS.cblas_zherk(order, uplo, trans, n, k,
          alpha1, A, _ldA, beta1, C,_ldC);
      }
      otherwise {
        halt("Unknown type in syrk");
      }
    }

  }


  /*
    Wrapper for the `SYR2K routines <http://www.netlib.org/lapack/explore-html/db/def/group__complex__blas__level3_gaa8320d51ded07cd3038db237fd400547.html#gaa8320d51ded07cd3038db237fd400547>`_

    Performs the matrix-matrix operation::

      C := alpha * A * B**T + alpha * B * A**T +  beta * C

    or::

      C := alpha * A**T * B + alpha * B**T * A +  beta * C

    where ``C`` is a symmetric matrix, and ``A**T`` and ``B**T`` are the
    transpose of ``A`` and ``B`` , respectively.
  */
  proc syr2k(A : [?Adom],  B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    order : Order = Order.Row,
    ldA : int = 0,  ldB : int = 0, ldC : int = 0)
    where (Adom.rank == 2) && (Bdom.rank==2) && (Cdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var n = Cdom.dim(1).size : c_int;
    var k : c_int;
    if trans == Op.N then k = Adom.dim(2).size : c_int;
                     else k = Adom.dim(1).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, order, ldA),
        _ldB = getLeadingDim(Bdom, order, ldB),
        _ldC = getLeadingDim(Cdom, order, ldC);

    select eltType {
      when real(32) {
        // ssymm
        C_BLAS.cblas_ssyr2k(order, uplo, trans, n, k,
          alpha, A, _ldA, B, _ldB, beta, C,_ldC);
      }
      when real(64) {
        // dsymm
        C_BLAS.cblas_dsyr2k(order, uplo, trans, n, k,
          alpha, A, _ldA, B, _ldB, beta, C,_ldC);
      }
      when complex(64) {
        // csymm
        var alpha1 = alpha : complex(64),
            beta1 = beta : complex(64);
        C_BLAS.cblas_csyr2k(order, uplo, trans, n, k,
          alpha1, A, _ldA, B, _ldB, beta1, C,_ldC);
      }
      when complex(128) {
        // zsymm
        var alpha1 = alpha : complex(128),
            beta1 = beta : complex(128);
        C_BLAS.cblas_zsyr2k(order, uplo, trans, n, k,
          alpha1, A, _ldA, B, _ldB, beta1, C,_ldC);
      }
      otherwise {
        halt("Unknown type in syr2k");
      }
    }

  }

  /*
    Wrapper for the `HER2K routines <http://www.netlib.org/lapack/explore-html/db/def/group__complex__blas__level3_gaf5266b622e0fbbd972cfc2df3061984f.html#gaf5266b622e0fbbd972cfc2df3061984f>`_

    Performs the matrix-matrix operation::

      C := alpha * A * B**H + conjg(alpha) * B * A**H +  beta * C

    or::

      C := alpha * A**H * B + conjg(alpha) * B**H * A +  beta * C

    where ``C`` is an hermitian matrix, ``B**H`` is the conjugate transpose of
    ``B`` , ``conjg(alpha)`` is the complex conjugate of ``alpha``.
  */
  proc her2k(A : [?Adom],  B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    order : Order = Order.Row,
    ldA : int = 0,  ldB : int = 0, ldC : int = 0)
    where (Adom.rank == 2) && (Bdom.rank==2) && (Cdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var n = Cdom.dim(1).size : c_int;
    var k : c_int;
    if trans == Op.N then k = Adom.dim(2).size : c_int;
                     else k = Adom.dim(1).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, order, ldA),
        _ldB = getLeadingDim(Bdom, order, ldB),
        _ldC = getLeadingDim(Cdom, order, ldC);

    select eltType {
      when complex(64) {
        var alpha1 = alpha : complex(64),
            beta1 = beta : real(32);
        C_BLAS.cblas_cher2k(order, uplo, trans, n, k,
          alpha1, A, _ldA, B, _ldB, beta1, C,_ldC);
      }
      when complex(128) {
        var alpha1 = alpha : complex(128),
            beta1 = beta : real(64);
        C_BLAS.cblas_zher2k(order, uplo, trans, n, k,
          alpha1, A, _ldA, B, _ldB, beta1, C,_ldC);
      }
      otherwise {
        halt("Unknown type in her2k");
      }
    }

  }

  /*
    Wrapper for the `TRMM routines <http://www.netlib.org/lapack/explore-html/db/def/group__complex__blas__level3_gad7c297c05b482699b6d60a29c8d4a165.html#gad7c297c05b482699b6d60a29c8d4a165>`_

    Performs the matrix-matrix operation::

      B := alpha * op(A) * B

    or::

      B := alpha * B * op(A)

    where ``A`` is a triangular matrix.
  */
  proc trmm(A : [?Adom],  B : [?Bdom],
    alpha,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    side : Side = Side.Left, diag : Diag = Diag.NonUnit,
    order : Order = Order.Row,
    ldA : int = 0,  ldB : int = 0)
    where (Adom.rank == 2) && (Bdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var m = Bdom.dim(1).size : c_int,
        n = Bdom.dim(2).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, order, ldA),
        _ldB = getLeadingDim(Bdom, order, ldB);

    select eltType {
      when real(32) {
        C_BLAS.cblas_strmm(order, side, uplo, trans, diag,
          m, n, alpha, A, _ldA, B, _ldB);
      }
      when real(64) {
        C_BLAS.cblas_dtrmm(order, side, uplo, trans, diag,
          m, n, alpha, A, _ldA, B, _ldB);
      }
      when complex(64) {
        var alpha1 = alpha : complex(64);
        C_BLAS.cblas_ctrmm(order, side, uplo, trans, diag,
          m, n, alpha1, A, _ldA, B, _ldB);
      }
      when complex(128) {
        var alpha1 = alpha : complex(128);
        C_BLAS.cblas_ztrmm(order, side, uplo, trans, diag,
          m, n, alpha1, A, _ldA, B, _ldB);
      }
      otherwise {
        halt("Unknown type in trmm");
      }
    }

  }

  /*
    Wrapper for the `TRSM routines <http://www.netlib.org/lapack/explore-html/db/def/group__complex__blas__level3_gaf33844c7fd27e5434496d2ce0c1fc9d4.html#gaf33844c7fd27e5434496d2ce0c1fc9d4>`_

    Solves the matrix equation::

      op(A) * X = alpha * B

    or::

      X * op(A) = alpha * B

    where ``A`` is a triangular matrix.
  */
  proc trsm(A : [?Adom],  B : [?Bdom],
    alpha,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    side : Side = Side.Left, diag : Diag = Diag.NonUnit,
    order : Order = Order.Row,
    ldA : int = 0,  ldB : int = 0)
    where (Adom.rank == 2) && (Bdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var m = Bdom.dim(1).size : c_int,
        n = Bdom.dim(2).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, order, ldA),
        _ldB = getLeadingDim(Bdom, order, ldB);

    select eltType {
      when real(32) {
        C_BLAS.cblas_strsm(order, side, uplo, trans, diag,
          m, n, alpha, A, _ldA, B, _ldB);
      }
      when real(64) {
        C_BLAS.cblas_dtrsm(order, side, uplo, trans, diag,
          m, n, alpha, A, _ldA, B, _ldB);
      }
      when complex(64) {
        var alpha1 = alpha : complex(64);
        C_BLAS.cblas_ctrsm(order, side, uplo, trans, diag,
          m, n, alpha1, A, _ldA, B, _ldB);
      }
      when complex(128) {
        var alpha1 = alpha : complex(128);
        C_BLAS.cblas_ztrsm(order, side, uplo, trans, diag,
          m, n, alpha1, A, _ldA, B, _ldB);
      }
      otherwise {
        halt("Unknown type in trsm");
      }
    }

  }


  // Helper function
  pragma "no doc"
  private inline proc getLeadingDim(Adom : domain(2), order : Order, ldA : int) : c_int {
    var _ldA = ldA : c_int;
    if ldA==0 {
      if order==Order.Row
                  then _ldA = Adom.dim(2).size : c_int;
                  else _ldA = Adom.dim(1).size : c_int;
    }
    return _ldA;
  }


  /*

    Support for low-level native CBLAS bindings.

    This submodule wraps the netlib CBLAS implementation, providing access to
    all of CBLAS calls.

    Arrays are passed in directly, while pointers to scalar
    quantities (including complex numbers) are passed by reference (removing
    the need to wrap these with ``c_ptrTo``). As with BLAS calls in C,
    the user is responsible for passing in array dimensions etc. Furthermore,
    different array element types require using different functions.

    Refer to the
    `CBLAS documentation <http://www.netlib.org/lapack/explore-html/dir_f88bc7ad48bfd56d75bf9d4836a2bb00.html>`_
    of the reference version for the usage of this module.

  */
  module C_BLAS {
    extern type CBLAS_INDEX;

    // Define the external types
    // These are C enums, so we define these as c_ints;
    extern type CBLAS_ORDER = c_int;
    extern type CBLAS_TRANSPOSE = c_int;
    extern type CBLAS_UPLO = c_int;
    extern type CBLAS_DIAG = c_int;
    extern type CBLAS_SIDE = c_int;

    extern const CblasRowMajor : CBLAS_ORDER;
    extern const CblasColMajor : CBLAS_ORDER;
    extern const CblasNoTrans : CBLAS_TRANSPOSE;
    extern const CblasTrans   : CBLAS_TRANSPOSE;
    extern const CblasConjTrans : CBLAS_TRANSPOSE;
    extern const CblasUpper : CBLAS_UPLO;
    extern const CblasLower : CBLAS_UPLO;
    extern const CblasNonUnit : CBLAS_DIAG;
    extern const CblasUnit : CBLAS_DIAG;
    extern const CblasLeft : CBLAS_SIDE;
    extern const CblasRight : CBLAS_SIDE;

    {
      assert(Order.Row:c_int == CblasRowMajor,"Enum value for Order.Row does not agree with CblasRowMajor");
      assert(Order.Col:c_int == CblasColMajor,"Enum value for Order.Col does not agree with CblasColMajor");
      assert(Op.N:c_int == CblasNoTrans,"Enum value for Op.N does not agree with CblasNoTrans");
      assert(Op.T:c_int == CblasTrans,"Enum value for Op.T does not agree with CblasTrans");
      assert(Op.H:c_int == CblasConjTrans,"Enum value for Op.H does not agree with CblasConjTrans");
      assert(Uplo.Upper:c_int == CblasUpper,"Enum value for Uplo.Upper does not agree with CblasUpper");
      assert(Uplo.Lower:c_int == CblasLower,"Enum value for Uplo.Lower does not agree with CblasLower");
      assert(Diag.NonUnit:c_int == CblasNonUnit,"Enum value for Diag.NonUnit does not agree with CblasNonUnit");
      assert(Diag.Unit:c_int == CblasUnit,"Enum value for Diag.Unit does not agree with CblasUnit");
      assert(Side.Left:c_int == CblasLeft,"Enum value for Side.Left does not agree with CblasLeft");
      assert(Side.Right:c_int == CblasRight,"Enum value for Side.Right does not agree with CblasRight");
    }

    extern proc cblas_sdsdot (N: c_int, alpha: c_float, X: []c_float, incX: c_int, Y: []c_float, incY: c_int): c_float;
    extern proc cblas_dsdot (N: c_int, X: []c_float, incX: c_int, Y: []c_float, incY: c_int): c_double;
    extern proc cblas_sdot (N: c_int, X: []c_float, incX: c_int, Y: []c_float, incY: c_int): c_float;
    extern proc cblas_ddot (N: c_int, X: []c_double, incX: c_int, Y: []c_double, incY: c_int): c_double;
    extern proc cblas_cdotu_sub (N: c_int, X: [], incX: c_int, Y: [], incY: c_int, ref dotu);
    extern proc cblas_cdotc_sub (N: c_int, X: [], incX: c_int, Y: [], incY: c_int, ref dotc);
    extern proc cblas_zdotu_sub (N: c_int, X: [], incX: c_int, Y: [], incY: c_int, ref dotu);
    extern proc cblas_zdotc_sub (N: c_int, X: [], incX: c_int, Y: [], incY: c_int, ref dotc);
    extern proc cblas_snrm2 (N: c_int, X: []c_float, incX: c_int): c_float;
    extern proc cblas_sasum (N: c_int, X: []c_float, incX: c_int): c_float;
    extern proc cblas_dnrm2 (N: c_int, X: []c_double, incX: c_int): c_double;
    extern proc cblas_dasum (N: c_int, X: []c_double, incX: c_int): c_double;
    extern proc cblas_scnrm2 (N: c_int, X: [], incX: c_int): c_float;
    extern proc cblas_scasum (N: c_int, X: [], incX: c_int): c_float;
    extern proc cblas_dznrm2 (N: c_int, X: [], incX: c_int): c_double;
    extern proc cblas_dzasum (N: c_int, X: [], incX: c_int): c_double;
    extern proc cblas_isamax (N: c_int, X: []c_float, incX: c_int): CBLAS_INDEX;
    extern proc cblas_idamax (N: c_int, X: []c_double, incX: c_int): CBLAS_INDEX;
    extern proc cblas_icamax (N: c_int, X: [], incX: c_int): CBLAS_INDEX;
    extern proc cblas_izamax (N: c_int, X: [], incX: c_int): CBLAS_INDEX;
    extern proc cblas_sswap (N: c_int, X: []c_float, incX: c_int, Y: []c_float, incY: c_int);
    extern proc cblas_scopy (N: c_int, X: []c_float, incX: c_int, Y: []c_float, incY: c_int);
    extern proc cblas_saxpy (N: c_int, alpha: c_float, X: []c_float, incX: c_int, Y: []c_float, incY: c_int);
    extern proc cblas_dswap (N: c_int, X: []c_double, incX: c_int, Y: []c_double, incY: c_int);
    extern proc cblas_dcopy (N: c_int, X: []c_double, incX: c_int, Y: []c_double, incY: c_int);
    extern proc cblas_daxpy (N: c_int, alpha: c_double, X: []c_double, incX: c_int, Y: []c_double, incY: c_int);
    extern proc cblas_cswap (N: c_int, X: [], incX: c_int, Y: [], incY: c_int);
    extern proc cblas_ccopy (N: c_int, X: [], incX: c_int, Y: [], incY: c_int);
    extern proc cblas_caxpy (N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int);
    extern proc cblas_zswap (N: c_int, X: [], incX: c_int, Y: [], incY: c_int);
    extern proc cblas_zcopy (N: c_int, X: [], incX: c_int, Y: [], incY: c_int);
    extern proc cblas_zaxpy (N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int);
    extern proc cblas_srotg (ref a: c_float, ref b: c_float, ref c: c_float, ref s: c_float);
    extern proc cblas_srotmg (ref d1: c_float, ref d2: c_float, ref b1: c_float, b2: c_float, P: []c_float);
    extern proc cblas_srot (N: c_int, X: []c_float, incX: c_int, Y: []c_float, incY: c_int, c: c_float, s: c_float);
    extern proc cblas_srotm (N: c_int, X: []c_float, incX: c_int, Y: []c_float, incY: c_int, P: []c_float);
    extern proc cblas_drotg (ref a: c_double, ref b: c_double, ref c: c_double, ref s: c_double);
    extern proc cblas_drotmg (ref d1: c_double, ref d2: c_double, ref b1: c_double, b2: c_double, P: []c_double);
    extern proc cblas_drot (N: c_int, X: []c_double, incX: c_int, Y: []c_double, incY: c_int, c: c_double, s: c_double);
    extern proc cblas_drotm (N: c_int, X: []c_double, incX: c_int, Y: []c_double, incY: c_int, P: []c_double);
    extern proc cblas_sscal (N: c_int, alpha: c_float, X: []c_float, incX: c_int);
    extern proc cblas_dscal (N: c_int, alpha: c_double, X: []c_double, incX: c_int);
    extern proc cblas_cscal (N: c_int, ref alpha, X: [], incX: c_int);
    extern proc cblas_zscal (N: c_int, ref alpha, X: [], incX: c_int);
    extern proc cblas_csscal (N: c_int, alpha: c_float, X: [], incX: c_int);
    extern proc cblas_zdscal (N: c_int, alpha: c_double, X: [], incX: c_int);
    extern proc cblas_sgemv (order, TransA, M: c_int, N: c_int, alpha: c_float, A: []c_float, lda: c_int, X: []c_float, incX: c_int, beta: c_float, Y: []c_float, incY: c_int);
    extern proc cblas_sgbmv (order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, alpha: c_float, A: []c_float, lda: c_int, X: []c_float, incX: c_int, beta: c_float, Y: []c_float, incY: c_int);
    extern proc cblas_strmv (order, Uplo, TransA, Diag, N: c_int, A: []c_float, lda: c_int, X: []c_float, incX: c_int);
    extern proc cblas_stbmv (order, Uplo, TransA, Diag, N: c_int, K: c_int, A: []c_float, lda: c_int, X: []c_float, incX: c_int);
    extern proc cblas_stpmv (order, Uplo, TransA, Diag, N: c_int, Ap: []c_float, X: []c_float, incX: c_int);
    extern proc cblas_strsv (order, Uplo, TransA, Diag, N: c_int, A: []c_float, lda: c_int, X: []c_float, incX: c_int);
    extern proc cblas_stbsv (order, Uplo, TransA, Diag, N: c_int, K: c_int, A: []c_float, lda: c_int, X: []c_float, incX: c_int);
    extern proc cblas_stpsv (order, Uplo, TransA, Diag, N: c_int, Ap: []c_float, X: []c_float, incX: c_int);
    extern proc cblas_dgemv (order, TransA, M: c_int, N: c_int, alpha: c_double, A: []c_double, lda: c_int, X: []c_double, incX: c_int, beta: c_double, Y: []c_double, incY: c_int);
    extern proc cblas_dgbmv (order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, alpha: c_double, A: []c_double, lda: c_int, X: []c_double, incX: c_int, beta: c_double, Y: []c_double, incY: c_int);
    extern proc cblas_dtrmv (order, Uplo, TransA, Diag, N: c_int, A: []c_double, lda: c_int, X: []c_double, incX: c_int);
    extern proc cblas_dtbmv (order, Uplo, TransA, Diag, N: c_int, K: c_int, A: []c_double, lda: c_int, X: []c_double, incX: c_int);
    extern proc cblas_dtpmv (order, Uplo, TransA, Diag, N: c_int, Ap: []c_double, X: []c_double, incX: c_int);
    extern proc cblas_dtrsv (order, Uplo, TransA, Diag, N: c_int, A: []c_double, lda: c_int, X: []c_double, incX: c_int);
    extern proc cblas_dtbsv (order, Uplo, TransA, Diag, N: c_int, K: c_int, A: []c_double, lda: c_int, X: []c_double, incX: c_int);
    extern proc cblas_dtpsv (order, Uplo, TransA, Diag, N: c_int, Ap: []c_double, X: []c_double, incX: c_int);
    extern proc cblas_cgemv (order, TransA, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int);
    extern proc cblas_cgbmv (order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int);
    extern proc cblas_ctrmv (order, Uplo, TransA, Diag, N: c_int, A: [], lda: c_int, X: [], incX: c_int);
    extern proc cblas_ctbmv (order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [], lda: c_int, X: [], incX: c_int);
    extern proc cblas_ctpmv (order, Uplo, TransA, Diag, N: c_int, Ap: [], X: [], incX: c_int);
    extern proc cblas_ctrsv (order, Uplo, TransA, Diag, N: c_int, A: [], lda: c_int, X: [], incX: c_int);
    extern proc cblas_ctbsv (order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [], lda: c_int, X: [], incX: c_int);
    extern proc cblas_ctpsv (order, Uplo, TransA, Diag, N: c_int, Ap: [], X: [], incX: c_int);
    extern proc cblas_zgemv (order, TransA, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int);
    extern proc cblas_zgbmv (order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int);
    extern proc cblas_ztrmv (order, Uplo, TransA, Diag, N: c_int, A: [], lda: c_int, X: [], incX: c_int);
    extern proc cblas_ztbmv (order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [], lda: c_int, X: [], incX: c_int);
    extern proc cblas_ztpmv (order, Uplo, TransA, Diag, N: c_int, Ap: [], X: [], incX: c_int);
    extern proc cblas_ztrsv (order, Uplo, TransA, Diag, N: c_int, A: [], lda: c_int, X: [], incX: c_int);
    extern proc cblas_ztbsv (order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [], lda: c_int, X: [], incX: c_int);
    extern proc cblas_ztpsv (order, Uplo, TransA, Diag, N: c_int, Ap: [], X: [], incX: c_int);
    extern proc cblas_ssymv (order, Uplo, N: c_int, alpha: c_float, A: []c_float, lda: c_int, X: []c_float, incX: c_int, beta: c_float, Y: []c_float, incY: c_int);
    extern proc cblas_ssbmv (order, Uplo, N: c_int, K: c_int, alpha: c_float, A: []c_float, lda: c_int, X: []c_float, incX: c_int, beta: c_float, Y: []c_float, incY: c_int);
    extern proc cblas_sspmv (order, Uplo, N: c_int, alpha: c_float, Ap: []c_float, X: []c_float, incX: c_int, beta: c_float, Y: []c_float, incY: c_int);
    extern proc cblas_sger (order, M: c_int, N: c_int, alpha: c_float, X: []c_float, incX: c_int, Y: []c_float, incY: c_int, A: []c_float, lda: c_int);
    extern proc cblas_ssyr (order, Uplo, N: c_int, alpha: c_float, X: []c_float, incX: c_int, A: []c_float, lda: c_int);
    extern proc cblas_sspr (order, Uplo, N: c_int, alpha: c_float, X: []c_float, incX: c_int, Ap: []c_float);
    extern proc cblas_ssyr2 (order, Uplo, N: c_int, alpha: c_float, X: []c_float, incX: c_int, Y: []c_float, incY: c_int, A: []c_float, lda: c_int);
    extern proc cblas_sspr2 (order, Uplo, N: c_int, alpha: c_float, X: []c_float, incX: c_int, Y: []c_float, incY: c_int, A: []c_float);
    extern proc cblas_dsymv (order, Uplo, N: c_int, alpha: c_double, A: []c_double, lda: c_int, X: []c_double, incX: c_int, beta: c_double, Y: []c_double, incY: c_int);
    extern proc cblas_dsbmv (order, Uplo, N: c_int, K: c_int, alpha: c_double, A: []c_double, lda: c_int, X: []c_double, incX: c_int, beta: c_double, Y: []c_double, incY: c_int);
    extern proc cblas_dspmv (order, Uplo, N: c_int, alpha: c_double, Ap: []c_double, X: []c_double, incX: c_int, beta: c_double, Y: []c_double, incY: c_int);
    extern proc cblas_dger (order, M: c_int, N: c_int, alpha: c_double, X: []c_double, incX: c_int, Y: []c_double, incY: c_int, A: []c_double, lda: c_int);
    extern proc cblas_dsyr (order, Uplo, N: c_int, alpha: c_double, X: []c_double, incX: c_int, A: []c_double, lda: c_int);
    extern proc cblas_dspr (order, Uplo, N: c_int, alpha: c_double, X: []c_double, incX: c_int, Ap: []c_double);
    extern proc cblas_dsyr2 (order, Uplo, N: c_int, alpha: c_double, X: []c_double, incX: c_int, Y: []c_double, incY: c_int, A: []c_double, lda: c_int);
    extern proc cblas_dspr2 (order, Uplo, N: c_int, alpha: c_double, X: []c_double, incX: c_int, Y: []c_double, incY: c_int, A: []c_double);
    extern proc cblas_chemv (order, Uplo, N: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int);
    extern proc cblas_chbmv (order, Uplo, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int);
    extern proc cblas_chpmv (order, Uplo, N: c_int, ref alpha, Ap: [], X: [], incX: c_int, ref beta, Y: [], incY: c_int);
    extern proc cblas_cgeru (order, M: c_int, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int);
    extern proc cblas_cgerc (order, M: c_int, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int);
    extern proc cblas_cher (order, Uplo, N: c_int, alpha: c_float, X: [], incX: c_int, A: [], lda: c_int);
    extern proc cblas_chpr (order, Uplo, N: c_int, alpha: c_float, X: [], incX: c_int, A: []);
    extern proc cblas_cher2 (order, Uplo, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int);
    extern proc cblas_chpr2 (order, Uplo, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, Ap: []);
    extern proc cblas_zhemv (order, Uplo, N: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int);
    extern proc cblas_zhbmv (order, Uplo, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int);
    extern proc cblas_zhpmv (order, Uplo, N: c_int, ref alpha, Ap: [], X: [], incX: c_int, ref beta, Y: [], incY: c_int);
    extern proc cblas_zgeru (order, M: c_int, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int);
    extern proc cblas_zgerc (order, M: c_int, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int);
    extern proc cblas_zher (order, Uplo, N: c_int, alpha: c_double, X: [], incX: c_int, A: [], lda: c_int);
    extern proc cblas_zhpr (order, Uplo, N: c_int, alpha: c_double, X: [], incX: c_int, A: []);
    extern proc cblas_zher2 (order, Uplo, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int);
    extern proc cblas_zhpr2 (order, Uplo, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, Ap: []);
    extern proc cblas_sgemm (Order, TransA, TransB, M: c_int, N: c_int, K: c_int, alpha: c_float, A: []c_float, lda: c_int, B: []c_float, ldb: c_int, beta: c_float, C: []c_float, ldc: c_int);
    extern proc cblas_ssymm (Order, Side, Uplo, M: c_int, N: c_int, alpha: c_float, A: []c_float, lda: c_int, B: []c_float, ldb: c_int, beta: c_float, C: []c_float, ldc: c_int);
    extern proc cblas_ssyrk (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_float, A: []c_float, lda: c_int, beta: c_float, C: []c_float, ldc: c_int);
    extern proc cblas_ssyr2k (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_float, A: []c_float, lda: c_int, B: []c_float, ldb: c_int, beta: c_float, C: []c_float, ldc: c_int);
    extern proc cblas_strmm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_float, A: []c_float, lda: c_int, B: []c_float, ldb: c_int);
    extern proc cblas_strsm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_float, A: []c_float, lda: c_int, B: []c_float, ldb: c_int);
    extern proc cblas_dgemm (Order, TransA, TransB, M: c_int, N: c_int, K: c_int, alpha: c_double, A: []c_double, lda: c_int, B: []c_double, ldb: c_int, beta: c_double, C: []c_double, ldc: c_int);
    extern proc cblas_dsymm (Order, Side, Uplo, M: c_int, N: c_int, alpha: c_double, A: []c_double, lda: c_int, B: []c_double, ldb: c_int, beta: c_double, C: []c_double, ldc: c_int);
    extern proc cblas_dsyrk (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_double, A: []c_double, lda: c_int, beta: c_double, C: []c_double, ldc: c_int);
    extern proc cblas_dsyr2k (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_double, A: []c_double, lda: c_int, B: []c_double, ldb: c_int, beta: c_double, C: []c_double, ldc: c_int);
    extern proc cblas_dtrmm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_double, A: []c_double, lda: c_int, B: []c_double, ldb: c_int);
    extern proc cblas_dtrsm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_double, A: []c_double, lda: c_int, B: []c_double, ldb: c_int);
    extern proc cblas_cgemm (Order, TransA, TransB, M: c_int, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int);
    extern proc cblas_csymm (Order, Side, Uplo, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int);
    extern proc cblas_csyrk (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, ref beta, C: [], ldc: c_int);
    extern proc cblas_csyr2k (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int);
    extern proc cblas_ctrmm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int);
    extern proc cblas_ctrsm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int);
    extern proc cblas_zgemm (Order, TransA, TransB, M: c_int, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int);
    extern proc cblas_zsymm (Order, Side, Uplo, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int);
    extern proc cblas_zsyrk (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, ref beta, C: [], ldc: c_int);
    extern proc cblas_zsyr2k (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int);
    extern proc cblas_ztrmm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int);
    extern proc cblas_ztrsm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int);
    extern proc cblas_chemm (Order, Side, Uplo, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int);
    extern proc cblas_cherk (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_float, A: [], lda: c_int, beta: c_float, C: [], ldc: c_int);
    extern proc cblas_cher2k (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, beta: c_float, C: [], ldc: c_int);
    extern proc cblas_zhemm (Order, Side, Uplo, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int);
    extern proc cblas_zherk (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_double, A: [], lda: c_int, beta: c_double, C: [], ldc: c_int);
    extern proc cblas_zher2k (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, beta: c_double, C: [], ldc: c_int);

  }


}

