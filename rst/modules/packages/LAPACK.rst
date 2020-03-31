.. default-domain:: chpl

.. module:: LAPACK
   :synopsis: Chapel idiomatic wrappers for the LAPACK library.

LAPACK
======
**Usage**

.. code-block:: chapel

   use LAPACK;



Chapel idiomatic wrappers for the LAPACK library.

.. note::

  Because of the volume of procedures provided, and because their behavior is virtually unchanged, in-depth documentation on each procedure's purpose and use is not given here.
  
Consult the `Netlibs LAPACK <https://www.netlib.org/lapack/>`_ site, and the
`Netlibs <https://www.netlib.org/lapack/explore-html/globals_func.html>`_ and
`Intel
<https://software.intel.com/en-us/mkl-developer-reference-fortran-lapack-routines>`_
LAPACK documents for that information.

Additionally, only a small set of LAPACK procedures have been tested for correctness.

Differences from LAPACK
-----------------------

The `XYYZZZ <https://www.netlib.org/lapack/individualroutines.html>`_ naming convention for procedures has been only slightly modified with the removal of the type identifier in the procedure name,
creating a set of polymorphic procedures under the name YYZZZ.

Procedure arguments have been simplified. 
Using the original Fortran documentation, groupings of arguments pertaining to matrices and arrays were found, and encapsulated by Chapel arrays.

For example, instead of 

.. code-block:: chapel

  LAPACKE_sgesv(matrix_order : lapack_memory_order, n : c_int, nrhs : c_int, a : [] c_float, lda : c_int, ipiv : [] c_int, b : [] c_float, ldb : c_int) : c_int;

the arguments n, nrhs, lda, and ldb can be encapsulated by properties of Chapel arrays and their domains, giving the LAPACK procedure:

.. code-block:: chapel

  gesv(matrix_order : lapack_memory_order, a : [] real(32), ipiv : [] c_int, b : [] real(32) ): c_int;

This requires that your matrices are stored in a two dimensional form.

Type Adjustments
----------------

The LAPACKE types ``lapack_int``, ``lapack_float``, ``lapack_double``, ``lapack_complex_float``, ``lapack_complex_double``, and ``lapack_logical`` are not defined at all, but rather are replaced by the types ``c_int``, ``real(32)``, ``real(64)``, ``complex(64)``, ``complex(128)``, and ``c_int`` respectively.



The :enum:`lapack_memory_order` enumeration is a replacement for the ``c_int`` constants :data:`LAPACK_ROW_MAJOR` and :data:`LAPACK_COL_MAJOR` defined by LAPACK and used by ``matrix_order`` which is an argument present in essentially all procedures.
The value of each enumeration is equivalent (both in type and value) of the LAPACK constants.
However the constants are also provided and can be used as well.

The ``LAPACK_SELECT`` types are function pointer types, whose procedures need to be declared and implemented in C, then be declared as ``extern var`` on the Chapel side. 
See the dgees test (``$CHPL_HOME/test/library/packages/LAPACK/dgees.chpl``) for a concrete example.

Compiling with LAPACK
------------------------

Using this module require that you have LAPACK (binaries and C interface) on your system, as it is not bundled with Chapel. 

You can download and build the `Netlibs LAPACK <https://www.netlib.org/lapack/>`_ , if it is not already installed, and this section will assume usage of the Netlibs LAPACK.
You must also have libgfortran installed on your system (typically installed along with gcc).

To compile with LAPACK, Chapel needs to know:

1. Where the LAPACKE header (lapacke.h) is.

2. Where the libgfortran binary is (sometimes it is not to be found by ``ld``).

3. Where the various LAPACK binaries (lapacke, lapack, and refblas) are.

Once the details are worked out, compiling is quite simple and nearly identical to how one would compile a C program to work with LAPACK.

.. code-block:: sh

  chpl -I$PATH_TO_LAPACKE_INCLUDE_DIR \
       -L$PATH_TO_LIBGFORTRAN -lgfortran \
       -L$PATH_TO_LAPACK_BINARIES -llapacke -llapack -lrefblas \
       source.chpl

As an example,

.. code-block:: sh

  chpl -I$HOME/LAPACK/lapacke/include \
       -L/usr/lib/gcc/stuff -lgfortran \
       -L$HOME/LAPACK -llapacke -llapack -lrefblas \
       source.chpl

would be the command to use if Netlibs LAPACK had been built in ``$HOME/LAPACK``, and the libgfortran binary found to be located in ``/usr/lib/gcc/stuff``

You can refer to the COMPOPTS file of the LAPACK test suit (``$CHPL_HOME/test/library/packages/LAPACK/``) for an example

Future Work
-----------

We anticipate the following additions:

1. Better documentation on each individual procedure, mostly derived from the original Fortran documentation.

2. Larger test coverage of the provided procedures.

3. Enumerated values for arguments of procedures that currently take strings to denote different options.



.. enum:: enum LapackImpl { lapack, mkl, off }

   Available LAPACK implementations for ``lapackImpl`` 


.. data:: config param lapackImpl = LapackImpl.lapack

   
   Specifies which header filename to include, based on the lapack
   implementation.
   
   Most LAPACK implementations rely on ``lapacke.h``, which is used when
   ``lapackImpl = lapack``, the default setting.
   
    - ``LapackImpl.lapack`` includes ``lapacke.h`` (default)
    - ``LapackImpl.mkl`` includes ``mkl_lapacke.h``
    - ``LapackImpl.off`` includes nothing
   
   .. warning::
   
     MKL does not currently support the low-level ``LAPACK_*`` interface, due
     to breaking naming conventions.
   
   

.. data:: config param lapackHeader = ""

   
   Manually specifies the header filename to include. This flag overrides.
   the header determined by ``lapackImpl``.
   
   This flag should only be necessary if using an ``LAPACK`` implementation
   with a unique header name that is not supported by ``lapackImpl``.
   However, no guarantees can be made about this module working with untested
   implementations.
   
   

.. function:: proc isLAPACKType(type t) param: bool

   Return `true` if type is supported by LAPACK 

.. type:: type LAPACK_C_SELECT1

   External function pointer type LAPACK_C_SELECT1.

.. type:: type LAPACK_Z_SELECT1

   External function pointer type LAPACK_Z_SELECT1.

.. type:: type LAPACK_C_SELECT2

   External function pointer type LAPACK_C_SELECT2.

.. type:: type LAPACK_D_SELECT2

   External function pointer type LAPACK_D_SELECT2.

.. type:: type LAPACK_S_SELECT2

   External function pointer type LAPACK_S_SELECT2.

.. type:: type LAPACK_Z_SELECT2

   External function pointer type LAPACK_Z_SELECT2.

.. type:: type LAPACK_S_SELECT3

   External function pointer type LAPACK_S_SELECT3.

.. type:: type LAPACK_D_SELECT3

   External function pointer type LAPACK_D_SELECT3.

.. data:: const LAPACK_ROW_MAJOR: c_int

   External const LAPACK_ROW_MAJOR.

.. data:: const LAPACK_COL_MAJOR: c_int

   External const LAPACK_COL_MAJOR.

.. data:: const LAPACK_WORK_MEMORY_ERROR: c_int

   External const LAPACK_WORK_MEMORY_ERROR.

.. data:: const LAPACK_TRANSPOSE_MEMORY_ERROR: c_int

   External const LAPACK_TRANSPOSE_MEMORY_ERROR.

.. enum:: enum lapack_memory_order { row_major = 101: c_int, column_major = 102: c_int }

   Mirrors the :data:`LAPACK_ROW_MAJOR` and :data:`LAPACK_COL_MAJOR` consts defined by LAPACK.
   
   The enum values values are the same as the const value such that ``lapack_memory_order.row_major == LAPACK_ROW_MAJOR`` and ``lapack_memory_order.column_major == LAPACK_COL_MAJOR``, allowing the enum to be used with pure LAPACK procedures.
    


.. function:: proc bdsdc(matrix_order: lapack_memory_order, uplo: string, compq: string, n: c_int, d: [] real(32), e: [] real(32), u: [] real(32), vt: [] real(32), q: [] real(32), iq: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sbdsdc for the type real(32).
    

.. function:: proc bdsdc(matrix_order: lapack_memory_order, uplo: string, compq: string, n: c_int, d: [] real(64), e: [] real(64), u: [] real(64), vt: [] real(64), q: [] real(64), iq: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dbdsdc for the type real(64).
    

.. function:: proc bdsqr(matrix_order: lapack_memory_order, uplo: string, d: [] real(32), e: [] real(32), vt: [] real(32), u: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sbdsqr for the type real(32).
    

.. function:: proc bdsqr(matrix_order: lapack_memory_order, uplo: string, d: [] real(64), e: [] real(64), vt: [] real(64), u: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dbdsqr for the type real(64).
    

.. function:: proc bdsqr(matrix_order: lapack_memory_order, uplo: string, d: [] real(32), e: [] real(32), vt: [] complex(64), u: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cbdsqr for the type complex(64).
    

.. function:: proc bdsqr(matrix_order: lapack_memory_order, uplo: string, d: [] real(64), e: [] real(64), vt: [] complex(128), u: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zbdsqr for the type complex(128).
    

.. function:: proc disna(job: string, m: c_int, n: c_int, d: [] real(32), sep: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sdisna for the type real(32).
    

.. function:: proc disna(job: string, m: c_int, n: c_int, d: [] real(64), sep: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_ddisna for the type real(64).
    

.. function:: proc gbbrd(matrix_order: lapack_memory_order, vect: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(32), d: [] real(32), e: [] real(32), q: [] real(32), pt: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgbbrd for the type real(32).
    

.. function:: proc gbbrd(matrix_order: lapack_memory_order, vect: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(64), d: [] real(64), e: [] real(64), q: [] real(64), pt: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgbbrd for the type real(64).
    

.. function:: proc gbbrd(matrix_order: lapack_memory_order, vect: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(64), d: [] real(32), e: [] real(32), q: [] complex(64), pt: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgbbrd for the type complex(64).
    

.. function:: proc gbbrd(matrix_order: lapack_memory_order, vect: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(128), d: [] real(64), e: [] real(64), q: [] complex(128), pt: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgbbrd for the type complex(128).
    

.. function:: proc gbcon(matrix_order: lapack_memory_order, norm: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(32), ipiv: [] c_int, anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgbcon for the type real(32).
    

.. function:: proc gbcon(matrix_order: lapack_memory_order, norm: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(64), ipiv: [] c_int, anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgbcon for the type real(64).
    

.. function:: proc gbcon(matrix_order: lapack_memory_order, norm: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(64), ipiv: [] c_int, anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgbcon for the type complex(64).
    

.. function:: proc gbcon(matrix_order: lapack_memory_order, norm: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(128), ipiv: [] c_int, anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgbcon for the type complex(128).
    

.. function:: proc gbequ(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] real(32), r: [] real(32), c: [] real(32), ref rowcnd: real(32), ref colcnd: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgbequ for the type real(32).
    

.. function:: proc gbequ(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] real(64), r: [] real(64), c: [] real(64), ref rowcnd: real(64), ref colcnd: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgbequ for the type real(64).
    

.. function:: proc gbequ(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] complex(64), r: [] real(32), c: [] real(32), ref rowcnd: real(32), ref colcnd: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgbequ for the type complex(64).
    

.. function:: proc gbequ(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] complex(128), r: [] real(64), c: [] real(64), ref rowcnd: real(64), ref colcnd: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgbequ for the type complex(128).
    

.. function:: proc gbequb(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] real(32), r: [] real(32), c: [] real(32), ref rowcnd: real(32), ref colcnd: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgbequb for the type real(32).
    

.. function:: proc gbequb(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] real(64), r: [] real(64), c: [] real(64), ref rowcnd: real(64), ref colcnd: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgbequb for the type real(64).
    

.. function:: proc gbequb(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] complex(64), r: [] real(32), c: [] real(32), ref rowcnd: real(32), ref colcnd: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgbequb for the type complex(64).
    

.. function:: proc gbequb(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] complex(128), r: [] real(64), c: [] real(64), ref rowcnd: real(64), ref colcnd: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgbequb for the type complex(128).
    

.. function:: proc gbrfs(matrix_order: lapack_memory_order, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(32), afb: [] real(32), ipiv: [] c_int, b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgbrfs for the type real(32).
    

.. function:: proc gbrfs(matrix_order: lapack_memory_order, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(64), afb: [] real(64), ipiv: [] c_int, b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgbrfs for the type real(64).
    

.. function:: proc gbrfs(matrix_order: lapack_memory_order, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(64), afb: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgbrfs for the type complex(64).
    

.. function:: proc gbrfs(matrix_order: lapack_memory_order, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(128), afb: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgbrfs for the type complex(128).
    

.. function:: proc gbrfsx(matrix_order: lapack_memory_order, trans: string, equed: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(32), afb: [] real(32), ipiv: [] c_int, r: [] real(32), c: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgbrfsx for the type real(32).
    

.. function:: proc gbrfsx(matrix_order: lapack_memory_order, trans: string, equed: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(64), afb: [] real(64), ipiv: [] c_int, r: [] real(64), c: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgbrfsx for the type real(64).
    

.. function:: proc gbrfsx(matrix_order: lapack_memory_order, trans: string, equed: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(64), afb: [] complex(64), ipiv: [] c_int, r: [] real(32), c: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgbrfsx for the type complex(64).
    

.. function:: proc gbrfsx(matrix_order: lapack_memory_order, trans: string, equed: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(128), afb: [] complex(128), ipiv: [] c_int, r: [] real(64), c: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgbrfsx for the type complex(128).
    

.. function:: proc gbsv(matrix_order: lapack_memory_order, n: c_int, kl: c_int, ku: c_int, ab: [] real(32), ipiv: [] c_int, b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgbsv for the type real(32).
    

.. function:: proc gbsv(matrix_order: lapack_memory_order, n: c_int, kl: c_int, ku: c_int, ab: [] real(64), ipiv: [] c_int, b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgbsv for the type real(64).
    

.. function:: proc gbsv(matrix_order: lapack_memory_order, n: c_int, kl: c_int, ku: c_int, ab: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgbsv for the type complex(64).
    

.. function:: proc gbsv(matrix_order: lapack_memory_order, n: c_int, kl: c_int, ku: c_int, ab: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgbsv for the type complex(128).
    

.. function:: proc gbsvx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(32), afb: [] real(32), ipiv: [] c_int, ref equed: string, r: [] real(32), c: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), ferr: [] real(32), berr: [] real(32), rpivot: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgbsvx for the type real(32).
    

.. function:: proc gbsvx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(64), afb: [] real(64), ipiv: [] c_int, ref equed: string, r: [] real(64), c: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), ferr: [] real(64), berr: [] real(64), rpivot: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgbsvx for the type real(64).
    

.. function:: proc gbsvx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(64), afb: [] complex(64), ipiv: [] c_int, ref equed: string, r: [] real(32), c: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), ferr: [] real(32), berr: [] real(32), rpivot: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgbsvx for the type complex(64).
    

.. function:: proc gbsvx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(128), afb: [] complex(128), ipiv: [] c_int, ref equed: string, r: [] real(64), c: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), ferr: [] real(64), berr: [] real(64), rpivot: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgbsvx for the type complex(128).
    

.. function:: proc gbsvxx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(32), afb: [] real(32), ipiv: [] c_int, ref equed: string, r: [] real(32), c: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), ref rpvgrw: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgbsvxx for the type real(32).
    

.. function:: proc gbsvxx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(64), afb: [] real(64), ipiv: [] c_int, ref equed: string, r: [] real(64), c: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), ref rpvgrw: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgbsvxx for the type real(64).
    

.. function:: proc gbsvxx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(64), afb: [] complex(64), ipiv: [] c_int, ref equed: string, r: [] real(32), c: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), ref rpvgrw: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgbsvxx for the type complex(64).
    

.. function:: proc gbsvxx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(128), afb: [] complex(128), ipiv: [] c_int, ref equed: string, r: [] real(64), c: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), ref rpvgrw: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgbsvxx for the type complex(128).
    

.. function:: proc gbtrf(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] real(32), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sgbtrf for the type real(32).
    

.. function:: proc gbtrf(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] real(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dgbtrf for the type real(64).
    

.. function:: proc gbtrf(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_cgbtrf for the type complex(64).
    

.. function:: proc gbtrf(matrix_order: lapack_memory_order, m: c_int, n: c_int, kl: c_int, ku: c_int, ab: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zgbtrf for the type complex(128).
    

.. function:: proc gbtrs(matrix_order: lapack_memory_order, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(32), ipiv: [] c_int, b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgbtrs for the type real(32).
    

.. function:: proc gbtrs(matrix_order: lapack_memory_order, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] real(64), ipiv: [] c_int, b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgbtrs for the type real(64).
    

.. function:: proc gbtrs(matrix_order: lapack_memory_order, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgbtrs for the type complex(64).
    

.. function:: proc gbtrs(matrix_order: lapack_memory_order, trans: string, n: c_int, kl: c_int, ku: c_int, ab: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgbtrs for the type complex(128).
    

.. function:: proc gebak(matrix_order: lapack_memory_order, job: string, side: string, ilo: c_int, ihi: c_int, scale: [] real(32), v: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgebak for the type real(32).
    

.. function:: proc gebak(matrix_order: lapack_memory_order, job: string, side: string, ilo: c_int, ihi: c_int, scale: [] real(64), v: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgebak for the type real(64).
    

.. function:: proc gebak(matrix_order: lapack_memory_order, job: string, side: string, ilo: c_int, ihi: c_int, scale: [] real(32), v: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgebak for the type complex(64).
    

.. function:: proc gebak(matrix_order: lapack_memory_order, job: string, side: string, ilo: c_int, ihi: c_int, scale: [] real(64), v: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgebak for the type complex(128).
    

.. function:: proc gebal(matrix_order: lapack_memory_order, job: string, a: [] real(32), ref ilo: c_int, ref ihi: c_int, scale: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgebal for the type real(32).
    

.. function:: proc gebal(matrix_order: lapack_memory_order, job: string, a: [] real(64), ref ilo: c_int, ref ihi: c_int, scale: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgebal for the type real(64).
    

.. function:: proc gebal(matrix_order: lapack_memory_order, job: string, a: [] complex(64), ref ilo: c_int, ref ihi: c_int, scale: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgebal for the type complex(64).
    

.. function:: proc gebal(matrix_order: lapack_memory_order, job: string, a: [] complex(128), ref ilo: c_int, ref ihi: c_int, scale: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgebal for the type complex(128).
    

.. function:: proc gebrd(matrix_order: lapack_memory_order, a: [] real(32), d: [] real(32), e: [] real(32), tauq: [] real(32), taup: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgebrd for the type real(32).
    

.. function:: proc gebrd(matrix_order: lapack_memory_order, a: [] real(64), d: [] real(64), e: [] real(64), tauq: [] real(64), taup: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgebrd for the type real(64).
    

.. function:: proc gebrd(matrix_order: lapack_memory_order, a: [] complex(64), d: [] real(32), e: [] real(32), tauq: [] complex(64), taup: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgebrd for the type complex(64).
    

.. function:: proc gebrd(matrix_order: lapack_memory_order, a: [] complex(128), d: [] real(64), e: [] real(64), tauq: [] complex(128), taup: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgebrd for the type complex(128).
    

.. function:: proc gecon(matrix_order: lapack_memory_order, norm: string, a: [] real(32), anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgecon for the type real(32).
    

.. function:: proc gecon(matrix_order: lapack_memory_order, norm: string, a: [] real(64), anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgecon for the type real(64).
    

.. function:: proc gecon(matrix_order: lapack_memory_order, norm: string, a: [] complex(64), anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgecon for the type complex(64).
    

.. function:: proc gecon(matrix_order: lapack_memory_order, norm: string, a: [] complex(128), anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgecon for the type complex(128).
    

.. function:: proc geequ(matrix_order: lapack_memory_order, a: [] real(32), r: [] real(32), c: [] real(32), ref rowcnd: real(32), ref colcnd: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeequ for the type real(32).
    

.. function:: proc geequ(matrix_order: lapack_memory_order, a: [] real(64), r: [] real(64), c: [] real(64), ref rowcnd: real(64), ref colcnd: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeequ for the type real(64).
    

.. function:: proc geequ(matrix_order: lapack_memory_order, a: [] complex(64), r: [] real(32), c: [] real(32), ref rowcnd: real(32), ref colcnd: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgeequ for the type complex(64).
    

.. function:: proc geequ(matrix_order: lapack_memory_order, a: [] complex(128), r: [] real(64), c: [] real(64), ref rowcnd: real(64), ref colcnd: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgeequ for the type complex(128).
    

.. function:: proc geequb(matrix_order: lapack_memory_order, a: [] real(32), r: [] real(32), c: [] real(32), ref rowcnd: real(32), ref colcnd: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeequb for the type real(32).
    

.. function:: proc geequb(matrix_order: lapack_memory_order, a: [] real(64), r: [] real(64), c: [] real(64), ref rowcnd: real(64), ref colcnd: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeequb for the type real(64).
    

.. function:: proc geequb(matrix_order: lapack_memory_order, a: [] complex(64), r: [] real(32), c: [] real(32), ref rowcnd: real(32), ref colcnd: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgeequb for the type complex(64).
    

.. function:: proc geequb(matrix_order: lapack_memory_order, a: [] complex(128), r: [] real(64), c: [] real(64), ref rowcnd: real(64), ref colcnd: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgeequb for the type complex(128).
    

.. function:: proc gees(matrix_order: lapack_memory_order, jobvs: string, sort: string, chlapack_select: LAPACK_S_SELECT2, a: [] real(32), ref sdim: c_int, wr: [] real(32), wi: [] real(32), vs: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgees for the type real(32).
    

.. function:: proc gees(matrix_order: lapack_memory_order, jobvs: string, sort: string, chlapack_select: LAPACK_D_SELECT2, a: [] real(64), ref sdim: c_int, wr: [] real(64), wi: [] real(64), vs: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgees for the type real(64).
    

.. function:: proc gees(matrix_order: lapack_memory_order, jobvs: string, sort: string, chlapack_select: LAPACK_C_SELECT1, a: [] complex(64), ref sdim: c_int, w: [] complex(64), vs: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgees for the type complex(64).
    

.. function:: proc gees(matrix_order: lapack_memory_order, jobvs: string, sort: string, chlapack_select: LAPACK_Z_SELECT1, a: [] complex(128), ref sdim: c_int, w: [] complex(128), vs: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgees for the type complex(128).
    

.. function:: proc geesx(matrix_order: lapack_memory_order, jobvs: string, sort: string, chlapack_select: LAPACK_S_SELECT2, sense: string, a: [] real(32), ref sdim: c_int, wr: [] real(32), wi: [] real(32), vs: [] real(32), ref rconde: real(32), ref rcondv: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeesx for the type real(32).
    

.. function:: proc geesx(matrix_order: lapack_memory_order, jobvs: string, sort: string, chlapack_select: LAPACK_D_SELECT2, sense: string, a: [] real(64), ref sdim: c_int, wr: [] real(64), wi: [] real(64), vs: [] real(64), ref rconde: real(64), ref rcondv: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeesx for the type real(64).
    

.. function:: proc geesx(matrix_order: lapack_memory_order, jobvs: string, sort: string, chlapack_select: LAPACK_C_SELECT1, sense: string, a: [] complex(64), ref sdim: c_int, w: [] complex(64), vs: [] complex(64), ref rconde: real(32), ref rcondv: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgeesx for the type complex(64).
    

.. function:: proc geesx(matrix_order: lapack_memory_order, jobvs: string, sort: string, chlapack_select: LAPACK_Z_SELECT1, sense: string, a: [] complex(128), ref sdim: c_int, w: [] complex(128), vs: [] complex(128), ref rconde: real(64), ref rcondv: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgeesx for the type complex(128).
    

.. function:: proc geev(matrix_order: lapack_memory_order, jobvl: string, jobvr: string, a: [] real(32), wr: [] real(32), wi: [] real(32), vl: [] real(32), vr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeev for the type real(32).
    

.. function:: proc geev(matrix_order: lapack_memory_order, jobvl: string, jobvr: string, a: [] real(64), wr: [] real(64), wi: [] real(64), vl: [] real(64), vr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeev for the type real(64).
    

.. function:: proc geev(matrix_order: lapack_memory_order, jobvl: string, jobvr: string, a: [] complex(64), w: [] complex(64), vl: [] complex(64), vr: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgeev for the type complex(64).
    

.. function:: proc geev(matrix_order: lapack_memory_order, jobvl: string, jobvr: string, a: [] complex(128), w: [] complex(128), vl: [] complex(128), vr: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgeev for the type complex(128).
    

.. function:: proc geevx(matrix_order: lapack_memory_order, balanc: string, jobvl: string, jobvr: string, sense: string, a: [] real(32), wr: [] real(32), wi: [] real(32), vl: [] real(32), vr: [] real(32), ref ilo: c_int, ref ihi: c_int, scale: [] real(32), ref abnrm: real(32), rconde: [] real(32), rcondv: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeevx for the type real(32).
    

.. function:: proc geevx(matrix_order: lapack_memory_order, balanc: string, jobvl: string, jobvr: string, sense: string, a: [] real(64), wr: [] real(64), wi: [] real(64), vl: [] real(64), vr: [] real(64), ref ilo: c_int, ref ihi: c_int, scale: [] real(64), ref abnrm: real(64), rconde: [] real(64), rcondv: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeevx for the type real(64).
    

.. function:: proc geevx(matrix_order: lapack_memory_order, balanc: string, jobvl: string, jobvr: string, sense: string, a: [] complex(64), w: [] complex(64), vl: [] complex(64), vr: [] complex(64), ref ilo: c_int, ref ihi: c_int, scale: [] real(32), ref abnrm: real(32), rconde: [] real(32), rcondv: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgeevx for the type complex(64).
    

.. function:: proc geevx(matrix_order: lapack_memory_order, balanc: string, jobvl: string, jobvr: string, sense: string, a: [] complex(128), w: [] complex(128), vl: [] complex(128), vr: [] complex(128), ref ilo: c_int, ref ihi: c_int, scale: [] real(64), ref abnrm: real(64), rconde: [] real(64), rcondv: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgeevx for the type complex(128).
    

.. function:: proc gehrd(matrix_order: lapack_memory_order, ilo: c_int, ihi: c_int, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgehrd for the type real(32).
    

.. function:: proc gehrd(matrix_order: lapack_memory_order, ilo: c_int, ihi: c_int, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgehrd for the type real(64).
    

.. function:: proc gehrd(matrix_order: lapack_memory_order, ilo: c_int, ihi: c_int, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgehrd for the type complex(64).
    

.. function:: proc gehrd(matrix_order: lapack_memory_order, ilo: c_int, ihi: c_int, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgehrd for the type complex(128).
    

.. function:: proc gejsv(matrix_order: lapack_memory_order, joba: string, jobu: string, jobv: string, jobr: string, jobt: string, jobp: string, a: [] real(32), sva: [] real(32), u: [] real(32), v: [] real(32), stat: [] real(32), istat: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sgejsv for the type real(32).
    

.. function:: proc gejsv(matrix_order: lapack_memory_order, joba: string, jobu: string, jobv: string, jobr: string, jobt: string, jobp: string, a: [] real(64), sva: [] real(64), u: [] real(64), v: [] real(64), stat: [] real(64), istat: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dgejsv for the type real(64).
    

.. function:: proc gelq2(matrix_order: lapack_memory_order, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgelq2 for the type real(32).
    

.. function:: proc gelq2(matrix_order: lapack_memory_order, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgelq2 for the type real(64).
    

.. function:: proc gelq2(matrix_order: lapack_memory_order, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgelq2 for the type complex(64).
    

.. function:: proc gelq2(matrix_order: lapack_memory_order, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgelq2 for the type complex(128).
    

.. function:: proc gelqf(matrix_order: lapack_memory_order, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgelqf for the type real(32).
    

.. function:: proc gelqf(matrix_order: lapack_memory_order, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgelqf for the type real(64).
    

.. function:: proc gelqf(matrix_order: lapack_memory_order, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgelqf for the type complex(64).
    

.. function:: proc gelqf(matrix_order: lapack_memory_order, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgelqf for the type complex(128).
    

.. function:: proc gels(matrix_order: lapack_memory_order, trans: string, a: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgels for the type real(32).
    

.. function:: proc gels(matrix_order: lapack_memory_order, trans: string, a: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgels for the type real(64).
    

.. function:: proc gels(matrix_order: lapack_memory_order, trans: string, a: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgels for the type complex(64).
    

.. function:: proc gels(matrix_order: lapack_memory_order, trans: string, a: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgels for the type complex(128).
    

.. function:: proc gelsd(matrix_order: lapack_memory_order, a: [] real(32), b: [] real(32), s: [] real(32), rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgelsd for the type real(32).
    

.. function:: proc gelsd(matrix_order: lapack_memory_order, a: [] real(64), b: [] real(64), s: [] real(64), rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgelsd for the type real(64).
    

.. function:: proc gelsd(matrix_order: lapack_memory_order, a: [] complex(64), b: [] complex(64), s: [] real(32), rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgelsd for the type complex(64).
    

.. function:: proc gelsd(matrix_order: lapack_memory_order, a: [] complex(128), b: [] complex(128), s: [] real(64), rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgelsd for the type complex(128).
    

.. function:: proc gelss(matrix_order: lapack_memory_order, a: [] real(32), b: [] real(32), s: [] real(32), rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgelss for the type real(32).
    

.. function:: proc gelss(matrix_order: lapack_memory_order, a: [] real(64), b: [] real(64), s: [] real(64), rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgelss for the type real(64).
    

.. function:: proc gelss(matrix_order: lapack_memory_order, a: [] complex(64), b: [] complex(64), s: [] real(32), rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgelss for the type complex(64).
    

.. function:: proc gelss(matrix_order: lapack_memory_order, a: [] complex(128), b: [] complex(128), s: [] real(64), rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgelss for the type complex(128).
    

.. function:: proc gelsy(matrix_order: lapack_memory_order, a: [] real(32), b: [] real(32), jpvt: [] c_int, rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgelsy for the type real(32).
    

.. function:: proc gelsy(matrix_order: lapack_memory_order, a: [] real(64), b: [] real(64), jpvt: [] c_int, rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgelsy for the type real(64).
    

.. function:: proc gelsy(matrix_order: lapack_memory_order, a: [] complex(64), b: [] complex(64), jpvt: [] c_int, rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgelsy for the type complex(64).
    

.. function:: proc gelsy(matrix_order: lapack_memory_order, a: [] complex(128), b: [] complex(128), jpvt: [] c_int, rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgelsy for the type complex(128).
    

.. function:: proc geqlf(matrix_order: lapack_memory_order, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeqlf for the type real(32).
    

.. function:: proc geqlf(matrix_order: lapack_memory_order, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeqlf for the type real(64).
    

.. function:: proc geqlf(matrix_order: lapack_memory_order, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgeqlf for the type complex(64).
    

.. function:: proc geqlf(matrix_order: lapack_memory_order, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgeqlf for the type complex(128).
    

.. function:: proc geqp3(matrix_order: lapack_memory_order, a: [] real(32), jpvt: [] c_int, tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeqp3 for the type real(32).
    

.. function:: proc geqp3(matrix_order: lapack_memory_order, a: [] real(64), jpvt: [] c_int, tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeqp3 for the type real(64).
    

.. function:: proc geqp3(matrix_order: lapack_memory_order, a: [] complex(64), jpvt: [] c_int, tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgeqp3 for the type complex(64).
    

.. function:: proc geqp3(matrix_order: lapack_memory_order, a: [] complex(128), jpvt: [] c_int, tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgeqp3 for the type complex(128).
    

.. function:: proc geqpf(matrix_order: lapack_memory_order, a: [] real(32), jpvt: [] c_int, tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeqpf for the type real(32).
    

.. function:: proc geqpf(matrix_order: lapack_memory_order, a: [] real(64), jpvt: [] c_int, tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeqpf for the type real(64).
    

.. function:: proc geqpf(matrix_order: lapack_memory_order, a: [] complex(64), jpvt: [] c_int, tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgeqpf for the type complex(64).
    

.. function:: proc geqpf(matrix_order: lapack_memory_order, a: [] complex(128), jpvt: [] c_int, tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgeqpf for the type complex(128).
    

.. function:: proc geqr2(matrix_order: lapack_memory_order, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeqr2 for the type real(32).
    

.. function:: proc geqr2(matrix_order: lapack_memory_order, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeqr2 for the type real(64).
    

.. function:: proc geqr2(matrix_order: lapack_memory_order, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgeqr2 for the type complex(64).
    

.. function:: proc geqr2(matrix_order: lapack_memory_order, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgeqr2 for the type complex(128).
    

.. function:: proc geqrf(matrix_order: lapack_memory_order, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeqrf for the type real(32).
    

.. function:: proc geqrf(matrix_order: lapack_memory_order, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeqrf for the type real(64).
    

.. function:: proc geqrf(matrix_order: lapack_memory_order, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgeqrf for the type complex(64).
    

.. function:: proc geqrf(matrix_order: lapack_memory_order, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgeqrf for the type complex(128).
    

.. function:: proc geqrfp(matrix_order: lapack_memory_order, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeqrfp for the type real(32).
    

.. function:: proc geqrfp(matrix_order: lapack_memory_order, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeqrfp for the type real(64).
    

.. function:: proc geqrfp(matrix_order: lapack_memory_order, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgeqrfp for the type complex(64).
    

.. function:: proc geqrfp(matrix_order: lapack_memory_order, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgeqrfp for the type complex(128).
    

.. function:: proc gerfs(matrix_order: lapack_memory_order, trans: string, a: [] real(32), af: [] real(32), ipiv: [] c_int, b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgerfs for the type real(32).
    

.. function:: proc gerfs(matrix_order: lapack_memory_order, trans: string, a: [] real(64), af: [] real(64), ipiv: [] c_int, b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgerfs for the type real(64).
    

.. function:: proc gerfs(matrix_order: lapack_memory_order, trans: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgerfs for the type complex(64).
    

.. function:: proc gerfs(matrix_order: lapack_memory_order, trans: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgerfs for the type complex(128).
    

.. function:: proc gerfsx(matrix_order: lapack_memory_order, trans: string, equed: string, a: [] real(32), af: [] real(32), ipiv: [] c_int, r: [] real(32), c: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgerfsx for the type real(32).
    

.. function:: proc gerfsx(matrix_order: lapack_memory_order, trans: string, equed: string, a: [] real(64), af: [] real(64), ipiv: [] c_int, r: [] real(64), c: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgerfsx for the type real(64).
    

.. function:: proc gerfsx(matrix_order: lapack_memory_order, trans: string, equed: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, r: [] real(32), c: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgerfsx for the type complex(64).
    

.. function:: proc gerfsx(matrix_order: lapack_memory_order, trans: string, equed: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, r: [] real(64), c: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgerfsx for the type complex(128).
    

.. function:: proc gerqf(matrix_order: lapack_memory_order, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgerqf for the type real(32).
    

.. function:: proc gerqf(matrix_order: lapack_memory_order, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgerqf for the type real(64).
    

.. function:: proc gerqf(matrix_order: lapack_memory_order, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgerqf for the type complex(64).
    

.. function:: proc gerqf(matrix_order: lapack_memory_order, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgerqf for the type complex(128).
    

.. function:: proc gesdd(matrix_order: lapack_memory_order, jobz: string, a: [] real(32), s: [] real(32), u: [] real(32), vt: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgesdd for the type real(32).
    

.. function:: proc gesdd(matrix_order: lapack_memory_order, jobz: string, a: [] real(64), s: [] real(64), u: [] real(64), vt: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgesdd for the type real(64).
    

.. function:: proc gesdd(matrix_order: lapack_memory_order, jobz: string, a: [] complex(64), s: [] real(32), u: [] complex(64), vt: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgesdd for the type complex(64).
    

.. function:: proc gesdd(matrix_order: lapack_memory_order, jobz: string, a: [] complex(128), s: [] real(64), u: [] complex(128), vt: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgesdd for the type complex(128).
    

.. function:: proc gesv(matrix_order: lapack_memory_order, a: [] real(32), ipiv: [] c_int, b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgesv for the type real(32).
    

.. function:: proc gesv(matrix_order: lapack_memory_order, a: [] real(64), ipiv: [] c_int, b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgesv for the type real(64).
    

.. function:: proc gesv(matrix_order: lapack_memory_order, a: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgesv for the type complex(64).
    

.. function:: proc gesv(matrix_order: lapack_memory_order, a: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgesv for the type complex(128).
    

.. function:: proc gesv(matrix_order: lapack_memory_order, a: [] real(64), ipiv: [] c_int, b: [] real(64), x: [] real(64), ref chlapack_iter: c_int): c_int

   
   Wrapped procedure of LAPACKE_dsgesv for the type real(64).
    

.. function:: proc gesv(matrix_order: lapack_memory_order, a: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ref chlapack_iter: c_int): c_int

   
   Wrapped procedure of LAPACKE_zcgesv for the type complex(128).
    

.. function:: proc gesvd(matrix_order: lapack_memory_order, jobu: string, jobvt: string, a: [] real(32), s: [] real(32), u: [] real(32), vt: [] real(32), superb: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgesvd for the type real(32).
    

.. function:: proc gesvd(matrix_order: lapack_memory_order, jobu: string, jobvt: string, a: [] real(64), s: [] real(64), u: [] real(64), vt: [] real(64), superb: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgesvd for the type real(64).
    

.. function:: proc gesvd(matrix_order: lapack_memory_order, jobu: string, jobvt: string, a: [] complex(64), s: [] real(32), u: [] complex(64), vt: [] complex(64), superb: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgesvd for the type complex(64).
    

.. function:: proc gesvd(matrix_order: lapack_memory_order, jobu: string, jobvt: string, a: [] complex(128), s: [] real(64), u: [] complex(128), vt: [] complex(128), superb: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgesvd for the type complex(128).
    

.. function:: proc gesvj(matrix_order: lapack_memory_order, joba: string, jobu: string, jobv: string, a: [] real(32), sva: [] real(32), mv: c_int, v: [] real(32), stat: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgesvj for the type real(32).
    

.. function:: proc gesvj(matrix_order: lapack_memory_order, joba: string, jobu: string, jobv: string, a: [] real(64), sva: [] real(64), mv: c_int, v: [] real(64), stat: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgesvj for the type real(64).
    

.. function:: proc gesvx(matrix_order: lapack_memory_order, fact: string, trans: string, a: [] real(32), af: [] real(32), ipiv: [] c_int, ref equed: string, r: [] real(32), c: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), ferr: [] real(32), berr: [] real(32), rpivot: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgesvx for the type real(32).
    

.. function:: proc gesvx(matrix_order: lapack_memory_order, fact: string, trans: string, a: [] real(64), af: [] real(64), ipiv: [] c_int, ref equed: string, r: [] real(64), c: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), ferr: [] real(64), berr: [] real(64), rpivot: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgesvx for the type real(64).
    

.. function:: proc gesvx(matrix_order: lapack_memory_order, fact: string, trans: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, ref equed: string, r: [] real(32), c: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), ferr: [] real(32), berr: [] real(32), rpivot: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgesvx for the type complex(64).
    

.. function:: proc gesvx(matrix_order: lapack_memory_order, fact: string, trans: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, ref equed: string, r: [] real(64), c: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), ferr: [] real(64), berr: [] real(64), rpivot: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgesvx for the type complex(128).
    

.. function:: proc gesvxx(matrix_order: lapack_memory_order, fact: string, trans: string, a: [] real(32), af: [] real(32), ipiv: [] c_int, ref equed: string, r: [] real(32), c: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), ref rpvgrw: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgesvxx for the type real(32).
    

.. function:: proc gesvxx(matrix_order: lapack_memory_order, fact: string, trans: string, a: [] real(64), af: [] real(64), ipiv: [] c_int, ref equed: string, r: [] real(64), c: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), ref rpvgrw: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgesvxx for the type real(64).
    

.. function:: proc gesvxx(matrix_order: lapack_memory_order, fact: string, trans: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, ref equed: string, r: [] real(32), c: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), ref rpvgrw: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgesvxx for the type complex(64).
    

.. function:: proc gesvxx(matrix_order: lapack_memory_order, fact: string, trans: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, ref equed: string, r: [] real(64), c: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), ref rpvgrw: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgesvxx for the type complex(128).
    

.. function:: proc getf2(matrix_order: lapack_memory_order, a: [] real(32), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sgetf2 for the type real(32).
    

.. function:: proc getf2(matrix_order: lapack_memory_order, a: [] real(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dgetf2 for the type real(64).
    

.. function:: proc getf2(matrix_order: lapack_memory_order, a: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_cgetf2 for the type complex(64).
    

.. function:: proc getf2(matrix_order: lapack_memory_order, a: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zgetf2 for the type complex(128).
    

.. function:: proc getrf(matrix_order: lapack_memory_order, a: [] real(32), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sgetrf for the type real(32).
    

.. function:: proc getrf(matrix_order: lapack_memory_order, a: [] real(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dgetrf for the type real(64).
    

.. function:: proc getrf(matrix_order: lapack_memory_order, a: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_cgetrf for the type complex(64).
    

.. function:: proc getrf(matrix_order: lapack_memory_order, a: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zgetrf for the type complex(128).
    

.. function:: proc getri(matrix_order: lapack_memory_order, a: [] real(32), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sgetri for the type real(32).
    

.. function:: proc getri(matrix_order: lapack_memory_order, a: [] real(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dgetri for the type real(64).
    

.. function:: proc getri(matrix_order: lapack_memory_order, a: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_cgetri for the type complex(64).
    

.. function:: proc getri(matrix_order: lapack_memory_order, a: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zgetri for the type complex(128).
    

.. function:: proc getrs(matrix_order: lapack_memory_order, trans: string, a: [] real(32), ipiv: [] c_int, b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgetrs for the type real(32).
    

.. function:: proc getrs(matrix_order: lapack_memory_order, trans: string, a: [] real(64), ipiv: [] c_int, b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgetrs for the type real(64).
    

.. function:: proc getrs(matrix_order: lapack_memory_order, trans: string, a: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgetrs for the type complex(64).
    

.. function:: proc getrs(matrix_order: lapack_memory_order, trans: string, a: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgetrs for the type complex(128).
    

.. function:: proc ggbak(matrix_order: lapack_memory_order, job: string, side: string, ilo: c_int, ihi: c_int, lscale: [] real(32), rscale: [] real(32), v: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sggbak for the type real(32).
    

.. function:: proc ggbak(matrix_order: lapack_memory_order, job: string, side: string, ilo: c_int, ihi: c_int, lscale: [] real(64), rscale: [] real(64), v: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dggbak for the type real(64).
    

.. function:: proc ggbak(matrix_order: lapack_memory_order, job: string, side: string, ilo: c_int, ihi: c_int, lscale: [] real(32), rscale: [] real(32), v: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cggbak for the type complex(64).
    

.. function:: proc ggbak(matrix_order: lapack_memory_order, job: string, side: string, ilo: c_int, ihi: c_int, lscale: [] real(64), rscale: [] real(64), v: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zggbak for the type complex(128).
    

.. function:: proc ggbal(matrix_order: lapack_memory_order, job: string, a: [] real(32), b: [] real(32), ref ilo: c_int, ref ihi: c_int, lscale: [] real(32), rscale: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sggbal for the type real(32).
    

.. function:: proc ggbal(matrix_order: lapack_memory_order, job: string, a: [] real(64), b: [] real(64), ref ilo: c_int, ref ihi: c_int, lscale: [] real(64), rscale: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dggbal for the type real(64).
    

.. function:: proc ggbal(matrix_order: lapack_memory_order, job: string, a: [] complex(64), b: [] complex(64), ref ilo: c_int, ref ihi: c_int, lscale: [] real(32), rscale: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cggbal for the type complex(64).
    

.. function:: proc ggbal(matrix_order: lapack_memory_order, job: string, a: [] complex(128), b: [] complex(128), ref ilo: c_int, ref ihi: c_int, lscale: [] real(64), rscale: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zggbal for the type complex(128).
    

.. function:: proc gges(matrix_order: lapack_memory_order, jobvsl: string, jobvsr: string, sort: string, selctg: LAPACK_S_SELECT3, a: [] real(32), b: [] real(32), ref sdim: c_int, alphar: [] real(32), alphai: [] real(32), beta: [] real(32), vsl: [] real(32), vsr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgges for the type real(32).
    

.. function:: proc gges(matrix_order: lapack_memory_order, jobvsl: string, jobvsr: string, sort: string, selctg: LAPACK_D_SELECT3, a: [] real(64), b: [] real(64), ref sdim: c_int, alphar: [] real(64), alphai: [] real(64), beta: [] real(64), vsl: [] real(64), vsr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgges for the type real(64).
    

.. function:: proc gges(matrix_order: lapack_memory_order, jobvsl: string, jobvsr: string, sort: string, selctg: LAPACK_C_SELECT2, a: [] complex(64), b: [] complex(64), ref sdim: c_int, alpha: [] complex(64), beta: [] complex(64), vsl: [] complex(64), vsr: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgges for the type complex(64).
    

.. function:: proc gges(matrix_order: lapack_memory_order, jobvsl: string, jobvsr: string, sort: string, selctg: LAPACK_Z_SELECT2, a: [] complex(128), b: [] complex(128), ref sdim: c_int, alpha: [] complex(128), beta: [] complex(128), vsl: [] complex(128), vsr: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgges for the type complex(128).
    

.. function:: proc ggesx(matrix_order: lapack_memory_order, jobvsl: string, jobvsr: string, sort: string, selctg: LAPACK_S_SELECT3, sense: string, a: [] real(32), b: [] real(32), ref sdim: c_int, alphar: [] real(32), alphai: [] real(32), beta: [] real(32), vsl: [] real(32), vsr: [] real(32), rconde: [] real(32), rcondv: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sggesx for the type real(32).
    

.. function:: proc ggesx(matrix_order: lapack_memory_order, jobvsl: string, jobvsr: string, sort: string, selctg: LAPACK_D_SELECT3, sense: string, a: [] real(64), b: [] real(64), ref sdim: c_int, alphar: [] real(64), alphai: [] real(64), beta: [] real(64), vsl: [] real(64), vsr: [] real(64), rconde: [] real(64), rcondv: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dggesx for the type real(64).
    

.. function:: proc ggesx(matrix_order: lapack_memory_order, jobvsl: string, jobvsr: string, sort: string, selctg: LAPACK_C_SELECT2, sense: string, a: [] complex(64), b: [] complex(64), ref sdim: c_int, alpha: [] complex(64), beta: [] complex(64), vsl: [] complex(64), vsr: [] complex(64), rconde: [] real(32), rcondv: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cggesx for the type complex(64).
    

.. function:: proc ggesx(matrix_order: lapack_memory_order, jobvsl: string, jobvsr: string, sort: string, selctg: LAPACK_Z_SELECT2, sense: string, a: [] complex(128), b: [] complex(128), ref sdim: c_int, alpha: [] complex(128), beta: [] complex(128), vsl: [] complex(128), vsr: [] complex(128), rconde: [] real(64), rcondv: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zggesx for the type complex(128).
    

.. function:: proc ggev(matrix_order: lapack_memory_order, jobvl: string, jobvr: string, a: [] real(32), b: [] real(32), alphar: [] real(32), alphai: [] real(32), beta: [] real(32), vl: [] real(32), vr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sggev for the type real(32).
    

.. function:: proc ggev(matrix_order: lapack_memory_order, jobvl: string, jobvr: string, a: [] real(64), b: [] real(64), alphar: [] real(64), alphai: [] real(64), beta: [] real(64), vl: [] real(64), vr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dggev for the type real(64).
    

.. function:: proc ggev(matrix_order: lapack_memory_order, jobvl: string, jobvr: string, a: [] complex(64), b: [] complex(64), alpha: [] complex(64), beta: [] complex(64), vl: [] complex(64), vr: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cggev for the type complex(64).
    

.. function:: proc ggev(matrix_order: lapack_memory_order, jobvl: string, jobvr: string, a: [] complex(128), b: [] complex(128), alpha: [] complex(128), beta: [] complex(128), vl: [] complex(128), vr: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zggev for the type complex(128).
    

.. function:: proc ggevx(matrix_order: lapack_memory_order, balanc: string, jobvl: string, jobvr: string, sense: string, a: [] real(32), b: [] real(32), alphar: [] real(32), alphai: [] real(32), beta: [] real(32), vl: [] real(32), vr: [] real(32), ref ilo: c_int, ref ihi: c_int, lscale: [] real(32), rscale: [] real(32), ref abnrm: real(32), ref bbnrm: real(32), rconde: [] real(32), rcondv: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sggevx for the type real(32).
    

.. function:: proc ggevx(matrix_order: lapack_memory_order, balanc: string, jobvl: string, jobvr: string, sense: string, a: [] real(64), b: [] real(64), alphar: [] real(64), alphai: [] real(64), beta: [] real(64), vl: [] real(64), vr: [] real(64), ref ilo: c_int, ref ihi: c_int, lscale: [] real(64), rscale: [] real(64), ref abnrm: real(64), ref bbnrm: real(64), rconde: [] real(64), rcondv: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dggevx for the type real(64).
    

.. function:: proc ggevx(matrix_order: lapack_memory_order, balanc: string, jobvl: string, jobvr: string, sense: string, a: [] complex(64), b: [] complex(64), alpha: [] complex(64), beta: [] complex(64), vl: [] complex(64), vr: [] complex(64), ref ilo: c_int, ref ihi: c_int, lscale: [] real(32), rscale: [] real(32), ref abnrm: real(32), ref bbnrm: real(32), rconde: [] real(32), rcondv: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cggevx for the type complex(64).
    

.. function:: proc ggevx(matrix_order: lapack_memory_order, balanc: string, jobvl: string, jobvr: string, sense: string, a: [] complex(128), b: [] complex(128), alpha: [] complex(128), beta: [] complex(128), vl: [] complex(128), vr: [] complex(128), ref ilo: c_int, ref ihi: c_int, lscale: [] real(64), rscale: [] real(64), ref abnrm: real(64), ref bbnrm: real(64), rconde: [] real(64), rcondv: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zggevx for the type complex(128).
    

.. function:: proc ggglm(matrix_order: lapack_memory_order, a: [] real(32), b: [] real(32), d: [] real(32), x: [] real(32), y: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sggglm for the type real(32).
    

.. function:: proc ggglm(matrix_order: lapack_memory_order, a: [] real(64), b: [] real(64), d: [] real(64), x: [] real(64), y: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dggglm for the type real(64).
    

.. function:: proc ggglm(matrix_order: lapack_memory_order, a: [] complex(64), b: [] complex(64), d: [] complex(64), x: [] complex(64), y: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cggglm for the type complex(64).
    

.. function:: proc ggglm(matrix_order: lapack_memory_order, a: [] complex(128), b: [] complex(128), d: [] complex(128), x: [] complex(128), y: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zggglm for the type complex(128).
    

.. function:: proc gghrd(matrix_order: lapack_memory_order, compq: string, compz: string, ilo: c_int, ihi: c_int, a: [] real(32), b: [] real(32), q: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgghrd for the type real(32).
    

.. function:: proc gghrd(matrix_order: lapack_memory_order, compq: string, compz: string, ilo: c_int, ihi: c_int, a: [] real(64), b: [] real(64), q: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgghrd for the type real(64).
    

.. function:: proc gghrd(matrix_order: lapack_memory_order, compq: string, compz: string, ilo: c_int, ihi: c_int, a: [] complex(64), b: [] complex(64), q: [] complex(64), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgghrd for the type complex(64).
    

.. function:: proc gghrd(matrix_order: lapack_memory_order, compq: string, compz: string, ilo: c_int, ihi: c_int, a: [] complex(128), b: [] complex(128), q: [] complex(128), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgghrd for the type complex(128).
    

.. function:: proc gglse(matrix_order: lapack_memory_order, a: [] real(32), b: [] real(32), c: [] real(32), d: [] real(32), x: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgglse for the type real(32).
    

.. function:: proc gglse(matrix_order: lapack_memory_order, a: [] real(64), b: [] real(64), c: [] real(64), d: [] real(64), x: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgglse for the type real(64).
    

.. function:: proc gglse(matrix_order: lapack_memory_order, a: [] complex(64), b: [] complex(64), c: [] complex(64), d: [] complex(64), x: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgglse for the type complex(64).
    

.. function:: proc gglse(matrix_order: lapack_memory_order, a: [] complex(128), b: [] complex(128), c: [] complex(128), d: [] complex(128), x: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgglse for the type complex(128).
    

.. function:: proc ggqrf(matrix_order: lapack_memory_order, a: [] real(32), taua: [] real(32), b: [] real(32), taub: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sggqrf for the type real(32).
    

.. function:: proc ggqrf(matrix_order: lapack_memory_order, a: [] real(64), taua: [] real(64), b: [] real(64), taub: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dggqrf for the type real(64).
    

.. function:: proc ggqrf(matrix_order: lapack_memory_order, a: [] complex(64), taua: [] complex(64), b: [] complex(64), taub: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cggqrf for the type complex(64).
    

.. function:: proc ggqrf(matrix_order: lapack_memory_order, a: [] complex(128), taua: [] complex(128), b: [] complex(128), taub: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zggqrf for the type complex(128).
    

.. function:: proc ggrqf(matrix_order: lapack_memory_order, a: [] real(32), taua: [] real(32), b: [] real(32), taub: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sggrqf for the type real(32).
    

.. function:: proc ggrqf(matrix_order: lapack_memory_order, a: [] real(64), taua: [] real(64), b: [] real(64), taub: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dggrqf for the type real(64).
    

.. function:: proc ggrqf(matrix_order: lapack_memory_order, a: [] complex(64), taua: [] complex(64), b: [] complex(64), taub: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cggrqf for the type complex(64).
    

.. function:: proc ggrqf(matrix_order: lapack_memory_order, a: [] complex(128), taua: [] complex(128), b: [] complex(128), taub: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zggrqf for the type complex(128).
    

.. function:: proc ggsvd(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, ref k: c_int, ref l: c_int, a: [] real(32), b: [] real(32), alpha: [] real(32), beta: [] real(32), u: [] real(32), v: [] real(32), q: [] real(32), iwork: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sggsvd for the type real(32).
    

.. function:: proc ggsvd(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, ref k: c_int, ref l: c_int, a: [] real(64), b: [] real(64), alpha: [] real(64), beta: [] real(64), u: [] real(64), v: [] real(64), q: [] real(64), iwork: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dggsvd for the type real(64).
    

.. function:: proc ggsvd(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, ref k: c_int, ref l: c_int, a: [] complex(64), b: [] complex(64), alpha: [] real(32), beta: [] real(32), u: [] complex(64), v: [] complex(64), q: [] complex(64), iwork: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_cggsvd for the type complex(64).
    

.. function:: proc ggsvd(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, ref k: c_int, ref l: c_int, a: [] complex(128), b: [] complex(128), alpha: [] real(64), beta: [] real(64), u: [] complex(128), v: [] complex(128), q: [] complex(128), iwork: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zggsvd for the type complex(128).
    

.. function:: proc ggsvp(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, a: [] real(32), b: [] real(32), tola: real(32), tolb: real(32), ref k: c_int, ref l: c_int, u: [] real(32), v: [] real(32), q: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sggsvp for the type real(32).
    

.. function:: proc ggsvp(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, a: [] real(64), b: [] real(64), tola: real(64), tolb: real(64), ref k: c_int, ref l: c_int, u: [] real(64), v: [] real(64), q: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dggsvp for the type real(64).
    

.. function:: proc ggsvp(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, a: [] complex(64), b: [] complex(64), tola: real(32), tolb: real(32), ref k: c_int, ref l: c_int, u: [] complex(64), v: [] complex(64), q: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cggsvp for the type complex(64).
    

.. function:: proc ggsvp(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, a: [] complex(128), b: [] complex(128), tola: real(64), tolb: real(64), ref k: c_int, ref l: c_int, u: [] complex(128), v: [] complex(128), q: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zggsvp for the type complex(128).
    

.. function:: proc gtcon(norm: string, n: c_int, dl: [] real(32), d: [] real(32), du: [] real(32), du2: [] real(32), ipiv: [] c_int, anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgtcon for the type real(32).
    

.. function:: proc gtcon(norm: string, n: c_int, dl: [] real(64), d: [] real(64), du: [] real(64), du2: [] real(64), ipiv: [] c_int, anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgtcon for the type real(64).
    

.. function:: proc gtcon(norm: string, n: c_int, dl: [] complex(64), d: [] complex(64), du: [] complex(64), du2: [] complex(64), ipiv: [] c_int, anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgtcon for the type complex(64).
    

.. function:: proc gtcon(norm: string, n: c_int, dl: [] complex(128), d: [] complex(128), du: [] complex(128), du2: [] complex(128), ipiv: [] c_int, anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgtcon for the type complex(128).
    

.. function:: proc gtrfs(matrix_order: lapack_memory_order, trans: string, n: c_int, dl: [] real(32), d: [] real(32), du: [] real(32), dlf: [] real(32), df: [] real(32), duf: [] real(32), du2: [] real(32), ipiv: [] c_int, b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgtrfs for the type real(32).
    

.. function:: proc gtrfs(matrix_order: lapack_memory_order, trans: string, n: c_int, dl: [] real(64), d: [] real(64), du: [] real(64), dlf: [] real(64), df: [] real(64), duf: [] real(64), du2: [] real(64), ipiv: [] c_int, b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgtrfs for the type real(64).
    

.. function:: proc gtrfs(matrix_order: lapack_memory_order, trans: string, n: c_int, dl: [] complex(64), d: [] complex(64), du: [] complex(64), dlf: [] complex(64), df: [] complex(64), duf: [] complex(64), du2: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgtrfs for the type complex(64).
    

.. function:: proc gtrfs(matrix_order: lapack_memory_order, trans: string, n: c_int, dl: [] complex(128), d: [] complex(128), du: [] complex(128), dlf: [] complex(128), df: [] complex(128), duf: [] complex(128), du2: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgtrfs for the type complex(128).
    

.. function:: proc gtsv(matrix_order: lapack_memory_order, dl: [] real(32), d: [] real(32), du: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgtsv for the type real(32).
    

.. function:: proc gtsv(matrix_order: lapack_memory_order, dl: [] real(64), d: [] real(64), du: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgtsv for the type real(64).
    

.. function:: proc gtsv(matrix_order: lapack_memory_order, n: c_int, dl: [] complex(64), d: [] complex(64), du: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgtsv for the type complex(64).
    

.. function:: proc gtsv(matrix_order: lapack_memory_order, n: c_int, dl: [] complex(128), d: [] complex(128), du: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgtsv for the type complex(128).
    

.. function:: proc gtsvx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, dl: [] real(32), d: [] real(32), du: [] real(32), dlf: [] real(32), df: [] real(32), duf: [] real(32), du2: [] real(32), ipiv: [] c_int, b: [] real(32), x: [] real(32), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgtsvx for the type real(32).
    

.. function:: proc gtsvx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, dl: [] real(64), d: [] real(64), du: [] real(64), dlf: [] real(64), df: [] real(64), duf: [] real(64), du2: [] real(64), ipiv: [] c_int, b: [] real(64), x: [] real(64), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgtsvx for the type real(64).
    

.. function:: proc gtsvx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, dl: [] complex(64), d: [] complex(64), du: [] complex(64), dlf: [] complex(64), df: [] complex(64), duf: [] complex(64), du2: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cgtsvx for the type complex(64).
    

.. function:: proc gtsvx(matrix_order: lapack_memory_order, fact: string, trans: string, n: c_int, dl: [] complex(128), d: [] complex(128), du: [] complex(128), dlf: [] complex(128), df: [] complex(128), duf: [] complex(128), du2: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zgtsvx for the type complex(128).
    

.. function:: proc gttrf(n: c_int, dl: [] real(32), d: [] real(32), du: [] real(32), du2: [] real(32), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sgttrf for the type real(32).
    

.. function:: proc gttrf(n: c_int, dl: [] real(64), d: [] real(64), du: [] real(64), du2: [] real(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dgttrf for the type real(64).
    

.. function:: proc gttrf(n: c_int, dl: [] complex(64), d: [] complex(64), du: [] complex(64), du2: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_cgttrf for the type complex(64).
    

.. function:: proc gttrf(n: c_int, dl: [] complex(128), d: [] complex(128), du: [] complex(128), du2: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zgttrf for the type complex(128).
    

.. function:: proc gttrs(matrix_order: lapack_memory_order, trans: string, n: c_int, dl: [] real(32), d: [] real(32), du: [] real(32), du2: [] real(32), ipiv: [] c_int, b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgttrs for the type real(32).
    

.. function:: proc gttrs(matrix_order: lapack_memory_order, trans: string, n: c_int, dl: [] real(64), d: [] real(64), du: [] real(64), du2: [] real(64), ipiv: [] c_int, b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgttrs for the type real(64).
    

.. function:: proc gttrs(matrix_order: lapack_memory_order, trans: string, n: c_int, dl: [] complex(64), d: [] complex(64), du: [] complex(64), du2: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgttrs for the type complex(64).
    

.. function:: proc gttrs(matrix_order: lapack_memory_order, trans: string, n: c_int, dl: [] complex(128), d: [] complex(128), du: [] complex(128), du2: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgttrs for the type complex(128).
    

.. function:: proc hbev(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, kd: c_int, ab: [] complex(64), w: [] real(32), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chbev for the type complex(64).
    

.. function:: proc hbev(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, kd: c_int, ab: [] complex(128), w: [] real(64), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhbev for the type complex(128).
    

.. function:: proc hbevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, kd: c_int, ab: [] complex(64), w: [] real(32), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chbevd for the type complex(64).
    

.. function:: proc hbevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, kd: c_int, ab: [] complex(128), w: [] real(64), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhbevd for the type complex(128).
    

.. function:: proc hbevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, n: c_int, kd: c_int, ab: [] complex(64), q: [] complex(64), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] complex(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_chbevx for the type complex(64).
    

.. function:: proc hbevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, n: c_int, kd: c_int, ab: [] complex(128), q: [] complex(128), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] complex(128), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zhbevx for the type complex(128).
    

.. function:: proc hbgst(matrix_order: lapack_memory_order, vect: string, uplo: string, ka: c_int, kb: c_int, ab: [] complex(64), bb: [] complex(64), x: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chbgst for the type complex(64).
    

.. function:: proc hbgst(matrix_order: lapack_memory_order, vect: string, uplo: string, ka: c_int, kb: c_int, ab: [] complex(128), bb: [] complex(128), x: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhbgst for the type complex(128).
    

.. function:: proc hbgv(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ka: c_int, kb: c_int, ab: [] complex(64), bb: [] complex(64), w: [] real(32), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chbgv for the type complex(64).
    

.. function:: proc hbgv(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ka: c_int, kb: c_int, ab: [] complex(128), bb: [] complex(128), w: [] real(64), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhbgv for the type complex(128).
    

.. function:: proc hbgvd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ka: c_int, kb: c_int, ab: [] complex(64), bb: [] complex(64), w: [] real(32), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chbgvd for the type complex(64).
    

.. function:: proc hbgvd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ka: c_int, kb: c_int, ab: [] complex(128), bb: [] complex(128), w: [] real(64), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhbgvd for the type complex(128).
    

.. function:: proc hbgvx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, ka: c_int, kb: c_int, ab: [] complex(64), bb: [] complex(64), q: [] complex(64), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] complex(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_chbgvx for the type complex(64).
    

.. function:: proc hbgvx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, ka: c_int, kb: c_int, ab: [] complex(128), bb: [] complex(128), q: [] complex(128), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] complex(128), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zhbgvx for the type complex(128).
    

.. function:: proc hbtrd(matrix_order: lapack_memory_order, vect: string, uplo: string, n: c_int, kd: c_int, ab: [] complex(64), d: [] real(32), e: [] real(32), q: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chbtrd for the type complex(64).
    

.. function:: proc hbtrd(matrix_order: lapack_memory_order, vect: string, uplo: string, n: c_int, kd: c_int, ab: [] complex(128), d: [] real(64), e: [] real(64), q: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhbtrd for the type complex(128).
    

.. function:: proc hecon(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int, anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_checon for the type complex(64).
    

.. function:: proc hecon(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int, anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zhecon for the type complex(128).
    

.. function:: proc heequb(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), s: [] real(32), ref scond: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cheequb for the type complex(64).
    

.. function:: proc heequb(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), s: [] real(64), ref scond: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zheequb for the type complex(128).
    

.. function:: proc heev(matrix_order: lapack_memory_order, jobz: string, uplo: string, a: [] complex(64), w: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cheev for the type complex(64).
    

.. function:: proc heev(matrix_order: lapack_memory_order, jobz: string, uplo: string, a: [] complex(128), w: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zheev for the type complex(128).
    

.. function:: proc heevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, a: [] complex(64), w: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cheevd for the type complex(64).
    

.. function:: proc heevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, a: [] complex(128), w: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zheevd for the type complex(128).
    

.. function:: proc heevr(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, a: [] complex(64), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] complex(64), isuppz: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_cheevr for the type complex(64).
    

.. function:: proc heevr(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, a: [] complex(128), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] complex(128), isuppz: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zheevr for the type complex(128).
    

.. function:: proc heevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, a: [] complex(64), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] complex(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_cheevx for the type complex(64).
    

.. function:: proc heevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, a: [] complex(128), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] complex(128), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zheevx for the type complex(128).
    

.. function:: proc hegst(matrix_order: lapack_memory_order, itype: c_int, uplo: string, a: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chegst for the type complex(64).
    

.. function:: proc hegst(matrix_order: lapack_memory_order, itype: c_int, uplo: string, a: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhegst for the type complex(128).
    

.. function:: proc hegv(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, a: [] complex(64), b: [] complex(64), w: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_chegv for the type complex(64).
    

.. function:: proc hegv(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, a: [] complex(128), b: [] complex(128), w: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zhegv for the type complex(128).
    

.. function:: proc hegvd(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, a: [] complex(64), b: [] complex(64), w: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_chegvd for the type complex(64).
    

.. function:: proc hegvd(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, a: [] complex(128), b: [] complex(128), w: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zhegvd for the type complex(128).
    

.. function:: proc hegvx(matrix_order: lapack_memory_order, itype: c_int, jobz: string, range: string, uplo: string, a: [] complex(64), b: [] complex(64), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] complex(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_chegvx for the type complex(64).
    

.. function:: proc hegvx(matrix_order: lapack_memory_order, itype: c_int, jobz: string, range: string, uplo: string, a: [] complex(128), b: [] complex(128), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] complex(128), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zhegvx for the type complex(128).
    

.. function:: proc herfs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cherfs for the type complex(64).
    

.. function:: proc herfs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zherfs for the type complex(128).
    

.. function:: proc herfsx(matrix_order: lapack_memory_order, uplo: string, equed: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, s: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cherfsx for the type complex(64).
    

.. function:: proc herfsx(matrix_order: lapack_memory_order, uplo: string, equed: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, s: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zherfsx for the type complex(128).
    

.. function:: proc hesv(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chesv for the type complex(64).
    

.. function:: proc hesv(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhesv for the type complex(128).
    

.. function:: proc hesvx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_chesvx for the type complex(64).
    

.. function:: proc hesvx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zhesvx for the type complex(128).
    

.. function:: proc hesvxx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, ref equed: string, s: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), ref rpvgrw: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_chesvxx for the type complex(64).
    

.. function:: proc hesvxx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, ref equed: string, s: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), ref rpvgrw: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zhesvxx for the type complex(128).
    

.. function:: proc hetrd(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), d: [] real(32), e: [] real(32), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chetrd for the type complex(64).
    

.. function:: proc hetrd(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), d: [] real(64), e: [] real(64), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhetrd for the type complex(128).
    

.. function:: proc hetrf(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_chetrf for the type complex(64).
    

.. function:: proc hetrf(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zhetrf for the type complex(128).
    

.. function:: proc hetri(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_chetri for the type complex(64).
    

.. function:: proc hetri(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zhetri for the type complex(128).
    

.. function:: proc hetrs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chetrs for the type complex(64).
    

.. function:: proc hetrs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhetrs for the type complex(128).
    

.. function:: proc hfrk(matrix_order: lapack_memory_order, transr: string, uplo: string, trans: string, alpha: real(32), a: [] complex(64), beta: real(32), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chfrk for the type complex(64).
    

.. function:: proc hfrk(matrix_order: lapack_memory_order, transr: string, uplo: string, trans: string, alpha: real(64), a: [] complex(128), beta: real(64), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhfrk for the type complex(128).
    

.. function:: proc hgeqz(matrix_order: lapack_memory_order, job: string, compq: string, compz: string, ilo: c_int, ihi: c_int, h: [] real(32), t: [] real(32), alphar: [] real(32), alphai: [] real(32), beta: [] real(32), q: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_shgeqz for the type real(32).
    

.. function:: proc hgeqz(matrix_order: lapack_memory_order, job: string, compq: string, compz: string, ilo: c_int, ihi: c_int, h: [] real(64), t: [] real(64), alphar: [] real(64), alphai: [] real(64), beta: [] real(64), q: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dhgeqz for the type real(64).
    

.. function:: proc hgeqz(matrix_order: lapack_memory_order, job: string, compq: string, compz: string, ilo: c_int, ihi: c_int, h: [] complex(64), t: [] complex(64), alpha: [] complex(64), beta: [] complex(64), q: [] complex(64), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chgeqz for the type complex(64).
    

.. function:: proc hgeqz(matrix_order: lapack_memory_order, job: string, compq: string, compz: string, ilo: c_int, ihi: c_int, h: [] complex(128), t: [] complex(128), alpha: [] complex(128), beta: [] complex(128), q: [] complex(128), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhgeqz for the type complex(128).
    

.. function:: proc hpcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), ipiv: [] c_int, anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_chpcon for the type complex(64).
    

.. function:: proc hpcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), ipiv: [] c_int, anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zhpcon for the type complex(128).
    

.. function:: proc hpev(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ap: [] complex(64), w: [] real(32), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chpev for the type complex(64).
    

.. function:: proc hpev(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ap: [] complex(128), w: [] real(64), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhpev for the type complex(128).
    

.. function:: proc hpevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ap: [] complex(64), w: [] real(32), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chpevd for the type complex(64).
    

.. function:: proc hpevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ap: [] complex(128), w: [] real(64), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhpevd for the type complex(128).
    

.. function:: proc hpevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, n: c_int, ap: [] complex(64), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] complex(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_chpevx for the type complex(64).
    

.. function:: proc hpevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, n: c_int, ap: [] complex(128), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] complex(128), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zhpevx for the type complex(128).
    

.. function:: proc hpgst(matrix_order: lapack_memory_order, itype: c_int, uplo: string, n: c_int, ap: [] complex(64), bp: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chpgst for the type complex(64).
    

.. function:: proc hpgst(matrix_order: lapack_memory_order, itype: c_int, uplo: string, n: c_int, ap: [] complex(128), bp: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhpgst for the type complex(128).
    

.. function:: proc hpgv(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, n: c_int, ap: [] complex(64), bp: [] complex(64), w: [] real(32), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chpgv for the type complex(64).
    

.. function:: proc hpgv(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, n: c_int, ap: [] complex(128), bp: [] complex(128), w: [] real(64), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhpgv for the type complex(128).
    

.. function:: proc hpgvd(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, n: c_int, ap: [] complex(64), bp: [] complex(64), w: [] real(32), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chpgvd for the type complex(64).
    

.. function:: proc hpgvd(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, n: c_int, ap: [] complex(128), bp: [] complex(128), w: [] real(64), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhpgvd for the type complex(128).
    

.. function:: proc hpgvx(matrix_order: lapack_memory_order, itype: c_int, jobz: string, range: string, uplo: string, n: c_int, ap: [] complex(64), bp: [] complex(64), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] complex(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_chpgvx for the type complex(64).
    

.. function:: proc hpgvx(matrix_order: lapack_memory_order, itype: c_int, jobz: string, range: string, uplo: string, n: c_int, ap: [] complex(128), bp: [] complex(128), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] complex(128), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zhpgvx for the type complex(128).
    

.. function:: proc hprfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), afp: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_chprfs for the type complex(64).
    

.. function:: proc hprfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), afp: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zhprfs for the type complex(128).
    

.. function:: proc hpsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chpsv for the type complex(64).
    

.. function:: proc hpsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhpsv for the type complex(128).
    

.. function:: proc hpsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, ap: [] complex(64), afp: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_chpsvx for the type complex(64).
    

.. function:: proc hpsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, ap: [] complex(128), afp: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zhpsvx for the type complex(128).
    

.. function:: proc hptrd(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), d: [] real(32), e: [] real(32), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chptrd for the type complex(64).
    

.. function:: proc hptrd(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), d: [] real(64), e: [] real(64), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhptrd for the type complex(128).
    

.. function:: proc hptrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_chptrf for the type complex(64).
    

.. function:: proc hptrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zhptrf for the type complex(128).
    

.. function:: proc hptri(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_chptri for the type complex(64).
    

.. function:: proc hptri(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zhptri for the type complex(128).
    

.. function:: proc hptrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chptrs for the type complex(64).
    

.. function:: proc hptrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhptrs for the type complex(128).
    

.. function:: proc hsein(matrix_order: lapack_memory_order, job: string, eigsrc: string, initv: string, chlapack_select: [] c_int, h: [] real(32), wr: [] real(32), wi: [] real(32), vl: [] real(32), vr: [] real(32), mm: c_int, ref m: c_int, ifaill: [] c_int, ifailr: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_shsein for the type real(32).
    

.. function:: proc hsein(matrix_order: lapack_memory_order, job: string, eigsrc: string, initv: string, chlapack_select: [] c_int, h: [] real(64), wr: [] real(64), wi: [] real(64), vl: [] real(64), vr: [] real(64), mm: c_int, ref m: c_int, ifaill: [] c_int, ifailr: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dhsein for the type real(64).
    

.. function:: proc hsein(matrix_order: lapack_memory_order, job: string, eigsrc: string, initv: string, chlapack_select: [] c_int, h: [] complex(64), w: [] complex(64), vl: [] complex(64), vr: [] complex(64), mm: c_int, ref m: c_int, ifaill: [] c_int, ifailr: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_chsein for the type complex(64).
    

.. function:: proc hsein(matrix_order: lapack_memory_order, job: string, eigsrc: string, initv: string, chlapack_select: [] c_int, h: [] complex(128), w: [] complex(128), vl: [] complex(128), vr: [] complex(128), mm: c_int, ref m: c_int, ifaill: [] c_int, ifailr: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zhsein for the type complex(128).
    

.. function:: proc hseqr(matrix_order: lapack_memory_order, job: string, compz: string, ilo: c_int, ihi: c_int, h: [] real(32), wr: [] real(32), wi: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_shseqr for the type real(32).
    

.. function:: proc hseqr(matrix_order: lapack_memory_order, job: string, compz: string, ilo: c_int, ihi: c_int, h: [] real(64), wr: [] real(64), wi: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dhseqr for the type real(64).
    

.. function:: proc hseqr(matrix_order: lapack_memory_order, job: string, compz: string, ilo: c_int, ihi: c_int, h: [] complex(64), w: [] complex(64), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chseqr for the type complex(64).
    

.. function:: proc hseqr(matrix_order: lapack_memory_order, job: string, compz: string, ilo: c_int, ihi: c_int, h: [] complex(128), w: [] complex(128), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhseqr for the type complex(128).
    

.. function:: proc lacgv(n: c_int, x: [] complex(64), incx: c_int): c_int

   
   Wrapped procedure of LAPACKE_clacgv for the type complex(64).
    

.. function:: proc lacgv(n: c_int, x: [] complex(128), incx: c_int): c_int

   
   Wrapped procedure of LAPACKE_zlacgv for the type complex(128).
    

.. function:: proc lacn2(n: c_int, v: [] real(32), x: [] real(32), isgn: [] c_int, ref est: real(32), ref kase: c_int, isave: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_slacn2 for the type real(32).
    

.. function:: proc lacn2(n: c_int, v: [] real(64), x: [] real(64), isgn: [] c_int, ref est: real(64), ref kase: c_int, isave: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dlacn2 for the type real(64).
    

.. function:: proc lacn2(n: c_int, v: [] complex(64), x: [] complex(64), ref est: real(32), ref kase: c_int, isave: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_clacn2 for the type complex(64).
    

.. function:: proc lacn2(n: c_int, v: [] complex(128), x: [] complex(128), ref est: real(64), ref kase: c_int, isave: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zlacn2 for the type complex(128).
    

.. function:: proc lacpy(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_slacpy for the type real(32).
    

.. function:: proc lacpy(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlacpy for the type real(64).
    

.. function:: proc lacpy(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_clacpy for the type complex(64).
    

.. function:: proc lacpy(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zlacpy for the type complex(128).
    

.. function:: proc lacp2(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_clacp2 for the type complex(64).
    

.. function:: proc lacp2(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zlacp2 for the type complex(128).
    

.. function:: proc lag2c(matrix_order: lapack_memory_order, a: [] complex(128), sa: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_zlag2c for the type complex(128).
    

.. function:: proc lag2d(matrix_order: lapack_memory_order, sa: [] real(32), a: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_slag2d for the type real(32).
    

.. function:: proc lag2s(matrix_order: lapack_memory_order, a: [] real(64), sa: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_dlag2s for the type real(64).
    

.. function:: proc lag2z(matrix_order: lapack_memory_order, sa: [] complex(64), a: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_clag2z for the type complex(64).
    

.. function:: proc lagge(matrix_order: lapack_memory_order, kl: c_int, ku: c_int, d: [] real(32), a: [] real(32), iseed: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_slagge for the type real(32).
    

.. function:: proc lagge(matrix_order: lapack_memory_order, kl: c_int, ku: c_int, d: [] real(64), a: [] real(64), iseed: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dlagge for the type real(64).
    

.. function:: proc lagge(matrix_order: lapack_memory_order, kl: c_int, ku: c_int, d: [] real(32), a: [] complex(64), iseed: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_clagge for the type complex(64).
    

.. function:: proc lagge(matrix_order: lapack_memory_order, kl: c_int, ku: c_int, d: [] real(64), a: [] complex(128), iseed: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zlagge for the type complex(128).
    

.. function:: proc lamch(cmach: string): c_float

   
   Wrapped procedure of LAPACKE_slamch for the type real(32).
    

.. function:: proc lamch(cmach: string): c_double

   
   Wrapped procedure of LAPACKE_dlamch for the type real(64).
    

.. function:: proc lange(matrix_order: lapack_memory_order, norm: string, a: [] real(32)): c_float

   
   Wrapped procedure of LAPACKE_slange for the type real(32).
    

.. function:: proc lange(matrix_order: lapack_memory_order, norm: string, a: [] real(64)): c_double

   
   Wrapped procedure of LAPACKE_dlange for the type real(64).
    

.. function:: proc lange(matrix_order: lapack_memory_order, norm: string, a: [] complex(64)): c_float

   
   Wrapped procedure of LAPACKE_clange for the type complex(64).
    

.. function:: proc lange(matrix_order: lapack_memory_order, norm: string, a: [] complex(128)): c_double

   
   Wrapped procedure of LAPACKE_zlange for the type complex(128).
    

.. function:: proc lanhe(matrix_order: lapack_memory_order, norm: string, uplo: string, a: [] complex(64)): c_float

   
   Wrapped procedure of LAPACKE_clanhe for the type complex(64).
    

.. function:: proc lanhe(matrix_order: lapack_memory_order, norm: string, uplo: string, a: [] complex(128)): c_double

   
   Wrapped procedure of LAPACKE_zlanhe for the type complex(128).
    

.. function:: proc lansy(matrix_order: lapack_memory_order, norm: string, uplo: string, a: [] real(32)): c_float

   
   Wrapped procedure of LAPACKE_slansy for the type real(32).
    

.. function:: proc lansy(matrix_order: lapack_memory_order, norm: string, uplo: string, a: [] real(64)): c_double

   
   Wrapped procedure of LAPACKE_dlansy for the type real(64).
    

.. function:: proc lansy(matrix_order: lapack_memory_order, norm: string, uplo: string, a: [] complex(64)): c_float

   
   Wrapped procedure of LAPACKE_clansy for the type complex(64).
    

.. function:: proc lansy(matrix_order: lapack_memory_order, norm: string, uplo: string, a: [] complex(128)): c_double

   
   Wrapped procedure of LAPACKE_zlansy for the type complex(128).
    

.. function:: proc lantr(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, a: [] real(32)): c_float

   
   Wrapped procedure of LAPACKE_slantr for the type real(32).
    

.. function:: proc lantr(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, a: [] real(64)): c_double

   
   Wrapped procedure of LAPACKE_dlantr for the type real(64).
    

.. function:: proc lantr(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, a: [] complex(64)): c_float

   
   Wrapped procedure of LAPACKE_clantr for the type complex(64).
    

.. function:: proc lantr(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, a: [] complex(128)): c_double

   
   Wrapped procedure of LAPACKE_zlantr for the type complex(128).
    

.. function:: proc larfb(matrix_order: lapack_memory_order, side: string, trans: string, direct: string, storev: string, v: [] real(32), t: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_slarfb for the type real(32).
    

.. function:: proc larfb(matrix_order: lapack_memory_order, side: string, trans: string, direct: string, storev: string, v: [] real(64), t: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlarfb for the type real(64).
    

.. function:: proc larfb(matrix_order: lapack_memory_order, side: string, trans: string, direct: string, storev: string, v: [] complex(64), t: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_clarfb for the type complex(64).
    

.. function:: proc larfb(matrix_order: lapack_memory_order, side: string, trans: string, direct: string, storev: string, v: [] complex(128), t: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zlarfb for the type complex(128).
    

.. function:: proc larfg(n: c_int, ref alpha: real(32), x: [] real(32), incx: c_int, ref tau: real(32)): c_int

   
   Wrapped procedure of LAPACKE_slarfg for the type real(32).
    

.. function:: proc larfg(n: c_int, ref alpha: real(64), x: [] real(64), incx: c_int, ref tau: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlarfg for the type real(64).
    

.. function:: proc larfg(n: c_int, ref alpha: complex(64), x: [] complex(64), incx: c_int, ref tau: complex(64)): c_int

   
   Wrapped procedure of LAPACKE_clarfg for the type complex(64).
    

.. function:: proc larfg(n: c_int, ref alpha: complex(128), x: [] complex(128), incx: c_int, ref tau: complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zlarfg for the type complex(128).
    

.. function:: proc larft(matrix_order: lapack_memory_order, direct: string, storev: string, n: c_int, k: c_int, v: [] real(32), tau: [] real(32), t: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_slarft for the type real(32).
    

.. function:: proc larft(matrix_order: lapack_memory_order, direct: string, storev: string, n: c_int, k: c_int, v: [] real(64), tau: [] real(64), t: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlarft for the type real(64).
    

.. function:: proc larft(matrix_order: lapack_memory_order, direct: string, storev: string, n: c_int, k: c_int, v: [] complex(64), tau: [] complex(64), t: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_clarft for the type complex(64).
    

.. function:: proc larft(matrix_order: lapack_memory_order, direct: string, storev: string, n: c_int, k: c_int, v: [] complex(128), tau: [] complex(128), t: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zlarft for the type complex(128).
    

.. function:: proc larfx(matrix_order: lapack_memory_order, side: string, v: [] real(32), tau: real(32), c: [] real(32), work: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_slarfx for the type real(32).
    

.. function:: proc larfx(matrix_order: lapack_memory_order, side: string, v: [] real(64), tau: real(64), c: [] real(64), work: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlarfx for the type real(64).
    

.. function:: proc larfx(matrix_order: lapack_memory_order, side: string, v: [] complex(64), tau: complex(64), c: [] complex(64), work: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_clarfx for the type complex(64).
    

.. function:: proc larfx(matrix_order: lapack_memory_order, side: string, v: [] complex(128), tau: complex(128), c: [] complex(128), work: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zlarfx for the type complex(128).
    

.. function:: proc larnv(idist: c_int, iseed: [] c_int, n: c_int, x: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_slarnv for the type real(32).
    

.. function:: proc larnv(idist: c_int, iseed: [] c_int, n: c_int, x: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlarnv for the type real(64).
    

.. function:: proc larnv(idist: c_int, iseed: [] c_int, n: c_int, x: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_clarnv for the type complex(64).
    

.. function:: proc larnv(idist: c_int, iseed: [] c_int, n: c_int, x: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zlarnv for the type complex(128).
    

.. function:: proc laset(matrix_order: lapack_memory_order, uplo: string, alpha: real(32), beta: real(32), a: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_slaset for the type real(32).
    

.. function:: proc laset(matrix_order: lapack_memory_order, uplo: string, alpha: real(64), beta: real(64), a: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlaset for the type real(64).
    

.. function:: proc laset(matrix_order: lapack_memory_order, uplo: string, alpha: complex(64), beta: complex(64), a: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_claset for the type complex(64).
    

.. function:: proc laset(matrix_order: lapack_memory_order, uplo: string, alpha: complex(128), beta: complex(128), a: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zlaset for the type complex(128).
    

.. function:: proc lasrt(id: string, n: c_int, d: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_slasrt for the type real(32).
    

.. function:: proc lasrt(id: string, n: c_int, d: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlasrt for the type real(64).
    

.. function:: proc laswp(matrix_order: lapack_memory_order, a: [] real(32), k1: c_int, k2: c_int, ipiv: [] c_int, incx: c_int): c_int

   
   Wrapped procedure of LAPACKE_slaswp for the type real(32).
    

.. function:: proc laswp(matrix_order: lapack_memory_order, a: [] real(64), k1: c_int, k2: c_int, ipiv: [] c_int, incx: c_int): c_int

   
   Wrapped procedure of LAPACKE_dlaswp for the type real(64).
    

.. function:: proc laswp(matrix_order: lapack_memory_order, a: [] complex(64), k1: c_int, k2: c_int, ipiv: [] c_int, incx: c_int): c_int

   
   Wrapped procedure of LAPACKE_claswp for the type complex(64).
    

.. function:: proc laswp(matrix_order: lapack_memory_order, a: [] complex(128), k1: c_int, k2: c_int, ipiv: [] c_int, incx: c_int): c_int

   
   Wrapped procedure of LAPACKE_zlaswp for the type complex(128).
    

.. function:: proc latms(matrix_order: lapack_memory_order, dist: string, iseed: [] c_int, sym: string, d: [] real(32), mode: c_int, cond: real(32), dmax: real(32), kl: c_int, ku: c_int, pack: string, a: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_slatms for the type real(32).
    

.. function:: proc latms(matrix_order: lapack_memory_order, dist: string, iseed: [] c_int, sym: string, d: [] real(64), mode: c_int, cond: real(64), dmax: real(64), kl: c_int, ku: c_int, pack: string, a: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlatms for the type real(64).
    

.. function:: proc latms(matrix_order: lapack_memory_order, dist: string, iseed: [] c_int, sym: string, d: [] real(32), mode: c_int, cond: real(32), dmax: real(32), kl: c_int, ku: c_int, pack: string, a: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_clatms for the type complex(64).
    

.. function:: proc latms(matrix_order: lapack_memory_order, dist: string, iseed: [] c_int, sym: string, d: [] real(64), mode: c_int, cond: real(64), dmax: real(64), kl: c_int, ku: c_int, pack: string, a: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zlatms for the type complex(128).
    

.. function:: proc lauum(matrix_order: lapack_memory_order, uplo: string, n: c_int, a: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_slauum for the type real(32).
    

.. function:: proc lauum(matrix_order: lapack_memory_order, uplo: string, n: c_int, a: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlauum for the type real(64).
    

.. function:: proc lauum(matrix_order: lapack_memory_order, uplo: string, n: c_int, a: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_clauum for the type complex(64).
    

.. function:: proc lauum(matrix_order: lapack_memory_order, uplo: string, n: c_int, a: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zlauum for the type complex(128).
    

.. function:: proc opgtr(matrix_order: lapack_memory_order, uplo: string, ap: [] real(32), tau: [] real(32), q: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sopgtr for the type real(32).
    

.. function:: proc opgtr(matrix_order: lapack_memory_order, uplo: string, ap: [] real(64), tau: [] real(64), q: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dopgtr for the type real(64).
    

.. function:: proc opmtr(matrix_order: lapack_memory_order, side: string, uplo: string, trans: string, ap: [] real(32), tau: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sopmtr for the type real(32).
    

.. function:: proc opmtr(matrix_order: lapack_memory_order, side: string, uplo: string, trans: string, ap: [] real(64), tau: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dopmtr for the type real(64).
    

.. function:: proc orgbr(matrix_order: lapack_memory_order, vect: string, k: c_int, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sorgbr for the type real(32).
    

.. function:: proc orgbr(matrix_order: lapack_memory_order, vect: string, k: c_int, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dorgbr for the type real(64).
    

.. function:: proc orghr(matrix_order: lapack_memory_order, n: c_int, ilo: c_int, ihi: c_int, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sorghr for the type real(32).
    

.. function:: proc orghr(matrix_order: lapack_memory_order, n: c_int, ilo: c_int, ihi: c_int, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dorghr for the type real(64).
    

.. function:: proc orglq(matrix_order: lapack_memory_order, k: c_int, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sorglq for the type real(32).
    

.. function:: proc orglq(matrix_order: lapack_memory_order, k: c_int, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dorglq for the type real(64).
    

.. function:: proc orgql(matrix_order: lapack_memory_order, k: c_int, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sorgql for the type real(32).
    

.. function:: proc orgql(matrix_order: lapack_memory_order, k: c_int, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dorgql for the type real(64).
    

.. function:: proc orgqr(matrix_order: lapack_memory_order, k: c_int, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sorgqr for the type real(32).
    

.. function:: proc orgqr(matrix_order: lapack_memory_order, k: c_int, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dorgqr for the type real(64).
    

.. function:: proc orgrq(matrix_order: lapack_memory_order, k: c_int, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sorgrq for the type real(32).
    

.. function:: proc orgrq(matrix_order: lapack_memory_order, k: c_int, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dorgrq for the type real(64).
    

.. function:: proc orgtr(matrix_order: lapack_memory_order, uplo: string, n: c_int, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sorgtr for the type real(32).
    

.. function:: proc orgtr(matrix_order: lapack_memory_order, uplo: string, n: c_int, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dorgtr for the type real(64).
    

.. function:: proc ormbr(matrix_order: lapack_memory_order, vect: string, side: string, trans: string, k: c_int, a: [] real(32), tau: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sormbr for the type real(32).
    

.. function:: proc ormbr(matrix_order: lapack_memory_order, vect: string, side: string, trans: string, k: c_int, a: [] real(64), tau: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dormbr for the type real(64).
    

.. function:: proc ormhr(matrix_order: lapack_memory_order, side: string, trans: string, ilo: c_int, ihi: c_int, a: [] real(32), tau: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sormhr for the type real(32).
    

.. function:: proc ormhr(matrix_order: lapack_memory_order, side: string, trans: string, ilo: c_int, ihi: c_int, a: [] real(64), tau: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dormhr for the type real(64).
    

.. function:: proc ormlq(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] real(32), tau: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sormlq for the type real(32).
    

.. function:: proc ormlq(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] real(64), tau: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dormlq for the type real(64).
    

.. function:: proc ormql(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] real(32), tau: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sormql for the type real(32).
    

.. function:: proc ormql(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] real(64), tau: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dormql for the type real(64).
    

.. function:: proc ormqr(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] real(32), tau: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sormqr for the type real(32).
    

.. function:: proc ormqr(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] real(64), tau: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dormqr for the type real(64).
    

.. function:: proc ormrq(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] real(32), tau: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sormrq for the type real(32).
    

.. function:: proc ormrq(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] real(64), tau: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dormrq for the type real(64).
    

.. function:: proc ormrz(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, l: c_int, a: [] real(32), tau: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sormrz for the type real(32).
    

.. function:: proc ormrz(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, l: c_int, a: [] real(64), tau: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dormrz for the type real(64).
    

.. function:: proc ormtr(matrix_order: lapack_memory_order, side: string, uplo: string, trans: string, a: [] real(32), tau: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sormtr for the type real(32).
    

.. function:: proc ormtr(matrix_order: lapack_memory_order, side: string, uplo: string, trans: string, a: [] real(64), tau: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dormtr for the type real(64).
    

.. function:: proc pbcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(32), anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_spbcon for the type real(32).
    

.. function:: proc pbcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(64), anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpbcon for the type real(64).
    

.. function:: proc pbcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(64), anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cpbcon for the type complex(64).
    

.. function:: proc pbcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(128), anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zpbcon for the type complex(128).
    

.. function:: proc pbequ(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(32), s: [] real(32), ref scond: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_spbequ for the type real(32).
    

.. function:: proc pbequ(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(64), s: [] real(64), ref scond: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpbequ for the type real(64).
    

.. function:: proc pbequ(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(64), s: [] real(32), ref scond: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cpbequ for the type complex(64).
    

.. function:: proc pbequ(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(128), s: [] real(64), ref scond: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zpbequ for the type complex(128).
    

.. function:: proc pbrfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(32), afb: [] real(32), b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spbrfs for the type real(32).
    

.. function:: proc pbrfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(64), afb: [] real(64), b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpbrfs for the type real(64).
    

.. function:: proc pbrfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(64), afb: [] complex(64), b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cpbrfs for the type complex(64).
    

.. function:: proc pbrfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(128), afb: [] complex(128), b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zpbrfs for the type complex(128).
    

.. function:: proc pbstf(matrix_order: lapack_memory_order, uplo: string, n: c_int, kb: c_int, bb: [] real(32), ldbb: c_int): c_int

   
   Wrapped procedure of LAPACKE_spbstf for the type real(32).
    

.. function:: proc pbstf(matrix_order: lapack_memory_order, uplo: string, n: c_int, kb: c_int, bb: [] real(64), ldbb: c_int): c_int

   
   Wrapped procedure of LAPACKE_dpbstf for the type real(64).
    

.. function:: proc pbstf(matrix_order: lapack_memory_order, uplo: string, n: c_int, kb: c_int, bb: [] complex(64), ldbb: c_int): c_int

   
   Wrapped procedure of LAPACKE_cpbstf for the type complex(64).
    

.. function:: proc pbstf(matrix_order: lapack_memory_order, uplo: string, n: c_int, kb: c_int, bb: [] complex(128), ldbb: c_int): c_int

   
   Wrapped procedure of LAPACKE_zpbstf for the type complex(128).
    

.. function:: proc pbsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spbsv for the type real(32).
    

.. function:: proc pbsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpbsv for the type real(64).
    

.. function:: proc pbsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpbsv for the type complex(64).
    

.. function:: proc pbsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpbsv for the type complex(128).
    

.. function:: proc pbsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, kd: c_int, ab: [] real(32), afb: [] real(32), ref equed: string, s: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spbsvx for the type real(32).
    

.. function:: proc pbsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, kd: c_int, ab: [] real(64), afb: [] real(64), ref equed: string, s: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpbsvx for the type real(64).
    

.. function:: proc pbsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, kd: c_int, ab: [] complex(64), afb: [] complex(64), ref equed: string, s: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cpbsvx for the type complex(64).
    

.. function:: proc pbsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, kd: c_int, ab: [] complex(128), afb: [] complex(128), ref equed: string, s: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zpbsvx for the type complex(128).
    

.. function:: proc pbtrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spbtrf for the type real(32).
    

.. function:: proc pbtrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpbtrf for the type real(64).
    

.. function:: proc pbtrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpbtrf for the type complex(64).
    

.. function:: proc pbtrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpbtrf for the type complex(128).
    

.. function:: proc pbtrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spbtrs for the type real(32).
    

.. function:: proc pbtrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpbtrs for the type real(64).
    

.. function:: proc pbtrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpbtrs for the type complex(64).
    

.. function:: proc pbtrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, kd: c_int, ab: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpbtrs for the type complex(128).
    

.. function:: proc pftrf(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spftrf for the type real(32).
    

.. function:: proc pftrf(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpftrf for the type real(64).
    

.. function:: proc pftrf(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpftrf for the type complex(64).
    

.. function:: proc pftrf(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpftrf for the type complex(128).
    

.. function:: proc pftri(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spftri for the type real(32).
    

.. function:: proc pftri(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpftri for the type real(64).
    

.. function:: proc pftri(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpftri for the type complex(64).
    

.. function:: proc pftri(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpftri for the type complex(128).
    

.. function:: proc pftrs(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spftrs for the type real(32).
    

.. function:: proc pftrs(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpftrs for the type real(64).
    

.. function:: proc pftrs(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpftrs for the type complex(64).
    

.. function:: proc pftrs(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpftrs for the type complex(128).
    

.. function:: proc pocon(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_spocon for the type real(32).
    

.. function:: proc pocon(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpocon for the type real(64).
    

.. function:: proc pocon(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cpocon for the type complex(64).
    

.. function:: proc pocon(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zpocon for the type complex(128).
    

.. function:: proc poequ(matrix_order: lapack_memory_order, a: [] real(32), s: [] real(32), ref scond: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_spoequ for the type real(32).
    

.. function:: proc poequ(matrix_order: lapack_memory_order, a: [] real(64), s: [] real(64), ref scond: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpoequ for the type real(64).
    

.. function:: proc poequ(matrix_order: lapack_memory_order, a: [] complex(64), s: [] real(32), ref scond: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cpoequ for the type complex(64).
    

.. function:: proc poequ(matrix_order: lapack_memory_order, a: [] complex(128), s: [] real(64), ref scond: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zpoequ for the type complex(128).
    

.. function:: proc poequb(matrix_order: lapack_memory_order, a: [] real(32), s: [] real(32), ref scond: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_spoequb for the type real(32).
    

.. function:: proc poequb(matrix_order: lapack_memory_order, a: [] real(64), s: [] real(64), ref scond: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpoequb for the type real(64).
    

.. function:: proc poequb(matrix_order: lapack_memory_order, a: [] complex(64), s: [] real(32), ref scond: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cpoequb for the type complex(64).
    

.. function:: proc poequb(matrix_order: lapack_memory_order, a: [] complex(128), s: [] real(64), ref scond: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zpoequb for the type complex(128).
    

.. function:: proc porfs(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), af: [] real(32), b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sporfs for the type real(32).
    

.. function:: proc porfs(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), af: [] real(64), b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dporfs for the type real(64).
    

.. function:: proc porfs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), af: [] complex(64), b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cporfs for the type complex(64).
    

.. function:: proc porfs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), af: [] complex(128), b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zporfs for the type complex(128).
    

.. function:: proc porfsx(matrix_order: lapack_memory_order, uplo: string, equed: string, a: [] real(32), af: [] real(32), s: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sporfsx for the type real(32).
    

.. function:: proc porfsx(matrix_order: lapack_memory_order, uplo: string, equed: string, a: [] real(64), af: [] real(64), s: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dporfsx for the type real(64).
    

.. function:: proc porfsx(matrix_order: lapack_memory_order, uplo: string, equed: string, a: [] complex(64), af: [] complex(64), s: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cporfsx for the type complex(64).
    

.. function:: proc porfsx(matrix_order: lapack_memory_order, uplo: string, equed: string, a: [] complex(128), af: [] complex(128), s: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zporfsx for the type complex(128).
    

.. function:: proc posv(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sposv for the type real(32).
    

.. function:: proc posv(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dposv for the type real(64).
    

.. function:: proc posv(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cposv for the type complex(64).
    

.. function:: proc posv(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zposv for the type complex(128).
    

.. function:: proc posv(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), b: [] real(64), x: [] real(64), ref chlapack_iter: c_int): c_int

   
   Wrapped procedure of LAPACKE_dsposv for the type real(64).
    

.. function:: proc posv(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), b: [] complex(128), x: [] complex(128), ref chlapack_iter: c_int): c_int

   
   Wrapped procedure of LAPACKE_zcposv for the type complex(128).
    

.. function:: proc posvx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] real(32), af: [] real(32), ref equed: string, s: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sposvx for the type real(32).
    

.. function:: proc posvx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] real(64), af: [] real(64), ref equed: string, s: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dposvx for the type real(64).
    

.. function:: proc posvx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(64), af: [] complex(64), ref equed: string, s: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cposvx for the type complex(64).
    

.. function:: proc posvx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(128), af: [] complex(128), ref equed: string, s: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zposvx for the type complex(128).
    

.. function:: proc posvxx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] real(32), af: [] real(32), ref equed: string, s: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), ref rpvgrw: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sposvxx for the type real(32).
    

.. function:: proc posvxx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] real(64), af: [] real(64), ref equed: string, s: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), ref rpvgrw: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dposvxx for the type real(64).
    

.. function:: proc posvxx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(64), af: [] complex(64), ref equed: string, s: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), ref rpvgrw: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cposvxx for the type complex(64).
    

.. function:: proc posvxx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(128), af: [] complex(128), ref equed: string, s: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), ref rpvgrw: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zposvxx for the type complex(128).
    

.. function:: proc potrf(matrix_order: lapack_memory_order, uplo: string, a: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spotrf for the type real(32).
    

.. function:: proc potrf(matrix_order: lapack_memory_order, uplo: string, a: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpotrf for the type real(64).
    

.. function:: proc potrf(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpotrf for the type complex(64).
    

.. function:: proc potrf(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpotrf for the type complex(128).
    

.. function:: proc potri(matrix_order: lapack_memory_order, uplo: string, a: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spotri for the type real(32).
    

.. function:: proc potri(matrix_order: lapack_memory_order, uplo: string, a: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpotri for the type real(64).
    

.. function:: proc potri(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpotri for the type complex(64).
    

.. function:: proc potri(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpotri for the type complex(128).
    

.. function:: proc potrs(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spotrs for the type real(32).
    

.. function:: proc potrs(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpotrs for the type real(64).
    

.. function:: proc potrs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpotrs for the type complex(64).
    

.. function:: proc potrs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpotrs for the type complex(128).
    

.. function:: proc ppcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sppcon for the type real(32).
    

.. function:: proc ppcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dppcon for the type real(64).
    

.. function:: proc ppcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cppcon for the type complex(64).
    

.. function:: proc ppcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zppcon for the type complex(128).
    

.. function:: proc ppequ(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), s: [] real(32), ref scond: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sppequ for the type real(32).
    

.. function:: proc ppequ(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), s: [] real(64), ref scond: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dppequ for the type real(64).
    

.. function:: proc ppequ(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), s: [] real(32), ref scond: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cppequ for the type complex(64).
    

.. function:: proc ppequ(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), s: [] real(64), ref scond: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zppequ for the type complex(128).
    

.. function:: proc pprfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), afp: [] real(32), b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spprfs for the type real(32).
    

.. function:: proc pprfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), afp: [] real(64), b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpprfs for the type real(64).
    

.. function:: proc pprfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), afp: [] complex(64), b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cpprfs for the type complex(64).
    

.. function:: proc pprfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), afp: [] complex(128), b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zpprfs for the type complex(128).
    

.. function:: proc ppsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sppsv for the type real(32).
    

.. function:: proc ppsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dppsv for the type real(64).
    

.. function:: proc ppsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cppsv for the type complex(64).
    

.. function:: proc ppsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zppsv for the type complex(128).
    

.. function:: proc ppsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, ap: [] real(32), afp: [] real(32), ref equed: string, s: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sppsvx for the type real(32).
    

.. function:: proc ppsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, ap: [] real(64), afp: [] real(64), ref equed: string, s: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dppsvx for the type real(64).
    

.. function:: proc ppsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, ap: [] complex(64), afp: [] complex(64), ref equed: string, s: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cppsvx for the type complex(64).
    

.. function:: proc ppsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, ap: [] complex(128), afp: [] complex(128), ref equed: string, s: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zppsvx for the type complex(128).
    

.. function:: proc pptrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spptrf for the type real(32).
    

.. function:: proc pptrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpptrf for the type real(64).
    

.. function:: proc pptrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpptrf for the type complex(64).
    

.. function:: proc pptrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpptrf for the type complex(128).
    

.. function:: proc pptri(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spptri for the type real(32).
    

.. function:: proc pptri(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpptri for the type real(64).
    

.. function:: proc pptri(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpptri for the type complex(64).
    

.. function:: proc pptri(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpptri for the type complex(128).
    

.. function:: proc pptrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spptrs for the type real(32).
    

.. function:: proc pptrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpptrs for the type real(64).
    

.. function:: proc pptrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpptrs for the type complex(64).
    

.. function:: proc pptrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpptrs for the type complex(128).
    

.. function:: proc pstrf(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), piv: [] c_int, ref rank: c_int, tol: real(32)): c_int

   
   Wrapped procedure of LAPACKE_spstrf for the type real(32).
    

.. function:: proc pstrf(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), piv: [] c_int, ref rank: c_int, tol: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpstrf for the type real(64).
    

.. function:: proc pstrf(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), piv: [] c_int, ref rank: c_int, tol: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cpstrf for the type complex(64).
    

.. function:: proc pstrf(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), piv: [] c_int, ref rank: c_int, tol: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zpstrf for the type complex(128).
    

.. function:: proc ptcon(n: c_int, d: [] real(32), e: [] real(32), anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sptcon for the type real(32).
    

.. function:: proc ptcon(n: c_int, d: [] real(64), e: [] real(64), anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dptcon for the type real(64).
    

.. function:: proc ptcon(n: c_int, d: [] real(32), e: [] complex(64), anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cptcon for the type complex(64).
    

.. function:: proc ptcon(n: c_int, d: [] real(64), e: [] complex(128), anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zptcon for the type complex(128).
    

.. function:: proc pteqr(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(32), e: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spteqr for the type real(32).
    

.. function:: proc pteqr(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(64), e: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpteqr for the type real(64).
    

.. function:: proc pteqr(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(32), e: [] real(32), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpteqr for the type complex(64).
    

.. function:: proc pteqr(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(64), e: [] real(64), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpteqr for the type complex(128).
    

.. function:: proc ptrfs(matrix_order: lapack_memory_order, n: c_int, d: [] real(32), e: [] real(32), df: [] real(32), ef: [] real(32), b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sptrfs for the type real(32).
    

.. function:: proc ptrfs(matrix_order: lapack_memory_order, n: c_int, d: [] real(64), e: [] real(64), df: [] real(64), ef: [] real(64), b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dptrfs for the type real(64).
    

.. function:: proc ptrfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, d: [] real(32), e: [] complex(64), df: [] real(32), ef: [] complex(64), b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cptrfs for the type complex(64).
    

.. function:: proc ptrfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, d: [] real(64), e: [] complex(128), df: [] real(64), ef: [] complex(128), b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zptrfs for the type complex(128).
    

.. function:: proc ptsv(matrix_order: lapack_memory_order, n: c_int, d: [] real(32), e: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sptsv for the type real(32).
    

.. function:: proc ptsv(matrix_order: lapack_memory_order, n: c_int, d: [] real(64), e: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dptsv for the type real(64).
    

.. function:: proc ptsv(matrix_order: lapack_memory_order, n: c_int, d: [] real(32), e: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cptsv for the type complex(64).
    

.. function:: proc ptsv(matrix_order: lapack_memory_order, n: c_int, d: [] real(64), e: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zptsv for the type complex(128).
    

.. function:: proc ptsvx(matrix_order: lapack_memory_order, fact: string, n: c_int, d: [] real(32), e: [] real(32), df: [] real(32), ef: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sptsvx for the type real(32).
    

.. function:: proc ptsvx(matrix_order: lapack_memory_order, fact: string, n: c_int, d: [] real(64), e: [] real(64), df: [] real(64), ef: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dptsvx for the type real(64).
    

.. function:: proc ptsvx(matrix_order: lapack_memory_order, fact: string, n: c_int, d: [] real(32), e: [] complex(64), df: [] real(32), ef: [] complex(64), b: [] complex(64), x: [] complex(64), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cptsvx for the type complex(64).
    

.. function:: proc ptsvx(matrix_order: lapack_memory_order, fact: string, n: c_int, d: [] real(64), e: [] complex(128), df: [] real(64), ef: [] complex(128), b: [] complex(128), x: [] complex(128), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zptsvx for the type complex(128).
    

.. function:: proc pttrf(n: c_int, d: [] real(32), e: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spttrf for the type real(32).
    

.. function:: proc pttrf(n: c_int, d: [] real(64), e: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpttrf for the type real(64).
    

.. function:: proc pttrf(n: c_int, d: [] real(32), e: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpttrf for the type complex(64).
    

.. function:: proc pttrf(n: c_int, d: [] real(64), e: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpttrf for the type complex(128).
    

.. function:: proc pttrs(matrix_order: lapack_memory_order, n: c_int, d: [] real(32), e: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_spttrs for the type real(32).
    

.. function:: proc pttrs(matrix_order: lapack_memory_order, n: c_int, d: [] real(64), e: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dpttrs for the type real(64).
    

.. function:: proc pttrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, d: [] real(32), e: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cpttrs for the type complex(64).
    

.. function:: proc pttrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, d: [] real(64), e: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zpttrs for the type complex(128).
    

.. function:: proc sbev(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, kd: c_int, ab: [] real(32), w: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssbev for the type real(32).
    

.. function:: proc sbev(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, kd: c_int, ab: [] real(64), w: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsbev for the type real(64).
    

.. function:: proc sbevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, kd: c_int, ab: [] real(32), w: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssbevd for the type real(32).
    

.. function:: proc sbevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, kd: c_int, ab: [] real(64), w: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsbevd for the type real(64).
    

.. function:: proc sbevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, n: c_int, kd: c_int, ab: [] real(32), q: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] real(32), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_ssbevx for the type real(32).
    

.. function:: proc sbevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, n: c_int, kd: c_int, ab: [] real(64), q: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] real(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dsbevx for the type real(64).
    

.. function:: proc sbgst(matrix_order: lapack_memory_order, vect: string, uplo: string, ka: c_int, kb: c_int, ab: [] real(32), bb: [] real(32), x: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssbgst for the type real(32).
    

.. function:: proc sbgst(matrix_order: lapack_memory_order, vect: string, uplo: string, ka: c_int, kb: c_int, ab: [] real(64), bb: [] real(64), x: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsbgst for the type real(64).
    

.. function:: proc sbgv(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ka: c_int, kb: c_int, ab: [] real(32), bb: [] real(32), w: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssbgv for the type real(32).
    

.. function:: proc sbgv(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ka: c_int, kb: c_int, ab: [] real(64), bb: [] real(64), w: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsbgv for the type real(64).
    

.. function:: proc sbgvd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ka: c_int, kb: c_int, ab: [] real(32), bb: [] real(32), w: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssbgvd for the type real(32).
    

.. function:: proc sbgvd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ka: c_int, kb: c_int, ab: [] real(64), bb: [] real(64), w: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsbgvd for the type real(64).
    

.. function:: proc sbgvx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, ka: c_int, kb: c_int, ab: [] real(32), bb: [] real(32), q: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] real(32), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_ssbgvx for the type real(32).
    

.. function:: proc sbgvx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, ka: c_int, kb: c_int, ab: [] real(64), bb: [] real(64), q: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] real(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dsbgvx for the type real(64).
    

.. function:: proc sbtrd(matrix_order: lapack_memory_order, vect: string, uplo: string, n: c_int, kd: c_int, ab: [] real(32), d: [] real(32), e: [] real(32), q: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssbtrd for the type real(32).
    

.. function:: proc sbtrd(matrix_order: lapack_memory_order, vect: string, uplo: string, n: c_int, kd: c_int, ab: [] real(64), d: [] real(64), e: [] real(64), q: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsbtrd for the type real(64).
    

.. function:: proc sfrk(matrix_order: lapack_memory_order, transr: string, uplo: string, trans: string, alpha: real(32), a: [] real(32), beta: real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssfrk for the type real(32).
    

.. function:: proc sfrk(matrix_order: lapack_memory_order, transr: string, uplo: string, trans: string, alpha: real(64), a: [] real(64), beta: real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsfrk for the type real(64).
    

.. function:: proc spcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), ipiv: [] c_int, anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_sspcon for the type real(32).
    

.. function:: proc spcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), ipiv: [] c_int, anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dspcon for the type real(64).
    

.. function:: proc spcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), ipiv: [] c_int, anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_cspcon for the type complex(64).
    

.. function:: proc spcon(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), ipiv: [] c_int, anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zspcon for the type complex(128).
    

.. function:: proc spev(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ap: [] real(32), w: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sspev for the type real(32).
    

.. function:: proc spev(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ap: [] real(64), w: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dspev for the type real(64).
    

.. function:: proc spevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ap: [] real(32), w: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sspevd for the type real(32).
    

.. function:: proc spevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, n: c_int, ap: [] real(64), w: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dspevd for the type real(64).
    

.. function:: proc spevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, n: c_int, ap: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] real(32), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sspevx for the type real(32).
    

.. function:: proc spevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, n: c_int, ap: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] real(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dspevx for the type real(64).
    

.. function:: proc spgst(matrix_order: lapack_memory_order, itype: c_int, uplo: string, n: c_int, ap: [] real(32), bp: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sspgst for the type real(32).
    

.. function:: proc spgst(matrix_order: lapack_memory_order, itype: c_int, uplo: string, n: c_int, ap: [] real(64), bp: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dspgst for the type real(64).
    

.. function:: proc spgv(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, n: c_int, ap: [] real(32), bp: [] real(32), w: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sspgv for the type real(32).
    

.. function:: proc spgv(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, n: c_int, ap: [] real(64), bp: [] real(64), w: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dspgv for the type real(64).
    

.. function:: proc spgvd(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, n: c_int, ap: [] real(32), bp: [] real(32), w: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sspgvd for the type real(32).
    

.. function:: proc spgvd(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, n: c_int, ap: [] real(64), bp: [] real(64), w: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dspgvd for the type real(64).
    

.. function:: proc spgvx(matrix_order: lapack_memory_order, itype: c_int, jobz: string, range: string, uplo: string, n: c_int, ap: [] real(32), bp: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] real(32), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sspgvx for the type real(32).
    

.. function:: proc spgvx(matrix_order: lapack_memory_order, itype: c_int, jobz: string, range: string, uplo: string, n: c_int, ap: [] real(64), bp: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] real(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dspgvx for the type real(64).
    

.. function:: proc sprfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), afp: [] real(32), ipiv: [] c_int, b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssprfs for the type real(32).
    

.. function:: proc sprfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), afp: [] real(64), ipiv: [] c_int, b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsprfs for the type real(64).
    

.. function:: proc sprfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), afp: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_csprfs for the type complex(64).
    

.. function:: proc sprfs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), afp: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zsprfs for the type complex(128).
    

.. function:: proc spsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), ipiv: [] c_int, b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sspsv for the type real(32).
    

.. function:: proc spsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), ipiv: [] c_int, b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dspsv for the type real(64).
    

.. function:: proc spsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cspsv for the type complex(64).
    

.. function:: proc spsv(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zspsv for the type complex(128).
    

.. function:: proc spsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, ap: [] real(32), afp: [] real(32), ipiv: [] c_int, b: [] real(32), x: [] real(32), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sspsvx for the type real(32).
    

.. function:: proc spsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, ap: [] real(64), afp: [] real(64), ipiv: [] c_int, b: [] real(64), x: [] real(64), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dspsvx for the type real(64).
    

.. function:: proc spsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, ap: [] complex(64), afp: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cspsvx for the type complex(64).
    

.. function:: proc spsvx(matrix_order: lapack_memory_order, fact: string, uplo: string, n: c_int, ap: [] complex(128), afp: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zspsvx for the type complex(128).
    

.. function:: proc sptrd(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), d: [] real(32), e: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssptrd for the type real(32).
    

.. function:: proc sptrd(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), d: [] real(64), e: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsptrd for the type real(64).
    

.. function:: proc sptrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_ssptrf for the type real(32).
    

.. function:: proc sptrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dsptrf for the type real(64).
    

.. function:: proc sptrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_csptrf for the type complex(64).
    

.. function:: proc sptrf(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zsptrf for the type complex(128).
    

.. function:: proc sptri(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_ssptri for the type real(32).
    

.. function:: proc sptri(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dsptri for the type real(64).
    

.. function:: proc sptri(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_csptri for the type complex(64).
    

.. function:: proc sptri(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zsptri for the type complex(128).
    

.. function:: proc sptrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(32), ipiv: [] c_int, b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssptrs for the type real(32).
    

.. function:: proc sptrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] real(64), ipiv: [] c_int, b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsptrs for the type real(64).
    

.. function:: proc sptrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_csptrs for the type complex(64).
    

.. function:: proc sptrs(matrix_order: lapack_memory_order, uplo: string, n: c_int, ap: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zsptrs for the type complex(128).
    

.. function:: proc stebz(range: string, order: string, n: c_int, vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), d: [] real(32), e: [] real(32), ref m: c_int, ref nsplit: c_int, w: [] real(32), iblock: [] c_int, isplit: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sstebz for the type real(32).
    

.. function:: proc stebz(range: string, order: string, n: c_int, vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), d: [] real(64), e: [] real(64), ref m: c_int, ref nsplit: c_int, w: [] real(64), iblock: [] c_int, isplit: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dstebz for the type real(64).
    

.. function:: proc stedc(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(32), e: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sstedc for the type real(32).
    

.. function:: proc stedc(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(64), e: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dstedc for the type real(64).
    

.. function:: proc stedc(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(32), e: [] real(32), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cstedc for the type complex(64).
    

.. function:: proc stedc(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(64), e: [] real(64), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zstedc for the type complex(128).
    

.. function:: proc stegr(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(32), e: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] real(32), isuppz: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sstegr for the type real(32).
    

.. function:: proc stegr(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(64), e: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] real(64), isuppz: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dstegr for the type real(64).
    

.. function:: proc stegr(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(32), e: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] complex(64), isuppz: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_cstegr for the type complex(64).
    

.. function:: proc stegr(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(64), e: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] complex(128), isuppz: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zstegr for the type complex(128).
    

.. function:: proc stein(matrix_order: lapack_memory_order, n: c_int, d: [] real(32), e: [] real(32), m: c_int, w: [] real(32), iblock: [] c_int, isplit: [] c_int, z: [] real(32), ifailv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sstein for the type real(32).
    

.. function:: proc stein(matrix_order: lapack_memory_order, n: c_int, d: [] real(64), e: [] real(64), m: c_int, w: [] real(64), iblock: [] c_int, isplit: [] c_int, z: [] real(64), ifailv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dstein for the type real(64).
    

.. function:: proc stein(matrix_order: lapack_memory_order, n: c_int, d: [] real(32), e: [] real(32), m: c_int, w: [] real(32), iblock: [] c_int, isplit: [] c_int, z: [] complex(64), ifailv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_cstein for the type complex(64).
    

.. function:: proc stein(matrix_order: lapack_memory_order, n: c_int, d: [] real(64), e: [] real(64), m: c_int, w: [] real(64), iblock: [] c_int, isplit: [] c_int, z: [] complex(128), ifailv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zstein for the type complex(128).
    

.. function:: proc stemr(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(32), e: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, ref m: c_int, w: [] real(32), z: [] real(32), nzc: c_int, isuppz: [] c_int, ref tryrac: c_int): c_int

   
   Wrapped procedure of LAPACKE_sstemr for the type real(32).
    

.. function:: proc stemr(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(64), e: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, ref m: c_int, w: [] real(64), z: [] real(64), nzc: c_int, isuppz: [] c_int, ref tryrac: c_int): c_int

   
   Wrapped procedure of LAPACKE_dstemr for the type real(64).
    

.. function:: proc stemr(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(32), e: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, ref m: c_int, w: [] real(32), z: [] complex(64), nzc: c_int, isuppz: [] c_int, ref tryrac: c_int): c_int

   
   Wrapped procedure of LAPACKE_cstemr for the type complex(64).
    

.. function:: proc stemr(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(64), e: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, ref m: c_int, w: [] real(64), z: [] complex(128), nzc: c_int, isuppz: [] c_int, ref tryrac: c_int): c_int

   
   Wrapped procedure of LAPACKE_zstemr for the type complex(128).
    

.. function:: proc steqr(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(32), e: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssteqr for the type real(32).
    

.. function:: proc steqr(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(64), e: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsteqr for the type real(64).
    

.. function:: proc steqr(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(32), e: [] real(32), z: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_csteqr for the type complex(64).
    

.. function:: proc steqr(matrix_order: lapack_memory_order, compz: string, n: c_int, d: [] real(64), e: [] real(64), z: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zsteqr for the type complex(128).
    

.. function:: proc sterf(n: c_int, d: [] real(32), e: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssterf for the type real(32).
    

.. function:: proc sterf(n: c_int, d: [] real(64), e: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsterf for the type real(64).
    

.. function:: proc stev(matrix_order: lapack_memory_order, jobz: string, n: c_int, d: [] real(32), e: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sstev for the type real(32).
    

.. function:: proc stev(matrix_order: lapack_memory_order, jobz: string, n: c_int, d: [] real(64), e: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dstev for the type real(64).
    

.. function:: proc stevd(matrix_order: lapack_memory_order, jobz: string, n: c_int, d: [] real(32), e: [] real(32), z: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sstevd for the type real(32).
    

.. function:: proc stevd(matrix_order: lapack_memory_order, jobz: string, n: c_int, d: [] real(64), e: [] real(64), z: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dstevd for the type real(64).
    

.. function:: proc stevr(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(32), e: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] real(32), isuppz: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sstevr for the type real(32).
    

.. function:: proc stevr(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(64), e: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] real(64), isuppz: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dstevr for the type real(64).
    

.. function:: proc stevx(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(32), e: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] real(32), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_sstevx for the type real(32).
    

.. function:: proc stevx(matrix_order: lapack_memory_order, jobz: string, range: string, n: c_int, d: [] real(64), e: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] real(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dstevx for the type real(64).
    

.. function:: proc sycon(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), ipiv: [] c_int, anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssycon for the type real(32).
    

.. function:: proc sycon(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), ipiv: [] c_int, anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsycon for the type real(64).
    

.. function:: proc sycon(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int, anorm: real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_csycon for the type complex(64).
    

.. function:: proc sycon(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int, anorm: real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zsycon for the type complex(128).
    

.. function:: proc syequb(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), s: [] real(32), ref scond: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssyequb for the type real(32).
    

.. function:: proc syequb(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), s: [] real(64), ref scond: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsyequb for the type real(64).
    

.. function:: proc syequb(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), s: [] real(32), ref scond: real(32), ref amax: real(32)): c_int

   
   Wrapped procedure of LAPACKE_csyequb for the type complex(64).
    

.. function:: proc syequb(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), s: [] real(64), ref scond: real(64), ref amax: real(64)): c_int

   
   Wrapped procedure of LAPACKE_zsyequb for the type complex(128).
    

.. function:: proc syev(matrix_order: lapack_memory_order, jobz: string, uplo: string, a: [] real(32), w: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssyev for the type real(32).
    

.. function:: proc syev(matrix_order: lapack_memory_order, jobz: string, uplo: string, a: [] real(64), w: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsyev for the type real(64).
    

.. function:: proc syevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, a: [] real(32), w: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssyevd for the type real(32).
    

.. function:: proc syevd(matrix_order: lapack_memory_order, jobz: string, uplo: string, a: [] real(64), w: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsyevd for the type real(64).
    

.. function:: proc syevr(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, a: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] real(32), isuppz: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_ssyevr for the type real(32).
    

.. function:: proc syevr(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, a: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] real(64), isuppz: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dsyevr for the type real(64).
    

.. function:: proc syevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, a: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] real(32), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_ssyevx for the type real(32).
    

.. function:: proc syevx(matrix_order: lapack_memory_order, jobz: string, range: string, uplo: string, a: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] real(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dsyevx for the type real(64).
    

.. function:: proc sygst(matrix_order: lapack_memory_order, itype: c_int, uplo: string, a: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssygst for the type real(32).
    

.. function:: proc sygst(matrix_order: lapack_memory_order, itype: c_int, uplo: string, a: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsygst for the type real(64).
    

.. function:: proc sygv(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, a: [] real(32), b: [] real(32), w: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssygv for the type real(32).
    

.. function:: proc sygv(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, a: [] real(64), b: [] real(64), w: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsygv for the type real(64).
    

.. function:: proc sygvd(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, a: [] real(32), b: [] real(32), w: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssygvd for the type real(32).
    

.. function:: proc sygvd(matrix_order: lapack_memory_order, itype: c_int, jobz: string, uplo: string, a: [] real(64), b: [] real(64), w: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsygvd for the type real(64).
    

.. function:: proc sygvx(matrix_order: lapack_memory_order, itype: c_int, jobz: string, range: string, uplo: string, n: c_int, a: [] real(32), b: [] real(32), vl: real(32), vu: real(32), il: c_int, iu: c_int, abstol: real(32), ref m: c_int, w: [] real(32), z: [] real(32), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_ssygvx for the type real(32).
    

.. function:: proc sygvx(matrix_order: lapack_memory_order, itype: c_int, jobz: string, range: string, uplo: string, n: c_int, a: [] real(64), b: [] real(64), vl: real(64), vu: real(64), il: c_int, iu: c_int, abstol: real(64), ref m: c_int, w: [] real(64), z: [] real(64), ifail: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dsygvx for the type real(64).
    

.. function:: proc syrfs(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), af: [] real(32), ipiv: [] c_int, b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssyrfs for the type real(32).
    

.. function:: proc syrfs(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), af: [] real(64), ipiv: [] c_int, b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsyrfs for the type real(64).
    

.. function:: proc syrfs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_csyrfs for the type complex(64).
    

.. function:: proc syrfs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zsyrfs for the type complex(128).
    

.. function:: proc syrfsx(matrix_order: lapack_memory_order, uplo: string, equed: string, a: [] real(32), af: [] real(32), ipiv: [] c_int, s: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssyrfsx for the type real(32).
    

.. function:: proc syrfsx(matrix_order: lapack_memory_order, uplo: string, equed: string, a: [] real(64), af: [] real(64), ipiv: [] c_int, s: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsyrfsx for the type real(64).
    

.. function:: proc syrfsx(matrix_order: lapack_memory_order, uplo: string, equed: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, s: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_csyrfsx for the type complex(64).
    

.. function:: proc syrfsx(matrix_order: lapack_memory_order, uplo: string, equed: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, s: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zsyrfsx for the type complex(128).
    

.. function:: proc sysv(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), ipiv: [] c_int, b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssysv for the type real(32).
    

.. function:: proc sysv(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), ipiv: [] c_int, b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsysv for the type real(64).
    

.. function:: proc sysv(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_csysv for the type complex(64).
    

.. function:: proc sysv(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zsysv for the type complex(128).
    

.. function:: proc sysvx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] real(32), af: [] real(32), ipiv: [] c_int, b: [] real(32), x: [] real(32), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssysvx for the type real(32).
    

.. function:: proc sysvx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] real(64), af: [] real(64), ipiv: [] c_int, b: [] real(64), x: [] real(64), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsysvx for the type real(64).
    

.. function:: proc sysvx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, b: [] complex(64), x: [] complex(64), ref rcond: real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_csysvx for the type complex(64).
    

.. function:: proc sysvx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, b: [] complex(128), x: [] complex(128), ref rcond: real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zsysvx for the type complex(128).
    

.. function:: proc sysvxx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] real(32), af: [] real(32), ipiv: [] c_int, ref equed: string, s: [] real(32), b: [] real(32), x: [] real(32), ref rcond: real(32), ref rpvgrw: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssysvxx for the type real(32).
    

.. function:: proc sysvxx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] real(64), af: [] real(64), ipiv: [] c_int, ref equed: string, s: [] real(64), b: [] real(64), x: [] real(64), ref rcond: real(64), ref rpvgrw: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsysvxx for the type real(64).
    

.. function:: proc sysvxx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(64), af: [] complex(64), ipiv: [] c_int, ref equed: string, s: [] real(32), b: [] complex(64), x: [] complex(64), ref rcond: real(32), ref rpvgrw: real(32), berr: [] real(32), n_err_bnds: c_int, err_bnds_norm: [] real(32), err_bnds_comp: [] real(32), nparams: c_int, params: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_csysvxx for the type complex(64).
    

.. function:: proc sysvxx(matrix_order: lapack_memory_order, fact: string, uplo: string, a: [] complex(128), af: [] complex(128), ipiv: [] c_int, ref equed: string, s: [] real(64), b: [] complex(128), x: [] complex(128), ref rcond: real(64), ref rpvgrw: real(64), berr: [] real(64), n_err_bnds: c_int, err_bnds_norm: [] real(64), err_bnds_comp: [] real(64), nparams: c_int, params: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zsysvxx for the type complex(128).
    

.. function:: proc sytrd(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), d: [] real(32), e: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssytrd for the type real(32).
    

.. function:: proc sytrd(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), d: [] real(64), e: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsytrd for the type real(64).
    

.. function:: proc sytrf(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_ssytrf for the type real(32).
    

.. function:: proc sytrf(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dsytrf for the type real(64).
    

.. function:: proc sytrf(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_csytrf for the type complex(64).
    

.. function:: proc sytrf(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zsytrf for the type complex(128).
    

.. function:: proc sytri(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_ssytri for the type real(32).
    

.. function:: proc sytri(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dsytri for the type real(64).
    

.. function:: proc sytri(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_csytri for the type complex(64).
    

.. function:: proc sytri(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zsytri for the type complex(128).
    

.. function:: proc sytrs(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), ipiv: [] c_int, b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssytrs for the type real(32).
    

.. function:: proc sytrs(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), ipiv: [] c_int, b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsytrs for the type real(64).
    

.. function:: proc sytrs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_csytrs for the type complex(64).
    

.. function:: proc sytrs(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zsytrs for the type complex(128).
    

.. function:: proc tbcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, n: c_int, kd: c_int, ab: [] real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_stbcon for the type real(32).
    

.. function:: proc tbcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, n: c_int, kd: c_int, ab: [] real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtbcon for the type real(64).
    

.. function:: proc tbcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, n: c_int, kd: c_int, ab: [] complex(64), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_ctbcon for the type complex(64).
    

.. function:: proc tbcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, n: c_int, kd: c_int, ab: [] complex(128), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_ztbcon for the type complex(128).
    

.. function:: proc tbrfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, kd: c_int, ab: [] real(32), b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stbrfs for the type real(32).
    

.. function:: proc tbrfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, kd: c_int, ab: [] real(64), b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtbrfs for the type real(64).
    

.. function:: proc tbrfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, kd: c_int, ab: [] complex(64), b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ctbrfs for the type complex(64).
    

.. function:: proc tbrfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, kd: c_int, ab: [] complex(128), b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_ztbrfs for the type complex(128).
    

.. function:: proc tbtrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, kd: c_int, ab: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stbtrs for the type real(32).
    

.. function:: proc tbtrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, kd: c_int, ab: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtbtrs for the type real(64).
    

.. function:: proc tbtrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, kd: c_int, ab: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctbtrs for the type complex(64).
    

.. function:: proc tbtrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, kd: c_int, ab: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztbtrs for the type complex(128).
    

.. function:: proc tfsm(matrix_order: lapack_memory_order, transr: string, side: string, uplo: string, trans: string, diag: string, alpha: real(32), a: [] real(32), b: [] real(32), ldb: c_int): c_int

   
   Wrapped procedure of LAPACKE_stfsm for the type real(32).
    

.. function:: proc tfsm(matrix_order: lapack_memory_order, transr: string, side: string, uplo: string, trans: string, diag: string, alpha: real(64), a: [] real(64), b: [] real(64), ldb: c_int): c_int

   
   Wrapped procedure of LAPACKE_dtfsm for the type real(64).
    

.. function:: proc tfsm(matrix_order: lapack_memory_order, transr: string, side: string, uplo: string, trans: string, diag: string, alpha: complex(64), a: [] complex(64), b: [] complex(64), ldb: c_int): c_int

   
   Wrapped procedure of LAPACKE_ctfsm for the type complex(64).
    

.. function:: proc tfsm(matrix_order: lapack_memory_order, transr: string, side: string, uplo: string, trans: string, diag: string, alpha: complex(128), a: [] complex(128), b: [] complex(128), ldb: c_int): c_int

   
   Wrapped procedure of LAPACKE_ztfsm for the type complex(128).
    

.. function:: proc tftri(matrix_order: lapack_memory_order, transr: string, uplo: string, diag: string, a: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stftri for the type real(32).
    

.. function:: proc tftri(matrix_order: lapack_memory_order, transr: string, uplo: string, diag: string, a: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtftri for the type real(64).
    

.. function:: proc tftri(matrix_order: lapack_memory_order, transr: string, uplo: string, diag: string, a: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctftri for the type complex(64).
    

.. function:: proc tftri(matrix_order: lapack_memory_order, transr: string, uplo: string, diag: string, a: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztftri for the type complex(128).
    

.. function:: proc tfttp(matrix_order: lapack_memory_order, transr: string, uplo: string, n: c_int, arf: [] real(32), ap: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stfttp for the type real(32).
    

.. function:: proc tfttp(matrix_order: lapack_memory_order, transr: string, uplo: string, n: c_int, arf: [] real(64), ap: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtfttp for the type real(64).
    

.. function:: proc tfttp(matrix_order: lapack_memory_order, transr: string, uplo: string, n: c_int, arf: [] complex(64), ap: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctfttp for the type complex(64).
    

.. function:: proc tfttp(matrix_order: lapack_memory_order, transr: string, uplo: string, n: c_int, arf: [] complex(128), ap: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztfttp for the type complex(128).
    

.. function:: proc tfttr(matrix_order: lapack_memory_order, transr: string, uplo: string, arf: [] real(32), a: [] real(32), lda: c_int): c_int

   
   Wrapped procedure of LAPACKE_stfttr for the type real(32).
    

.. function:: proc tfttr(matrix_order: lapack_memory_order, transr: string, uplo: string, arf: [] real(64), a: [] real(64), lda: c_int): c_int

   
   Wrapped procedure of LAPACKE_dtfttr for the type real(64).
    

.. function:: proc tfttr(matrix_order: lapack_memory_order, transr: string, uplo: string, arf: [] complex(64), a: [] complex(64), lda: c_int): c_int

   
   Wrapped procedure of LAPACKE_ctfttr for the type complex(64).
    

.. function:: proc tfttr(matrix_order: lapack_memory_order, transr: string, uplo: string, arf: [] complex(128), a: [] complex(128), lda: c_int): c_int

   
   Wrapped procedure of LAPACKE_ztfttr for the type complex(128).
    

.. function:: proc tgevc(matrix_order: lapack_memory_order, side: string, howmny: string, chlapack_select: [] c_int, s: [] real(32), p: [] real(32), vl: [] real(32), vr: [] real(32), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_stgevc for the type real(32).
    

.. function:: proc tgevc(matrix_order: lapack_memory_order, side: string, howmny: string, chlapack_select: [] c_int, s: [] real(64), p: [] real(64), vl: [] real(64), vr: [] real(64), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_dtgevc for the type real(64).
    

.. function:: proc tgevc(matrix_order: lapack_memory_order, side: string, howmny: string, chlapack_select: [] c_int, s: [] complex(64), p: [] complex(64), vl: [] complex(64), vr: [] complex(64), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_ctgevc for the type complex(64).
    

.. function:: proc tgevc(matrix_order: lapack_memory_order, side: string, howmny: string, chlapack_select: [] c_int, s: [] complex(128), p: [] complex(128), vl: [] complex(128), vr: [] complex(128), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_ztgevc for the type complex(128).
    

.. function:: proc tgexc(matrix_order: lapack_memory_order, wantq: c_int, wantz: c_int, a: [] real(32), b: [] real(32), q: [] real(32), z: [] real(32), ref ifst: c_int, ref ilst: c_int): c_int

   
   Wrapped procedure of LAPACKE_stgexc for the type real(32).
    

.. function:: proc tgexc(matrix_order: lapack_memory_order, wantq: c_int, wantz: c_int, a: [] real(64), b: [] real(64), q: [] real(64), z: [] real(64), ref ifst: c_int, ref ilst: c_int): c_int

   
   Wrapped procedure of LAPACKE_dtgexc for the type real(64).
    

.. function:: proc tgexc(matrix_order: lapack_memory_order, wantq: c_int, wantz: c_int, a: [] complex(64), b: [] complex(64), q: [] complex(64), z: [] complex(64), ifst: c_int, ilst: c_int): c_int

   
   Wrapped procedure of LAPACKE_ctgexc for the type complex(64).
    

.. function:: proc tgexc(matrix_order: lapack_memory_order, wantq: c_int, wantz: c_int, a: [] complex(128), b: [] complex(128), q: [] complex(128), z: [] complex(128), ifst: c_int, ilst: c_int): c_int

   
   Wrapped procedure of LAPACKE_ztgexc for the type complex(128).
    

.. function:: proc tgsen(matrix_order: lapack_memory_order, ijob: c_int, wantq: c_int, wantz: c_int, chlapack_select: [] c_int, a: [] real(32), b: [] real(32), alphar: [] real(32), alphai: [] real(32), beta: [] real(32), q: [] real(32), z: [] real(32), ref m: c_int, ref pl: real(32), ref pr: real(32), dif: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stgsen for the type real(32).
    

.. function:: proc tgsen(matrix_order: lapack_memory_order, ijob: c_int, wantq: c_int, wantz: c_int, chlapack_select: [] c_int, a: [] real(64), b: [] real(64), alphar: [] real(64), alphai: [] real(64), beta: [] real(64), q: [] real(64), z: [] real(64), ref m: c_int, ref pl: real(64), ref pr: real(64), dif: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtgsen for the type real(64).
    

.. function:: proc tgsen(matrix_order: lapack_memory_order, ijob: c_int, wantq: c_int, wantz: c_int, chlapack_select: [] c_int, a: [] complex(64), b: [] complex(64), alpha: [] complex(64), beta: [] complex(64), q: [] complex(64), z: [] complex(64), ref m: c_int, ref pl: real(32), ref pr: real(32), dif: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ctgsen for the type complex(64).
    

.. function:: proc tgsen(matrix_order: lapack_memory_order, ijob: c_int, wantq: c_int, wantz: c_int, chlapack_select: [] c_int, a: [] complex(128), b: [] complex(128), alpha: [] complex(128), beta: [] complex(128), q: [] complex(128), z: [] complex(128), ref m: c_int, ref pl: real(64), ref pr: real(64), dif: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_ztgsen for the type complex(128).
    

.. function:: proc tgsja(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, k: c_int, l: c_int, a: [] real(32), b: [] real(32), tola: real(32), tolb: real(32), alpha: [] real(32), beta: [] real(32), u: [] real(32), v: [] real(32), q: [] real(32), ref ncycle: c_int): c_int

   
   Wrapped procedure of LAPACKE_stgsja for the type real(32).
    

.. function:: proc tgsja(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, k: c_int, l: c_int, a: [] real(64), b: [] real(64), tola: real(64), tolb: real(64), alpha: [] real(64), beta: [] real(64), u: [] real(64), v: [] real(64), q: [] real(64), ref ncycle: c_int): c_int

   
   Wrapped procedure of LAPACKE_dtgsja for the type real(64).
    

.. function:: proc tgsja(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, k: c_int, l: c_int, a: [] complex(64), b: [] complex(64), tola: real(32), tolb: real(32), alpha: [] real(32), beta: [] real(32), u: [] complex(64), v: [] complex(64), q: [] complex(64), ref ncycle: c_int): c_int

   
   Wrapped procedure of LAPACKE_ctgsja for the type complex(64).
    

.. function:: proc tgsja(matrix_order: lapack_memory_order, jobu: string, jobv: string, jobq: string, k: c_int, l: c_int, a: [] complex(128), b: [] complex(128), tola: real(64), tolb: real(64), alpha: [] real(64), beta: [] real(64), u: [] complex(128), v: [] complex(128), q: [] complex(128), ref ncycle: c_int): c_int

   
   Wrapped procedure of LAPACKE_ztgsja for the type complex(128).
    

.. function:: proc tgsna(matrix_order: lapack_memory_order, job: string, howmny: string, chlapack_select: [] c_int, n: c_int, a: [] real(32), b: [] real(32), vl: [] real(32), vr: [] real(32), s: [] real(32), dif: [] real(32), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_stgsna for the type real(32).
    

.. function:: proc tgsna(matrix_order: lapack_memory_order, job: string, howmny: string, chlapack_select: [] c_int, n: c_int, a: [] real(64), b: [] real(64), vl: [] real(64), vr: [] real(64), s: [] real(64), dif: [] real(64), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_dtgsna for the type real(64).
    

.. function:: proc tgsna(matrix_order: lapack_memory_order, job: string, howmny: string, chlapack_select: [] c_int, n: c_int, a: [] complex(64), b: [] complex(64), vl: [] complex(64), vr: [] complex(64), s: [] real(32), dif: [] real(32), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_ctgsna for the type complex(64).
    

.. function:: proc tgsna(matrix_order: lapack_memory_order, job: string, howmny: string, chlapack_select: [] c_int, n: c_int, a: [] complex(128), b: [] complex(128), vl: [] complex(128), vr: [] complex(128), s: [] real(64), dif: [] real(64), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_ztgsna for the type complex(128).
    

.. function:: proc tgsyl(matrix_order: lapack_memory_order, trans: string, ijob: c_int, a: [] real(32), b: [] real(32), c: [] real(32), d: [] real(32), e: [] real(32), f: [] real(32), ref scale: real(32), ref dif: real(32)): c_int

   
   Wrapped procedure of LAPACKE_stgsyl for the type real(32).
    

.. function:: proc tgsyl(matrix_order: lapack_memory_order, trans: string, ijob: c_int, a: [] real(64), b: [] real(64), c: [] real(64), d: [] real(64), e: [] real(64), f: [] real(64), ref scale: real(64), ref dif: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtgsyl for the type real(64).
    

.. function:: proc tgsyl(matrix_order: lapack_memory_order, trans: string, ijob: c_int, a: [] complex(64), b: [] complex(64), c: [] complex(64), d: [] complex(64), e: [] complex(64), f: [] complex(64), ref scale: real(32), ref dif: real(32)): c_int

   
   Wrapped procedure of LAPACKE_ctgsyl for the type complex(64).
    

.. function:: proc tgsyl(matrix_order: lapack_memory_order, trans: string, ijob: c_int, a: [] complex(128), b: [] complex(128), c: [] complex(128), d: [] complex(128), e: [] complex(128), f: [] complex(128), ref scale: real(64), ref dif: real(64)): c_int

   
   Wrapped procedure of LAPACKE_ztgsyl for the type complex(128).
    

.. function:: proc tpcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, n: c_int, ap: [] real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_stpcon for the type real(32).
    

.. function:: proc tpcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, n: c_int, ap: [] real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtpcon for the type real(64).
    

.. function:: proc tpcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, n: c_int, ap: [] complex(64), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_ctpcon for the type complex(64).
    

.. function:: proc tpcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, n: c_int, ap: [] complex(128), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_ztpcon for the type complex(128).
    

.. function:: proc tprfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, ap: [] real(32), b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stprfs for the type real(32).
    

.. function:: proc tprfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, ap: [] real(64), b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtprfs for the type real(64).
    

.. function:: proc tprfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, ap: [] complex(64), b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ctprfs for the type complex(64).
    

.. function:: proc tprfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, ap: [] complex(128), b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_ztprfs for the type complex(128).
    

.. function:: proc tptri(matrix_order: lapack_memory_order, uplo: string, diag: string, n: c_int, ap: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stptri for the type real(32).
    

.. function:: proc tptri(matrix_order: lapack_memory_order, uplo: string, diag: string, n: c_int, ap: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtptri for the type real(64).
    

.. function:: proc tptri(matrix_order: lapack_memory_order, uplo: string, diag: string, n: c_int, ap: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctptri for the type complex(64).
    

.. function:: proc tptri(matrix_order: lapack_memory_order, uplo: string, diag: string, n: c_int, ap: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztptri for the type complex(128).
    

.. function:: proc tptrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, ap: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stptrs for the type real(32).
    

.. function:: proc tptrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, ap: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtptrs for the type real(64).
    

.. function:: proc tptrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, ap: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctptrs for the type complex(64).
    

.. function:: proc tptrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, n: c_int, ap: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztptrs for the type complex(128).
    

.. function:: proc tpttf(matrix_order: lapack_memory_order, transr: string, uplo: string, n: c_int, ap: [] real(32), arf: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stpttf for the type real(32).
    

.. function:: proc tpttf(matrix_order: lapack_memory_order, transr: string, uplo: string, n: c_int, ap: [] real(64), arf: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtpttf for the type real(64).
    

.. function:: proc tpttf(matrix_order: lapack_memory_order, transr: string, uplo: string, n: c_int, ap: [] complex(64), arf: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctpttf for the type complex(64).
    

.. function:: proc tpttf(matrix_order: lapack_memory_order, transr: string, uplo: string, n: c_int, ap: [] complex(128), arf: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztpttf for the type complex(128).
    

.. function:: proc tpttr(matrix_order: lapack_memory_order, uplo: string, ap: [] real(32), a: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stpttr for the type real(32).
    

.. function:: proc tpttr(matrix_order: lapack_memory_order, uplo: string, ap: [] real(64), a: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtpttr for the type real(64).
    

.. function:: proc tpttr(matrix_order: lapack_memory_order, uplo: string, ap: [] complex(64), a: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctpttr for the type complex(64).
    

.. function:: proc tpttr(matrix_order: lapack_memory_order, uplo: string, ap: [] complex(128), a: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztpttr for the type complex(128).
    

.. function:: proc trcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, a: [] real(32), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_strcon for the type real(32).
    

.. function:: proc trcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, a: [] real(64), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtrcon for the type real(64).
    

.. function:: proc trcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, a: [] complex(64), ref rcond: real(32)): c_int

   
   Wrapped procedure of LAPACKE_ctrcon for the type complex(64).
    

.. function:: proc trcon(matrix_order: lapack_memory_order, norm: string, uplo: string, diag: string, a: [] complex(128), ref rcond: real(64)): c_int

   
   Wrapped procedure of LAPACKE_ztrcon for the type complex(128).
    

.. function:: proc trevc(matrix_order: lapack_memory_order, side: string, howmny: string, chlapack_select: [] c_int, t: [] real(32), vl: [] real(32), vr: [] real(32), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_strevc for the type real(32).
    

.. function:: proc trevc(matrix_order: lapack_memory_order, side: string, howmny: string, chlapack_select: [] c_int, t: [] real(64), vl: [] real(64), vr: [] real(64), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_dtrevc for the type real(64).
    

.. function:: proc trevc(matrix_order: lapack_memory_order, side: string, howmny: string, chlapack_select: [] c_int, t: [] complex(64), vl: [] complex(64), vr: [] complex(64), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_ctrevc for the type complex(64).
    

.. function:: proc trevc(matrix_order: lapack_memory_order, side: string, howmny: string, chlapack_select: [] c_int, t: [] complex(128), vl: [] complex(128), vr: [] complex(128), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_ztrevc for the type complex(128).
    

.. function:: proc trexc(matrix_order: lapack_memory_order, compq: string, t: [] real(32), q: [] real(32), ref ifst: c_int, ref ilst: c_int): c_int

   
   Wrapped procedure of LAPACKE_strexc for the type real(32).
    

.. function:: proc trexc(matrix_order: lapack_memory_order, compq: string, t: [] real(64), q: [] real(64), ref ifst: c_int, ref ilst: c_int): c_int

   
   Wrapped procedure of LAPACKE_dtrexc for the type real(64).
    

.. function:: proc trexc(matrix_order: lapack_memory_order, compq: string, t: [] complex(64), q: [] complex(64), ifst: c_int, ilst: c_int): c_int

   
   Wrapped procedure of LAPACKE_ctrexc for the type complex(64).
    

.. function:: proc trexc(matrix_order: lapack_memory_order, compq: string, t: [] complex(128), q: [] complex(128), ifst: c_int, ilst: c_int): c_int

   
   Wrapped procedure of LAPACKE_ztrexc for the type complex(128).
    

.. function:: proc trrfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, a: [] real(32), b: [] real(32), x: [] real(32), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_strrfs for the type real(32).
    

.. function:: proc trrfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, a: [] real(64), b: [] real(64), x: [] real(64), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtrrfs for the type real(64).
    

.. function:: proc trrfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, a: [] complex(64), b: [] complex(64), x: [] complex(64), ferr: [] real(32), berr: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ctrrfs for the type complex(64).
    

.. function:: proc trrfs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, a: [] complex(128), b: [] complex(128), x: [] complex(128), ferr: [] real(64), berr: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_ztrrfs for the type complex(128).
    

.. function:: proc trsen(matrix_order: lapack_memory_order, job: string, compq: string, chlapack_select: [] c_int, t: [] real(32), q: [] real(32), wr: [] real(32), wi: [] real(32), ref m: c_int, ref s: real(32), ref sep: real(32)): c_int

   
   Wrapped procedure of LAPACKE_strsen for the type real(32).
    

.. function:: proc trsen(matrix_order: lapack_memory_order, job: string, compq: string, chlapack_select: [] c_int, t: [] real(64), q: [] real(64), wr: [] real(64), wi: [] real(64), ref m: c_int, ref s: real(64), ref sep: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtrsen for the type real(64).
    

.. function:: proc trsen(matrix_order: lapack_memory_order, job: string, compq: string, chlapack_select: [] c_int, t: [] complex(64), q: [] complex(64), w: [] complex(64), ref m: c_int, ref s: real(32), ref sep: real(32)): c_int

   
   Wrapped procedure of LAPACKE_ctrsen for the type complex(64).
    

.. function:: proc trsen(matrix_order: lapack_memory_order, job: string, compq: string, chlapack_select: [] c_int, t: [] complex(128), q: [] complex(128), w: [] complex(128), ref m: c_int, ref s: real(64), ref sep: real(64)): c_int

   
   Wrapped procedure of LAPACKE_ztrsen for the type complex(128).
    

.. function:: proc trsna(matrix_order: lapack_memory_order, job: string, howmny: string, chlapack_select: [] c_int, t: [] real(32), vl: [] real(32), vr: [] real(32), s: [] real(32), sep: [] real(32), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_strsna for the type real(32).
    

.. function:: proc trsna(matrix_order: lapack_memory_order, job: string, howmny: string, chlapack_select: [] c_int, t: [] real(64), vl: [] real(64), vr: [] real(64), s: [] real(64), sep: [] real(64), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_dtrsna for the type real(64).
    

.. function:: proc trsna(matrix_order: lapack_memory_order, job: string, howmny: string, chlapack_select: [] c_int, t: [] complex(64), vl: [] complex(64), vr: [] complex(64), s: [] real(32), sep: [] real(32), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_ctrsna for the type complex(64).
    

.. function:: proc trsna(matrix_order: lapack_memory_order, job: string, howmny: string, chlapack_select: [] c_int, t: [] complex(128), vl: [] complex(128), vr: [] complex(128), s: [] real(64), sep: [] real(64), mm: c_int, ref m: c_int): c_int

   
   Wrapped procedure of LAPACKE_ztrsna for the type complex(128).
    

.. function:: proc trsyl(matrix_order: lapack_memory_order, trana: string, tranb: string, isgn: c_int, a: [] real(32), b: [] real(32), c: [] real(32), ref scale: real(32)): c_int

   
   Wrapped procedure of LAPACKE_strsyl for the type real(32).
    

.. function:: proc trsyl(matrix_order: lapack_memory_order, trana: string, tranb: string, isgn: c_int, a: [] real(64), b: [] real(64), c: [] real(64), ref scale: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtrsyl for the type real(64).
    

.. function:: proc trsyl(matrix_order: lapack_memory_order, trana: string, tranb: string, isgn: c_int, a: [] complex(64), b: [] complex(64), c: [] complex(64), ref scale: real(32)): c_int

   
   Wrapped procedure of LAPACKE_ctrsyl for the type complex(64).
    

.. function:: proc trsyl(matrix_order: lapack_memory_order, trana: string, tranb: string, isgn: c_int, a: [] complex(128), b: [] complex(128), c: [] complex(128), ref scale: real(64)): c_int

   
   Wrapped procedure of LAPACKE_ztrsyl for the type complex(128).
    

.. function:: proc trtri(matrix_order: lapack_memory_order, uplo: string, diag: string, a: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_strtri for the type real(32).
    

.. function:: proc trtri(matrix_order: lapack_memory_order, uplo: string, diag: string, a: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtrtri for the type real(64).
    

.. function:: proc trtri(matrix_order: lapack_memory_order, uplo: string, diag: string, a: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctrtri for the type complex(64).
    

.. function:: proc trtri(matrix_order: lapack_memory_order, uplo: string, diag: string, a: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztrtri for the type complex(128).
    

.. function:: proc trtrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, a: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_strtrs for the type real(32).
    

.. function:: proc trtrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, a: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtrtrs for the type real(64).
    

.. function:: proc trtrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, a: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctrtrs for the type complex(64).
    

.. function:: proc trtrs(matrix_order: lapack_memory_order, uplo: string, trans: string, diag: string, a: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztrtrs for the type complex(128).
    

.. function:: proc trttf(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] real(32), lda: c_int, arf: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_strttf for the type real(32).
    

.. function:: proc trttf(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] real(64), lda: c_int, arf: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtrttf for the type real(64).
    

.. function:: proc trttf(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] complex(64), lda: c_int, arf: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctrttf for the type complex(64).
    

.. function:: proc trttf(matrix_order: lapack_memory_order, transr: string, uplo: string, a: [] complex(128), lda: c_int, arf: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztrttf for the type complex(128).
    

.. function:: proc trttp(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), ap: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_strttp for the type real(32).
    

.. function:: proc trttp(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), ap: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtrttp for the type real(64).
    

.. function:: proc trttp(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ap: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctrttp for the type complex(64).
    

.. function:: proc trttp(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ap: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztrttp for the type complex(128).
    

.. function:: proc tzrzf(matrix_order: lapack_memory_order, a: [] real(32), tau: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stzrzf for the type real(32).
    

.. function:: proc tzrzf(matrix_order: lapack_memory_order, a: [] real(64), tau: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtzrzf for the type real(64).
    

.. function:: proc tzrzf(matrix_order: lapack_memory_order, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctzrzf for the type complex(64).
    

.. function:: proc tzrzf(matrix_order: lapack_memory_order, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztzrzf for the type complex(128).
    

.. function:: proc ungbr(matrix_order: lapack_memory_order, vect: string, k: c_int, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cungbr for the type complex(64).
    

.. function:: proc ungbr(matrix_order: lapack_memory_order, vect: string, k: c_int, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zungbr for the type complex(128).
    

.. function:: proc unghr(matrix_order: lapack_memory_order, n: c_int, ilo: c_int, ihi: c_int, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cunghr for the type complex(64).
    

.. function:: proc unghr(matrix_order: lapack_memory_order, n: c_int, ilo: c_int, ihi: c_int, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zunghr for the type complex(128).
    

.. function:: proc unglq(matrix_order: lapack_memory_order, k: c_int, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cunglq for the type complex(64).
    

.. function:: proc unglq(matrix_order: lapack_memory_order, k: c_int, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zunglq for the type complex(128).
    

.. function:: proc ungql(matrix_order: lapack_memory_order, k: c_int, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cungql for the type complex(64).
    

.. function:: proc ungql(matrix_order: lapack_memory_order, k: c_int, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zungql for the type complex(128).
    

.. function:: proc ungqr(matrix_order: lapack_memory_order, k: c_int, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cungqr for the type complex(64).
    

.. function:: proc ungqr(matrix_order: lapack_memory_order, k: c_int, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zungqr for the type complex(128).
    

.. function:: proc ungrq(matrix_order: lapack_memory_order, k: c_int, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cungrq for the type complex(64).
    

.. function:: proc ungrq(matrix_order: lapack_memory_order, k: c_int, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zungrq for the type complex(128).
    

.. function:: proc ungtr(matrix_order: lapack_memory_order, uplo: string, n: c_int, a: [] complex(64), tau: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cungtr for the type complex(64).
    

.. function:: proc ungtr(matrix_order: lapack_memory_order, uplo: string, n: c_int, a: [] complex(128), tau: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zungtr for the type complex(128).
    

.. function:: proc unmbr(matrix_order: lapack_memory_order, vect: string, side: string, trans: string, k: c_int, a: [] complex(64), tau: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cunmbr for the type complex(64).
    

.. function:: proc unmbr(matrix_order: lapack_memory_order, vect: string, side: string, trans: string, k: c_int, a: [] complex(128), tau: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zunmbr for the type complex(128).
    

.. function:: proc unmhr(matrix_order: lapack_memory_order, side: string, trans: string, ilo: c_int, ihi: c_int, a: [] complex(64), tau: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cunmhr for the type complex(64).
    

.. function:: proc unmhr(matrix_order: lapack_memory_order, side: string, trans: string, ilo: c_int, ihi: c_int, a: [] complex(128), tau: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zunmhr for the type complex(128).
    

.. function:: proc unmlq(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] complex(64), tau: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cunmlq for the type complex(64).
    

.. function:: proc unmlq(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] complex(128), tau: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zunmlq for the type complex(128).
    

.. function:: proc unmql(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] complex(64), tau: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cunmql for the type complex(64).
    

.. function:: proc unmql(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] complex(128), tau: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zunmql for the type complex(128).
    

.. function:: proc unmqr(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] complex(64), tau: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cunmqr for the type complex(64).
    

.. function:: proc unmqr(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] complex(128), tau: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zunmqr for the type complex(128).
    

.. function:: proc unmrq(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] complex(64), tau: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cunmrq for the type complex(64).
    

.. function:: proc unmrq(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, a: [] complex(128), tau: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zunmrq for the type complex(128).
    

.. function:: proc unmrz(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, l: c_int, a: [] complex(64), tau: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cunmrz for the type complex(64).
    

.. function:: proc unmrz(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, l: c_int, a: [] complex(128), tau: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zunmrz for the type complex(128).
    

.. function:: proc unmtr(matrix_order: lapack_memory_order, side: string, uplo: string, trans: string, a: [] complex(64), tau: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cunmtr for the type complex(64).
    

.. function:: proc unmtr(matrix_order: lapack_memory_order, side: string, uplo: string, trans: string, a: [] complex(128), tau: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zunmtr for the type complex(128).
    

.. function:: proc upgtr(matrix_order: lapack_memory_order, uplo: string, ap: [] complex(64), tau: [] complex(64), q: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cupgtr for the type complex(64).
    

.. function:: proc upgtr(matrix_order: lapack_memory_order, uplo: string, ap: [] complex(128), tau: [] complex(128), q: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zupgtr for the type complex(128).
    

.. function:: proc upmtr(matrix_order: lapack_memory_order, side: string, uplo: string, trans: string, ap: [] complex(64), tau: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cupmtr for the type complex(64).
    

.. function:: proc upmtr(matrix_order: lapack_memory_order, side: string, uplo: string, trans: string, ap: [] complex(128), tau: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zupmtr for the type complex(128).
    

.. function:: proc laghe(matrix_order: lapack_memory_order, k: c_int, d: [] real(32), a: [] complex(64), iseed: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_claghe for the type complex(64).
    

.. function:: proc laghe(matrix_order: lapack_memory_order, k: c_int, d: [] real(64), a: [] complex(128), iseed: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zlaghe for the type complex(128).
    

.. function:: proc lagsy(matrix_order: lapack_memory_order, k: c_int, d: [] real(32), a: [] real(32), iseed: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_slagsy for the type real(32).
    

.. function:: proc lagsy(matrix_order: lapack_memory_order, k: c_int, d: [] real(64), a: [] real(64), iseed: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dlagsy for the type real(64).
    

.. function:: proc lagsy(matrix_order: lapack_memory_order, k: c_int, d: [] real(32), a: [] complex(64), iseed: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_clagsy for the type complex(64).
    

.. function:: proc lagsy(matrix_order: lapack_memory_order, k: c_int, d: [] real(64), a: [] complex(128), iseed: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zlagsy for the type complex(128).
    

.. function:: proc lapmr(matrix_order: lapack_memory_order, forwrd: c_int, x: [] real(32), k: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_slapmr for the type real(32).
    

.. function:: proc lapmr(matrix_order: lapack_memory_order, forwrd: c_int, x: [] real(64), k: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dlapmr for the type real(64).
    

.. function:: proc lapmr(matrix_order: lapack_memory_order, forwrd: c_int, x: [] complex(64), k: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_clapmr for the type complex(64).
    

.. function:: proc lapmr(matrix_order: lapack_memory_order, forwrd: c_int, x: [] complex(128), k: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zlapmr for the type complex(128).
    

.. function:: proc lapy2(x: real(32), y: real(32)): c_float

   
   Wrapped procedure of LAPACKE_slapy2 for the type real(32).
    

.. function:: proc lapy2(x: real(64), y: real(64)): c_double

   
   Wrapped procedure of LAPACKE_dlapy2 for the type real(64).
    

.. function:: proc lapy3(x: real(32), y: real(32), z: real(32)): c_float

   
   Wrapped procedure of LAPACKE_slapy3 for the type real(32).
    

.. function:: proc lapy3(x: real(64), y: real(64), z: real(64)): c_double

   
   Wrapped procedure of LAPACKE_dlapy3 for the type real(64).
    

.. function:: proc lartgp(f: real(32), g: real(32), ref cs: real(32), ref sn: real(32), ref r: real(32)): c_int

   
   Wrapped procedure of LAPACKE_slartgp for the type real(32).
    

.. function:: proc lartgp(f: real(64), g: real(64), ref cs: real(64), ref sn: real(64), ref r: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlartgp for the type real(64).
    

.. function:: proc lartgs(x: real(32), y: real(32), sigma: real(32), ref cs: real(32), ref sn: real(32)): c_int

   
   Wrapped procedure of LAPACKE_slartgs for the type real(32).
    

.. function:: proc lartgs(x: real(64), y: real(64), sigma: real(64), ref cs: real(64), ref sn: real(64)): c_int

   
   Wrapped procedure of LAPACKE_dlartgs for the type real(64).
    

.. function:: proc bbcsd(matrix_order: lapack_memory_order, jobu1: string, jobu2: string, jobv1t: string, jobv2t: string, trans: string, m: c_int, theta: [] real(32), phi: [] real(32), u1: [] complex(64), u2: [] complex(64), v1t: [] complex(64), v2t: [] complex(64), b11d: [] real(32), b11e: [] real(32), b12d: [] real(32), b12e: [] real(32), b21d: [] real(32), b21e: [] real(32), b22d: [] real(32), b22e: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_cbbcsd for the type complex(64).
    

.. function:: proc heswapr(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), i1: c_int, i2: c_int): c_int

   
   Wrapped procedure of LAPACKE_cheswapr for the type complex(64).
    

.. function:: proc hetri2(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_chetri2 for the type complex(64).
    

.. function:: proc hetri2x(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int, nb: c_int): c_int

   
   Wrapped procedure of LAPACKE_chetri2x for the type complex(64).
    

.. function:: proc hetrs2(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_chetrs2 for the type complex(64).
    

.. function:: proc syconv(matrix_order: lapack_memory_order, uplo: string, way: string, a: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_csyconv for the type complex(64).
    

.. function:: proc syswapr(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), i1: c_int, i2: c_int): c_int

   
   Wrapped procedure of LAPACKE_csyswapr for the type complex(64).
    

.. function:: proc sytri2(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_csytri2 for the type complex(64).
    

.. function:: proc sytri2x(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int, nb: c_int): c_int

   
   Wrapped procedure of LAPACKE_csytri2x for the type complex(64).
    

.. function:: proc sytrs2(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_csytrs2 for the type complex(64).
    

.. function:: proc unbdb(matrix_order: lapack_memory_order, trans: string, signs: string, m: c_int, x11: [] complex(64), x12: [] complex(64), x21: [] complex(64), x22: [] complex(64), theta: [] real(32), phi: [] real(32), taup1: [] complex(64), taup2: [] complex(64), tauq1: [] complex(64), tauq2: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cunbdb for the type complex(64).
    

.. function:: proc uncsd(matrix_order: lapack_memory_order, jobu1: string, jobu2: string, jobv1t: string, jobv2t: string, trans: string, signs: string, m: c_int, x11: [] complex(64), x12: [] complex(64), x21: [] complex(64), x22: [] complex(64), theta: [] real(32), u1: [] complex(64), u2: [] complex(64), v1t: [] complex(64), v2t: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cuncsd for the type complex(64).
    

.. function:: proc bbcsd(matrix_order: lapack_memory_order, jobu1: string, jobu2: string, jobv1t: string, jobv2t: string, trans: string, m: c_int, theta: [] real(64), phi: [] real(64), u1: [] real(64), u2: [] real(64), v1t: [] real(64), v2t: [] real(64), b11d: [] real(64), b11e: [] real(64), b12d: [] real(64), b12e: [] real(64), b21d: [] real(64), b21e: [] real(64), b22d: [] real(64), b22e: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dbbcsd for the type real(64).
    

.. function:: proc orbdb(matrix_order: lapack_memory_order, trans: string, signs: string, m: c_int, x11: [] real(64), x12: [] real(64), x21: [] real(64), x22: [] real(64), theta: [] real(64), phi: [] real(64), taup1: [] real(64), taup2: [] real(64), tauq1: [] real(64), tauq2: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dorbdb for the type real(64).
    

.. function:: proc orcsd(matrix_order: lapack_memory_order, jobu1: string, jobu2: string, jobv1t: string, jobv2t: string, trans: string, signs: string, m: c_int, x11: [] real(64), x12: [] real(64), x21: [] real(64), x22: [] real(64), theta: [] real(64), u1: [] real(64), u2: [] real(64), v1t: [] real(64), v2t: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dorcsd for the type real(64).
    

.. function:: proc syconv(matrix_order: lapack_memory_order, uplo: string, way: string, a: [] real(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dsyconv for the type real(64).
    

.. function:: proc syswapr(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), i1: c_int, i2: c_int): c_int

   
   Wrapped procedure of LAPACKE_dsyswapr for the type real(64).
    

.. function:: proc sytri2(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_dsytri2 for the type real(64).
    

.. function:: proc sytri2x(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), ipiv: [] c_int, nb: c_int): c_int

   
   Wrapped procedure of LAPACKE_dsytri2x for the type real(64).
    

.. function:: proc sytrs2(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), ipiv: [] c_int, b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsytrs2 for the type real(64).
    

.. function:: proc bbcsd(matrix_order: lapack_memory_order, jobu1: string, jobu2: string, jobv1t: string, jobv2t: string, trans: string, m: c_int, theta: [] real(32), phi: [] real(32), u1: [] real(32), u2: [] real(32), v1t: [] real(32), v2t: [] real(32), b11d: [] real(32), b11e: [] real(32), b12d: [] real(32), b12e: [] real(32), b21d: [] real(32), b21e: [] real(32), b22d: [] real(32), b22e: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sbbcsd for the type real(32).
    

.. function:: proc orbdb(matrix_order: lapack_memory_order, trans: string, signs: string, m: c_int, x11: [] real(32), x12: [] real(32), x21: [] real(32), x22: [] real(32), theta: [] real(32), phi: [] real(32), taup1: [] real(32), taup2: [] real(32), tauq1: [] real(32), tauq2: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sorbdb for the type real(32).
    

.. function:: proc orcsd(matrix_order: lapack_memory_order, jobu1: string, jobu2: string, jobv1t: string, jobv2t: string, trans: string, signs: string, m: c_int, x11: [] real(32), x12: [] real(32), x21: [] real(32), x22: [] real(32), theta: [] real(32), u1: [] real(32), u2: [] real(32), v1t: [] real(32), v2t: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sorcsd for the type real(32).
    

.. function:: proc syconv(matrix_order: lapack_memory_order, uplo: string, way: string, a: [] real(32), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_ssyconv for the type real(32).
    

.. function:: proc syswapr(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), i1: c_int, i2: c_int): c_int

   
   Wrapped procedure of LAPACKE_ssyswapr for the type real(32).
    

.. function:: proc sytri2(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_ssytri2 for the type real(32).
    

.. function:: proc sytri2x(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), ipiv: [] c_int, nb: c_int): c_int

   
   Wrapped procedure of LAPACKE_ssytri2x for the type real(32).
    

.. function:: proc sytrs2(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), ipiv: [] c_int, b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssytrs2 for the type real(32).
    

.. function:: proc bbcsd(matrix_order: lapack_memory_order, jobu1: string, jobu2: string, jobv1t: string, jobv2t: string, trans: string, m: c_int, theta: [] real(64), phi: [] real(64), u1: [] complex(128), u2: [] complex(128), v1t: [] complex(128), v2t: [] complex(128), b11d: [] real(64), b11e: [] real(64), b12d: [] real(64), b12e: [] real(64), b21d: [] real(64), b21e: [] real(64), b22d: [] real(64), b22e: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_zbbcsd for the type complex(128).
    

.. function:: proc heswapr(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), i1: c_int, i2: c_int): c_int

   
   Wrapped procedure of LAPACKE_zheswapr for the type complex(128).
    

.. function:: proc hetri2(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zhetri2 for the type complex(128).
    

.. function:: proc hetri2x(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int, nb: c_int): c_int

   
   Wrapped procedure of LAPACKE_zhetri2x for the type complex(128).
    

.. function:: proc hetrs2(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zhetrs2 for the type complex(128).
    

.. function:: proc syconv(matrix_order: lapack_memory_order, uplo: string, way: string, a: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zsyconv for the type complex(128).
    

.. function:: proc syswapr(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), i1: c_int, i2: c_int): c_int

   
   Wrapped procedure of LAPACKE_zsyswapr for the type complex(128).
    

.. function:: proc sytri2(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int): c_int

   
   Wrapped procedure of LAPACKE_zsytri2 for the type complex(128).
    

.. function:: proc sytri2x(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int, nb: c_int): c_int

   
   Wrapped procedure of LAPACKE_zsytri2x for the type complex(128).
    

.. function:: proc sytrs2(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zsytrs2 for the type complex(128).
    

.. function:: proc unbdb(matrix_order: lapack_memory_order, trans: string, signs: string, m: c_int, x11: [] complex(128), x12: [] complex(128), x21: [] complex(128), x22: [] complex(128), theta: [] real(64), phi: [] real(64), taup1: [] complex(128), taup2: [] complex(128), tauq1: [] complex(128), tauq2: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zunbdb for the type complex(128).
    

.. function:: proc uncsd(matrix_order: lapack_memory_order, jobu1: string, jobu2: string, jobv1t: string, jobv2t: string, trans: string, signs: string, m: c_int, x11: [] complex(128), x12: [] complex(128), x21: [] complex(128), x22: [] complex(128), theta: [] real(64), u1: [] complex(128), u2: [] complex(128), v1t: [] complex(128), v2t: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zuncsd for the type complex(128).
    

.. function:: proc gemqrt(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, v: [] real(32), t: [] real(32), c: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgemqrt for the type real(32).
    

.. function:: proc gemqrt(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, v: [] real(64), t: [] real(64), c: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgemqrt for the type real(64).
    

.. function:: proc gemqrt(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, v: [] complex(64), t: [] complex(64), c: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgemqrt for the type complex(64).
    

.. function:: proc gemqrt(matrix_order: lapack_memory_order, side: string, trans: string, k: c_int, v: [] complex(128), t: [] complex(128), c: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgemqrt for the type complex(128).
    

.. function:: proc geqrt(matrix_order: lapack_memory_order, nb: c_int, a: [] real(32), t: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeqrt for the type real(32).
    

.. function:: proc geqrt(matrix_order: lapack_memory_order, nb: c_int, a: [] real(64), t: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeqrt for the type real(64).
    

.. function:: proc geqrt(matrix_order: lapack_memory_order, nb: c_int, a: [] complex(64), t: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgeqrt for the type complex(64).
    

.. function:: proc geqrt(matrix_order: lapack_memory_order, nb: c_int, a: [] complex(128), t: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgeqrt for the type complex(128).
    

.. function:: proc geqrt2(matrix_order: lapack_memory_order, a: [] real(32), t: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeqrt2 for the type real(32).
    

.. function:: proc geqrt2(matrix_order: lapack_memory_order, a: [] real(64), t: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeqrt2 for the type real(64).
    

.. function:: proc geqrt2(matrix_order: lapack_memory_order, a: [] complex(64), t: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgeqrt2 for the type complex(64).
    

.. function:: proc geqrt2(matrix_order: lapack_memory_order, a: [] complex(128), t: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgeqrt2 for the type complex(128).
    

.. function:: proc geqrt3(matrix_order: lapack_memory_order, a: [] real(32), t: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_sgeqrt3 for the type real(32).
    

.. function:: proc geqrt3(matrix_order: lapack_memory_order, a: [] real(64), t: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dgeqrt3 for the type real(64).
    

.. function:: proc geqrt3(matrix_order: lapack_memory_order, a: [] complex(64), t: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_cgeqrt3 for the type complex(64).
    

.. function:: proc geqrt3(matrix_order: lapack_memory_order, a: [] complex(128), t: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zgeqrt3 for the type complex(128).
    

.. function:: proc tpmqrt(matrix_order: lapack_memory_order, side: string, trans: string, l: c_int, v: [] real(32), t: [] real(32), a: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stpmqrt for the type real(32).
    

.. function:: proc tpmqrt(matrix_order: lapack_memory_order, side: string, trans: string, l: c_int, v: [] real(64), t: [] real(64), a: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtpmqrt for the type real(64).
    

.. function:: proc tpmqrt(matrix_order: lapack_memory_order, side: string, trans: string, l: c_int, v: [] complex(64), t: [] complex(64), a: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctpmqrt for the type complex(64).
    

.. function:: proc tpmqrt(matrix_order: lapack_memory_order, side: string, trans: string, l: c_int, v: [] complex(128), t: [] complex(128), a: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztpmqrt for the type complex(128).
    

.. function:: proc tpqrt(matrix_order: lapack_memory_order, l: c_int, nb: c_int, a: [] real(64), b: [] real(64), t: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtpqrt for the type real(64).
    

.. function:: proc tpqrt(matrix_order: lapack_memory_order, l: c_int, nb: c_int, a: [] complex(64), b: [] complex(64), t: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctpqrt for the type complex(64).
    

.. function:: proc tpqrt(matrix_order: lapack_memory_order, l: c_int, nb: c_int, a: [] complex(128), b: [] complex(128), t: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztpqrt for the type complex(128).
    

.. function:: proc tpqrt2(matrix_order: lapack_memory_order, l: c_int, a: [] real(32), b: [] real(32), t: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stpqrt2 for the type real(32).
    

.. function:: proc tpqrt2(matrix_order: lapack_memory_order, l: c_int, a: [] real(64), b: [] real(64), t: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtpqrt2 for the type real(64).
    

.. function:: proc tpqrt2(matrix_order: lapack_memory_order, l: c_int, a: [] complex(64), b: [] complex(64), t: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctpqrt2 for the type complex(64).
    

.. function:: proc tpqrt2(matrix_order: lapack_memory_order, l: c_int, a: [] complex(128), b: [] complex(128), t: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztpqrt2 for the type complex(128).
    

.. function:: proc tprfb(matrix_order: lapack_memory_order, side: string, trans: string, direct: string, storev: string, l: c_int, v: [] real(32), t: [] real(32), a: [] real(32), b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_stprfb for the type real(32).
    

.. function:: proc tprfb(matrix_order: lapack_memory_order, side: string, trans: string, direct: string, storev: string, l: c_int, v: [] real(64), t: [] real(64), a: [] real(64), b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dtprfb for the type real(64).
    

.. function:: proc tprfb(matrix_order: lapack_memory_order, side: string, trans: string, direct: string, storev: string, l: c_int, v: [] complex(64), t: [] complex(64), a: [] complex(64), b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_ctprfb for the type complex(64).
    

.. function:: proc tprfb(matrix_order: lapack_memory_order, side: string, trans: string, direct: string, storev: string, l: c_int, v: [] complex(128), t: [] complex(128), a: [] complex(128), b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_ztprfb for the type complex(128).
    

.. function:: proc sysv_rook(matrix_order: lapack_memory_order, uplo: string, a: [] real(32), ipiv: [] c_int, b: [] real(32)): c_int

   
   Wrapped procedure of LAPACKE_ssysv_rook for the type real(32).
    

.. function:: proc sysv_rook(matrix_order: lapack_memory_order, uplo: string, a: [] real(64), ipiv: [] c_int, b: [] real(64)): c_int

   
   Wrapped procedure of LAPACKE_dsysv_rook for the type real(64).
    

.. function:: proc sysv_rook(matrix_order: lapack_memory_order, uplo: string, a: [] complex(64), ipiv: [] c_int, b: [] complex(64)): c_int

   
   Wrapped procedure of LAPACKE_csysv_rook for the type complex(64).
    

.. function:: proc sysv_rook(matrix_order: lapack_memory_order, uplo: string, a: [] complex(128), ipiv: [] c_int, b: [] complex(128)): c_int

   
   Wrapped procedure of LAPACKE_zsysv_rook for the type complex(128).
    

