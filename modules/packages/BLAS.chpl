/*
 * Copyright 2004-2018 Cray Inc.
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

BLAS (Basic Linear Algebra Subprograms) are the de facto standard routines for
low-level matrix, vector, and scalar operations. While `netlib
<http://www.netlib.org/blas/#_documentation>`_ provides the official
reference version of BLAS, this documentation refers to the
`MKL BLAS <https://software.intel.com/en-us/node/520725>`_ documentation, due
to interface similarities.

This module is intended to work with non-distributed dense rectangular arrays.


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
  <http://www.netlib.org/blas/#_cblas>`_, but many other implementations are
  compatible as well.
  Using a version of BLAS optimized for the user's system will yield the best
  performance.

  There are a few known caveats with some popular implementations:

  * **ATLAS**

    * The compilation command above likely requires the additional flag: ``-latlas``

  * **MKL**

    * Compile with :param:`isBLAS_MKL` if using MKL BLAS.

  * **OpenBLAS**

    * The header files that are included with OpenBLAS differ from the reference
      C_BLAS prototypes for complex arguments by using ``float*`` and ``double*``
      pointers, instead of ``void*`` pointers.  Using this will likely result in
      warnings about incompatible pointer types. These may be ignored.

Cray Systems:
  No compiler flags should be necessary when compiling BLAS programs on
  Crays. The **CrayBLAS** implementation is made available through Cray's libsci,
  which comes installed on all Cray systems. This is typically loaded by
  default, but can be manually loaded with ``module load cray-libsci`` as well.
  Chapel programs compiled on Crays utilize the ``cc`` wrapper as the backend
  compiler, which implicitly links against the libsci library. Therefore, no
  additional steps are required of the user.

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
``C_BLAS`` submodule.

The ``ldA`` argument is omitted from the Chapel BLAS API. Chapel determines the
dimensions of the matrices from the arrays that are passed in, even when one is
passing in a sub-array such that the array elements are not contiguously stored
in memory.

.. warning::

  The ``CHPL_LOCALE_MODEL=numa`` configuration is currently not supported by
  this module.

.. MKL Documentation References

.. _GEMM:   https://software.intel.com/en-us/node/ae8380b9-cac8-4c57-9af3-2eaac6acfc1b
.. _HEMM:   https://software.intel.com/en-us/node/ecdcb863-3cb8-40ce-84f6-4cbc66cfc659
.. _HERK:   https://software.intel.com/en-us/node/4ee4425c-42b6-4cc0-b2f0-6cfaa2117cb8
.. _HER2K:  https://software.intel.com/en-us/node/5c17da7b-d8f9-44ec-921e-d6285238d48e
.. _SYMM:   https://software.intel.com/en-us/node/8273eec0-2663-4843-90cd-12fd7a1baa6c
.. _SYRK:   https://software.intel.com/en-us/node/04ca14b7-ec46-4836-8693-c4c7de26048e
.. _SYR2K:  https://software.intel.com/en-us/node/d2a17c66-4265-4c56-84b9-2817ded7ee29
.. _TRMM:   https://software.intel.com/en-us/node/60775edf-9a2b-4ad4-a627-dabbd24dcc80
.. _TRSM:   https://software.intel.com/en-us/node/fc848297-8a33-42e6-826f-f4ae361c1800
.. _GBMV:   https://software.intel.com/en-us/node/181e3c0c-e04a-4464-b87c-7e5d085f38a8
.. _GEMV:   https://software.intel.com/en-us/node/78589c78-9c40-4846-b698-2c36e04b8869
.. _GER:    https://software.intel.com/en-us/node/94156ede-4add-4830-940e-1ca5688abe88
.. _GERC:   https://software.intel.com/en-us/node/f0469e83-40cc-40a0-9cd7-eeb8ea718aae
.. _GERU:   https://software.intel.com/en-us/node/e62ae6d4-ba79-4034-96cc-46753ad0ec63
.. _HBMV:   https://software.intel.com/en-us/node/02ca78c6-cad1-4c31-a6eb-5c645e862a52
.. _HEMV:   https://software.intel.com/en-us/node/5c5972fb-d88f-4e32-8c4c-66f004945097
.. _HER:    https://software.intel.com/en-us/node/c47a886b-8816-4cb6-be84-124b7ac70259
.. _HER2:   https://software.intel.com/en-us/node/e2ddc00b-d8a5-4cc7-96f7-d08882151431
.. _HPMV:   https://software.intel.com/en-us/node/fc602a99-7cb3-44f7-a22e-a89de255181a
.. _HPR:    https://software.intel.com/en-us/node/60392a82-4400-4c9c-9c14-7a2e0cea4025
.. _HPR2:   https://software.intel.com/en-us/node/163c6278-762f-40d7-8214-788d46ca4ea9
.. _SBMV:   https://software.intel.com/en-us/node/9a02d0da-7aa4-4c5d-be91-78d4ce4a42e9
.. _SPMV:   https://software.intel.com/en-us/node/fcd03a47-3896-4ad9-b0f9-1600c67deaa8
.. _SPR:    https://software.intel.com/en-us/node/3a77ea66-38a4-42cd-ac6b-68c34c9f7e13
.. _SPR2:   https://software.intel.com/en-us/node/9e17a098-ccbb-4cd4-9695-72c60a69904b
.. _SYMV:   https://software.intel.com/en-us/node/687edc3e-fe76-4635-a425-40aab089cefc
.. _SYR:    https://software.intel.com/en-us/node/08ab04b6-7aa7-4ce0-a69c-d4a4bc326ebe
.. _SYR2:   https://software.intel.com/en-us/node/c2c54a2e-98d0-466a-a6b3-ae5f8cada666
.. _TBMV:   https://software.intel.com/en-us/node/ae4b8c9d-8af2-4c5c-9e30-a8075cfef635
.. _TBSV:   https://software.intel.com/en-us/node/f66b8633-aa82-4a54-ba5f-7aeaa40fc4f0
.. _TPMV:   https://software.intel.com/en-us/node/00b446f2-5adb-4c7d-a4f5-3a63fee8f20d
.. _TPSV:   https://software.intel.com/en-us/node/daea8c6c-7e29-461a-86d9-1ad01ae82cd6
.. _TRMV:   https://software.intel.com/en-us/node/ce930002-ce05-4c38-a613-62931e024078
.. _TRSV:   https://software.intel.com/en-us/node/4a450630-f24a-442e-94e2-e8f4e4ae4819
.. _ASUM:   https://software.intel.com/en-us/node/ae455054-52b2-460a-8887-12192e01d899
.. _AXPY:   https://software.intel.com/en-us/node/c8cbb256-eab7-4629-80ff-14029038e6b7
.. _COPY:   https://software.intel.com/en-us/node/7e841640-7c31-4cd1-bc44-34f7d2343215
.. _DOT:    https://software.intel.com/en-us/node/961a869b-14d9-4e4e-98fd-9ca13802c671
.. _SDOT:   https://software.intel.com/en-us/node/7eaa9448-1ef0-404d-b8fb-c233a6b0142f
.. _DOTC:   https://software.intel.com/en-us/node/a2c46a8c-343b-401d-88b6-9a6b88a4fa47
.. _DOTU:   https://software.intel.com/en-us/node/e81ff20a-c401-4a15-a64f-cc0daa2bd65d
.. _NRM2:   https://software.intel.com/en-us/node/aca1a6dc-60a0-4689-becc-2e0f4662c093
.. _ROT:    https://software.intel.com/en-us/node/d8e66614-3ad9-4a37-90e8-3ef59e559ad6
.. _ROTG:   https://software.intel.com/en-us/node/1a7e7c3a-e209-4c7a-b8ea-00c60e103e1f
.. _ROTM:   https://software.intel.com/en-us/node/104aa6a5-5eef-4a3b-badf-78534ee1dee4
.. _ROTMG:  https://software.intel.com/en-us/node/9ea3f8dd-fe3f-4a8a-ba12-ec25fabda6e6
.. _SCAL:   https://software.intel.com/en-us/node/54f97254-4ed4-46ce-90f9-aa3c745ad840
.. _SWAP:   https://software.intel.com/en-us/node/54a2904f-b8ed-4a5f-a0f2-a2be10d61495
.. _AMAX:   https://software.intel.com/en-us/node/acb0d496-e032-4cf3-82f9-92b2e0bfc6d1
.. _AMIN:   https://software.intel.com/en-us/node/50dceaa5-3463-402f-8065-a48bc68e0888
.. _CABS1:  https://software.intel.com/en-us/node/64961e94-92d0-4671-90e6-86995e259a85

.. BLAS Module TODO:
  - Clearer compiler errors instead of using where-clauses
  - Support more implementations using config param-wrapped require statements
    - related: general RequireMKL module
  - More consistent documentation
  - Support banded/packed matrix routines

*/
module BLAS {

  /*
    Tells the BLAS module to look for ``mkl_cblas.h`` instead of ``cblas.h``.
    Set this to `true` if you are using the Intel MKL BLAS implementation.
  */
  config param isBLAS_MKL=false;

  use C_BLAS;

  use SysCTypes;

  if (isBLAS_MKL) {
    require "mkl_cblas.h";
  } else {
    require "cblas.h";
  }

  /* Return `true` if type is supported by BLAS */
  proc isBLASType(type t) param: bool{
    return isRealType(t) || isComplexType(t);
  }

  /* Define row or column order */
  enum Order {Row=101: c_int, Col};

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
    Wrapper for the `GEMM`_ routines::

      C := alpha * op(A) * op(B) + beta * C

  */
  proc gemm(A : [?Adom], B : [?Bdom], C : [?Cdom],
    alpha, beta,
    opA : Op = Op.N, opB : Op = Op.N,
    order : Order = Order.Row)
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
    var _ldA = getLeadingDim(A, order),
        _ldB = getLeadingDim(B, order),
        _ldC = getLeadingDim(C, order);

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
        compilerError("Unknown type in gemm: ", eltType:string);
      }
    }

  }

  /*
    Wrapper for the `SYMM`_ routines::

      C := alpha * A * B + beta * C

    or::

      C := alpha * B * A + beta * C

    where ``A`` is a symmetric matrix.
  */
  proc symm(A : [?Adom], B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  side : Side = Side.Left,
    order : Order = Order.Row)
    where (Adom.rank == 2) && (Bdom.rank==2) && (Cdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var m = Cdom.dim(1).size : c_int,
        n = Cdom.dim(2).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order),
        _ldB = getLeadingDim(B, order),
        _ldC = getLeadingDim(C, order);

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
        compilerError("Unknown type in symm: ", eltType:string);
      }
    }

  }

  /*
    Wrapper for the `HEMM`_ routines::

      C := alpha * A * B + beta * C

    or::

      C := alpha * B * A + beta * C

    where ``A`` is an hermitian matrix.
  */
  proc hemm(A : [?Adom], B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  side : Side = Side.Left,
    order : Order = Order.Row)
    where (Adom.rank == 2) && (Bdom.rank==2) && (Cdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var m = Cdom.dim(1).size : c_int,
        n = Cdom.dim(2).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order),
        _ldB = getLeadingDim(B, order),
        _ldC = getLeadingDim(C, order);

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
        compilerError("Unknown type in hemm: ", eltType:string);
      }
    }

  }

  /*
    Wrapper for the `SYRK`_ routines::

      C := alpha * A * A**T + beta * C

    or::

      C := alpha * A**T * A + beta * C


    where ``C`` is a symmetric matrix, and ``A**T`` is the transpose of ``A``.
  */
  proc syrk(A : [?Adom],  C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    order : Order = Order.Row)
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
    var _ldA = getLeadingDim(A, order),
        _ldC = getLeadingDim(C, order);

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
        compilerError("Unknown type in syrk: ", eltType:string);
      }
    }

  }

  /*
    Wrapper for the `HERK`_ routines::

      C := alpha * A * A**H + beta * C

    or::

      C := alpha * A**H * A + beta * C

    where ``C`` is an hermitian matrix, and ``A**H`` is the conjugate transpose
    of ``A``.
  */
  proc herk(A : [?Adom],  C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    order : Order = Order.Row)
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
    var _ldA = getLeadingDim(A, order),
        _ldC = getLeadingDim(C, order);

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
        compilerError("Unknown type in syrk: ", eltType:string);
      }
    }

  }


  /*
    Wrapper for the `SYR2K`_ routines::

      C := alpha * A * B**T + alpha * B * A**T +  beta * C

    or::

      C := alpha * A**T * B + alpha * B**T * A +  beta * C

    where ``C`` is a symmetric matrix, and ``A**T`` and ``B**T`` are the
    transpose of ``A`` and ``B`` , respectively.
  */
  proc syr2k(A : [?Adom],  B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    order : Order = Order.Row)
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
    var _ldA = getLeadingDim(A, order),
        _ldB = getLeadingDim(B, order),
        _ldC = getLeadingDim(C, order);

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
        compilerError("Unknown type in syr2k: ", eltType:string);
      }
    }

  }

  /*
    Wrapper for the `HER2K`_ routines::

      C := alpha * A * B**H + conjg(alpha) * B * A**H +  beta * C

    or::

      C := alpha * A**H * B + conjg(alpha) * B**H * A +  beta * C

    where ``C`` is an hermitian matrix, ``B**H`` is the conjugate transpose of
    ``B`` , ``conjg(alpha)`` is the complex conjugate of ``alpha``.
  */
  proc her2k(A : [?Adom],  B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    order : Order = Order.Row)
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
    var _ldA = getLeadingDim(A, order),
        _ldB = getLeadingDim(B, order),
        _ldC = getLeadingDim(C, order);

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
        compilerError("Unknown type in her2k: ", eltType:string);
      }
    }

  }

  /*
    Wrapper for the `TRMM`_ routines::

      B := alpha * op(A) * B

    or::

      B := alpha * B * op(A)

    where ``A`` is a triangular matrix.
  */
  proc trmm(A : [?Adom] ?eltType,  B : [?Bdom] eltType,
    alpha,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    side : Side = Side.Left, diag : Diag = Diag.NonUnit,
    order : Order = Order.Row)
    throws where (Adom.rank == 2) && (Bdom.rank==2)
  {


    // Determine sizes
    var m = Bdom.dim(1).size : c_int,
        n = Bdom.dim(2).size : c_int;

    if m != n then
      throw new IllegalArgumentError("B", "Non-square array of dimensions %ix%i passed to trmm".format(m, n));

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order),
        _ldB = getLeadingDim(B, order);

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
        compilerError("Unknown type in trmm: ", eltType:string);
      }
    }

  }

  /*
    Wrapper for the `TRSM`_ routines::

      op(A) * X = alpha * B

    or::

      X * op(A) = alpha * B

    where ``A`` is a triangular matrix.
  */
  proc trsm(A : [?Adom],  B : [?Bdom],
    alpha,
    uplo : Uplo = Uplo.Upper,  trans : Op = Op.N,
    side : Side = Side.Left, diag : Diag = Diag.NonUnit,
    order : Order = Order.Row)
    throws where (Adom.rank == 2) && (Bdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var m = Bdom.dim(1).size : c_int,
        n = Bdom.dim(2).size : c_int;

    if m != n then
      throw new IllegalArgumentError("B", "Non-square array of dimensions %ix%i passed to trsm".format(m, n));

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order),
        _ldB = getLeadingDim(B, order);

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
        compilerError("Unknown type in trsm: ", eltType:string);
      }
    }

  }


  /* Level 2 BLAS */

  pragma "no doc"
  /*
    Wrapper for the `GBMV`_ routines::

      y := alpha*op(A)*x + beta*y,

    where ``A`` is an ``m``x``n`` band matrix, with ``kl`` sub-diagonals and
    ``ku`` super-diagonals.

    TODO -- Get banded array routines working
   */
  proc gbmv(A : [?Adom] ?eltType,
            X : [?Xdom] eltType, Y : [?Ydom] eltType,
            ref alpha: eltType, ref beta: eltType,
            kl : int = 0, ku : int = 0,
            trans : Op =  Op.N,
            order : Order = Order.Row, incx : c_int = 1, incy : c_int = 1)
            where (Adom.rank == 2) && (Xdom.rank==1) && (Ydom.rank == 1)
  {
    // Determine sizes
    var m = Ydom.dim(1).size : c_int,
        n = Xdom.dim(1).size : c_int;

    // TODO -- 'order' may need to be swapped for banded matrices
    var _ldA = getLeadingDim(A, order);

    var _kl = kl : c_int,
        _ku = ku : c_int;

    select eltType {
      when real(32) {
        C_BLAS.cblas_sgbmv(order, trans, m, n, _kl, _ku, alpha,
            A, _ldA, X, incx, beta, Y, incy);
      }
      when real(64) {
        C_BLAS.cblas_dgbmv(order, trans, m, n, _kl, _ku, alpha,
            A, _ldA, X, incx, beta, Y, incy);
      }
      when complex(64) {
        C_BLAS.cblas_cgbmv(order, trans, m, n, _kl, _ku, alpha,
            A, _ldA, X, incx, beta, Y, incy);
      }
      when complex(128) {
        C_BLAS.cblas_zgbmv(order, trans, m, n, _kl, _ku, alpha,
            A, _ldA, X, incx, beta, Y, incy);
      }
      otherwise {
        compilerError("Unknown type in gbmv: ", eltType:string);
      }
    }
  }

  /*
    Wrapper for the `GEMV`_ routines::

      y := alpha*op(A)*x + beta*y,

    where ``A`` is an ``m``x``n`` matrix.
  */
  proc gemv(A : [?Adom] ?eltType, x : [?xdom] eltType, y : [?ydom] eltType,
            alpha, beta,
            opA : Op = Op.N,
            order : Order = Order.Row,
            incx : c_int = 1, incy : c_int = 1)
            where (Adom.rank == 2) && (xdom.rank == 1) && (ydom.rank == 1)
  {
    // Determine sizes
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    select eltType {
      when real(32) {
        C_BLAS.cblas_sgemv(order, opA, m, n, alpha, A,
          _ldA, x, incx, beta, y, incy);
      }
      when real(64) {
        C_BLAS.cblas_dgemv(order, opA, m, n, alpha, A,
          _ldA, x, incx, beta, y, incy);
      }
      when complex(64) {
        var alpha1 = alpha: complex(64),
            beta1 = beta: complex(64);
        C_BLAS.cblas_cgemv(order, opA, m, n, alpha1, A,
          _ldA, x, incx, beta1, y, incy);
      }
      when complex(128) {
        var alpha1 = alpha: complex(128),
            beta1 = beta: complex(128);
        C_BLAS.cblas_zgemv(order, opA, m, n, alpha1, A,
          _ldA, x, incx, beta1, y, incy);
      }
      otherwise {
        compilerError("Unknown type in gemv: ", eltType:string);
      }
    }
  }

  /*
    Wrapper for `GER`_ routines::

      A := alpha*x*y'+ A

  */
  proc ger(A: [?Adom] ?eltType, X: [?Xdom] eltType, Y: [?Ydom] eltType, alpha,
           order : Order = Order.Row, incx : c_int = 1, incy : c_int = 1)
           where (Adom.rank == 2) && (Xdom.rank == 1) && (Ydom.rank == 1)
  {

    // Determine sizes
    var m = Xdom.dim(1).size : c_int,
        n = Ydom.dim(1).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    select eltType {
      when real(32) {
        C_BLAS.cblas_sger(order, m, n, alpha, X, incx, Y, incy, A, _ldA);
      }
      when real(64) {
        C_BLAS.cblas_dger(order, m, n, alpha, X, incx, Y, incy, A, _ldA);
      }
      otherwise {
        compilerError("Unknown type in ger: ", eltType:string);
      }
    }

  }

  /*
    Wrapper for `GERC`_ routines::

      A := alpha*x*conjg(y') + A

  */
  proc gerc(A: [?Adom] ?eltType, X: [?Xdom] eltType, Y: [?Ydom] eltType,
            ref alpha: eltType,
            order : Order = Order.Row, incx : c_int = 1, incy : c_int = 1)
            where (Adom.rank == 2) && (Xdom.rank == 1) && (Ydom.rank == 1)
  {
    var m = Xdom.dim(1).size : c_int,
        n = Ydom.dim(1).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    select eltType {
      when complex(64) {
        C_BLAS.cblas_cgerc(order, m, n, alpha, X, incx, Y, incy, A, _ldA);
      }
      when complex(128) {
        C_BLAS.cblas_zgerc(order, m, n, alpha, X, incx, Y, incy, A, _ldA);
      }
      otherwise {
        compilerError("Unknown type in gerc: ", eltType:string);
      }
    }

  }

  /*
    Wrapper for the `GERU`_ routines::

      A := alpha*x*y' + A

  */
  proc geru(A: [?Adom] ?eltType, X: [?Xdom] eltType, Y: [?Ydom] eltType,
            ref alpha: eltType,
            order : Order = Order.Row, incx : c_int = 1, incy : c_int = 1)
            where (Adom.rank == 2) && (Xdom.rank == 1) && (Ydom.rank == 1)
  {
    var m = Xdom.dim(1).size : c_int,
        n = Ydom.dim(1).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    select eltType {
      when complex(64) {
        C_BLAS.cblas_cgeru(order, m, n, alpha, X, incx, Y, incy, A, _ldA);
      }
      when complex(128) {
        C_BLAS.cblas_zgeru(order, m, n, alpha, X, incx, Y, incy, A, _ldA);
      }
      otherwise {
        compilerError("Unknown type in geru: ", eltType:string);
      }
    }

  }


  pragma "no doc"
  /*
    Wrapper for the `HBMV`_ routines::

      y := alpha*A*x + beta*y,

    TODO -- Get banded array routines working
  */
  proc hbmv(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType,
            ref alpha: eltType, ref beta: eltType,
            k: int = 0,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper, incx : c_int = 1, incy : c_int = 1)
            where (Adom.rank == 2) && (vDom.rank == 1)
  {
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    var _k = k : c_int;

    select eltType {
      when complex(64) {
        C_BLAS.cblas_chbmv(order, uplo, n, _k, alpha, A, _ldA, X, incx, beta, Y, incy);
      }
      when complex(128) {
        C_BLAS.cblas_zhbmv(order, uplo, n, _k, alpha, A, _ldA, X, incx, beta, Y, incy);
      }
      otherwise {
        compilerError("Unknown type in hbmv: ", eltType:string);
      }
    }
  }

  /*
    Wrapper for the `HEMV`_ routines::

      y := alpha*A*x + beta*y

  */
  proc hemv(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType,
            ref alpha: eltType, ref beta: eltType,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper, incx : c_int = 1, incy : c_int = 1)
            throws where (Adom.rank == 2) && (vDom.rank == 1)
    {
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    if m != n then
      throw new IllegalArgumentError("A", "Non-square array of dimensions %ix%i passed to hemv".format(m, n));

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    select eltType {
      when complex(64) {
        C_BLAS.cblas_chemv(order, uplo, m, alpha, A, _ldA, X, incx, beta, Y, incy);
      }
      when complex(128) {
        C_BLAS.cblas_zhemv(order, uplo, m, alpha, A, _ldA, X, incx, beta, Y, incy);
      }
      otherwise {
        compilerError("Unknown type in hemv: ", eltType:string);
      }
    }
  }

  /*
    Wrapper for the `HER`_ routines::

      A := alpha*x*conjg(x') + A

  */
  proc her(A: [?Adom] ?eltType, X: [?vDom] eltType, alpha,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper, incx : c_int = 1)
            throws where (Adom.rank == 2) && (vDom.rank == 1)
  {
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    if m != n then
      throw new IllegalArgumentError("A", "Non-square array of dimensions %ix%i passed to her".format(m, n));

    // TODO -- Assert alpha is real

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    select eltType {
      when complex(64) {
        C_BLAS.cblas_cher(order, uplo, n, alpha, X, incx, A, _ldA);
      }
      when complex(128) {
        C_BLAS.cblas_zher(order, uplo, n, alpha, X, incx, A, _ldA);
      }
      otherwise {
        compilerError("Unknown type in her: ", eltType:string);
      }
    }
  }
  /*
    Wrapper for `HER2`_ routines::

      A := alpha *x*conjg(y') + conjg(alpha)*y *conjg(x') + A

  */
  proc her2(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType,
            ref alpha: eltType,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper, incx : c_int = 1, incy : c_int = 1)
            throws where (Adom.rank == 2) && (vDom.rank == 1)
    {
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    if m != n then
      throw new IllegalArgumentError("A", "Non-square array of dimensions %ix%i passed to her2".format(m, n));


    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    select eltType {
      when complex(64) {
        C_BLAS.cblas_cher2 (order, uplo, n, alpha, X, incx, Y, incy, A, _ldA);
      }
      when complex(128) {
        C_BLAS.cblas_zher2 (order, uplo, n, alpha, X, incx, Y, incy, A, _ldA);
      }
      otherwise {
        compilerError("Unknown type in her2: ", eltType:string);
      }
    }
  }


  pragma "no doc"
  /*
    Wrapper for the `HPMV`_ routines::

      y := alpha*A*x + beta*y

    TODO -- Get packed array routines working

  */
  proc hpmv(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType,
            ref alpha: eltType, ref beta: eltType,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            incx : c_int = 1, incy : c_int = 1)
            where (Adom.rank == 2) && (vDom.rank == 1)
  {
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    select eltType {
      when complex(64) {
        C_BLAS.cblas_chpmv(order, uplo, n, alpha, A, X, incx, beta, Y, incy);
      }
      when complex(128) {
        C_BLAS.cblas_zhpmv(order, uplo, n, alpha, A, X, incx, beta, Y, incy);
      }
      otherwise {
        compilerError("Unknown type in hpmv: ", eltType:string);
      }
    }
  }


  pragma "no doc"
  /*
    Wrapper for the `HPR`_ routines::

      A := alpha*x*conjg(x') + A

    TODO -- Get packed array routines working

  */
  proc hpr(A: [?Adom] ?eltType, X: [?vDom] eltType, alpha,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            incx : c_int = 1)
            where (Adom.rank == 2) && (vDom.rank == 1)
  {
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    select eltType {
      when complex(64) {
        var alpha1 = alpha: complex(64);
        C_BLAS.cblas_chpr(order, uplo, n, alpha1, X, incx, A);
      }
      when complex(128) {
        var alpha1 = alpha: complex(128);
        C_BLAS.cblas_zhpr(order, uplo, n, alpha1, X, incx, A);
      }
      otherwise {
        compilerError("Unknown type in hpr: ", eltType:string);
      }
    }
  }


  pragma "no doc"
  /*
    Wrapper for the `HPR2`_ routines::

      A := alpha*x*conjg(y') + conjg(alpha)*y*conjg(x') + A

    TODO -- Get packed array routines working

  */
  proc hpr2(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType,
            ref alpha: eltType,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            incx : c_int = 1, incy : c_int = 1)
            where (Adom.rank == 2) && (vDom.rank == 1)
    {
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    select eltType {
      when complex(64) {
        C_BLAS.cblas_chpr2(order, uplo, n, alpha, X, incx, Y, incy, A);
      }
      when complex(128) {
        C_BLAS.cblas_zhpr2(order, uplo, n, alpha, X, incx, Y, incy, A);
      }
      otherwise {
        compilerError("Unknown type in hpr2: ", eltType:string);
      }
    }
  }


  pragma "no doc"
  /*
    Wrapper for the `SBMV`_ routines::

      y := alpha*A*x + beta*y

    TODO -- Get banded array routines working

  */
  proc sbmv(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType,
            alpha, beta,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            k : int = 0,
            incx : c_int = 1, incy : c_int = 1)
            where (Adom.rank == 2) && (vDom.rank == 1)
  {
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    var _k = k: c_int;

    select eltType {
      when real(32) {
        C_BLAS.cblas_ssbmv(order, uplo, n, _k, alpha, A, _ldA, X, incx, beta, incy);
      }
      when real(64) {
        C_BLAS.cblas_dsbmv(order, uplo, n, _k, alpha, A, _ldA, X, incx, beta, incy);
      }
      otherwise {
        compilerError("Unknown type in sbmv: ", eltType:string);
      }
    }
  }

  pragma "no doc"
  /*
    Wrapper for `SPMV`_ routines::

      y := alpha*A*x + beta*y

    TODO -- Get packed array routines working

  */
  proc spmv(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType,
            alpha, beta,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            incx : c_int = 1, incy : c_int = 1)
            where (Adom.rank == 2) && (vDom.rank == 1)
  {
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    select eltType {
      when real(32) {
        C_BLAS.cblas_sspmv(order, uplo, n, alpha, A, X, incx, beta, Y, incy);
      }
      when real(64) {
        C_BLAS.cblas_dspmv(order, uplo, n, alpha, A, X, incx, beta, Y, incy);
      }
      otherwise {
        compilerError("Unknown type in spmv: ", eltType:string);
      }
    }
  }

  pragma "no doc"
  /*
    Wrapper for `SPR`_ routines::

      a:= alpha*x*x'+ A

    TODO -- Get packed array routines working
  */
  proc spr(A: [?Adom] ?eltType, X: [?vDom] eltType,
           alpha,
           order : Order = Order.Row,
           uplo : Uplo = Uplo.Upper,
           incx : c_int = 1)
           where (Adom.rank == 2) && (vDom.rank == 1)
  {

    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    select eltType {
      when real(32) {
        C_BLAS.cblas_sspr(order, uplo, n, alpha, X, incx, A);
      }
      when real(64) {
        C_BLAS.cblas_dspr(order, uplo, n, alpha, X, incx, A);
      }
      otherwise {
        compilerError("Unknown type in spr: ", eltType:string);
      }
    }
  }

  pragma "no doc"
  /*
    Wrapper for `SPR2`_ routines::

      A:= alpha*x*y'+ alpha*y*x' + A

    TODO -- Get packed array routines working
  */
  proc spr2(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType,
            alpha,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            incx : c_int = 1, incy : c_int = 1)
            where (Adom.rank == 2) && (vDom.rank == 1)
  {
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    select eltType {
      when real(32) {
        C_BLAS.cblas_dspr2(order, uplo, n, alpha, X, incx, Y, incy, A);
      }
      when real(64) {
        C_BLAS.cblas_sspr2(order, uplo, n, alpha, X, incx, Y, incy, A);
      }
      otherwise {
        compilerError("Unknown type in spr2: ", eltType:string);
      }
    }
  }

  /*
    Wrapper for the `SYMV`_ routines::

      y := alpha*A*x + beta*y

  */
  proc symv(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType,
            alpha, beta,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            incx : c_int = 1, incy : c_int = 1)
            throws where (Adom.rank == 2) && (vDom.rank == 1)
  {
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    if m != n then
      throw new IllegalArgumentError("A", "Non-square array of dimensions %ix%i passed to symv".format(m, n));

    var _ldA = getLeadingDim(A, order);

    select eltType {
      when real(32) {
        C_BLAS.cblas_ssymv(order, uplo, n, alpha, A, _ldA, X, incx, beta, Y, incy);
      }
      when real(64) {
        C_BLAS.cblas_dsymv(order, uplo, n, alpha, A, _ldA, X, incx, beta, Y, incy);
      }
      otherwise {
        compilerError("Unknown type in symv: ", eltType:string);
      }
    }
  }

  /*
    Wrapper for `SYR`_ routines::

      A := alpha*x*x' + A

  */
  proc syr(A: [?Adom] ?eltType, X: [?vDom] eltType,
           alpha,
           order : Order = Order.Row,
           uplo : Uplo = Uplo.Upper,
           incx : c_int = 1)
           throws where (Adom.rank == 2) && (vDom.rank == 1)
  {

    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    if m != n then
      throw new IllegalArgumentError("A", "Non-square array of dimensions %ix%i passed to syr".format(m, n));

    var _ldA = getLeadingDim(A, order);

    select eltType {
      when real(32) {
        C_BLAS.cblas_ssyr(order, uplo, n, alpha, X, incx, A, _ldA);
      }
      when real(64) {
        C_BLAS.cblas_dsyr(order, uplo, n, alpha, X, incx, A, _ldA);
      }
      otherwise {
        compilerError("Unknown type in syr: ", eltType:string);
      }
    }
  }

  /*
    Wrapper for `SYR2`_ routines::

      A := alpha*x*y'+ alpha*y*x' + A

  */
  proc syr2(A: [?Adom] ?eltType, X: [?vDom] eltType, Y: [vDom] eltType,
            alpha,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            incx : c_int = 1, incy : c_int = 1)
            throws where (Adom.rank == 2) && (vDom.rank == 1)
  {

    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    if m != n then
      throw new IllegalArgumentError("A", "Non-square array of dimensions %ix%i passed to syr2".format(m, n));

    var _ldA = getLeadingDim(A, order);

    select eltType {
      when real(32) {
        C_BLAS.cblas_ssyr2(order, uplo, n, alpha, X, incx, Y, incy, A, _ldA);
      }
      when real(64) {
        C_BLAS.cblas_dsyr2(order, uplo, n, alpha, X, incx, Y, incy, A, _ldA);
      }
      otherwise {
        compilerError("Unknown type in syr2: ", eltType:string);
      }
    }
  }

  pragma "no doc"
  /*
    Wrapper for the `TBMV`_ routines::

      x := A*x,         when trans = Op.N
      x := A'*x,        when trans = Op.T
      x := conjg(A')*x, when trans = Op.H

    TODO -- Get banded array routines working

  */
  proc tbmv(A: [?Adom] ?eltType, X: [?vDom] eltType,
            k: c_int,
            trans : Op = Op.N,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            diag : Diag = Diag.NonUnit,
            incx : c_int = 1)
            where (Adom.rank == 2) && (vDom.rank == 1)
  {
    // Determine sizes
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    select eltType {
      when real(32) {
        C_BLAS.cblas_stbmv(order, uplo, trans, diag, m, k, A, _ldA, X, incx);
      }
      when real(64) {
        C_BLAS.cblas_dtbmv(order, uplo, trans, diag, m, k, A, _ldA, X, incx);
      }
      when complex(64) {
        C_BLAS.cblas_ctbmv(order, uplo, trans, Diag, m, k, A, _ldA, X, incx);
      }
      when complex(128) {
        C_BLAS.cblas_ztbmv(order, uplo, trans, Diag, m, k, A, _ldA, X, incx);
      }
      otherwise {
        compilerError("Unknown type in tbmv: ", eltType:string);
      }
    }
  }

  pragma "no doc"
  /*
   Wrapper for the `TBSV`_ routines::

    A*x = b,         when trans = Op.N
    A'*x = b,        when trans = Op.T
    conjg(A')*x = b, when trans = Op.H

   TODO -- Get banded array routines working

  */
  proc tbsv(A: [?Adom] ?eltType, X: [?vDom] eltType,
            trans : Op = Op.N,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            diag : Diag = Diag.NonUnit,
            incx : c_int = 1)
            where (Adom.rank == 2) && (vDom.rank == 1)
  {

    // Determine sizes
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int,
        k : c_int;

    if trans > Op.N then k = Adom.dim(2).size : c_int;
                    else k = Adom.dim(1).size : c_int;

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    select eltType {
      when real(32) {
        C_BLAS.cblas_stbsv(order, uplo, trans, diag, n, k, A, _ldA, X, incx);
      }
      when real(64) {
        C_BLAS.cblas_dtbsv(order, uplo, trans, diag, n, k, A, _ldA, X, incx);
      }
      when complex(64) {
        C_BLAS.cblas_ctbsv(order, uplo, trans, diag, n, k, A, _ldA, X, incx);
      }
      when complex(128) {
        C_BLAS.cblas_ztbsv(order, uplo, trans, diag, n, k, A, _ldA, X, incx);
      }
      otherwise {
        compilerError("Unknown type in tbsv: ", eltType:string);
      }
    }
  }

  pragma "no doc"
  /*
   Wrapper for `TPMV`_ routines::

    x := A*x,         when trans = Op.N
    x := A'*x,        when trans = Op.T
    x := conjg(A')*x, when trans = Op.H

   TODO -- Get banded array routines working

  */
  proc tpmv(A: [?Adom] ?eltType, X: [?vDom] eltType,
            trans : Op = Op.N,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            diag : Diag = Diag.NonUnit,
            incx : c_int = 1)
            where (Adom.rank == 2) && (vDom.rank == 1)
  {

    // Determine sizes
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    select eltType {
      when real(32) {
        C_BLAS.cblas_stpmv(order, uplo, trans, diag, n, A, X, incx);
      }
      when real(64) {
        C_BLAS.cblas_dtpmv(order, uplo, trans, diag, n, A, X, incx);
      }
      when complex(64) {
        C_BLAS.cblas_ctpmv(order, uplo, trans, diag, n, A, X, incx);
      }
      when complex(128) {
        C_BLAS.cblas_ztpmv(order, uplo, trans, diag, n, A, X, incx);
      }
      otherwise {
        compilerError("Unknown type in tpmv: ", eltType:string);
      }
    }
  }

  pragma "no doc"
  /*
    Wrapper for `TPSV`_ routines::

      A*x = b, or A'*x = b, or conjg(A')*x = b

    TODO -- Get packed array routines working
  */
  proc tpsv(A: [?Adom] ?eltType, X: [?vDom] eltType,
            trans : Op = Op.N,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            diag : Diag = Diag.NonUnit,
            incx : c_int = 1)
            where (Adom.rank == 2) && (vDom.rank == 1)
  {

    // Determine sizes
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;
    // TODO -- check if m == n

    select eltType {
      when real(32) {
        C_BLAS.cblas_stpsv(order, uplo, trans, diag, n, A, X, incx);
      }
      when real(64) {
        C_BLAS.cblas_dtpsv(order, uplo, trans, diag, n, A, X, incx);
      }
      when complex(64) {
        C_BLAS.cblas_ctpsv(order, uplo, trans, diag, n, A, X, incx);
      }
      when complex(128) {
        C_BLAS.cblas_ztpsv(order, uplo, trans, diag, n, A, X, incx);
      }
      otherwise {
        compilerError("Unknown type in tpsv: ", eltType:string);
      }
    }
  }

  /*
    Wrapper for `TRMV`_ routines::

      x := op(A)*x

  */
  proc trmv(A: [?Adom] ?eltType, X: [?vDom] eltType,
            trans : Op = Op.N,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            diag : Diag = Diag.NonUnit,
            incx : c_int = 1)
            throws where (Adom.rank == 2) && (vDom.rank == 1)
  {

    // Determine sizes
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    if m != n then
      throw new IllegalArgumentError("A", "Non-square array of dimensions %ix%i passed to trmv".format(m, n));

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    select eltType {
      when real(32) {
        C_BLAS.cblas_strmv(order, uplo, trans, diag, m, A, _ldA, X, incx);
      }
      when real(64) {
        C_BLAS.cblas_dtrmv(order, uplo, trans, diag, m, A, _ldA, X, incx);
      }
      when complex(64) {
        C_BLAS.cblas_ctrmv(order, uplo, trans, diag, m, A, _ldA, X, incx);
      }
      when complex(128) {
        C_BLAS.cblas_ztrmv(order, uplo, trans, diag, m, A, _ldA, X, incx);
      }
      otherwise {
        compilerError("Unknown type in trmv: ", eltType:string);
      }
    }
  }

  /*
    Wrapper for the `TRSV`_ routines::

      A*op(x) = b

  */
  proc trsv(A: [?Adom] ?eltType, B: [?vDom] eltType,
            trans : Op = Op.N,
            order : Order = Order.Row,
            uplo : Uplo = Uplo.Upper,
            diag : Diag = Diag.NonUnit,
            incx : c_int = 1)
            throws where (Adom.rank == 2) && (vDom.rank == 1)
  {

    // Determine sizes
    var m = Adom.dim(1).size : c_int,
        n = Adom.dim(2).size : c_int;

    if m != n then
      throw new IllegalArgumentError("A", "Non-square array of dimensions %ix%i passed to trsv".format(m, n));

    // Set strides if necessary
    var _ldA = getLeadingDim(A, order);

    select eltType {
      when real(32) {
        C_BLAS.cblas_strsv(order, uplo, trans, diag, n, A, _ldA, B, incx);
      }
      when real(64) {
        C_BLAS.cblas_dtrsv(order, uplo, trans, diag, n, A, _ldA, B, incx);
      }
      when complex(64) {
        C_BLAS.cblas_ctrsv(order, uplo, trans, diag, n, A, _ldA, B, incx);
      }
      when complex(128) {
        C_BLAS.cblas_ztrsv(order, uplo, trans, diag, n, A, _ldA, B, incx);
      }
      otherwise {
        compilerError("Unknown type in trsv: ", eltType:string);
      }
    }
  }



  /* Level 1 BLAS */

  /*
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

  */
  proc rotg(ref a : ?eltType, ref b : eltType, ref c : eltType, ref s : eltType){
    select eltType {
      when real(32) do{
        cblas_srotg (a, b, c, s);
      }
      when real(64) do{
        cblas_drotg (a, b, c, s);
      }
      otherwise {
          compilerError("Unknown type in rotg: ", eltType:string);
      }
    }
  }

  /*
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



  */
  proc rotmg(ref d1: ?eltType, ref d2: eltType, ref b1: eltType, b2: eltType, P: []eltType) throws {

    if P.size != 5 then
      throw new IllegalArgumentError("P", "must consist of 5 elements, passed to rotmg");

    select eltType {
      when real(32) do{
        cblas_srotmg(d1,d2,b1,b2,P);
      }
      when real(64) do{
        cblas_drotmg(d1,d2,b1,b2,P);
      }
      otherwise {
          compilerError("Unknown type in rotmg: ", eltType:string);
      }
    }
  }

  /*
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


  */
  proc rot(X: [?D] ?eltType, Y: [D] eltType, c: eltType, s: eltType,  incY: c_int = 1, incX: c_int = 1)
  where D.rank == 1 {

    const N = D.size: c_int;

    select eltType {
      when real(32) do{
        cblas_srot(N, X, incX, Y, incY, c, s);
      }
      when real(64) do{
        cblas_drot(N, X, incX, Y, incY, c, s);
      }
      otherwise {
          compilerError("Unknown type in rot: ", eltType:string);
      }
    }
  }
  /*
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

  */
  proc rotm(X: [?D]?eltType,  Y: [D]eltType,  P: []eltType, incY: c_int = 1, incX: c_int = 1) throws where D.rank == 1 {

    if P.size != 5 then
      throw new IllegalArgumentError("P", "must consist of 5 elements, passed to rotm");

    const N = D.size: c_int;

    select eltType {
      when real(32) do{
        cblas_srotm(N, X, incX, Y, incY,  P);
      }
      when real(64) do{
        cblas_drotm(N, X, incX, Y, incY, P);
      }
      otherwise {
          compilerError("Unknown type in rotm: ", eltType:string);
      }
    }
  }


  /*
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

  */
  proc swap(X: [?D]?eltType, Y: [D]eltType, incY: c_int = 1, incX: c_int = 1)
   where D.rank == 1 {

    const N = D.size: c_int;

    select eltType {
      when real(32) do{
        cblas_sswap (N, X, incX, Y, incY);
      }
      when real(64) do{
        cblas_dswap (N, X, incX, Y, incY);
      }
      when complex(64) do{
        cblas_cswap (N, X, incX, Y, incY);
      }
      when complex(128) do{
        cblas_zswap (N, X, incX, Y, incY);
      }
      otherwise {
          compilerError("Unknown type in swap: ", eltType:string);
      }
    }
  }

  /*
      Wrapper for the `SCAL`_ routines

      Calculates the product of a vector ``X`` with scalar alpha::

        X' = alpha*X

      **Input:**

      - ``X``: Input vector
      - ``alpha``: Scalar parameter

      **Output:**

      - ``X``: Vector updated by the equation: ``X[i] = alpha*X[i]``

  */
  proc scal(X: [?D]?eltType, ref alpha:eltType, incX: c_int = 1)
  where D.rank == 1 {

    const N = D.size: c_int;

    select eltType {
      when real(32) do{
        cblas_sscal (N, alpha, X, incX);
      }
      when real(64) do{
        cblas_dscal (N, alpha, X, incX);
      }
      when complex(64) do{
        cblas_cscal (N, alpha, X, incX);
      }
      when complex(128) do{
        cblas_zscal (N, alpha, X, incX);
      }
      otherwise {
          compilerError("Unknown type in scal: ", eltType:string);
      }
    }
  }

  /*
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

   */
  proc copy(X: [?D]?eltType, Y: [D]eltType, incY: c_int = 1, incX: c_int = 1)
    where D.rank == 1 {

    const N = D.size: c_int;

    select eltType {
      when real(32) do{
        cblas_scopy (N, X, incX, Y, incY);
      }
      when real(64) do{
        cblas_dcopy (N, X, incX, Y, incY);
      }
      when complex(64) do{
        cblas_ccopy (N, X, incX, Y, incY);
      }
      when complex(128) do{
        cblas_zcopy (N, X, incX, Y, incY);
      }
      otherwise {
          compilerError("Unknown type in copy: ", eltType:string);
      }
    }
  }

  /*
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

  */

  proc axpy(X: [?D]?eltType, Y: [D]eltType, ref alpha:eltType, incY: c_int = 1, incX: c_int = 1)
   where D.rank == 1 {

    const N = D.size: c_int;
    select eltType {
      when real(32) do{
        cblas_saxpy (N, alpha, X, incX, Y, incY);
      }
      when real(64) do{
        cblas_daxpy (N, alpha, X, incX, Y, incY);
      }
      when complex(64) do{
        cblas_caxpy (N, alpha, X, incX, Y, incY);
      }
      when complex(128) do{
        cblas_zaxpy (N, alpha, X, incX, Y, incY);
      }
      otherwise {
          compilerError("Unknown type in axpy: ", eltType:string);
      }
    }
  }

  /*
      Wrapper for `DOT`_ routines

      Returns  the dot product of two vectors::

         X*Y

      **Input:**

      - ``X``: Input vector
      - ``Y``: Input vector
      - ``incX``: Defines the increment for the vector ``X``
      - ``incY``: Defines the increment for the vector ``Y``

      :returns: Scalar value of dot product
  */
  proc dot(X: [?xD]?eltType,  Y: [?yD] eltType, incY: c_int = 1, incX: c_int = 1) : eltType
  where xD.rank == 1 && yD.rank == 1 {

    const N = xD.size: c_int;

    select eltType {
      when real(32) do{
        return cblas_sdot (N, X, incX, Y, incY);
      }
      when real(64) do{
       return cblas_ddot (N,X, incX, Y, incY);
      }
      otherwise {
          compilerError("Unknown type in dot: ", eltType:string);
      }
    }

  }

  /*
      Wrapper for  `DOTU`_ routines (``DOTU_SUB``)

      Obtains the dot product of two complex vectors::

         X*Y

      **Input:**

      - ``X``: Input vector
      - ``Y``: Input vector
      - ``incX``: Defines the increment for the vector ``X``
      - ``incY``: Defines the increment for the vector ``Y``

    :returns: The complex dot product


  */
  proc dotu(X: [?D]?eltType,  Y: [D]eltType, incY: c_int = 1, incX: c_int = 1)
  where D.rank == 1 {

    const N = D.size: c_int;

    var res: eltType;

    select eltType {
      when complex(64) do{
        cblas_cdotu_sub (N, X, incX, Y, incY, res);
      }
      when complex(128) do{
        cblas_zdotu_sub (N, X, incX, Y, incY, res);
      }
      otherwise {
          compilerError("Unknown type in dotu_sub: ", eltType:string);
      }
    }

    return res;

  }


  /*
      Wrapper for `DOTC`_ routines (``DOTC_SUB``)

      Obtains the dot product of conjugated X vector with Y vector::

         conj(X)*Y

      **Input:**

      - ``X``: Conjugated input vector
      - ``Y``: Input vector
      - ``incX``: Defines the increment for the vector ``X``
      - ``incY``: Defines the increment for the vector ``Y``

      :returns: The complex dot product


  */
  proc dotc(X: [?D]?eltType, Y: [D]eltType, incY: c_int = 1, incX: c_int = 1)
   where D.rank == 1 {

    const N = D.size: c_int;

    var res: eltType;

    select eltType {
      when complex(64) do{
        cblas_cdotc_sub (N, X, incX, Y, incY, res);
      }
      when complex(128) do{
        cblas_zdotc_sub (N, X, incX, Y, incY, res);
      }
      otherwise {
          compilerError("Unknown type in dotc_sub: ", eltType:string);
      }
    }
    return res;
  }

  /*
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


  */
  proc dsdot(X: [?D] real(32), Y: [D] real(32), incY: c_int = 1,incX: c_int = 1): real(64)
   where D.rank == 1 {

    const N = D.size: c_int;
    return cblas_dsdot (N, X, incX, Y, incY);
  }


  /*
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


  */
  proc sdsdot(X: [?D] real(32), Y: [D] real(32), incY: c_int = 1,incX: c_int = 1): real(32)
   where D.rank == 1 {

    var alpha = 0: real(32);
    const N = D.size: c_int;

    return cblas_sdsdot (N, alpha, X, incX, Y, incY);
  }

  /*
      Wrapper for `NRM2`_ routines

      Returns the  Euclidean norm of vector ``X``::

        ||X||

      **Input:**

      - ``X``: Input vector
      - ``incX``: Defines the increment for the vector ``X``

      :returns: The 2-norm of ``X`` vector

  */
  proc nrm2(X: [?D]?eltType, incX: c_int = 1)
  where D.rank == 1 {

    const N = D.size: c_int;

    select eltType {
     when real(32) do{
        return cblas_snrm2 (N,  X, incX);
      }
      when real(64) do{
        return cblas_dnrm2 (N, X, incX);
      }
      when complex(64) do{
        return cblas_scnrm2 (N, X, incX);
      }
      when complex(128) do{
        return cblas_dznrm2 (N, X, incX);
      }
      otherwise {
          compilerError("Unknown type in nrm2: ", eltType:string);
      }
    }
  }

  /*
      Wrapper for the `ASUM`_ routines

      Returns the sum of the magnitude values of X elements::

        |Re X[1]| + |Im X[1]| + |Re  X[2]| + |Im  X[2]|+ ... + |Re  X[N]| + |Im X[N]|.

      **Input:**

      - ``X``: Input vector
      - ``incX``: Defines the increment for the vector ``X``

      :returns: The 1-norm of ``X`` vector

  */
  proc asum(X: [?D]?eltType, incX: c_int = 1)
  where D.rank == 1 {

    const N = D.size: c_int;

    select eltType {
     when real(32) do{
        return cblas_sasum(N, X, incX);
      }
     when real(64) do{
        return cblas_dasum(N, X, incX);
      }
     when complex(64) do{
        return cblas_scasum(N, X, incX);
      }
      when complex(128) do{
        return cblas_dzasum(N, X, incX);
      }
      otherwise {
          compilerError("Unknown type in asum: ", eltType:string);
      }
    }
  }

  /*
      Wrapper for `AMAX`_ routines

      Returns the index of element in the vector with maximum absolute value.

      **Input:**

      - ``X``: Input vector
      - ``incX``: Defines the increment for the vector ``X``

      :returns: The index of maximum absolute value

  */
  proc amax(X: [?D]?eltType, incX: c_int = 1)
  where D.rank == 1: D.idxType {

    const N = D.size: c_int;

    const r = D.dim(1);

    select eltType {
     when real(32) do{
        return r.orderToIndex(cblas_isamax(N, X, incX));
      }
     when real(64) do{
        return r.orderToIndex(cblas_idamax(N, X, incX));
      }
     when complex(64) do{
        return r.orderToIndex(cblas_icamax(N, X, incX));
      }
      when complex(128) do{
        return r.orderToIndex(cblas_izamax(N, X, incX));
      }
      otherwise {
          compilerError("Unknown type in amax: ", eltType:string);
      }
    }
  }

  //
  // Helper functions
  //

  pragma "no doc"
  inline proc getLeadingDim(A: [?Adom], order : Order) : c_int {
    if order==Order.Row then
      return Adom.dim(2).size : c_int;
    else
      return Adom.dim(1).size : c_int;
  }

  pragma "no doc"
  inline proc getLeadingDim(Arr: [], order : Order) : c_int
  where chpl__isArrayView(Arr)
  {
    const dims = chpl__getActualArray(Arr).dom.dsiDims();
    if order==Order.Row then
      return dims(2).size : c_int;
    else
      return dims(1).size : c_int;
  }


  /*

    Support for low-level native C_BLAS bindings.

    This submodule wraps the netlib C_BLAS implementation, providing access to
    all of C_BLAS calls.

    Arrays are passed in directly, while pointers to scalar
    quantities (including complex numbers) are passed by reference (removing
    the need to wrap these with ``c_ptrTo``). As with BLAS calls in C,
    the user is responsible for passing in array dimensions etc. Furthermore,
    different array element types require using different functions.

    Refer to the
    `C_BLAS documentation <http://www.netlib.org/lapack/explore-html/dir_f88bc7ad48bfd56d75bf9d4836a2bb00.html>`_
    of the reference version for the usage of this module.

  */
  module C_BLAS {
    extern type CBLAS_INDEX = c_int;

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

