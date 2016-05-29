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
  // These are C enums
  extern type CBLAS_ORDER;
  extern type CBLAS_TRANSPOSE;
  extern type CBLAS_UPLO;
  extern type CBLAS_DIAG;
  extern type CBLAS_SIDE;

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
    extern proc cblas_sgemv (order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, M: c_int, N: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int, beta: c_float, ref Y: c_float, incY: c_int);
    extern proc cblas_sgbmv (order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, M: c_int, N: c_int, KL: c_int, KU: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int, beta: c_float, ref Y: c_float, incY: c_int);
    extern proc cblas_strmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int);
    extern proc cblas_stbmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, K: c_int, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int);
    extern proc cblas_stpmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref Ap: c_float, ref X: c_float, incX: c_int);
    extern proc cblas_strsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int);
    extern proc cblas_stbsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, K: c_int, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int);
    extern proc cblas_stpsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref Ap: c_float, ref X: c_float, incX: c_int);
    extern proc cblas_dgemv (order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, M: c_int, N: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int, beta: c_double, ref Y: c_double, incY: c_int);
    extern proc cblas_dgbmv (order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, M: c_int, N: c_int, KL: c_int, KU: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int, beta: c_double, ref Y: c_double, incY: c_int);
    extern proc cblas_dtrmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int);
    extern proc cblas_dtbmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, K: c_int, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int);
    extern proc cblas_dtpmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref Ap: c_double, ref X: c_double, incX: c_int);
    extern proc cblas_dtrsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int);
    extern proc cblas_dtbsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, K: c_int, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int);
    extern proc cblas_dtpsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref Ap: c_double, ref X: c_double, incX: c_int);
    extern proc cblas_cgemv (order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_cgbmv (order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, M: c_int, N: c_int, KL: c_int, KU: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_ctrmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ctbmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, K: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ctpmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref Ap, ref X, incX: c_int);
    extern proc cblas_ctrsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ctbsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, K: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ctpsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref Ap, ref X, incX: c_int);
    extern proc cblas_zgemv (order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_zgbmv (order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, M: c_int, N: c_int, KL: c_int, KU: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_ztrmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ztbmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, K: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ztpmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref Ap, ref X, incX: c_int);
    extern proc cblas_ztrsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ztbsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, K: c_int, ref A, lda: c_int, ref X, incX: c_int);
    extern proc cblas_ztpsv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, N: c_int, ref Ap, ref X, incX: c_int);
    extern proc cblas_ssymv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int, beta: c_float, ref Y: c_float, incY: c_int);
    extern proc cblas_ssbmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, K: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref X: c_float, incX: c_int, beta: c_float, ref Y: c_float, incY: c_int);
    extern proc cblas_sspmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_float, ref Ap: c_float, ref X: c_float, incX: c_int, beta: c_float, ref Y: c_float, incY: c_int);
    extern proc cblas_sger (order: CBLAS_ORDER, M: c_int, N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int, ref A: c_float, lda: c_int);
    extern proc cblas_ssyr (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref A: c_float, lda: c_int);
    extern proc cblas_sspr (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref Ap: c_float);
    extern proc cblas_ssyr2 (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int, ref A: c_float, lda: c_int);
    extern proc cblas_sspr2 (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_float, ref X: c_float, incX: c_int, ref Y: c_float, incY: c_int, ref A: c_float);
    extern proc cblas_dsymv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int, beta: c_double, ref Y: c_double, incY: c_int);
    extern proc cblas_dsbmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, K: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref X: c_double, incX: c_int, beta: c_double, ref Y: c_double, incY: c_int);
    extern proc cblas_dspmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_double, ref Ap: c_double, ref X: c_double, incX: c_int, beta: c_double, ref Y: c_double, incY: c_int);
    extern proc cblas_dger (order: CBLAS_ORDER, M: c_int, N: c_int, alpha: c_double, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int, ref A: c_double, lda: c_int);
    extern proc cblas_dsyr (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_double, ref X: c_double, incX: c_int, ref A: c_double, lda: c_int);
    extern proc cblas_dspr (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_double, ref X: c_double, incX: c_int, ref Ap: c_double);
    extern proc cblas_dsyr2 (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_double, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int, ref A: c_double, lda: c_int);
    extern proc cblas_dspr2 (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_double, ref X: c_double, incX: c_int, ref Y: c_double, incY: c_int, ref A: c_double);
    extern proc cblas_chemv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_chbmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_chpmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, ref alpha, ref Ap, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_cgeru (order: CBLAS_ORDER, M: c_int, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_cgerc (order: CBLAS_ORDER, M: c_int, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_cher (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_float, ref X, incX: c_int, ref A, lda: c_int);
    extern proc cblas_chpr (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_float, ref X, incX: c_int, ref A);
    extern proc cblas_cher2 (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_chpr2 (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref Ap);
    extern proc cblas_zhemv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_zhbmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_zhpmv (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, ref alpha, ref Ap, ref X, incX: c_int, ref beta, ref Y, incY: c_int);
    extern proc cblas_zgeru (order: CBLAS_ORDER, M: c_int, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_zgerc (order: CBLAS_ORDER, M: c_int, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_zher (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_double, ref X, incX: c_int, ref A, lda: c_int);
    extern proc cblas_zhpr (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, alpha: c_double, ref X, incX: c_int, ref A);
    extern proc cblas_zher2 (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref A, lda: c_int);
    extern proc cblas_zhpr2 (order: CBLAS_ORDER, Uplo: CBLAS_UPLO, N: c_int, ref alpha, ref X, incX: c_int, ref Y, incY: c_int, ref Ap);
    extern proc cblas_sgemm (Order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, TransB: CBLAS_TRANSPOSE, M: c_int, N: c_int, K: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref B: c_float, ldb: c_int, beta: c_float, ref C: c_float, ldc: c_int);
    extern proc cblas_ssymm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, M: c_int, N: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref B: c_float, ldb: c_int, beta: c_float, ref C: c_float, ldc: c_int);
    extern proc cblas_ssyrk (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, alpha: c_float, ref A: c_float, lda: c_int, beta: c_float, ref C: c_float, ldc: c_int);
    extern proc cblas_ssyr2k (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref B: c_float, ldb: c_int, beta: c_float, ref C: c_float, ldc: c_int);
    extern proc cblas_strmm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, M: c_int, N: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref B: c_float, ldb: c_int);
    extern proc cblas_strsm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, M: c_int, N: c_int, alpha: c_float, ref A: c_float, lda: c_int, ref B: c_float, ldb: c_int);
    extern proc cblas_dgemm (Order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, TransB: CBLAS_TRANSPOSE, M: c_int, N: c_int, K: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref B: c_double, ldb: c_int, beta: c_double, ref C: c_double, ldc: c_int);
    extern proc cblas_dsymm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, M: c_int, N: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref B: c_double, ldb: c_int, beta: c_double, ref C: c_double, ldc: c_int);
    extern proc cblas_dsyrk (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, alpha: c_double, ref A: c_double, lda: c_int, beta: c_double, ref C: c_double, ldc: c_int);
    extern proc cblas_dsyr2k (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref B: c_double, ldb: c_int, beta: c_double, ref C: c_double, ldc: c_int);
    extern proc cblas_dtrmm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, M: c_int, N: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref B: c_double, ldb: c_int);
    extern proc cblas_dtrsm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, M: c_int, N: c_int, alpha: c_double, ref A: c_double, lda: c_int, ref B: c_double, ldb: c_int);
    extern proc cblas_cgemm (Order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, TransB: CBLAS_TRANSPOSE, M: c_int, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_csymm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_csyrk (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_csyr2k (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_ctrmm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int);
    extern proc cblas_ctrsm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int);
    extern proc cblas_zgemm (Order: CBLAS_ORDER, TransA: CBLAS_TRANSPOSE, TransB: CBLAS_TRANSPOSE, M: c_int, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_zsymm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_zsyrk (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_zsyr2k (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_ztrmm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int);
    extern proc cblas_ztrsm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, TransA: CBLAS_TRANSPOSE, Diag: CBLAS_DIAG, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int);
    extern proc cblas_chemm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_cherk (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, alpha: c_float, ref A, lda: c_int, beta: c_float, ref C, ldc: c_int);
    extern proc cblas_cher2k (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, beta: c_float, ref C, ldc: c_int);
    extern proc cblas_zhemm (Order: CBLAS_ORDER, Side: CBLAS_SIDE, Uplo: CBLAS_UPLO, M: c_int, N: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, ref beta, ref C, ldc: c_int);
    extern proc cblas_zherk (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, alpha: c_double, ref A, lda: c_int, beta: c_double, ref C, ldc: c_int);
    extern proc cblas_zher2k (Order: CBLAS_ORDER, Uplo: CBLAS_UPLO, Trans: CBLAS_TRANSPOSE, N: c_int, K: c_int, ref alpha, ref A, lda: c_int, ref B, ldb: c_int, beta: c_double, ref C, ldc: c_int);

  }


}
