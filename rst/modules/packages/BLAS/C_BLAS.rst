.. default-domain:: chpl

.. module:: C_BLAS
   :synopsis: Support for low-level native C_BLAS bindings.

C_BLAS
======
**Usage**

.. code-block:: chapel

   use BLAS.C_BLAS;



Support for low-level native C_BLAS bindings.

This submodule wraps the netlib C_BLAS implementation, providing access to
all of C_BLAS calls.

Direct usage of this module requires an explicit ``require`` statement of
the BLAS header file, for example::

  use C_BLAS;
  require "cblas.h";

Arrays are passed in directly, while pointers to scalar
quantities (including complex numbers) are passed by reference (removing
the need to wrap these with ``c_ptrTo``). As with BLAS calls in C,
the user is responsible for passing in array dimensions etc. Furthermore,
different array element types require using different functions.

Refer to the
`C_BLAS documentation <https://www.netlib.org/lapack/explore-html/dir_f88bc7ad48bfd56d75bf9d4836a2bb00.html>`_
of the reference version for the usage of this module.



.. type:: type CBLAS_INDEX = c_int

.. type:: type CBLAS_ORDER = c_int

.. type:: type CBLAS_TRANSPOSE = c_int

.. type:: type CBLAS_UPLO = c_int

.. type:: type CBLAS_DIAG = c_int

.. type:: type CBLAS_SIDE = c_int

.. data:: const CblasRowMajor: CBLAS_ORDER

.. data:: const CblasColMajor: CBLAS_ORDER

.. data:: const CblasNoTrans: CBLAS_TRANSPOSE

.. data:: const CblasTrans: CBLAS_TRANSPOSE

.. data:: const CblasConjTrans: CBLAS_TRANSPOSE

.. data:: const CblasUpper: CBLAS_UPLO

.. data:: const CblasLower: CBLAS_UPLO

.. data:: const CblasNonUnit: CBLAS_DIAG

.. data:: const CblasUnit: CBLAS_DIAG

.. data:: const CblasLeft: CBLAS_SIDE

.. data:: const CblasRight: CBLAS_SIDE

.. function:: proc cblas_sdsdot(N: c_int, alpha: c_float, X: [] c_float, incX: c_int, Y: [] c_float, incY: c_int): c_float

.. function:: proc cblas_dsdot(N: c_int, X: [] c_float, incX: c_int, Y: [] c_float, incY: c_int): c_double

.. function:: proc cblas_sdot(N: c_int, X: [] c_float, incX: c_int, Y: [] c_float, incY: c_int): c_float

.. function:: proc cblas_ddot(N: c_int, X: [] c_double, incX: c_int, Y: [] c_double, incY: c_int): c_double

.. function:: proc cblas_cdotu_sub(N: c_int, X: [], incX: c_int, Y: [], incY: c_int, ref dotu)

.. function:: proc cblas_cdotc_sub(N: c_int, X: [], incX: c_int, Y: [], incY: c_int, ref dotc)

.. function:: proc cblas_zdotu_sub(N: c_int, X: [], incX: c_int, Y: [], incY: c_int, ref dotu)

.. function:: proc cblas_zdotc_sub(N: c_int, X: [], incX: c_int, Y: [], incY: c_int, ref dotc)

.. function:: proc cblas_snrm2(N: c_int, X: [] c_float, incX: c_int): c_float

.. function:: proc cblas_sasum(N: c_int, X: [] c_float, incX: c_int): c_float

.. function:: proc cblas_dnrm2(N: c_int, X: [] c_double, incX: c_int): c_double

.. function:: proc cblas_dasum(N: c_int, X: [] c_double, incX: c_int): c_double

.. function:: proc cblas_scnrm2(N: c_int, X: [], incX: c_int): c_float

.. function:: proc cblas_scasum(N: c_int, X: [], incX: c_int): c_float

.. function:: proc cblas_dznrm2(N: c_int, X: [], incX: c_int): c_double

.. function:: proc cblas_dzasum(N: c_int, X: [], incX: c_int): c_double

.. function:: proc cblas_isamax(N: c_int, X: [] c_float, incX: c_int): CBLAS_INDEX

.. function:: proc cblas_idamax(N: c_int, X: [] c_double, incX: c_int): CBLAS_INDEX

.. function:: proc cblas_icamax(N: c_int, X: [], incX: c_int): CBLAS_INDEX

.. function:: proc cblas_izamax(N: c_int, X: [], incX: c_int): CBLAS_INDEX

.. function:: proc cblas_sswap(N: c_int, X: [] c_float, incX: c_int, Y: [] c_float, incY: c_int)

.. function:: proc cblas_scopy(N: c_int, X: [] c_float, incX: c_int, Y: [] c_float, incY: c_int)

.. function:: proc cblas_saxpy(N: c_int, alpha: c_float, X: [] c_float, incX: c_int, Y: [] c_float, incY: c_int)

.. function:: proc cblas_dswap(N: c_int, X: [] c_double, incX: c_int, Y: [] c_double, incY: c_int)

.. function:: proc cblas_dcopy(N: c_int, X: [] c_double, incX: c_int, Y: [] c_double, incY: c_int)

.. function:: proc cblas_daxpy(N: c_int, alpha: c_double, X: [] c_double, incX: c_int, Y: [] c_double, incY: c_int)

.. function:: proc cblas_cswap(N: c_int, X: [], incX: c_int, Y: [], incY: c_int)

.. function:: proc cblas_ccopy(N: c_int, X: [], incX: c_int, Y: [], incY: c_int)

.. function:: proc cblas_caxpy(N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int)

.. function:: proc cblas_zswap(N: c_int, X: [], incX: c_int, Y: [], incY: c_int)

.. function:: proc cblas_zcopy(N: c_int, X: [], incX: c_int, Y: [], incY: c_int)

.. function:: proc cblas_zaxpy(N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int)

.. function:: proc cblas_srotg(ref a: c_float, ref b: c_float, ref c: c_float, ref s: c_float)

.. function:: proc cblas_srotmg(ref d1: c_float, ref d2: c_float, ref b1: c_float, b2: c_float, P: [] c_float)

.. function:: proc cblas_srot(N: c_int, X: [] c_float, incX: c_int, Y: [] c_float, incY: c_int, c: c_float, s: c_float)

.. function:: proc cblas_srotm(N: c_int, X: [] c_float, incX: c_int, Y: [] c_float, incY: c_int, P: [] c_float)

.. function:: proc cblas_drotg(ref a: c_double, ref b: c_double, ref c: c_double, ref s: c_double)

.. function:: proc cblas_drotmg(ref d1: c_double, ref d2: c_double, ref b1: c_double, b2: c_double, P: [] c_double)

.. function:: proc cblas_drot(N: c_int, X: [] c_double, incX: c_int, Y: [] c_double, incY: c_int, c: c_double, s: c_double)

.. function:: proc cblas_drotm(N: c_int, X: [] c_double, incX: c_int, Y: [] c_double, incY: c_int, P: [] c_double)

.. function:: proc cblas_sscal(N: c_int, alpha: c_float, X: [] c_float, incX: c_int)

.. function:: proc cblas_dscal(N: c_int, alpha: c_double, X: [] c_double, incX: c_int)

.. function:: proc cblas_cscal(N: c_int, ref alpha, X: [], incX: c_int)

.. function:: proc cblas_zscal(N: c_int, ref alpha, X: [], incX: c_int)

.. function:: proc cblas_csscal(N: c_int, alpha: c_float, X: [], incX: c_int)

.. function:: proc cblas_zdscal(N: c_int, alpha: c_double, X: [], incX: c_int)

.. function:: proc cblas_sgemv(order, TransA, M: c_int, N: c_int, alpha: c_float, A: [] c_float, lda: c_int, X: [] c_float, incX: c_int, beta: c_float, Y: [] c_float, incY: c_int)

.. function:: proc cblas_sgbmv(order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, alpha: c_float, A: [] c_float, lda: c_int, X: [] c_float, incX: c_int, beta: c_float, Y: [] c_float, incY: c_int)

.. function:: proc cblas_strmv(order, Uplo, TransA, Diag, N: c_int, A: [] c_float, lda: c_int, X: [] c_float, incX: c_int)

.. function:: proc cblas_stbmv(order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [] c_float, lda: c_int, X: [] c_float, incX: c_int)

.. function:: proc cblas_stpmv(order, Uplo, TransA, Diag, N: c_int, Ap: [] c_float, X: [] c_float, incX: c_int)

.. function:: proc cblas_strsv(order, Uplo, TransA, Diag, N: c_int, A: [] c_float, lda: c_int, X: [] c_float, incX: c_int)

.. function:: proc cblas_stbsv(order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [] c_float, lda: c_int, X: [] c_float, incX: c_int)

.. function:: proc cblas_stpsv(order, Uplo, TransA, Diag, N: c_int, Ap: [] c_float, X: [] c_float, incX: c_int)

.. function:: proc cblas_dgemv(order, TransA, M: c_int, N: c_int, alpha: c_double, A: [] c_double, lda: c_int, X: [] c_double, incX: c_int, beta: c_double, Y: [] c_double, incY: c_int)

.. function:: proc cblas_dgbmv(order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, alpha: c_double, A: [] c_double, lda: c_int, X: [] c_double, incX: c_int, beta: c_double, Y: [] c_double, incY: c_int)

.. function:: proc cblas_dtrmv(order, Uplo, TransA, Diag, N: c_int, A: [] c_double, lda: c_int, X: [] c_double, incX: c_int)

.. function:: proc cblas_dtbmv(order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [] c_double, lda: c_int, X: [] c_double, incX: c_int)

.. function:: proc cblas_dtpmv(order, Uplo, TransA, Diag, N: c_int, Ap: [] c_double, X: [] c_double, incX: c_int)

.. function:: proc cblas_dtrsv(order, Uplo, TransA, Diag, N: c_int, A: [] c_double, lda: c_int, X: [] c_double, incX: c_int)

.. function:: proc cblas_dtbsv(order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [] c_double, lda: c_int, X: [] c_double, incX: c_int)

.. function:: proc cblas_dtpsv(order, Uplo, TransA, Diag, N: c_int, Ap: [] c_double, X: [] c_double, incX: c_int)

.. function:: proc cblas_cgemv(order, TransA, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int)

.. function:: proc cblas_cgbmv(order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int)

.. function:: proc cblas_ctrmv(order, Uplo, TransA, Diag, N: c_int, A: [], lda: c_int, X: [], incX: c_int)

.. function:: proc cblas_ctbmv(order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [], lda: c_int, X: [], incX: c_int)

.. function:: proc cblas_ctpmv(order, Uplo, TransA, Diag, N: c_int, Ap: [], X: [], incX: c_int)

.. function:: proc cblas_ctrsv(order, Uplo, TransA, Diag, N: c_int, A: [], lda: c_int, X: [], incX: c_int)

.. function:: proc cblas_ctbsv(order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [], lda: c_int, X: [], incX: c_int)

.. function:: proc cblas_ctpsv(order, Uplo, TransA, Diag, N: c_int, Ap: [], X: [], incX: c_int)

.. function:: proc cblas_zgemv(order, TransA, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int)

.. function:: proc cblas_zgbmv(order, TransA, M: c_int, N: c_int, KL: c_int, KU: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int)

.. function:: proc cblas_ztrmv(order, Uplo, TransA, Diag, N: c_int, A: [], lda: c_int, X: [], incX: c_int)

.. function:: proc cblas_ztbmv(order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [], lda: c_int, X: [], incX: c_int)

.. function:: proc cblas_ztpmv(order, Uplo, TransA, Diag, N: c_int, Ap: [], X: [], incX: c_int)

.. function:: proc cblas_ztrsv(order, Uplo, TransA, Diag, N: c_int, A: [], lda: c_int, X: [], incX: c_int)

.. function:: proc cblas_ztbsv(order, Uplo, TransA, Diag, N: c_int, K: c_int, A: [], lda: c_int, X: [], incX: c_int)

.. function:: proc cblas_ztpsv(order, Uplo, TransA, Diag, N: c_int, Ap: [], X: [], incX: c_int)

.. function:: proc cblas_ssymv(order, Uplo, N: c_int, alpha: c_float, A: [] c_float, lda: c_int, X: [] c_float, incX: c_int, beta: c_float, Y: [] c_float, incY: c_int)

.. function:: proc cblas_ssbmv(order, Uplo, N: c_int, K: c_int, alpha: c_float, A: [] c_float, lda: c_int, X: [] c_float, incX: c_int, beta: c_float, Y: [] c_float, incY: c_int)

.. function:: proc cblas_sspmv(order, Uplo, N: c_int, alpha: c_float, Ap: [] c_float, X: [] c_float, incX: c_int, beta: c_float, Y: [] c_float, incY: c_int)

.. function:: proc cblas_sger(order, M: c_int, N: c_int, alpha: c_float, X: [] c_float, incX: c_int, Y: [] c_float, incY: c_int, A: [] c_float, lda: c_int)

.. function:: proc cblas_ssyr(order, Uplo, N: c_int, alpha: c_float, X: [] c_float, incX: c_int, A: [] c_float, lda: c_int)

.. function:: proc cblas_sspr(order, Uplo, N: c_int, alpha: c_float, X: [] c_float, incX: c_int, Ap: [] c_float)

.. function:: proc cblas_ssyr2(order, Uplo, N: c_int, alpha: c_float, X: [] c_float, incX: c_int, Y: [] c_float, incY: c_int, A: [] c_float, lda: c_int)

.. function:: proc cblas_sspr2(order, Uplo, N: c_int, alpha: c_float, X: [] c_float, incX: c_int, Y: [] c_float, incY: c_int, A: [] c_float)

.. function:: proc cblas_dsymv(order, Uplo, N: c_int, alpha: c_double, A: [] c_double, lda: c_int, X: [] c_double, incX: c_int, beta: c_double, Y: [] c_double, incY: c_int)

.. function:: proc cblas_dsbmv(order, Uplo, N: c_int, K: c_int, alpha: c_double, A: [] c_double, lda: c_int, X: [] c_double, incX: c_int, beta: c_double, Y: [] c_double, incY: c_int)

.. function:: proc cblas_dspmv(order, Uplo, N: c_int, alpha: c_double, Ap: [] c_double, X: [] c_double, incX: c_int, beta: c_double, Y: [] c_double, incY: c_int)

.. function:: proc cblas_dger(order, M: c_int, N: c_int, alpha: c_double, X: [] c_double, incX: c_int, Y: [] c_double, incY: c_int, A: [] c_double, lda: c_int)

.. function:: proc cblas_dsyr(order, Uplo, N: c_int, alpha: c_double, X: [] c_double, incX: c_int, A: [] c_double, lda: c_int)

.. function:: proc cblas_dspr(order, Uplo, N: c_int, alpha: c_double, X: [] c_double, incX: c_int, Ap: [] c_double)

.. function:: proc cblas_dsyr2(order, Uplo, N: c_int, alpha: c_double, X: [] c_double, incX: c_int, Y: [] c_double, incY: c_int, A: [] c_double, lda: c_int)

.. function:: proc cblas_dspr2(order, Uplo, N: c_int, alpha: c_double, X: [] c_double, incX: c_int, Y: [] c_double, incY: c_int, A: [] c_double)

.. function:: proc cblas_chemv(order, Uplo, N: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int)

.. function:: proc cblas_chbmv(order, Uplo, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int)

.. function:: proc cblas_chpmv(order, Uplo, N: c_int, ref alpha, Ap: [], X: [], incX: c_int, ref beta, Y: [], incY: c_int)

.. function:: proc cblas_cgeru(order, M: c_int, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int)

.. function:: proc cblas_cgerc(order, M: c_int, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int)

.. function:: proc cblas_cher(order, Uplo, N: c_int, alpha: c_float, X: [], incX: c_int, A: [], lda: c_int)

.. function:: proc cblas_chpr(order, Uplo, N: c_int, alpha: c_float, X: [], incX: c_int, A: [])

.. function:: proc cblas_cher2(order, Uplo, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int)

.. function:: proc cblas_chpr2(order, Uplo, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, Ap: [])

.. function:: proc cblas_zhemv(order, Uplo, N: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int)

.. function:: proc cblas_zhbmv(order, Uplo, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, X: [], incX: c_int, ref beta, Y: [], incY: c_int)

.. function:: proc cblas_zhpmv(order, Uplo, N: c_int, ref alpha, Ap: [], X: [], incX: c_int, ref beta, Y: [], incY: c_int)

.. function:: proc cblas_zgeru(order, M: c_int, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int)

.. function:: proc cblas_zgerc(order, M: c_int, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int)

.. function:: proc cblas_zher(order, Uplo, N: c_int, alpha: c_double, X: [], incX: c_int, A: [], lda: c_int)

.. function:: proc cblas_zhpr(order, Uplo, N: c_int, alpha: c_double, X: [], incX: c_int, A: [])

.. function:: proc cblas_zher2(order, Uplo, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, A: [], lda: c_int)

.. function:: proc cblas_zhpr2(order, Uplo, N: c_int, ref alpha, X: [], incX: c_int, Y: [], incY: c_int, Ap: [])

.. function:: proc cblas_sgemm(Order, TransA, TransB, M: c_int, N: c_int, K: c_int, alpha: c_float, A: [] c_float, lda: c_int, B: [] c_float, ldb: c_int, beta: c_float, C: [] c_float, ldc: c_int)

.. function:: proc cblas_ssymm(Order, Side, Uplo, M: c_int, N: c_int, alpha: c_float, A: [] c_float, lda: c_int, B: [] c_float, ldb: c_int, beta: c_float, C: [] c_float, ldc: c_int)

.. function:: proc cblas_ssyrk(Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_float, A: [] c_float, lda: c_int, beta: c_float, C: [] c_float, ldc: c_int)

.. function:: proc cblas_ssyr2k(Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_float, A: [] c_float, lda: c_int, B: [] c_float, ldb: c_int, beta: c_float, C: [] c_float, ldc: c_int)

.. function:: proc cblas_strmm(Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_float, A: [] c_float, lda: c_int, B: [] c_float, ldb: c_int)

.. function:: proc cblas_strsm(Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_float, A: [] c_float, lda: c_int, B: [] c_float, ldb: c_int)

.. function:: proc cblas_dgemm(Order, TransA, TransB, M: c_int, N: c_int, K: c_int, alpha: c_double, A: [] c_double, lda: c_int, B: [] c_double, ldb: c_int, beta: c_double, C: [] c_double, ldc: c_int)

.. function:: proc cblas_dsymm(Order, Side, Uplo, M: c_int, N: c_int, alpha: c_double, A: [] c_double, lda: c_int, B: [] c_double, ldb: c_int, beta: c_double, C: [] c_double, ldc: c_int)

.. function:: proc cblas_dsyrk(Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_double, A: [] c_double, lda: c_int, beta: c_double, C: [] c_double, ldc: c_int)

.. function:: proc cblas_dsyr2k(Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_double, A: [] c_double, lda: c_int, B: [] c_double, ldb: c_int, beta: c_double, C: [] c_double, ldc: c_int)

.. function:: proc cblas_dtrmm(Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_double, A: [] c_double, lda: c_int, B: [] c_double, ldb: c_int)

.. function:: proc cblas_dtrsm(Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, alpha: c_double, A: [] c_double, lda: c_int, B: [] c_double, ldb: c_int)

.. function:: proc cblas_cgemm(Order, TransA, TransB, M: c_int, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int)

.. function:: proc cblas_csymm(Order, Side, Uplo, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int)

.. function:: proc cblas_csyrk(Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, ref beta, C: [], ldc: c_int)

.. function:: proc cblas_csyr2k(Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int)

.. function:: proc cblas_ctrmm(Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int)

.. function:: proc cblas_ctrsm(Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int)

.. function:: proc cblas_zgemm(Order, TransA, TransB, M: c_int, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int)

.. function:: proc cblas_zsymm(Order, Side, Uplo, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int)

.. function:: proc cblas_zsyrk(Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, ref beta, C: [], ldc: c_int)

.. function:: proc cblas_zsyr2k(Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int)

.. function:: proc cblas_ztrmm(Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int)

.. function:: proc cblas_ztrsm(Order, Side, Uplo, TransA, Diag, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int)

.. function:: proc cblas_chemm(Order, Side, Uplo, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int)

.. function:: proc cblas_cherk(Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_float, A: [], lda: c_int, beta: c_float, C: [], ldc: c_int)

.. function:: proc cblas_cher2k(Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, beta: c_float, C: [], ldc: c_int)

.. function:: proc cblas_zhemm(Order, Side, Uplo, M: c_int, N: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, ref beta, C: [], ldc: c_int)

.. function:: proc cblas_zherk(Order, Uplo, Trans, N: c_int, K: c_int, alpha: c_double, A: [], lda: c_int, beta: c_double, C: [], ldc: c_int)

.. function:: proc cblas_zher2k(Order, Uplo, Trans, N: c_int, K: c_int, ref alpha, A: [], lda: c_int, B: [], ldb: c_int, beta: c_double, C: [], ldc: c_int)

