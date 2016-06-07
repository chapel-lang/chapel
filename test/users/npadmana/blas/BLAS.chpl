module BLAS {

  use SysCTypes;
  require "cblas.h",'-lblas';

  extern type CBLAS_INDEX = size_t;

  // Do a size test.
  {
    pragma "no doc"
    pragma "no prototype"
    extern proc sizeof(type t): size_t;
    assert(sizeof(CBLAS_INDEX) == sizeof(size_t));
  }
  
  // Define the external types
  // These are C enums, so we define these as c_ints;
  extern type CBLAS_ORDER = c_int;
  extern type CBLAS_TRANSPOSE = c_int;
  extern type CBLAS_UPLO = c_int;
  extern type CBLAS_DIAG = c_int;
  extern type CBLAS_SIDE = c_int;

  // Constants
  extern const CblasRowMajor : CBLAS_ORDER,
               CblasColMajor : CBLAS_ORDER;
  extern const CblasNoTrans : CBLAS_TRANSPOSE,
               CblasTrans   : CBLAS_TRANSPOSE,
               CblasConjTrans : CBLAS_TRANSPOSE;
  extern const CblasUpper : CBLAS_UPLO,
               CblasLower : CBLAS_UPLO;
  extern const CblasNonUnit : CBLAS_DIAG,
               CblasUnit : CBLAS_DIAG;
  extern const CblasLeft : CBLAS_SIDE,
               CblasRight : CBLAS_SIDE;


  /* Convenience */
  enum Op {N, T, H}; // NoTranspose, Transpose, Adjoint
  const _BlasOp : [Op.N..Op.H] CBLAS_TRANSPOSE = [CblasNoTrans, CblasTrans, CblasConjTrans];

  /* Level 3 BLAS */

  /* GEMM : Matrix multiplication
  */
  proc gemm(A : [?Adom], B : [?Bdom], C : [?Cdom],
    alpha, beta,
    opA : Op = Op.N, opB : Op = Op.N,
    rowMajor : bool = true,
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

    // Set various parameters
    var order : CBLAS_ORDER = CblasRowMajor;
    if !rowMajor then order = CblasColMajor;
    var _opA = _BlasOp[opA],
        _opB = _BlasOp[opB];

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, rowMajor, ldA),
        _ldB = getLeadingDim(Bdom, rowMajor, ldB),
        _ldC = getLeadingDim(Cdom, rowMajor, ldC);

    select eltType {
      when real(32) {
        // sgemm
        cblas_sgemm(order, _opA, _opB, m, n, k,
          alpha, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta, C[Cdom.low],_ldC);
      }
      when real(64) {
        // dgemm
        cblas_dgemm(order, _opA, _opB, m, n, k,
          alpha, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta, C[Cdom.low],_ldC);
      }
      when complex(64) {
        // cgemm
        var alpha1 = alpha : complex(64),
            beta1 = beta : complex(64);
        cblas_cgemm(order, _opA, _opB, m, n, k,
          alpha1, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta1, C[Cdom.low],_ldC);
      }
      when complex(128) {
        // zgemm
        var alpha1 = alpha : complex(128),
            beta1 = beta : complex(128);
        cblas_zgemm(order, _opA, _opB, m, n, k,
          alpha1, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta1, C[Cdom.low],_ldC);
      }
      otherwise {
        halt("Unknown type in gemm");
      }
    }

  }

  /* SYMM : Matrix multiplication with a symmetric matrix A
  */
  proc symm(A : [?Adom], B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : CBLAS_UPLO = CblasUpper,  side : CBLAS_SIDE = CblasLeft,
    rowMajor : bool = true,
    ldA : int = 0, ldB : int = 0, ldC : int = 0)
    where (Adom.rank == 2) && (Bdom.rank==2) && (Cdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var m = Cdom.dim(1).size : c_int,
        n = Cdom.dim(2).size : c_int;

    // Set various parameters
    var order : CBLAS_ORDER = CblasRowMajor;
    if !rowMajor then order = CblasColMajor;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, rowMajor, ldA),
        _ldB = getLeadingDim(Bdom, rowMajor, ldB),
        _ldC = getLeadingDim(Cdom, rowMajor, ldC);

    select eltType {
      when real(32) {
        // ssymm
        cblas_ssymm(order, side, uplo, m, n,
          alpha, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta, C[Cdom.low],_ldC);
      }
      when real(64) {
        // dsymm
        cblas_dsymm(order, side, uplo, m, n,
          alpha, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta, C[Cdom.low],_ldC);
      }
      when complex(64) {
        // csymm
        var alpha1 = alpha : complex(64),
            beta1 = beta : complex(64);
        cblas_csymm(order, side, uplo, m, n,
          alpha1, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta1, C[Cdom.low],_ldC);
      }
      when complex(128) {
        // zsymm
        var alpha1 = alpha : complex(128),
            beta1 = beta : complex(128);
        cblas_zsymm(order, side, uplo, m, n,
          alpha1, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta1, C[Cdom.low],_ldC);
      }
      otherwise {
        halt("Unknown type in symm");
      }
    }

  }

  /* HEMM : Hermitian multiplications.... only for complex types*/
  proc hemm(A : [?Adom], B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : CBLAS_UPLO = CblasUpper,  side : CBLAS_SIDE = CblasLeft,
    rowMajor : bool = true,
    ldA : int = 0, ldB : int = 0, ldC : int = 0)
    where (Adom.rank == 2) && (Bdom.rank==2) && (Cdom.rank==2)
  {
    // Types
    type eltType = A.eltType;

    // Determine sizes
    var m = Cdom.dim(1).size : c_int,
        n = Cdom.dim(2).size : c_int;

    // Set various parameters
    var order : CBLAS_ORDER = CblasRowMajor;
    if !rowMajor then order = CblasColMajor;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, rowMajor, ldA),
        _ldB = getLeadingDim(Bdom, rowMajor, ldB),
        _ldC = getLeadingDim(Cdom, rowMajor, ldC);

    select eltType {
      when complex(64) {
        // csymm
        var alpha1 = alpha : complex(64),
            beta1 = beta : complex(64);
        cblas_chemm(order, side, uplo, m, n,
          alpha1, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta1, C[Cdom.low],_ldC);
      }
      when complex(128) {
        // zsymm
        var alpha1 = alpha : complex(128),
            beta1 = beta : complex(128);
        cblas_zhemm(order, side, uplo, m, n,
          alpha1, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta1, C[Cdom.low],_ldC);
      }
      otherwise {
        halt("Unknown type in hemm");
      }
    }

  }

  /* SYRK :
  */
  proc syrk(A : [?Adom],  C : [?Cdom],
    alpha, beta,
    uplo : CBLAS_UPLO = CblasUpper,  trans : Op = Op.N,
    rowMajor : bool = true,
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
    var _trans = _BlasOp[trans];

    // Set various parameters
    var order : CBLAS_ORDER = CblasRowMajor;
    if !rowMajor then order = CblasColMajor;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, rowMajor, ldA),
        _ldC = getLeadingDim(Cdom, rowMajor, ldC);

    select eltType {
      when real(32) {
        // ssymm
        cblas_ssyrk(order, uplo, _trans, n, k,
          alpha, A[Adom.low], _ldA, beta, C[Cdom.low],_ldC);
      }
      when real(64) {
        // dsymm
        cblas_dsyrk(order, uplo, _trans, n, k,
          alpha, A[Adom.low], _ldA, beta, C[Cdom.low],_ldC);
      }
      when complex(64) {
        // csymm
        var alpha1 = alpha : complex(64),
            beta1 = beta : complex(64);
        cblas_csyrk(order, uplo, _trans, n, k,
          alpha1, A[Adom.low], _ldA, beta1, C[Cdom.low],_ldC);
      }
      when complex(128) {
        // zsymm
        var alpha1 = alpha : complex(128),
            beta1 = beta : complex(128);
        cblas_zsyrk(order, uplo, _trans, n, k,
          alpha1, A[Adom.low], _ldA, beta1, C[Cdom.low],_ldC);
      }
      otherwise {
        halt("Unknown type in syrk");
      }
    }

  }

  /* HERK :
  */
  proc herk(A : [?Adom],  C : [?Cdom],
    alpha, beta,
    uplo : CBLAS_UPLO = CblasUpper,  trans : Op = Op.N,
    rowMajor : bool = true,
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
    var _trans = _BlasOp[trans];

    // Set various parameters
    var order : CBLAS_ORDER = CblasRowMajor;
    if !rowMajor then order = CblasColMajor;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, rowMajor, ldA),
        _ldC = getLeadingDim(Cdom, rowMajor, ldC);

    select eltType {
      when complex(64) {
        // csymm
        var alpha1 = alpha : real(32),
            beta1 = beta : real(32);
        cblas_cherk(order, uplo, _trans, n, k,
          alpha1, A[Adom.low], _ldA, beta1, C[Cdom.low],_ldC);
      }
      when complex(128) {
        // zsymm
        var alpha1 = alpha : real(64),
            beta1 = beta : real(64);
        cblas_zherk(order, uplo, _trans, n, k,
          alpha1, A[Adom.low], _ldA, beta1, C[Cdom.low],_ldC);
      }
      otherwise {
        halt("Unknown type in syrk");
      }
    }

  }


  /* SYR2K :
  */
  proc syr2k(A : [?Adom],  B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : CBLAS_UPLO = CblasUpper,  trans : Op = Op.N,
    rowMajor : bool = true,
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
    var _trans = _BlasOp[trans];

    // Set various parameters
    var order : CBLAS_ORDER = CblasRowMajor;
    if !rowMajor then order = CblasColMajor;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, rowMajor, ldA),
        _ldB = getLeadingDim(Bdom, rowMajor, ldB),
        _ldC = getLeadingDim(Cdom, rowMajor, ldC);

    select eltType {
      when real(32) {
        // ssymm
        cblas_ssyr2k(order, uplo, _trans, n, k,
          alpha, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta, C[Cdom.low],_ldC);
      }
      when real(64) {
        // dsymm
        cblas_dsyr2k(order, uplo, _trans, n, k,
          alpha, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta, C[Cdom.low],_ldC);
      }
      when complex(64) {
        // csymm
        var alpha1 = alpha : complex(64),
            beta1 = beta : complex(64);
        cblas_csyr2k(order, uplo, _trans, n, k,
          alpha1, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta1, C[Cdom.low],_ldC);
      }
      when complex(128) {
        // zsymm
        var alpha1 = alpha : complex(128),
            beta1 = beta : complex(128);
        cblas_zsyr2k(order, uplo, _trans, n, k,
          alpha1, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta1, C[Cdom.low],_ldC);
      }
      otherwise {
        halt("Unknown type in syr2k");
      }
    }

  }

  /* HER2K :
  */
  proc her2k(A : [?Adom],  B : [?Bdom], C : [?Cdom],
    alpha, beta,
    uplo : CBLAS_UPLO = CblasUpper,  trans : Op = Op.N,
    rowMajor : bool = true,
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
    var _trans = _BlasOp[trans];

    // Set various parameters
    var order : CBLAS_ORDER = CblasRowMajor;
    if !rowMajor then order = CblasColMajor;

    // Set strides if necessary
    var _ldA = getLeadingDim(Adom, rowMajor, ldA),
        _ldB = getLeadingDim(Bdom, rowMajor, ldB),
        _ldC = getLeadingDim(Cdom, rowMajor, ldC);

    select eltType {
      when complex(64) {
        var alpha1 = alpha : complex(64),
            beta1 = beta : real(32);
        cblas_cher2k(order, uplo, _trans, n, k,
          alpha1, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta1, C[Cdom.low],_ldC);
      }
      when complex(128) {
        var alpha1 = alpha : complex(128),
            beta1 = beta : real(64);
        cblas_zher2k(order, uplo, _trans, n, k,
          alpha1, A[Adom.low], _ldA, B[Bdom.low], _ldB, beta1, C[Cdom.low],_ldC);
      }
      otherwise {
        halt("Unknown type in her2k");
      }
    }

  }


  // Helper function
  inline proc getLeadingDim(Adom : domain(2), rowMajor : bool, ldA : int) : c_int {
    var _ldA = ldA : c_int;
    if ldA==0 {
      if rowMajor then _ldA = Adom.dim(2).size : c_int;
                  else _ldA = Adom.dim(1).size : c_int;
    }
    return _ldA;
  }



  module C_BLAS {

    extern proc cblas_sdsdot (N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int): c_float;
    extern proc cblas_dsdot (N: c_int, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int): c_double;
    extern proc cblas_sdot (N: c_int, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int): c_float;
    extern proc cblas_ddot (N: c_int, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int): c_double;
    extern proc cblas_cdotu_sub (N: c_int, ref X, incX: c_int, ref Y, incY: c_int, ref dotu);
    extern proc cblas_cdotc_sub (N: c_int, ref X, incX: c_int, ref Y, incY: c_int, ref dotc);
    extern proc cblas_zdotu_sub (N: c_int, ref X, incX: c_int, ref Y, incY: c_int, ref dotu);
    extern proc cblas_zdotc_sub (N: c_int, ref X, incX: c_int, ref Y, incY: c_int, ref dotc);
    extern proc cblas_snrm2 (N: c_int, ref X: c_float, incX: c_int): c_float;
    extern proc cblas_sasum (N: c_int, ref X: c_float, incX: c_int): c_float;
    extern proc cblas_dnrm2 (N: c_int, ref X: c_double, incX: c_int): c_double;
    extern proc cblas_dasum (N: c_int, ref X: c_double, incX: c_int): c_double;
    extern proc cblas_scnrm2 (N: c_int, ref X, incX: c_int): c_float;
    extern proc cblas_scasum (N: c_int, ref X, incX: c_int): c_float;
    extern proc cblas_dznrm2 (N: c_int, ref X, incX: c_int): c_double;
    extern proc cblas_dzasum (N: c_int, ref X, incX: c_int): c_double;
    extern proc cblas_isamax (N: c_int, ref X: c_float, incX: c_int): CBLAS_INDEX;
    extern proc cblas_idamax (N: c_int, ref X: c_double, incX: c_int): CBLAS_INDEX;
    extern proc cblas_icamax (N: c_int, ref X, incX: c_int): CBLAS_INDEX;
    extern proc cblas_izamax (N: c_int, ref X, incX: c_int): CBLAS_INDEX;
    extern proc cblas_sswap (N: c_int, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int);
    extern proc cblas_scopy (N: c_int, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int);
    extern proc cblas_saxpy (N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int);
    extern proc cblas_dswap (N: c_int, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int);
    extern proc cblas_dcopy (N: c_int, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int);
    extern proc cblas_daxpy (N: c_int, alpha: c_double, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int);
    extern proc cblas_cswap (N: c_int, ref X, incX: c_int, ref Y, incY: c_int);
    extern proc cblas_ccopy (N: c_int, ref X, incX: c_int, ref Y, incY: c_int);
    extern proc cblas_caxpy (N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int);
    extern proc cblas_zswap (N: c_int, ref X, incX: c_int, ref Y, incY: c_int);
    extern proc cblas_zcopy (N: c_int, ref X, incX: c_int, ref Y, incY: c_int);
    extern proc cblas_zaxpy (N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int);
    extern proc cblas_srotg (ref a: c_float, ref b: c_float, ref c: c_float, ref s: c_float);
    extern proc cblas_srotmg (ref d1: c_float, ref d2: c_float, ref b1: c_float, b2: c_float, ref P: c_float);
    extern proc cblas_srot (N: c_int, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int, c: c_float, s: c_float);
    extern proc cblas_srotm (N: c_int, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int, ref P: c_float);
    extern proc cblas_drotg (ref a: c_double, ref b: c_double, ref c: c_double, ref s: c_double);
    extern proc cblas_drotmg (ref d1: c_double, ref d2: c_double, ref b1: c_double, b2: c_double, ref P: c_double);
    extern proc cblas_drot (N: c_int, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int, c: c_double, s: c_double);
    extern proc cblas_drotm (N: c_int, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int, ref P: c_double);
    extern proc cblas_sscal (N: c_int, alpha: c_float, ref X: c_float, incX: c_int);
    extern proc cblas_dscal (N: c_int, alpha: c_double, ref X: c_double, incX: c_int);
    extern proc cblas_cscal (N: c_int, ref alpha, ref X, incX: c_int);
    extern proc cblas_zscal (N: c_int, ref alpha, ref X, incX: c_int);
    extern proc cblas_csscal (N: c_int, alpha: c_float, ref X, incX: c_int);
    extern proc cblas_zdscal (N: c_int, alpha: c_double, ref X, incX: c_int);
    extern proc cblas_sgemv (order, TransA, M: c_int, N: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int, beta: c_float, ref Y: c_float, incY: c_int);
    extern proc cblas_sgbmv (order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int, beta: c_float, ref Y: c_float, incY: c_int);
    extern proc cblas_strmv (order, Uplo, TransA, Diag, N: c_int, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int);
    extern proc cblas_stbmv (order, Uplo, TransA, Diag, N: c_int, K: c_int, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int);
    extern proc cblas_stpmv (order, Uplo, TransA, Diag, N: c_int, ref Ap: c_float, ref X: c_float, incX: c_int);
    extern proc cblas_strsv (order, Uplo, TransA, Diag, N: c_int, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int);
    extern proc cblas_stbsv (order, Uplo, TransA, Diag, N: c_int, K: c_int, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int);
    extern proc cblas_stpsv (order, Uplo, TransA, Diag, N: c_int, ref Ap: c_float, ref X: c_float, incX: c_int);
    extern proc cblas_dgemv (order, TransA, M: c_int, N: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int, beta: c_double, ref Y: c_double, incY: c_int);
    extern proc cblas_dgbmv (order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int, beta: c_double, ref Y: c_double, incY: c_int);
    extern proc cblas_dtrmv (order, Uplo, TransA, Diag, N: c_int, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int);
    extern proc cblas_dtbmv (order, Uplo, TransA, Diag, N: c_int, K: c_int, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int);
    extern proc cblas_dtpmv (order, Uplo, TransA, Diag, N: c_int, ref Ap: c_double, ref X: c_double, incX: c_int);
    extern proc cblas_dtrsv (order, Uplo, TransA, Diag, N: c_int, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int);
    extern proc cblas_dtbsv (order, Uplo, TransA, Diag, N: c_int, K: c_int, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int);
    extern proc cblas_dtpsv (order, Uplo, TransA, Diag, N: c_int, ref Ap: c_double, ref X: c_double, incX: c_int);
    extern proc cblas_cgemv (order, TransA, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_cgbmv (order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_ctrmv (order, Uplo, TransA, Diag, N: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ctbmv (order, Uplo, TransA, Diag, N: c_int, K: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ctpmv (order, Uplo, TransA, Diag, N: c_int, ref Ap, ref X, incX: c_int);
    extern proc cblas_ctrsv (order, Uplo, TransA, Diag, N: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ctbsv (order, Uplo, TransA, Diag, N: c_int, K: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ctpsv (order, Uplo, TransA, Diag, N: c_int, ref Ap, ref X, incX: c_int);
    extern proc cblas_zgemv (order, TransA, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_zgbmv (order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_ztrmv (order, Uplo, TransA, Diag, N: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ztbmv (order, Uplo, TransA, Diag, N: c_int, K: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ztpmv (order, Uplo, TransA, Diag, N: c_int, ref Ap, ref X, incX: c_int);
    extern proc cblas_ztrsv (order, Uplo, TransA, Diag, N: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ztbsv (order, Uplo, TransA, Diag, N: c_int, K: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ztpsv (order, Uplo, TransA, Diag, N: c_int, ref Ap, ref X, incX: c_int);
    extern proc cblas_ssymv (order, Uplo, N: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int, beta: c_float, ref Y: c_float, incY: c_int);
    extern proc cblas_ssbmv (order, Uplo, N: c_int, K: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int, beta: c_float, ref Y: c_float, incY: c_int);
    extern proc cblas_sspmv (order, Uplo, N: c_int, alpha: c_float, ref Ap: c_float, ref X: c_float, incX: c_int, beta: c_float, ref Y: c_float, incY: c_int);
    extern proc cblas_sger (order, M: c_int, N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int, ref A: c_float, lda: c_int);
    extern proc cblas_ssyr (order, Uplo, N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref A: c_float, lda: c_int);
    extern proc cblas_sspr (order, Uplo, N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref Ap: c_float);
    extern proc cblas_ssyr2 (order, Uplo, N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int, ref A: c_float, lda: c_int);
    extern proc cblas_sspr2 (order, Uplo, N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int, ref A: c_float);
    extern proc cblas_dsymv (order, Uplo, N: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int, beta: c_double, ref Y: c_double, incY: c_int);
    extern proc cblas_dsbmv (order, Uplo, N: c_int, K: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int, beta: c_double, ref Y: c_double, incY: c_int);
    extern proc cblas_dspmv (order, Uplo, N: c_int, alpha: c_double, ref Ap: c_double, ref X: c_double, incX: c_int, beta: c_double, ref Y: c_double, incY: c_int);
    extern proc cblas_dger (order, M: c_int, N: c_int, alpha: c_double, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int, ref A: c_double, lda: c_int);
    extern proc cblas_dsyr (order, Uplo, N: c_int, alpha: c_double, ref X: c_double, incX: c_int, ref A: c_double, lda: c_int);
    extern proc cblas_dspr (order, Uplo, N: c_int, alpha: c_double, ref X: c_double, incX: c_int, ref Ap: c_double);
    extern proc cblas_dsyr2 (order, Uplo, N: c_int, alpha: c_double, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int, ref A: c_double, lda: c_int);
    extern proc cblas_dspr2 (order, Uplo, N: c_int, alpha: c_double, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int, ref A: c_double);
    extern proc cblas_chemv (order, Uplo, N: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_chbmv (order, Uplo, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_chpmv (order, Uplo, N: c_int, ref alpha, ref Ap, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_cgeru (order, M: c_int, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_cgerc (order, M: c_int, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_cher (order, Uplo, N: c_int, alpha: c_float, ref X, incX: c_int, ref A, lda: c_int);
    extern proc cblas_chpr (order, Uplo, N: c_int, alpha: c_float, ref X, incX: c_int, ref A);
    extern proc cblas_cher2 (order, Uplo, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_chpr2 (order, Uplo, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref Ap);
    extern proc cblas_zhemv (order, Uplo, N: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_zhbmv (order, Uplo, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_zhpmv (order, Uplo, N: c_int, ref alpha, ref Ap, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_zgeru (order, M: c_int, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_zgerc (order, M: c_int, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_zher (order, Uplo, N: c_int, alpha: c_double, ref X, incX: c_int, ref A, lda: c_int);
    extern proc cblas_zhpr (order, Uplo, N: c_int, alpha: c_double, ref X, incX: c_int, ref A);
    extern proc cblas_zher2 (order, Uplo, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_zhpr2 (order, Uplo, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref Ap);
    extern proc cblas_sgemm (Order, TransA, TransB, M: c_int, N: c_int, K: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref B: c_float, ldb: c_int, beta: c_float, ref C: c_float, ldc: c_int);
    extern proc cblas_ssymm (Order, Side, Uplo, M: c_int, N: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref B: c_float, ldb: c_int, beta: c_float, ref C: c_float, ldc: c_int);
    extern proc cblas_ssyrk (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_float, ref A: c_float, lda: c_int, beta: c_float, ref C: c_float, ldc: c_int);
    extern proc cblas_ssyr2k (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref B: c_float, ldb: c_int, beta: c_float, ref C: c_float, ldc: c_int);
    extern proc cblas_strmm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref B: c_float, ldb: c_int);
    extern proc cblas_strsm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref B: c_float, ldb: c_int);
    extern proc cblas_dgemm (Order, TransA, TransB, M: c_int, N: c_int, K: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref B: c_double, ldb: c_int, beta: c_double, ref C: c_double, ldc: c_int);
    extern proc cblas_dsymm (Order, Side, Uplo, M: c_int, N: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref B: c_double, ldb: c_int, beta: c_double, ref C: c_double, ldc: c_int);
    extern proc cblas_dsyrk (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_double, ref A: c_double, lda: c_int, beta: c_double, ref C: c_double, ldc: c_int);
    extern proc cblas_dsyr2k (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref B: c_double, ldb: c_int, beta: c_double, ref C: c_double, ldc: c_int);
    extern proc cblas_dtrmm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref B: c_double, ldb: c_int);
    extern proc cblas_dtrsm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref B: c_double, ldb: c_int);
    extern proc cblas_cgemm (Order, TransA, TransB, M: c_int, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_csymm (Order, Side, Uplo, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_csyrk (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_csyr2k (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_ctrmm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int);
    extern proc cblas_ctrsm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int);
    extern proc cblas_zgemm (Order, TransA, TransB, M: c_int, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_zsymm (Order, Side, Uplo, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_zsyrk (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_zsyr2k (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_ztrmm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int);
    extern proc cblas_ztrsm (Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int);
    extern proc cblas_chemm (Order, Side, Uplo, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_cherk (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_float, ref A, lda: c_int, beta: c_float, ref C, ldc: c_int);
    extern proc cblas_cher2k (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, beta: c_float, ref C, ldc: c_int);
    extern proc cblas_zhemm (Order, Side, Uplo, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_zherk (Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_double, ref A, lda: c_int, beta: c_double, ref C, ldc: c_int);
    extern proc cblas_zher2k (Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, beta: c_double, ref C, ldc: c_int);

  }


}
