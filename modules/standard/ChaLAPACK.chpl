
/*
 * Copyright 2004-2015 Cray Inc.
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

Chapel idiomatic wrappers for the LAPACK library.

Consult the :mod:`LAPACK` module, which defines all the types and enumerations that this module uses, and demonstrates compiling with LAPACK binaries.
Also consult the :mod:`LAPACK` module for details pertaining the use and function of all LAPACK procedures, including both pure LAPACK and these ChaLAPACK wrappers.

.. note::

  Because of the volume of procedures provided, and because their behavior is virtually unchanged, in-depth documentation on each procedure's purpose and use is not given here.
  
  Consult the `Netlibs LAPACK <http://www.netlib.org/lapack/>`_ site, and the `Netlibs <http://www.netlib.org/lapack/explore-html/globals_func.html>`_ and `Intel <https://software.intel.com/en-us/node/501008>`_ LAPACK documents for that information.
  
  Additionally, only a small set of ChaLAPACK procedures have been tested for correctness.

Differences from LAPACK
-----------------------

The `XYYZZZ <http://www.netlib.org/lapack/individualroutines.html>`_ naming convention for procedures has been only slightly modified with the removal of the type identifier in the procedure name,
creating a set of polymorphic procedures under the name YYZZZ.

Procedure arguments have been simplified. 
Using the original Fortran documentation, groupings of arguments pertaining to matrices and arrays were found, and encapsulated by Chapel arrays.

For example, instead of 

.. code-block:: chapel

  LAPACKE_sgesv(matrix_order : lapack_memory_order, n : c_int, nrhs : c_int, a : [] c_float, lda : c_int, ipiv : [] c_int, b : [] c_float, ldb : c_int) : c_int;

the arguments n, nrhs, lda, and ldb can be encapsulated by properties of Chapel arrays and their domains, giving the ChaLAPACK procedure:

.. code-block:: chapel

  gesv(matrix_order : lapack_memory_order, a : [] c_float, ipiv : [] c_int, b : [] c_float): c_int;

This requires that your matrices are stored in a two dimensional form.

Compiling with LAPACK
------------------------

Using the :mod:`ChaLAPACK` or :mod:`LAPACK` modules require that you have LAPACK (binaries and C interface) on your system, as it is not bundled with Chapel. 

You can download and build the `Netlibs LAPACK <http://www.netlib.org/lapack/>`_ , if it is not already installed, and this section will assume usage of the Netlibs LAPACK.
You must also have libgfortran installed on your system (typically installed along with gcc).

To compile with LAPACK, Chapel needs to know:

1. Where the LAPACKE header (lapacke.h) is.

2. Where the libgfortran binary is (sometimes it is not to be found by ``ld``).

3. Where the various LAPACK binaries (lapacke, lapack, and refblas) are.

Once the details are worked out, compiling is quite simple and nearly identical to how one would compile a C program to work with LAPACK.

.. code-block:: sh

  chpl -I$PATH_TO_LAPACKE_INCLUDE_DIR \
       -L$PATH_TO_LIBGFORTRAN -lgfortran \
       -L$PATH_TO_LAPACK_BINARIES -llapacke -lapack -lrefblas \
       source.chpl

As an example,

.. code-block:: sh

  chpl -I$HOME/LAPACK/lapacke/include \
       -L/usr/lib/gcc/stuff -lgfortran \
       -L$HOME/LAPACK -llapacke -lapack -lrefblas \
       source.chpl

would be the command to use if Netlibs LAPACK had been built in ``$HOME/LAPACK``, and the libgfortran binary found to be located in ``/usr/lib/gcc/stuff``

Future Work
-----------

We anticipate the following additions:

1. Better documentation on each individual procedure, mostly derived from the original Fortran documentation.

2. Larger test coverage of the provided procedures.

3. Enumerated values for arguments of procedures that currently take strings to denote different options.

ChaLAPACK Procedures
--------------------

*/
module ChaLAPACK {
use LAPACK;
use "lapacke.h";
use SysCTypes;

use LAPACK;
/* 
Wrapped procedure of LAPACKE_sbdsdc for the type c_float.
 */
inline proc bdsdc(matrix_order : lapack_memory_order, uplo : string, compq : string, n : c_int, d : [] c_float, e : [] c_float, u : [] c_float, vt : [] c_float, q : [] c_float, iq : [] c_int): c_int{
  return LAPACKE_sbdsdc(matrix_order, ascii(uplo) : c_char, ascii(compq) : c_char, n, d, e, u, (u.domain.dim(2).size) : c_int, vt, (vt.domain.dim(2).size) : c_int, q, iq);
}

/* 
Wrapped procedure of LAPACKE_dbdsdc for the type c_double.
 */
inline proc bdsdc(matrix_order : lapack_memory_order, uplo : string, compq : string, n : c_int, d : [] c_double, e : [] c_double, u : [] c_double, vt : [] c_double, q : [] c_double, iq : [] c_int): c_int{
  return LAPACKE_dbdsdc(matrix_order, ascii(uplo) : c_char, ascii(compq) : c_char, n, d, e, u, (u.domain.dim(2).size) : c_int, vt, (vt.domain.dim(2).size) : c_int, q, iq);
}

/* 
Wrapped procedure of LAPACKE_sbdsqr for the type c_float.
 */
inline proc bdsqr(matrix_order : lapack_memory_order, uplo : string, d : [] c_float, e : [] c_float, vt : [] c_float, u : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sbdsqr(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then vt.domain.dim(2).size else vt.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then u.domain.dim(1).size else u.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, d, e, vt, (vt.domain.dim(2).size) : c_int, u, (u.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dbdsqr for the type c_double.
 */
inline proc bdsqr(matrix_order : lapack_memory_order, uplo : string, d : [] c_double, e : [] c_double, vt : [] c_double, u : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dbdsqr(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then vt.domain.dim(2).size else vt.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then u.domain.dim(1).size else u.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, d, e, vt, (vt.domain.dim(2).size) : c_int, u, (u.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cbdsqr for the type lapack_complex_float.
 */
inline proc bdsqr(matrix_order : lapack_memory_order, uplo : string, d : [] c_float, e : [] c_float, vt : [] complex(64), u : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cbdsqr(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then vt.domain.dim(2).size else vt.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then u.domain.dim(1).size else u.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, d, e, vt, (vt.domain.dim(2).size) : c_int, u, (u.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zbdsqr for the type lapack_complex_double.
 */
inline proc bdsqr(matrix_order : lapack_memory_order, uplo : string, d : [] c_double, e : [] c_double, vt : [] complex(128), u : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zbdsqr(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then vt.domain.dim(2).size else vt.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then u.domain.dim(1).size else u.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, d, e, vt, (vt.domain.dim(2).size) : c_int, u, (u.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sdisna for the type c_float.
 */
inline proc disna(job : string, m : c_int, n : c_int, d : [] c_float, sep : [] c_float): c_int{
  return LAPACKE_sdisna(ascii(job) : c_char, m, n, d, sep);
}

/* 
Wrapped procedure of LAPACKE_ddisna for the type c_double.
 */
inline proc disna(job : string, m : c_int, n : c_int, d : [] c_double, sep : [] c_double): c_int{
  return LAPACKE_ddisna(ascii(job) : c_char, m, n, d, sep);
}

/* 
Wrapped procedure of LAPACKE_sgbbrd for the type c_float.
 */
inline proc gbbrd(matrix_order : lapack_memory_order, vect : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_float, d : [] c_float, e : [] c_float, q : [] c_float, pt : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sgbbrd(matrix_order, ascii(vect) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, n, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, kl, ku, ab, (ab.domain.dim(2).size) : c_int, d, e, q, (q.domain.dim(2).size) : c_int, pt, (pt.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgbbrd for the type c_double.
 */
inline proc gbbrd(matrix_order : lapack_memory_order, vect : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_double, d : [] c_double, e : [] c_double, q : [] c_double, pt : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dgbbrd(matrix_order, ascii(vect) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, n, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, kl, ku, ab, (ab.domain.dim(2).size) : c_int, d, e, q, (q.domain.dim(2).size) : c_int, pt, (pt.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgbbrd for the type lapack_complex_float.
 */
inline proc gbbrd(matrix_order : lapack_memory_order, vect : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(64), d : [] c_float, e : [] c_float, q : [] complex(64), pt : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cgbbrd(matrix_order, ascii(vect) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, n, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, kl, ku, ab, (ab.domain.dim(2).size) : c_int, d, e, q, (q.domain.dim(2).size) : c_int, pt, (pt.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgbbrd for the type lapack_complex_double.
 */
inline proc gbbrd(matrix_order : lapack_memory_order, vect : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(128), d : [] c_double, e : [] c_double, q : [] complex(128), pt : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zgbbrd(matrix_order, ascii(vect) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, n, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, kl, ku, ab, (ab.domain.dim(2).size) : c_int, d, e, q, (q.domain.dim(2).size) : c_int, pt, (pt.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgbcon for the type c_float.
 */
inline proc gbcon(matrix_order : lapack_memory_order, norm : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_float, ipiv : [] c_int, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_sgbcon(matrix_order, ascii(norm) : c_char, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_dgbcon for the type c_double.
 */
inline proc gbcon(matrix_order : lapack_memory_order, norm : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_double, ipiv : [] c_int, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_dgbcon(matrix_order, ascii(norm) : c_char, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_cgbcon for the type lapack_complex_float.
 */
inline proc gbcon(matrix_order : lapack_memory_order, norm : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(64), ipiv : [] c_int, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_cgbcon(matrix_order, ascii(norm) : c_char, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_zgbcon for the type lapack_complex_double.
 */
inline proc gbcon(matrix_order : lapack_memory_order, norm : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(128), ipiv : [] c_int, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_zgbcon(matrix_order, ascii(norm) : c_char, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_sgbequ for the type c_float.
 */
inline proc gbequ(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] c_float, r : [] c_float, c : [] c_float, ref rowcnd : c_float, ref colcnd : c_float, ref amax : c_float): c_int{
  return LAPACKE_sgbequ(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_dgbequ for the type c_double.
 */
inline proc gbequ(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] c_double, r : [] c_double, c : [] c_double, ref rowcnd : c_double, ref colcnd : c_double, ref amax : c_double): c_int{
  return LAPACKE_dgbequ(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_cgbequ for the type lapack_complex_float.
 */
inline proc gbequ(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] complex(64), r : [] c_float, c : [] c_float, ref rowcnd : c_float, ref colcnd : c_float, ref amax : c_float): c_int{
  return LAPACKE_cgbequ(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_zgbequ for the type lapack_complex_double.
 */
inline proc gbequ(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] complex(128), r : [] c_double, c : [] c_double, ref rowcnd : c_double, ref colcnd : c_double, ref amax : c_double): c_int{
  return LAPACKE_zgbequ(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_sgbequb for the type c_float.
 */
inline proc gbequb(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] c_float, r : [] c_float, c : [] c_float, ref rowcnd : c_float, ref colcnd : c_float, ref amax : c_float): c_int{
  return LAPACKE_sgbequb(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_dgbequb for the type c_double.
 */
inline proc gbequb(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] c_double, r : [] c_double, c : [] c_double, ref rowcnd : c_double, ref colcnd : c_double, ref amax : c_double): c_int{
  return LAPACKE_dgbequb(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_cgbequb for the type lapack_complex_float.
 */
inline proc gbequb(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] complex(64), r : [] c_float, c : [] c_float, ref rowcnd : c_float, ref colcnd : c_float, ref amax : c_float): c_int{
  return LAPACKE_cgbequb(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_zgbequb for the type lapack_complex_double.
 */
inline proc gbequb(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] complex(128), r : [] c_double, c : [] c_double, ref rowcnd : c_double, ref colcnd : c_double, ref amax : c_double): c_int{
  return LAPACKE_zgbequb(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_sgbrfs for the type c_float.
 */
inline proc gbrfs(matrix_order : lapack_memory_order, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_float, afb : [] c_float, ipiv : [] c_int, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_sgbrfs(matrix_order, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dgbrfs for the type c_double.
 */
inline proc gbrfs(matrix_order : lapack_memory_order, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_double, afb : [] c_double, ipiv : [] c_int, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dgbrfs(matrix_order, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cgbrfs for the type lapack_complex_float.
 */
inline proc gbrfs(matrix_order : lapack_memory_order, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(64), afb : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cgbrfs(matrix_order, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zgbrfs for the type lapack_complex_double.
 */
inline proc gbrfs(matrix_order : lapack_memory_order, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(128), afb : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zgbrfs(matrix_order, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_sgbrfsx for the type c_float.
 */
inline proc gbrfsx(matrix_order : lapack_memory_order, trans : string, equed : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_float, afb : [] c_float, ipiv : [] c_int, r : [] c_float, c : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_sgbrfsx(matrix_order, ascii(trans) : c_char, ascii(equed) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_dgbrfsx for the type c_double.
 */
inline proc gbrfsx(matrix_order : lapack_memory_order, trans : string, equed : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_double, afb : [] c_double, ipiv : [] c_int, r : [] c_double, c : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_dgbrfsx(matrix_order, ascii(trans) : c_char, ascii(equed) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_cgbrfsx for the type lapack_complex_float.
 */
inline proc gbrfsx(matrix_order : lapack_memory_order, trans : string, equed : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(64), afb : [] complex(64), ipiv : [] c_int, r : [] c_float, c : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_cgbrfsx(matrix_order, ascii(trans) : c_char, ascii(equed) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_zgbrfsx for the type lapack_complex_double.
 */
inline proc gbrfsx(matrix_order : lapack_memory_order, trans : string, equed : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(128), afb : [] complex(128), ipiv : [] c_int, r : [] c_double, c : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_zgbrfsx(matrix_order, ascii(trans) : c_char, ascii(equed) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_sgbsv for the type c_float.
 */
inline proc gbsv(matrix_order : lapack_memory_order, n : c_int, kl : c_int, ku : c_int, ab : [] c_float, ipiv : [] c_int, b : [] c_float): c_int{
  return LAPACKE_sgbsv(matrix_order, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgbsv for the type c_double.
 */
inline proc gbsv(matrix_order : lapack_memory_order, n : c_int, kl : c_int, ku : c_int, ab : [] c_double, ipiv : [] c_int, b : [] c_double): c_int{
  return LAPACKE_dgbsv(matrix_order, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgbsv for the type lapack_complex_float.
 */
inline proc gbsv(matrix_order : lapack_memory_order, n : c_int, kl : c_int, ku : c_int, ab : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_cgbsv(matrix_order, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgbsv for the type lapack_complex_double.
 */
inline proc gbsv(matrix_order : lapack_memory_order, n : c_int, kl : c_int, ku : c_int, ab : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zgbsv(matrix_order, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgbsvx for the type c_float.
 */
inline proc gbsvx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_float, afb : [] c_float, ipiv : [] c_int, ref equed : string, r : [] c_float, c : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, ferr : [] c_float, berr : [] c_float, rpivot : [] c_float): c_int{
  return LAPACKE_sgbsvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr, rpivot);
}

/* 
Wrapped procedure of LAPACKE_dgbsvx for the type c_double.
 */
inline proc gbsvx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_double, afb : [] c_double, ipiv : [] c_int, ref equed : string, r : [] c_double, c : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, ferr : [] c_double, berr : [] c_double, rpivot : [] c_double): c_int{
  return LAPACKE_dgbsvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr, rpivot);
}

/* 
Wrapped procedure of LAPACKE_cgbsvx for the type lapack_complex_float.
 */
inline proc gbsvx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(64), afb : [] complex(64), ipiv : [] c_int, ref equed : string, r : [] c_float, c : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ferr : [] c_float, berr : [] c_float, rpivot : [] c_float): c_int{
  return LAPACKE_cgbsvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr, rpivot);
}

/* 
Wrapped procedure of LAPACKE_zgbsvx for the type lapack_complex_double.
 */
inline proc gbsvx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(128), afb : [] complex(128), ipiv : [] c_int, ref equed : string, r : [] c_double, c : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ferr : [] c_double, berr : [] c_double, rpivot : [] c_double): c_int{
  return LAPACKE_zgbsvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr, rpivot);
}

/* 
Wrapped procedure of LAPACKE_sgbsvxx for the type c_float.
 */
inline proc gbsvxx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_float, afb : [] c_float, ipiv : [] c_int, ref equed : string, r : [] c_float, c : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, ref rpvgrw : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_sgbsvxx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_dgbsvxx for the type c_double.
 */
inline proc gbsvxx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_double, afb : [] c_double, ipiv : [] c_int, ref equed : string, r : [] c_double, c : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, ref rpvgrw : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_dgbsvxx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_cgbsvxx for the type lapack_complex_float.
 */
inline proc gbsvxx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(64), afb : [] complex(64), ipiv : [] c_int, ref equed : string, r : [] c_float, c : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ref rpvgrw : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_cgbsvxx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_zgbsvxx for the type lapack_complex_double.
 */
inline proc gbsvxx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(128), afb : [] complex(128), ipiv : [] c_int, ref equed : string, r : [] c_double, c : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ref rpvgrw : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_zgbsvxx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_sgbtrf for the type c_float.
 */
inline proc gbtrf(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] c_float, ipiv : [] c_int): c_int{
  return LAPACKE_sgbtrf(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_dgbtrf for the type c_double.
 */
inline proc gbtrf(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] c_double, ipiv : [] c_int): c_int{
  return LAPACKE_dgbtrf(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_cgbtrf for the type lapack_complex_float.
 */
inline proc gbtrf(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_cgbtrf(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zgbtrf for the type lapack_complex_double.
 */
inline proc gbtrf(matrix_order : lapack_memory_order, m : c_int, n : c_int, kl : c_int, ku : c_int, ab : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zgbtrf(matrix_order, m, n, kl, ku, ab, (ab.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_sgbtrs for the type c_float.
 */
inline proc gbtrs(matrix_order : lapack_memory_order, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_float, ipiv : [] c_int, b : [] c_float): c_int{
  return LAPACKE_sgbtrs(matrix_order, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgbtrs for the type c_double.
 */
inline proc gbtrs(matrix_order : lapack_memory_order, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] c_double, ipiv : [] c_int, b : [] c_double): c_int{
  return LAPACKE_dgbtrs(matrix_order, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgbtrs for the type lapack_complex_float.
 */
inline proc gbtrs(matrix_order : lapack_memory_order, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_cgbtrs(matrix_order, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgbtrs for the type lapack_complex_double.
 */
inline proc gbtrs(matrix_order : lapack_memory_order, trans : string, n : c_int, kl : c_int, ku : c_int, ab : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zgbtrs(matrix_order, ascii(trans) : c_char, n, kl, ku, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgebak for the type c_float.
 */
inline proc gebak(matrix_order : lapack_memory_order, job : string, side : string, ilo : c_int, ihi : c_int, scale : [] c_float, v : [] c_float): c_int{
  return LAPACKE_sgebak(matrix_order, ascii(job) : c_char, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(1).size else v.domain.dim(2).size) : c_int, ilo, ihi, scale, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(2).size else v.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgebak for the type c_double.
 */
inline proc gebak(matrix_order : lapack_memory_order, job : string, side : string, ilo : c_int, ihi : c_int, scale : [] c_double, v : [] c_double): c_int{
  return LAPACKE_dgebak(matrix_order, ascii(job) : c_char, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(1).size else v.domain.dim(2).size) : c_int, ilo, ihi, scale, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(2).size else v.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgebak for the type lapack_complex_float.
 */
inline proc gebak(matrix_order : lapack_memory_order, job : string, side : string, ilo : c_int, ihi : c_int, scale : [] c_float, v : [] complex(64)): c_int{
  return LAPACKE_cgebak(matrix_order, ascii(job) : c_char, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(1).size else v.domain.dim(2).size) : c_int, ilo, ihi, scale, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(2).size else v.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgebak for the type lapack_complex_double.
 */
inline proc gebak(matrix_order : lapack_memory_order, job : string, side : string, ilo : c_int, ihi : c_int, scale : [] c_double, v : [] complex(128)): c_int{
  return LAPACKE_zgebak(matrix_order, ascii(job) : c_char, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(1).size else v.domain.dim(2).size) : c_int, ilo, ihi, scale, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(2).size else v.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgebal for the type c_float.
 */
inline proc gebal(matrix_order : lapack_memory_order, job : string, a : [] c_float, ref ilo : c_int, ref ihi : c_int, scale : [] c_float): c_int{
  return LAPACKE_sgebal(matrix_order, ascii(job) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ilo, ihi, scale);
}

/* 
Wrapped procedure of LAPACKE_dgebal for the type c_double.
 */
inline proc gebal(matrix_order : lapack_memory_order, job : string, a : [] c_double, ref ilo : c_int, ref ihi : c_int, scale : [] c_double): c_int{
  return LAPACKE_dgebal(matrix_order, ascii(job) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ilo, ihi, scale);
}

/* 
Wrapped procedure of LAPACKE_cgebal for the type lapack_complex_float.
 */
inline proc gebal(matrix_order : lapack_memory_order, job : string, a : [] complex(64), ref ilo : c_int, ref ihi : c_int, scale : [] c_float): c_int{
  return LAPACKE_cgebal(matrix_order, ascii(job) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ilo, ihi, scale);
}

/* 
Wrapped procedure of LAPACKE_zgebal for the type lapack_complex_double.
 */
inline proc gebal(matrix_order : lapack_memory_order, job : string, a : [] complex(128), ref ilo : c_int, ref ihi : c_int, scale : [] c_double): c_int{
  return LAPACKE_zgebal(matrix_order, ascii(job) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ilo, ihi, scale);
}

/* 
Wrapped procedure of LAPACKE_sgebrd for the type c_float.
 */
inline proc gebrd(matrix_order : lapack_memory_order, a : [] c_float, d : [] c_float, e : [] c_float, tauq : [] c_float, taup : [] c_float): c_int{
  return LAPACKE_sgebrd(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, d, e, tauq, taup);
}

/* 
Wrapped procedure of LAPACKE_dgebrd for the type c_double.
 */
inline proc gebrd(matrix_order : lapack_memory_order, a : [] c_double, d : [] c_double, e : [] c_double, tauq : [] c_double, taup : [] c_double): c_int{
  return LAPACKE_dgebrd(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, d, e, tauq, taup);
}

/* 
Wrapped procedure of LAPACKE_cgebrd for the type lapack_complex_float.
 */
inline proc gebrd(matrix_order : lapack_memory_order, a : [] complex(64), d : [] c_float, e : [] c_float, tauq : [] complex(64), taup : [] complex(64)): c_int{
  return LAPACKE_cgebrd(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, d, e, tauq, taup);
}

/* 
Wrapped procedure of LAPACKE_zgebrd for the type lapack_complex_double.
 */
inline proc gebrd(matrix_order : lapack_memory_order, a : [] complex(128), d : [] c_double, e : [] c_double, tauq : [] complex(128), taup : [] complex(128)): c_int{
  return LAPACKE_zgebrd(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, d, e, tauq, taup);
}

/* 
Wrapped procedure of LAPACKE_sgecon for the type c_float.
 */
inline proc gecon(matrix_order : lapack_memory_order, norm : string, a : [] c_float, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_sgecon(matrix_order, ascii(norm) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_dgecon for the type c_double.
 */
inline proc gecon(matrix_order : lapack_memory_order, norm : string, a : [] c_double, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_dgecon(matrix_order, ascii(norm) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_cgecon for the type lapack_complex_float.
 */
inline proc gecon(matrix_order : lapack_memory_order, norm : string, a : [] complex(64), anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_cgecon(matrix_order, ascii(norm) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_zgecon for the type lapack_complex_double.
 */
inline proc gecon(matrix_order : lapack_memory_order, norm : string, a : [] complex(128), anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_zgecon(matrix_order, ascii(norm) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_sgeequ for the type c_float.
 */
inline proc geequ(matrix_order : lapack_memory_order, a : [] c_float, r : [] c_float, c : [] c_float, ref rowcnd : c_float, ref colcnd : c_float, ref amax : c_float): c_int{
  return LAPACKE_sgeequ(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_dgeequ for the type c_double.
 */
inline proc geequ(matrix_order : lapack_memory_order, a : [] c_double, r : [] c_double, c : [] c_double, ref rowcnd : c_double, ref colcnd : c_double, ref amax : c_double): c_int{
  return LAPACKE_dgeequ(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_cgeequ for the type lapack_complex_float.
 */
inline proc geequ(matrix_order : lapack_memory_order, a : [] complex(64), r : [] c_float, c : [] c_float, ref rowcnd : c_float, ref colcnd : c_float, ref amax : c_float): c_int{
  return LAPACKE_cgeequ(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_zgeequ for the type lapack_complex_double.
 */
inline proc geequ(matrix_order : lapack_memory_order, a : [] complex(128), r : [] c_double, c : [] c_double, ref rowcnd : c_double, ref colcnd : c_double, ref amax : c_double): c_int{
  return LAPACKE_zgeequ(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_sgeequb for the type c_float.
 */
inline proc geequb(matrix_order : lapack_memory_order, a : [] c_float, r : [] c_float, c : [] c_float, ref rowcnd : c_float, ref colcnd : c_float, ref amax : c_float): c_int{
  return LAPACKE_sgeequb(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_dgeequb for the type c_double.
 */
inline proc geequb(matrix_order : lapack_memory_order, a : [] c_double, r : [] c_double, c : [] c_double, ref rowcnd : c_double, ref colcnd : c_double, ref amax : c_double): c_int{
  return LAPACKE_dgeequb(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_cgeequb for the type lapack_complex_float.
 */
inline proc geequb(matrix_order : lapack_memory_order, a : [] complex(64), r : [] c_float, c : [] c_float, ref rowcnd : c_float, ref colcnd : c_float, ref amax : c_float): c_int{
  return LAPACKE_cgeequb(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_zgeequb for the type lapack_complex_double.
 */
inline proc geequb(matrix_order : lapack_memory_order, a : [] complex(128), r : [] c_double, c : [] c_double, ref rowcnd : c_double, ref colcnd : c_double, ref amax : c_double): c_int{
  return LAPACKE_zgeequb(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, r, c, rowcnd, colcnd, amax);
}

/* 
Wrapped procedure of LAPACKE_sgees for the type c_float.
 */
inline proc gees(matrix_order : lapack_memory_order, jobvs : string, sort : string, chlapack_select : LAPACK_S_SELECT2, a : [] c_float, ref sdim : c_int, wr : [] c_float, wi : [] c_float, vs : [] c_float): c_int{
  return LAPACKE_sgees(matrix_order, ascii(jobvs) : c_char, ascii(sort) : c_char, chlapack_select, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sdim, wr, wi, vs, (vs.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgees for the type c_double.
 */
inline proc gees(matrix_order : lapack_memory_order, jobvs : string, sort : string, chlapack_select : LAPACK_D_SELECT2, a : [] c_double, ref sdim : c_int, wr : [] c_double, wi : [] c_double, vs : [] c_double): c_int{
  return LAPACKE_dgees(matrix_order, ascii(jobvs) : c_char, ascii(sort) : c_char, chlapack_select, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sdim, wr, wi, vs, (vs.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgees for the type lapack_complex_float.
 */
inline proc gees(matrix_order : lapack_memory_order, jobvs : string, sort : string, chlapack_select : LAPACK_C_SELECT1, a : [] complex(64), ref sdim : c_int, w : [] complex(64), vs : [] complex(64)): c_int{
  return LAPACKE_cgees(matrix_order, ascii(jobvs) : c_char, ascii(sort) : c_char, chlapack_select, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sdim, w, vs, (vs.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgees for the type lapack_complex_double.
 */
inline proc gees(matrix_order : lapack_memory_order, jobvs : string, sort : string, chlapack_select : LAPACK_Z_SELECT1, a : [] complex(128), ref sdim : c_int, w : [] complex(128), vs : [] complex(128)): c_int{
  return LAPACKE_zgees(matrix_order, ascii(jobvs) : c_char, ascii(sort) : c_char, chlapack_select, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sdim, w, vs, (vs.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgeesx for the type c_float.
 */
inline proc geesx(matrix_order : lapack_memory_order, jobvs : string, sort : string, chlapack_select : LAPACK_S_SELECT2, sense : string, a : [] c_float, ref sdim : c_int, wr : [] c_float, wi : [] c_float, vs : [] c_float, ref rconde : c_float, ref rcondv : c_float): c_int{
  return LAPACKE_sgeesx(matrix_order, ascii(jobvs) : c_char, ascii(sort) : c_char, chlapack_select, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sdim, wr, wi, vs, (vs.domain.dim(2).size) : c_int, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_dgeesx for the type c_double.
 */
inline proc geesx(matrix_order : lapack_memory_order, jobvs : string, sort : string, chlapack_select : LAPACK_D_SELECT2, sense : string, a : [] c_double, ref sdim : c_int, wr : [] c_double, wi : [] c_double, vs : [] c_double, ref rconde : c_double, ref rcondv : c_double): c_int{
  return LAPACKE_dgeesx(matrix_order, ascii(jobvs) : c_char, ascii(sort) : c_char, chlapack_select, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sdim, wr, wi, vs, (vs.domain.dim(2).size) : c_int, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_cgeesx for the type lapack_complex_float.
 */
inline proc geesx(matrix_order : lapack_memory_order, jobvs : string, sort : string, chlapack_select : LAPACK_C_SELECT1, sense : string, a : [] complex(64), ref sdim : c_int, w : [] complex(64), vs : [] complex(64), ref rconde : c_float, ref rcondv : c_float): c_int{
  return LAPACKE_cgeesx(matrix_order, ascii(jobvs) : c_char, ascii(sort) : c_char, chlapack_select, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sdim, w, vs, (vs.domain.dim(2).size) : c_int, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_zgeesx for the type lapack_complex_double.
 */
inline proc geesx(matrix_order : lapack_memory_order, jobvs : string, sort : string, chlapack_select : LAPACK_Z_SELECT1, sense : string, a : [] complex(128), ref sdim : c_int, w : [] complex(128), vs : [] complex(128), ref rconde : c_double, ref rcondv : c_double): c_int{
  return LAPACKE_zgeesx(matrix_order, ascii(jobvs) : c_char, ascii(sort) : c_char, chlapack_select, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sdim, w, vs, (vs.domain.dim(2).size) : c_int, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_sgeev for the type c_float.
 */
inline proc geev(matrix_order : lapack_memory_order, jobvl : string, jobvr : string, a : [] c_float, wr : [] c_float, wi : [] c_float, vl : [] c_float, vr : [] c_float): c_int{
  return LAPACKE_sgeev(matrix_order, ascii(jobvl) : c_char, ascii(jobvr) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, wr, wi, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgeev for the type c_double.
 */
inline proc geev(matrix_order : lapack_memory_order, jobvl : string, jobvr : string, a : [] c_double, wr : [] c_double, wi : [] c_double, vl : [] c_double, vr : [] c_double): c_int{
  return LAPACKE_dgeev(matrix_order, ascii(jobvl) : c_char, ascii(jobvr) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, wr, wi, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgeev for the type lapack_complex_float.
 */
inline proc geev(matrix_order : lapack_memory_order, jobvl : string, jobvr : string, a : [] complex(64), w : [] complex(64), vl : [] complex(64), vr : [] complex(64)): c_int{
  return LAPACKE_cgeev(matrix_order, ascii(jobvl) : c_char, ascii(jobvr) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgeev for the type lapack_complex_double.
 */
inline proc geev(matrix_order : lapack_memory_order, jobvl : string, jobvr : string, a : [] complex(128), w : [] complex(128), vl : [] complex(128), vr : [] complex(128)): c_int{
  return LAPACKE_zgeev(matrix_order, ascii(jobvl) : c_char, ascii(jobvr) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgeevx for the type c_float.
 */
inline proc geevx(matrix_order : lapack_memory_order, balanc : string, jobvl : string, jobvr : string, sense : string, a : [] c_float, wr : [] c_float, wi : [] c_float, vl : [] c_float, vr : [] c_float, ref ilo : c_int, ref ihi : c_int, scale : [] c_float, ref abnrm : c_float, rconde : [] c_float, rcondv : [] c_float): c_int{
  return LAPACKE_sgeevx(matrix_order, ascii(balanc) : c_char, ascii(jobvl) : c_char, ascii(jobvr) : c_char, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, wr, wi, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, ilo, ihi, scale, abnrm, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_dgeevx for the type c_double.
 */
inline proc geevx(matrix_order : lapack_memory_order, balanc : string, jobvl : string, jobvr : string, sense : string, a : [] c_double, wr : [] c_double, wi : [] c_double, vl : [] c_double, vr : [] c_double, ref ilo : c_int, ref ihi : c_int, scale : [] c_double, ref abnrm : c_double, rconde : [] c_double, rcondv : [] c_double): c_int{
  return LAPACKE_dgeevx(matrix_order, ascii(balanc) : c_char, ascii(jobvl) : c_char, ascii(jobvr) : c_char, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, wr, wi, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, ilo, ihi, scale, abnrm, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_cgeevx for the type lapack_complex_float.
 */
inline proc geevx(matrix_order : lapack_memory_order, balanc : string, jobvl : string, jobvr : string, sense : string, a : [] complex(64), w : [] complex(64), vl : [] complex(64), vr : [] complex(64), ref ilo : c_int, ref ihi : c_int, scale : [] c_float, ref abnrm : c_float, rconde : [] c_float, rcondv : [] c_float): c_int{
  return LAPACKE_cgeevx(matrix_order, ascii(balanc) : c_char, ascii(jobvl) : c_char, ascii(jobvr) : c_char, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, ilo, ihi, scale, abnrm, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_zgeevx for the type lapack_complex_double.
 */
inline proc geevx(matrix_order : lapack_memory_order, balanc : string, jobvl : string, jobvr : string, sense : string, a : [] complex(128), w : [] complex(128), vl : [] complex(128), vr : [] complex(128), ref ilo : c_int, ref ihi : c_int, scale : [] c_double, ref abnrm : c_double, rconde : [] c_double, rcondv : [] c_double): c_int{
  return LAPACKE_zgeevx(matrix_order, ascii(balanc) : c_char, ascii(jobvl) : c_char, ascii(jobvr) : c_char, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, ilo, ihi, scale, abnrm, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_sgehrd for the type c_float.
 */
inline proc gehrd(matrix_order : lapack_memory_order, ilo : c_int, ihi : c_int, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sgehrd(matrix_order, (a.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dgehrd for the type c_double.
 */
inline proc gehrd(matrix_order : lapack_memory_order, ilo : c_int, ihi : c_int, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dgehrd(matrix_order, (a.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cgehrd for the type lapack_complex_float.
 */
inline proc gehrd(matrix_order : lapack_memory_order, ilo : c_int, ihi : c_int, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cgehrd(matrix_order, (a.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zgehrd for the type lapack_complex_double.
 */
inline proc gehrd(matrix_order : lapack_memory_order, ilo : c_int, ihi : c_int, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zgehrd(matrix_order, (a.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sgejsv for the type c_float.
 */
inline proc gejsv(matrix_order : lapack_memory_order, joba : string, jobu : string, jobv : string, jobr : string, jobt : string, jobp : string, a : [] c_float, sva : [] c_float, u : [] c_float, v : [] c_float, stat : [] c_float, istat : [] c_int): c_int{
  return LAPACKE_sgejsv(matrix_order, ascii(joba) : c_char, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobr) : c_char, ascii(jobt) : c_char, ascii(jobp) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sva, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, stat, istat);
}

/* 
Wrapped procedure of LAPACKE_dgejsv for the type c_double.
 */
inline proc gejsv(matrix_order : lapack_memory_order, joba : string, jobu : string, jobv : string, jobr : string, jobt : string, jobp : string, a : [] c_double, sva : [] c_double, u : [] c_double, v : [] c_double, stat : [] c_double, istat : [] c_int): c_int{
  return LAPACKE_dgejsv(matrix_order, ascii(joba) : c_char, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobr) : c_char, ascii(jobt) : c_char, ascii(jobp) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sva, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, stat, istat);
}

/* 
Wrapped procedure of LAPACKE_sgelq2 for the type c_float.
 */
inline proc gelq2(matrix_order : lapack_memory_order, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sgelq2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dgelq2 for the type c_double.
 */
inline proc gelq2(matrix_order : lapack_memory_order, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dgelq2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cgelq2 for the type lapack_complex_float.
 */
inline proc gelq2(matrix_order : lapack_memory_order, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cgelq2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zgelq2 for the type lapack_complex_double.
 */
inline proc gelq2(matrix_order : lapack_memory_order, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zgelq2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sgelqf for the type c_float.
 */
inline proc gelqf(matrix_order : lapack_memory_order, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sgelqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dgelqf for the type c_double.
 */
inline proc gelqf(matrix_order : lapack_memory_order, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dgelqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cgelqf for the type lapack_complex_float.
 */
inline proc gelqf(matrix_order : lapack_memory_order, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cgelqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zgelqf for the type lapack_complex_double.
 */
inline proc gelqf(matrix_order : lapack_memory_order, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zgelqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sgels for the type c_float.
 */
inline proc gels(matrix_order : lapack_memory_order, trans : string, a : [] c_float, b : [] c_float): c_int{
  return LAPACKE_sgels(matrix_order, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgels for the type c_double.
 */
inline proc gels(matrix_order : lapack_memory_order, trans : string, a : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dgels(matrix_order, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgels for the type lapack_complex_float.
 */
inline proc gels(matrix_order : lapack_memory_order, trans : string, a : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_cgels(matrix_order, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgels for the type lapack_complex_double.
 */
inline proc gels(matrix_order : lapack_memory_order, trans : string, a : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zgels(matrix_order, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgelsd for the type c_float.
 */
inline proc gelsd(matrix_order : lapack_memory_order, a : [] c_float, b : [] c_float, s : [] c_float, rcond : c_float): c_int{
  return LAPACKE_sgelsd(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, s, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgelsd for the type c_double.
 */
inline proc gelsd(matrix_order : lapack_memory_order, a : [] c_double, b : [] c_double, s : [] c_double, rcond : c_double): c_int{
  return LAPACKE_dgelsd(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, s, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgelsd for the type lapack_complex_float.
 */
inline proc gelsd(matrix_order : lapack_memory_order, a : [] complex(64), b : [] complex(64), s : [] c_float, rcond : c_float): c_int{
  return LAPACKE_cgelsd(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, s, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgelsd for the type lapack_complex_double.
 */
inline proc gelsd(matrix_order : lapack_memory_order, a : [] complex(128), b : [] complex(128), s : [] c_double, rcond : c_double): c_int{
  return LAPACKE_zgelsd(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, s, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgelss for the type c_float.
 */
inline proc gelss(matrix_order : lapack_memory_order, a : [] c_float, b : [] c_float, s : [] c_float, rcond : c_float): c_int{
  return LAPACKE_sgelss(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, s, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgelss for the type c_double.
 */
inline proc gelss(matrix_order : lapack_memory_order, a : [] c_double, b : [] c_double, s : [] c_double, rcond : c_double): c_int{
  return LAPACKE_dgelss(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, s, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgelss for the type lapack_complex_float.
 */
inline proc gelss(matrix_order : lapack_memory_order, a : [] complex(64), b : [] complex(64), s : [] c_float, rcond : c_float): c_int{
  return LAPACKE_cgelss(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, s, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgelss for the type lapack_complex_double.
 */
inline proc gelss(matrix_order : lapack_memory_order, a : [] complex(128), b : [] complex(128), s : [] c_double, rcond : c_double): c_int{
  return LAPACKE_zgelss(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, s, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgelsy for the type c_float.
 */
inline proc gelsy(matrix_order : lapack_memory_order, a : [] c_float, b : [] c_float, jpvt : [] c_int, rcond : c_float): c_int{
  return LAPACKE_sgelsy(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, jpvt, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgelsy for the type c_double.
 */
inline proc gelsy(matrix_order : lapack_memory_order, a : [] c_double, b : [] c_double, jpvt : [] c_int, rcond : c_double): c_int{
  return LAPACKE_dgelsy(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, jpvt, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgelsy for the type lapack_complex_float.
 */
inline proc gelsy(matrix_order : lapack_memory_order, a : [] complex(64), b : [] complex(64), jpvt : [] c_int, rcond : c_float): c_int{
  return LAPACKE_cgelsy(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, jpvt, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgelsy for the type lapack_complex_double.
 */
inline proc gelsy(matrix_order : lapack_memory_order, a : [] complex(128), b : [] complex(128), jpvt : [] c_int, rcond : c_double): c_int{
  return LAPACKE_zgelsy(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, jpvt, rcond, (a.domain.dim(1).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgeqlf for the type c_float.
 */
inline proc geqlf(matrix_order : lapack_memory_order, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sgeqlf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dgeqlf for the type c_double.
 */
inline proc geqlf(matrix_order : lapack_memory_order, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dgeqlf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cgeqlf for the type lapack_complex_float.
 */
inline proc geqlf(matrix_order : lapack_memory_order, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cgeqlf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zgeqlf for the type lapack_complex_double.
 */
inline proc geqlf(matrix_order : lapack_memory_order, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zgeqlf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sgeqp3 for the type c_float.
 */
inline proc geqp3(matrix_order : lapack_memory_order, a : [] c_float, jpvt : [] c_int, tau : [] c_float): c_int{
  return LAPACKE_sgeqp3(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, jpvt, tau);
}

/* 
Wrapped procedure of LAPACKE_dgeqp3 for the type c_double.
 */
inline proc geqp3(matrix_order : lapack_memory_order, a : [] c_double, jpvt : [] c_int, tau : [] c_double): c_int{
  return LAPACKE_dgeqp3(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, jpvt, tau);
}

/* 
Wrapped procedure of LAPACKE_cgeqp3 for the type lapack_complex_float.
 */
inline proc geqp3(matrix_order : lapack_memory_order, a : [] complex(64), jpvt : [] c_int, tau : [] complex(64)): c_int{
  return LAPACKE_cgeqp3(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, jpvt, tau);
}

/* 
Wrapped procedure of LAPACKE_zgeqp3 for the type lapack_complex_double.
 */
inline proc geqp3(matrix_order : lapack_memory_order, a : [] complex(128), jpvt : [] c_int, tau : [] complex(128)): c_int{
  return LAPACKE_zgeqp3(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, jpvt, tau);
}

/* 
Wrapped procedure of LAPACKE_sgeqpf for the type c_float.
 */
inline proc geqpf(matrix_order : lapack_memory_order, a : [] c_float, jpvt : [] c_int, tau : [] c_float): c_int{
  return LAPACKE_sgeqpf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, jpvt, tau);
}

/* 
Wrapped procedure of LAPACKE_dgeqpf for the type c_double.
 */
inline proc geqpf(matrix_order : lapack_memory_order, a : [] c_double, jpvt : [] c_int, tau : [] c_double): c_int{
  return LAPACKE_dgeqpf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, jpvt, tau);
}

/* 
Wrapped procedure of LAPACKE_cgeqpf for the type lapack_complex_float.
 */
inline proc geqpf(matrix_order : lapack_memory_order, a : [] complex(64), jpvt : [] c_int, tau : [] complex(64)): c_int{
  return LAPACKE_cgeqpf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, jpvt, tau);
}

/* 
Wrapped procedure of LAPACKE_zgeqpf for the type lapack_complex_double.
 */
inline proc geqpf(matrix_order : lapack_memory_order, a : [] complex(128), jpvt : [] c_int, tau : [] complex(128)): c_int{
  return LAPACKE_zgeqpf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, jpvt, tau);
}

/* 
Wrapped procedure of LAPACKE_sgeqr2 for the type c_float.
 */
inline proc geqr2(matrix_order : lapack_memory_order, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sgeqr2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dgeqr2 for the type c_double.
 */
inline proc geqr2(matrix_order : lapack_memory_order, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dgeqr2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cgeqr2 for the type lapack_complex_float.
 */
inline proc geqr2(matrix_order : lapack_memory_order, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cgeqr2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zgeqr2 for the type lapack_complex_double.
 */
inline proc geqr2(matrix_order : lapack_memory_order, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zgeqr2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sgeqrf for the type c_float.
 */
inline proc geqrf(matrix_order : lapack_memory_order, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sgeqrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dgeqrf for the type c_double.
 */
inline proc geqrf(matrix_order : lapack_memory_order, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dgeqrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cgeqrf for the type lapack_complex_float.
 */
inline proc geqrf(matrix_order : lapack_memory_order, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cgeqrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zgeqrf for the type lapack_complex_double.
 */
inline proc geqrf(matrix_order : lapack_memory_order, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zgeqrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sgeqrfp for the type c_float.
 */
inline proc geqrfp(matrix_order : lapack_memory_order, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sgeqrfp(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dgeqrfp for the type c_double.
 */
inline proc geqrfp(matrix_order : lapack_memory_order, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dgeqrfp(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cgeqrfp for the type lapack_complex_float.
 */
inline proc geqrfp(matrix_order : lapack_memory_order, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cgeqrfp(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zgeqrfp for the type lapack_complex_double.
 */
inline proc geqrfp(matrix_order : lapack_memory_order, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zgeqrfp(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sgerfs for the type c_float.
 */
inline proc gerfs(matrix_order : lapack_memory_order, trans : string, a : [] c_float, af : [] c_float, ipiv : [] c_int, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_sgerfs(matrix_order, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dgerfs for the type c_double.
 */
inline proc gerfs(matrix_order : lapack_memory_order, trans : string, a : [] c_double, af : [] c_double, ipiv : [] c_int, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dgerfs(matrix_order, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cgerfs for the type lapack_complex_float.
 */
inline proc gerfs(matrix_order : lapack_memory_order, trans : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cgerfs(matrix_order, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zgerfs for the type lapack_complex_double.
 */
inline proc gerfs(matrix_order : lapack_memory_order, trans : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zgerfs(matrix_order, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_sgerfsx for the type c_float.
 */
inline proc gerfsx(matrix_order : lapack_memory_order, trans : string, equed : string, a : [] c_float, af : [] c_float, ipiv : [] c_int, r : [] c_float, c : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_sgerfsx(matrix_order, ascii(trans) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_dgerfsx for the type c_double.
 */
inline proc gerfsx(matrix_order : lapack_memory_order, trans : string, equed : string, a : [] c_double, af : [] c_double, ipiv : [] c_int, r : [] c_double, c : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_dgerfsx(matrix_order, ascii(trans) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_cgerfsx for the type lapack_complex_float.
 */
inline proc gerfsx(matrix_order : lapack_memory_order, trans : string, equed : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, r : [] c_float, c : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_cgerfsx(matrix_order, ascii(trans) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_zgerfsx for the type lapack_complex_double.
 */
inline proc gerfsx(matrix_order : lapack_memory_order, trans : string, equed : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, r : [] c_double, c : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_zgerfsx(matrix_order, ascii(trans) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_sgerqf for the type c_float.
 */
inline proc gerqf(matrix_order : lapack_memory_order, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sgerqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dgerqf for the type c_double.
 */
inline proc gerqf(matrix_order : lapack_memory_order, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dgerqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cgerqf for the type lapack_complex_float.
 */
inline proc gerqf(matrix_order : lapack_memory_order, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cgerqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zgerqf for the type lapack_complex_double.
 */
inline proc gerqf(matrix_order : lapack_memory_order, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zgerqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sgesdd for the type c_float.
 */
inline proc gesdd(matrix_order : lapack_memory_order, jobz : string, a : [] c_float, s : [] c_float, u : [] c_float, vt : [] c_float): c_int{
  return LAPACKE_sgesdd(matrix_order, ascii(jobz) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, u, (u.domain.dim(2).size) : c_int, vt, (vt.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgesdd for the type c_double.
 */
inline proc gesdd(matrix_order : lapack_memory_order, jobz : string, a : [] c_double, s : [] c_double, u : [] c_double, vt : [] c_double): c_int{
  return LAPACKE_dgesdd(matrix_order, ascii(jobz) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, u, (u.domain.dim(2).size) : c_int, vt, (vt.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgesdd for the type lapack_complex_float.
 */
inline proc gesdd(matrix_order : lapack_memory_order, jobz : string, a : [] complex(64), s : [] c_float, u : [] complex(64), vt : [] complex(64)): c_int{
  return LAPACKE_cgesdd(matrix_order, ascii(jobz) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, u, (u.domain.dim(2).size) : c_int, vt, (vt.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgesdd for the type lapack_complex_double.
 */
inline proc gesdd(matrix_order : lapack_memory_order, jobz : string, a : [] complex(128), s : [] c_double, u : [] complex(128), vt : [] complex(128)): c_int{
  return LAPACKE_zgesdd(matrix_order, ascii(jobz) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, u, (u.domain.dim(2).size) : c_int, vt, (vt.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgesv for the type c_float.
 */
inline proc gesv(matrix_order : lapack_memory_order, a : [] c_float, ipiv : [] c_int, b : [] c_float): c_int{
  return LAPACKE_sgesv(matrix_order, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgesv for the type c_double.
 */
inline proc gesv(matrix_order : lapack_memory_order, a : [] c_double, ipiv : [] c_int, b : [] c_double): c_int{
  return LAPACKE_dgesv(matrix_order, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgesv for the type lapack_complex_float.
 */
inline proc gesv(matrix_order : lapack_memory_order, a : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_cgesv(matrix_order, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgesv for the type lapack_complex_double.
 */
inline proc gesv(matrix_order : lapack_memory_order, a : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zgesv(matrix_order, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsgesv for the type c_double.
 */
inline proc gesv(matrix_order : lapack_memory_order, a : [] c_double, ipiv : [] c_int, b : [] c_double, x : [] c_double, ref chlapack_iter : c_int): c_int{
  return LAPACKE_dsgesv(matrix_order, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, chlapack_iter);
}

/* 
Wrapped procedure of LAPACKE_zcgesv for the type lapack_complex_double.
 */
inline proc gesv(matrix_order : lapack_memory_order, a : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ref chlapack_iter : c_int): c_int{
  return LAPACKE_zcgesv(matrix_order, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, chlapack_iter);
}

/* 
Wrapped procedure of LAPACKE_sgesvd for the type c_float.
 */
inline proc gesvd(matrix_order : lapack_memory_order, jobu : string, jobvt : string, a : [] c_float, s : [] c_float, u : [] c_float, vt : [] c_float, superb : [] c_float): c_int{
  return LAPACKE_sgesvd(matrix_order, ascii(jobu) : c_char, ascii(jobvt) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, u, (u.domain.dim(2).size) : c_int, vt, (vt.domain.dim(2).size) : c_int, superb);
}

/* 
Wrapped procedure of LAPACKE_dgesvd for the type c_double.
 */
inline proc gesvd(matrix_order : lapack_memory_order, jobu : string, jobvt : string, a : [] c_double, s : [] c_double, u : [] c_double, vt : [] c_double, superb : [] c_double): c_int{
  return LAPACKE_dgesvd(matrix_order, ascii(jobu) : c_char, ascii(jobvt) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, u, (u.domain.dim(2).size) : c_int, vt, (vt.domain.dim(2).size) : c_int, superb);
}

/* 
Wrapped procedure of LAPACKE_cgesvd for the type lapack_complex_float.
 */
inline proc gesvd(matrix_order : lapack_memory_order, jobu : string, jobvt : string, a : [] complex(64), s : [] c_float, u : [] complex(64), vt : [] complex(64), superb : [] c_float): c_int{
  return LAPACKE_cgesvd(matrix_order, ascii(jobu) : c_char, ascii(jobvt) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, u, (u.domain.dim(2).size) : c_int, vt, (vt.domain.dim(2).size) : c_int, superb);
}

/* 
Wrapped procedure of LAPACKE_zgesvd for the type lapack_complex_double.
 */
inline proc gesvd(matrix_order : lapack_memory_order, jobu : string, jobvt : string, a : [] complex(128), s : [] c_double, u : [] complex(128), vt : [] complex(128), superb : [] c_double): c_int{
  return LAPACKE_zgesvd(matrix_order, ascii(jobu) : c_char, ascii(jobvt) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, u, (u.domain.dim(2).size) : c_int, vt, (vt.domain.dim(2).size) : c_int, superb);
}

/* 
Wrapped procedure of LAPACKE_sgesvj for the type c_float.
 */
inline proc gesvj(matrix_order : lapack_memory_order, joba : string, jobu : string, jobv : string, a : [] c_float, sva : [] c_float, mv : c_int, v : [] c_float, stat : [] c_float): c_int{
  return LAPACKE_sgesvj(matrix_order, ascii(joba) : c_char, ascii(jobu) : c_char, ascii(jobv) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sva, mv, v, (v.domain.dim(2).size) : c_int, stat);
}

/* 
Wrapped procedure of LAPACKE_dgesvj for the type c_double.
 */
inline proc gesvj(matrix_order : lapack_memory_order, joba : string, jobu : string, jobv : string, a : [] c_double, sva : [] c_double, mv : c_int, v : [] c_double, stat : [] c_double): c_int{
  return LAPACKE_dgesvj(matrix_order, ascii(joba) : c_char, ascii(jobu) : c_char, ascii(jobv) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sva, mv, v, (v.domain.dim(2).size) : c_int, stat);
}

/* 
Wrapped procedure of LAPACKE_sgesvx for the type c_float.
 */
inline proc gesvx(matrix_order : lapack_memory_order, fact : string, trans : string, a : [] c_float, af : [] c_float, ipiv : [] c_int, ref equed : string, r : [] c_float, c : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, ferr : [] c_float, berr : [] c_float, rpivot : [] c_float): c_int{
  return LAPACKE_sgesvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr, rpivot);
}

/* 
Wrapped procedure of LAPACKE_dgesvx for the type c_double.
 */
inline proc gesvx(matrix_order : lapack_memory_order, fact : string, trans : string, a : [] c_double, af : [] c_double, ipiv : [] c_int, ref equed : string, r : [] c_double, c : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, ferr : [] c_double, berr : [] c_double, rpivot : [] c_double): c_int{
  return LAPACKE_dgesvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr, rpivot);
}

/* 
Wrapped procedure of LAPACKE_cgesvx for the type lapack_complex_float.
 */
inline proc gesvx(matrix_order : lapack_memory_order, fact : string, trans : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, ref equed : string, r : [] c_float, c : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ferr : [] c_float, berr : [] c_float, rpivot : [] c_float): c_int{
  return LAPACKE_cgesvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr, rpivot);
}

/* 
Wrapped procedure of LAPACKE_zgesvx for the type lapack_complex_double.
 */
inline proc gesvx(matrix_order : lapack_memory_order, fact : string, trans : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, ref equed : string, r : [] c_double, c : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ferr : [] c_double, berr : [] c_double, rpivot : [] c_double): c_int{
  return LAPACKE_zgesvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr, rpivot);
}

/* 
Wrapped procedure of LAPACKE_sgesvxx for the type c_float.
 */
inline proc gesvxx(matrix_order : lapack_memory_order, fact : string, trans : string, a : [] c_float, af : [] c_float, ipiv : [] c_int, ref equed : string, r : [] c_float, c : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, ref rpvgrw : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_sgesvxx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_dgesvxx for the type c_double.
 */
inline proc gesvxx(matrix_order : lapack_memory_order, fact : string, trans : string, a : [] c_double, af : [] c_double, ipiv : [] c_int, ref equed : string, r : [] c_double, c : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, ref rpvgrw : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_dgesvxx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_cgesvxx for the type lapack_complex_float.
 */
inline proc gesvxx(matrix_order : lapack_memory_order, fact : string, trans : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, ref equed : string, r : [] c_float, c : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ref rpvgrw : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_cgesvxx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_zgesvxx for the type lapack_complex_double.
 */
inline proc gesvxx(matrix_order : lapack_memory_order, fact : string, trans : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, ref equed : string, r : [] c_double, c : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ref rpvgrw : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_zgesvxx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, r, c, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_sgetf2 for the type c_float.
 */
inline proc getf2(matrix_order : lapack_memory_order, a : [] c_float, ipiv : [] c_int): c_int{
  return LAPACKE_sgetf2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_dgetf2 for the type c_double.
 */
inline proc getf2(matrix_order : lapack_memory_order, a : [] c_double, ipiv : [] c_int): c_int{
  return LAPACKE_dgetf2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_cgetf2 for the type lapack_complex_float.
 */
inline proc getf2(matrix_order : lapack_memory_order, a : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_cgetf2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zgetf2 for the type lapack_complex_double.
 */
inline proc getf2(matrix_order : lapack_memory_order, a : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zgetf2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_sgetrf for the type c_float.
 */
inline proc getrf(matrix_order : lapack_memory_order, a : [] c_float, ipiv : [] c_int): c_int{
  return LAPACKE_sgetrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_dgetrf for the type c_double.
 */
inline proc getrf(matrix_order : lapack_memory_order, a : [] c_double, ipiv : [] c_int): c_int{
  return LAPACKE_dgetrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_cgetrf for the type lapack_complex_float.
 */
inline proc getrf(matrix_order : lapack_memory_order, a : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_cgetrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zgetrf for the type lapack_complex_double.
 */
inline proc getrf(matrix_order : lapack_memory_order, a : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zgetrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_sgetri for the type c_float.
 */
inline proc getri(matrix_order : lapack_memory_order, a : [] c_float, ipiv : [] c_int): c_int{
  return LAPACKE_sgetri(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_dgetri for the type c_double.
 */
inline proc getri(matrix_order : lapack_memory_order, a : [] c_double, ipiv : [] c_int): c_int{
  return LAPACKE_dgetri(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_cgetri for the type lapack_complex_float.
 */
inline proc getri(matrix_order : lapack_memory_order, a : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_cgetri(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zgetri for the type lapack_complex_double.
 */
inline proc getri(matrix_order : lapack_memory_order, a : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zgetri(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_sgetrs for the type c_float.
 */
inline proc getrs(matrix_order : lapack_memory_order, trans : string, a : [] c_float, ipiv : [] c_int, b : [] c_float): c_int{
  return LAPACKE_sgetrs(matrix_order, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgetrs for the type c_double.
 */
inline proc getrs(matrix_order : lapack_memory_order, trans : string, a : [] c_double, ipiv : [] c_int, b : [] c_double): c_int{
  return LAPACKE_dgetrs(matrix_order, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgetrs for the type lapack_complex_float.
 */
inline proc getrs(matrix_order : lapack_memory_order, trans : string, a : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_cgetrs(matrix_order, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgetrs for the type lapack_complex_double.
 */
inline proc getrs(matrix_order : lapack_memory_order, trans : string, a : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zgetrs(matrix_order, ascii(trans) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sggbak for the type c_float.
 */
inline proc ggbak(matrix_order : lapack_memory_order, job : string, side : string, ilo : c_int, ihi : c_int, lscale : [] c_float, rscale : [] c_float, v : [] c_float): c_int{
  return LAPACKE_sggbak(matrix_order, ascii(job) : c_char, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(1).size else v.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(2).size else v.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dggbak for the type c_double.
 */
inline proc ggbak(matrix_order : lapack_memory_order, job : string, side : string, ilo : c_int, ihi : c_int, lscale : [] c_double, rscale : [] c_double, v : [] c_double): c_int{
  return LAPACKE_dggbak(matrix_order, ascii(job) : c_char, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(1).size else v.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(2).size else v.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cggbak for the type lapack_complex_float.
 */
inline proc ggbak(matrix_order : lapack_memory_order, job : string, side : string, ilo : c_int, ihi : c_int, lscale : [] c_float, rscale : [] c_float, v : [] complex(64)): c_int{
  return LAPACKE_cggbak(matrix_order, ascii(job) : c_char, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(1).size else v.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(2).size else v.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zggbak for the type lapack_complex_double.
 */
inline proc ggbak(matrix_order : lapack_memory_order, job : string, side : string, ilo : c_int, ihi : c_int, lscale : [] c_double, rscale : [] c_double, v : [] complex(128)): c_int{
  return LAPACKE_zggbak(matrix_order, ascii(job) : c_char, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(1).size else v.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale, (if matrix_order == lapack_memory_order.row_major then v.domain.dim(2).size else v.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sggbal for the type c_float.
 */
inline proc ggbal(matrix_order : lapack_memory_order, job : string, a : [] c_float, b : [] c_float, ref ilo : c_int, ref ihi : c_int, lscale : [] c_float, rscale : [] c_float): c_int{
  return LAPACKE_sggbal(matrix_order, ascii(job) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale);
}

/* 
Wrapped procedure of LAPACKE_dggbal for the type c_double.
 */
inline proc ggbal(matrix_order : lapack_memory_order, job : string, a : [] c_double, b : [] c_double, ref ilo : c_int, ref ihi : c_int, lscale : [] c_double, rscale : [] c_double): c_int{
  return LAPACKE_dggbal(matrix_order, ascii(job) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale);
}

/* 
Wrapped procedure of LAPACKE_cggbal for the type lapack_complex_float.
 */
inline proc ggbal(matrix_order : lapack_memory_order, job : string, a : [] complex(64), b : [] complex(64), ref ilo : c_int, ref ihi : c_int, lscale : [] c_float, rscale : [] c_float): c_int{
  return LAPACKE_cggbal(matrix_order, ascii(job) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale);
}

/* 
Wrapped procedure of LAPACKE_zggbal for the type lapack_complex_double.
 */
inline proc ggbal(matrix_order : lapack_memory_order, job : string, a : [] complex(128), b : [] complex(128), ref ilo : c_int, ref ihi : c_int, lscale : [] c_double, rscale : [] c_double): c_int{
  return LAPACKE_zggbal(matrix_order, ascii(job) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale);
}

/* 
Wrapped procedure of LAPACKE_sgges for the type c_float.
 */
inline proc gges(matrix_order : lapack_memory_order, jobvsl : string, jobvsr : string, sort : string, selctg : LAPACK_S_SELECT3, a : [] c_float, b : [] c_float, ref sdim : c_int, alphar : [] c_float, alphai : [] c_float, beta : [] c_float, vsl : [] c_float, vsr : [] c_float): c_int{
  return LAPACKE_sgges(matrix_order, ascii(jobvsl) : c_char, ascii(jobvsr) : c_char, ascii(sort) : c_char, selctg, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, sdim, alphar, alphai, beta, vsl, (vsl.domain.dim(2).size) : c_int, vsr, (vsr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgges for the type c_double.
 */
inline proc gges(matrix_order : lapack_memory_order, jobvsl : string, jobvsr : string, sort : string, selctg : LAPACK_D_SELECT3, a : [] c_double, b : [] c_double, ref sdim : c_int, alphar : [] c_double, alphai : [] c_double, beta : [] c_double, vsl : [] c_double, vsr : [] c_double): c_int{
  return LAPACKE_dgges(matrix_order, ascii(jobvsl) : c_char, ascii(jobvsr) : c_char, ascii(sort) : c_char, selctg, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, sdim, alphar, alphai, beta, vsl, (vsl.domain.dim(2).size) : c_int, vsr, (vsr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgges for the type lapack_complex_float.
 */
inline proc gges(matrix_order : lapack_memory_order, jobvsl : string, jobvsr : string, sort : string, selctg : LAPACK_C_SELECT2, a : [] complex(64), b : [] complex(64), ref sdim : c_int, alpha : [] complex(64), beta : [] complex(64), vsl : [] complex(64), vsr : [] complex(64)): c_int{
  return LAPACKE_cgges(matrix_order, ascii(jobvsl) : c_char, ascii(jobvsr) : c_char, ascii(sort) : c_char, selctg, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, sdim, alpha, beta, vsl, (vsl.domain.dim(2).size) : c_int, vsr, (vsr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgges for the type lapack_complex_double.
 */
inline proc gges(matrix_order : lapack_memory_order, jobvsl : string, jobvsr : string, sort : string, selctg : LAPACK_Z_SELECT2, a : [] complex(128), b : [] complex(128), ref sdim : c_int, alpha : [] complex(128), beta : [] complex(128), vsl : [] complex(128), vsr : [] complex(128)): c_int{
  return LAPACKE_zgges(matrix_order, ascii(jobvsl) : c_char, ascii(jobvsr) : c_char, ascii(sort) : c_char, selctg, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, sdim, alpha, beta, vsl, (vsl.domain.dim(2).size) : c_int, vsr, (vsr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sggesx for the type c_float.
 */
inline proc ggesx(matrix_order : lapack_memory_order, jobvsl : string, jobvsr : string, sort : string, selctg : LAPACK_S_SELECT3, sense : string, a : [] c_float, b : [] c_float, ref sdim : c_int, alphar : [] c_float, alphai : [] c_float, beta : [] c_float, vsl : [] c_float, vsr : [] c_float, rconde : [] c_float, rcondv : [] c_float): c_int{
  return LAPACKE_sggesx(matrix_order, ascii(jobvsl) : c_char, ascii(jobvsr) : c_char, ascii(sort) : c_char, selctg, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, sdim, alphar, alphai, beta, vsl, (vsl.domain.dim(2).size) : c_int, vsr, (vsr.domain.dim(2).size) : c_int, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_dggesx for the type c_double.
 */
inline proc ggesx(matrix_order : lapack_memory_order, jobvsl : string, jobvsr : string, sort : string, selctg : LAPACK_D_SELECT3, sense : string, a : [] c_double, b : [] c_double, ref sdim : c_int, alphar : [] c_double, alphai : [] c_double, beta : [] c_double, vsl : [] c_double, vsr : [] c_double, rconde : [] c_double, rcondv : [] c_double): c_int{
  return LAPACKE_dggesx(matrix_order, ascii(jobvsl) : c_char, ascii(jobvsr) : c_char, ascii(sort) : c_char, selctg, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, sdim, alphar, alphai, beta, vsl, (vsl.domain.dim(2).size) : c_int, vsr, (vsr.domain.dim(2).size) : c_int, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_cggesx for the type lapack_complex_float.
 */
inline proc ggesx(matrix_order : lapack_memory_order, jobvsl : string, jobvsr : string, sort : string, selctg : LAPACK_C_SELECT2, sense : string, a : [] complex(64), b : [] complex(64), ref sdim : c_int, alpha : [] complex(64), beta : [] complex(64), vsl : [] complex(64), vsr : [] complex(64), rconde : [] c_float, rcondv : [] c_float): c_int{
  return LAPACKE_cggesx(matrix_order, ascii(jobvsl) : c_char, ascii(jobvsr) : c_char, ascii(sort) : c_char, selctg, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, sdim, alpha, beta, vsl, (vsl.domain.dim(2).size) : c_int, vsr, (vsr.domain.dim(2).size) : c_int, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_zggesx for the type lapack_complex_double.
 */
inline proc ggesx(matrix_order : lapack_memory_order, jobvsl : string, jobvsr : string, sort : string, selctg : LAPACK_Z_SELECT2, sense : string, a : [] complex(128), b : [] complex(128), ref sdim : c_int, alpha : [] complex(128), beta : [] complex(128), vsl : [] complex(128), vsr : [] complex(128), rconde : [] c_double, rcondv : [] c_double): c_int{
  return LAPACKE_zggesx(matrix_order, ascii(jobvsl) : c_char, ascii(jobvsr) : c_char, ascii(sort) : c_char, selctg, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, sdim, alpha, beta, vsl, (vsl.domain.dim(2).size) : c_int, vsr, (vsr.domain.dim(2).size) : c_int, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_sggev for the type c_float.
 */
inline proc ggev(matrix_order : lapack_memory_order, jobvl : string, jobvr : string, a : [] c_float, b : [] c_float, alphar : [] c_float, alphai : [] c_float, beta : [] c_float, vl : [] c_float, vr : [] c_float): c_int{
  return LAPACKE_sggev(matrix_order, ascii(jobvl) : c_char, ascii(jobvr) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alphar, alphai, beta, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dggev for the type c_double.
 */
inline proc ggev(matrix_order : lapack_memory_order, jobvl : string, jobvr : string, a : [] c_double, b : [] c_double, alphar : [] c_double, alphai : [] c_double, beta : [] c_double, vl : [] c_double, vr : [] c_double): c_int{
  return LAPACKE_dggev(matrix_order, ascii(jobvl) : c_char, ascii(jobvr) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alphar, alphai, beta, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cggev for the type lapack_complex_float.
 */
inline proc ggev(matrix_order : lapack_memory_order, jobvl : string, jobvr : string, a : [] complex(64), b : [] complex(64), alpha : [] complex(64), beta : [] complex(64), vl : [] complex(64), vr : [] complex(64)): c_int{
  return LAPACKE_cggev(matrix_order, ascii(jobvl) : c_char, ascii(jobvr) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alpha, beta, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zggev for the type lapack_complex_double.
 */
inline proc ggev(matrix_order : lapack_memory_order, jobvl : string, jobvr : string, a : [] complex(128), b : [] complex(128), alpha : [] complex(128), beta : [] complex(128), vl : [] complex(128), vr : [] complex(128)): c_int{
  return LAPACKE_zggev(matrix_order, ascii(jobvl) : c_char, ascii(jobvr) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alpha, beta, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sggevx for the type c_float.
 */
inline proc ggevx(matrix_order : lapack_memory_order, balanc : string, jobvl : string, jobvr : string, sense : string, a : [] c_float, b : [] c_float, alphar : [] c_float, alphai : [] c_float, beta : [] c_float, vl : [] c_float, vr : [] c_float, ref ilo : c_int, ref ihi : c_int, lscale : [] c_float, rscale : [] c_float, ref abnrm : c_float, ref bbnrm : c_float, rconde : [] c_float, rcondv : [] c_float): c_int{
  return LAPACKE_sggevx(matrix_order, ascii(balanc) : c_char, ascii(jobvl) : c_char, ascii(jobvr) : c_char, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alphar, alphai, beta, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale, abnrm, bbnrm, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_dggevx for the type c_double.
 */
inline proc ggevx(matrix_order : lapack_memory_order, balanc : string, jobvl : string, jobvr : string, sense : string, a : [] c_double, b : [] c_double, alphar : [] c_double, alphai : [] c_double, beta : [] c_double, vl : [] c_double, vr : [] c_double, ref ilo : c_int, ref ihi : c_int, lscale : [] c_double, rscale : [] c_double, ref abnrm : c_double, ref bbnrm : c_double, rconde : [] c_double, rcondv : [] c_double): c_int{
  return LAPACKE_dggevx(matrix_order, ascii(balanc) : c_char, ascii(jobvl) : c_char, ascii(jobvr) : c_char, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alphar, alphai, beta, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale, abnrm, bbnrm, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_cggevx for the type lapack_complex_float.
 */
inline proc ggevx(matrix_order : lapack_memory_order, balanc : string, jobvl : string, jobvr : string, sense : string, a : [] complex(64), b : [] complex(64), alpha : [] complex(64), beta : [] complex(64), vl : [] complex(64), vr : [] complex(64), ref ilo : c_int, ref ihi : c_int, lscale : [] c_float, rscale : [] c_float, ref abnrm : c_float, ref bbnrm : c_float, rconde : [] c_float, rcondv : [] c_float): c_int{
  return LAPACKE_cggevx(matrix_order, ascii(balanc) : c_char, ascii(jobvl) : c_char, ascii(jobvr) : c_char, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alpha, beta, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale, abnrm, bbnrm, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_zggevx for the type lapack_complex_double.
 */
inline proc ggevx(matrix_order : lapack_memory_order, balanc : string, jobvl : string, jobvr : string, sense : string, a : [] complex(128), b : [] complex(128), alpha : [] complex(128), beta : [] complex(128), vl : [] complex(128), vr : [] complex(128), ref ilo : c_int, ref ihi : c_int, lscale : [] c_double, rscale : [] c_double, ref abnrm : c_double, ref bbnrm : c_double, rconde : [] c_double, rcondv : [] c_double): c_int{
  return LAPACKE_zggevx(matrix_order, ascii(balanc) : c_char, ascii(jobvl) : c_char, ascii(jobvr) : c_char, ascii(sense) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alpha, beta, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, ilo, ihi, lscale, rscale, abnrm, bbnrm, rconde, rcondv);
}

/* 
Wrapped procedure of LAPACKE_sggglm for the type c_float.
 */
inline proc ggglm(matrix_order : lapack_memory_order, a : [] c_float, b : [] c_float, d : [] c_float, x : [] c_float, y : [] c_float): c_int{
  return LAPACKE_sggglm(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, d, x, y);
}

/* 
Wrapped procedure of LAPACKE_dggglm for the type c_double.
 */
inline proc ggglm(matrix_order : lapack_memory_order, a : [] c_double, b : [] c_double, d : [] c_double, x : [] c_double, y : [] c_double): c_int{
  return LAPACKE_dggglm(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, d, x, y);
}

/* 
Wrapped procedure of LAPACKE_cggglm for the type lapack_complex_float.
 */
inline proc ggglm(matrix_order : lapack_memory_order, a : [] complex(64), b : [] complex(64), d : [] complex(64), x : [] complex(64), y : [] complex(64)): c_int{
  return LAPACKE_cggglm(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, d, x, y);
}

/* 
Wrapped procedure of LAPACKE_zggglm for the type lapack_complex_double.
 */
inline proc ggglm(matrix_order : lapack_memory_order, a : [] complex(128), b : [] complex(128), d : [] complex(128), x : [] complex(128), y : [] complex(128)): c_int{
  return LAPACKE_zggglm(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, d, x, y);
}

/* 
Wrapped procedure of LAPACKE_sgghrd for the type c_float.
 */
inline proc gghrd(matrix_order : lapack_memory_order, compq : string, compz : string, ilo : c_int, ihi : c_int, a : [] c_float, b : [] c_float, q : [] c_float, z : [] c_float): c_int{
  return LAPACKE_sgghrd(matrix_order, ascii(compq) : c_char, ascii(compz) : c_char, (a.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgghrd for the type c_double.
 */
inline proc gghrd(matrix_order : lapack_memory_order, compq : string, compz : string, ilo : c_int, ihi : c_int, a : [] c_double, b : [] c_double, q : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dgghrd(matrix_order, ascii(compq) : c_char, ascii(compz) : c_char, (a.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgghrd for the type lapack_complex_float.
 */
inline proc gghrd(matrix_order : lapack_memory_order, compq : string, compz : string, ilo : c_int, ihi : c_int, a : [] complex(64), b : [] complex(64), q : [] complex(64), z : [] complex(64)): c_int{
  return LAPACKE_cgghrd(matrix_order, ascii(compq) : c_char, ascii(compz) : c_char, (a.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgghrd for the type lapack_complex_double.
 */
inline proc gghrd(matrix_order : lapack_memory_order, compq : string, compz : string, ilo : c_int, ihi : c_int, a : [] complex(128), b : [] complex(128), q : [] complex(128), z : [] complex(128)): c_int{
  return LAPACKE_zgghrd(matrix_order, ascii(compq) : c_char, ascii(compz) : c_char, (a.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgglse for the type c_float.
 */
inline proc gglse(matrix_order : lapack_memory_order, a : [] c_float, b : [] c_float, c : [] c_float, d : [] c_float, x : [] c_float): c_int{
  return LAPACKE_sgglse(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, d, x);
}

/* 
Wrapped procedure of LAPACKE_dgglse for the type c_double.
 */
inline proc gglse(matrix_order : lapack_memory_order, a : [] c_double, b : [] c_double, c : [] c_double, d : [] c_double, x : [] c_double): c_int{
  return LAPACKE_dgglse(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, d, x);
}

/* 
Wrapped procedure of LAPACKE_cgglse for the type lapack_complex_float.
 */
inline proc gglse(matrix_order : lapack_memory_order, a : [] complex(64), b : [] complex(64), c : [] complex(64), d : [] complex(64), x : [] complex(64)): c_int{
  return LAPACKE_cgglse(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, d, x);
}

/* 
Wrapped procedure of LAPACKE_zgglse for the type lapack_complex_double.
 */
inline proc gglse(matrix_order : lapack_memory_order, a : [] complex(128), b : [] complex(128), c : [] complex(128), d : [] complex(128), x : [] complex(128)): c_int{
  return LAPACKE_zgglse(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, d, x);
}

/* 
Wrapped procedure of LAPACKE_sggqrf for the type c_float.
 */
inline proc ggqrf(matrix_order : lapack_memory_order, a : [] c_float, taua : [] c_float, b : [] c_float, taub : [] c_float): c_int{
  return LAPACKE_sggqrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, taua, b, (b.domain.dim(2).size) : c_int, taub);
}

/* 
Wrapped procedure of LAPACKE_dggqrf for the type c_double.
 */
inline proc ggqrf(matrix_order : lapack_memory_order, a : [] c_double, taua : [] c_double, b : [] c_double, taub : [] c_double): c_int{
  return LAPACKE_dggqrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, taua, b, (b.domain.dim(2).size) : c_int, taub);
}

/* 
Wrapped procedure of LAPACKE_cggqrf for the type lapack_complex_float.
 */
inline proc ggqrf(matrix_order : lapack_memory_order, a : [] complex(64), taua : [] complex(64), b : [] complex(64), taub : [] complex(64)): c_int{
  return LAPACKE_cggqrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, taua, b, (b.domain.dim(2).size) : c_int, taub);
}

/* 
Wrapped procedure of LAPACKE_zggqrf for the type lapack_complex_double.
 */
inline proc ggqrf(matrix_order : lapack_memory_order, a : [] complex(128), taua : [] complex(128), b : [] complex(128), taub : [] complex(128)): c_int{
  return LAPACKE_zggqrf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, taua, b, (b.domain.dim(2).size) : c_int, taub);
}

/* 
Wrapped procedure of LAPACKE_sggrqf for the type c_float.
 */
inline proc ggrqf(matrix_order : lapack_memory_order, a : [] c_float, taua : [] c_float, b : [] c_float, taub : [] c_float): c_int{
  return LAPACKE_sggrqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, taua, b, (b.domain.dim(2).size) : c_int, taub);
}

/* 
Wrapped procedure of LAPACKE_dggrqf for the type c_double.
 */
inline proc ggrqf(matrix_order : lapack_memory_order, a : [] c_double, taua : [] c_double, b : [] c_double, taub : [] c_double): c_int{
  return LAPACKE_dggrqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, taua, b, (b.domain.dim(2).size) : c_int, taub);
}

/* 
Wrapped procedure of LAPACKE_cggrqf for the type lapack_complex_float.
 */
inline proc ggrqf(matrix_order : lapack_memory_order, a : [] complex(64), taua : [] complex(64), b : [] complex(64), taub : [] complex(64)): c_int{
  return LAPACKE_cggrqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, taua, b, (b.domain.dim(2).size) : c_int, taub);
}

/* 
Wrapped procedure of LAPACKE_zggrqf for the type lapack_complex_double.
 */
inline proc ggrqf(matrix_order : lapack_memory_order, a : [] complex(128), taua : [] complex(128), b : [] complex(128), taub : [] complex(128)): c_int{
  return LAPACKE_zggrqf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, taua, b, (b.domain.dim(2).size) : c_int, taub);
}

/* 
Wrapped procedure of LAPACKE_sggsvd for the type c_float.
 */
inline proc ggsvd(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, ref k : c_int, ref l : c_int, a : [] c_float, b : [] c_float, alpha : [] c_float, beta : [] c_float, u : [] c_float, v : [] c_float, q : [] c_float, iwork : [] c_int): c_int{
  return LAPACKE_sggsvd(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alpha, beta, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, iwork);
}

/* 
Wrapped procedure of LAPACKE_dggsvd for the type c_double.
 */
inline proc ggsvd(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, ref k : c_int, ref l : c_int, a : [] c_double, b : [] c_double, alpha : [] c_double, beta : [] c_double, u : [] c_double, v : [] c_double, q : [] c_double, iwork : [] c_int): c_int{
  return LAPACKE_dggsvd(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alpha, beta, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, iwork);
}

/* 
Wrapped procedure of LAPACKE_cggsvd for the type lapack_complex_float.
 */
inline proc ggsvd(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, ref k : c_int, ref l : c_int, a : [] complex(64), b : [] complex(64), alpha : [] c_float, beta : [] c_float, u : [] complex(64), v : [] complex(64), q : [] complex(64), iwork : [] c_int): c_int{
  return LAPACKE_cggsvd(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alpha, beta, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, iwork);
}

/* 
Wrapped procedure of LAPACKE_zggsvd for the type lapack_complex_double.
 */
inline proc ggsvd(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, ref k : c_int, ref l : c_int, a : [] complex(128), b : [] complex(128), alpha : [] c_double, beta : [] c_double, u : [] complex(128), v : [] complex(128), q : [] complex(128), iwork : [] c_int): c_int{
  return LAPACKE_zggsvd(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alpha, beta, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, iwork);
}

/* 
Wrapped procedure of LAPACKE_sggsvp for the type c_float.
 */
inline proc ggsvp(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, a : [] c_float, b : [] c_float, tola : c_float, tolb : c_float, ref k : c_int, ref l : c_int, u : [] c_float, v : [] c_float, q : [] c_float): c_int{
  return LAPACKE_sggsvp(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, tola, tolb, k, l, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dggsvp for the type c_double.
 */
inline proc ggsvp(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, a : [] c_double, b : [] c_double, tola : c_double, tolb : c_double, ref k : c_int, ref l : c_int, u : [] c_double, v : [] c_double, q : [] c_double): c_int{
  return LAPACKE_dggsvp(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, tola, tolb, k, l, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cggsvp for the type lapack_complex_float.
 */
inline proc ggsvp(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, a : [] complex(64), b : [] complex(64), tola : c_float, tolb : c_float, ref k : c_int, ref l : c_int, u : [] complex(64), v : [] complex(64), q : [] complex(64)): c_int{
  return LAPACKE_cggsvp(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, tola, tolb, k, l, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zggsvp for the type lapack_complex_double.
 */
inline proc ggsvp(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, a : [] complex(128), b : [] complex(128), tola : c_double, tolb : c_double, ref k : c_int, ref l : c_int, u : [] complex(128), v : [] complex(128), q : [] complex(128)): c_int{
  return LAPACKE_zggsvp(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, tola, tolb, k, l, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgtcon for the type c_float.
 */
inline proc gtcon(norm : string, n : c_int, dl : [] c_float, d : [] c_float, du : [] c_float, du2 : [] c_float, ipiv : [] c_int, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_sgtcon(ascii(norm) : c_char, n, dl, d, du, du2, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_dgtcon for the type c_double.
 */
inline proc gtcon(norm : string, n : c_int, dl : [] c_double, d : [] c_double, du : [] c_double, du2 : [] c_double, ipiv : [] c_int, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_dgtcon(ascii(norm) : c_char, n, dl, d, du, du2, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_cgtcon for the type lapack_complex_float.
 */
inline proc gtcon(norm : string, n : c_int, dl : [] complex(64), d : [] complex(64), du : [] complex(64), du2 : [] complex(64), ipiv : [] c_int, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_cgtcon(ascii(norm) : c_char, n, dl, d, du, du2, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_zgtcon for the type lapack_complex_double.
 */
inline proc gtcon(norm : string, n : c_int, dl : [] complex(128), d : [] complex(128), du : [] complex(128), du2 : [] complex(128), ipiv : [] c_int, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_zgtcon(ascii(norm) : c_char, n, dl, d, du, du2, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_sgtrfs for the type c_float.
 */
inline proc gtrfs(matrix_order : lapack_memory_order, trans : string, n : c_int, dl : [] c_float, d : [] c_float, du : [] c_float, dlf : [] c_float, df : [] c_float, duf : [] c_float, du2 : [] c_float, ipiv : [] c_int, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_sgtrfs(matrix_order, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, dlf, df, duf, du2, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dgtrfs for the type c_double.
 */
inline proc gtrfs(matrix_order : lapack_memory_order, trans : string, n : c_int, dl : [] c_double, d : [] c_double, du : [] c_double, dlf : [] c_double, df : [] c_double, duf : [] c_double, du2 : [] c_double, ipiv : [] c_int, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dgtrfs(matrix_order, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, dlf, df, duf, du2, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cgtrfs for the type lapack_complex_float.
 */
inline proc gtrfs(matrix_order : lapack_memory_order, trans : string, n : c_int, dl : [] complex(64), d : [] complex(64), du : [] complex(64), dlf : [] complex(64), df : [] complex(64), duf : [] complex(64), du2 : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cgtrfs(matrix_order, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, dlf, df, duf, du2, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zgtrfs for the type lapack_complex_double.
 */
inline proc gtrfs(matrix_order : lapack_memory_order, trans : string, n : c_int, dl : [] complex(128), d : [] complex(128), du : [] complex(128), dlf : [] complex(128), df : [] complex(128), duf : [] complex(128), du2 : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zgtrfs(matrix_order, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, dlf, df, duf, du2, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_sgtsv for the type c_float.
 */
inline proc gtsv(matrix_order : lapack_memory_order, dl : [] c_float, d : [] c_float, du : [] c_float, b : [] c_float): c_int{
  return LAPACKE_sgtsv(matrix_order, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgtsv for the type c_double.
 */
inline proc gtsv(matrix_order : lapack_memory_order, dl : [] c_double, d : [] c_double, du : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dgtsv(matrix_order, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgtsv for the type lapack_complex_float.
 */
inline proc gtsv(matrix_order : lapack_memory_order, n : c_int, dl : [] complex(64), d : [] complex(64), du : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_cgtsv(matrix_order, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgtsv for the type lapack_complex_double.
 */
inline proc gtsv(matrix_order : lapack_memory_order, n : c_int, dl : [] complex(128), d : [] complex(128), du : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zgtsv(matrix_order, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgtsvx for the type c_float.
 */
inline proc gtsvx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, dl : [] c_float, d : [] c_float, du : [] c_float, dlf : [] c_float, df : [] c_float, duf : [] c_float, du2 : [] c_float, ipiv : [] c_int, b : [] c_float, x : [] c_float, ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_sgtsvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, dlf, df, duf, du2, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dgtsvx for the type c_double.
 */
inline proc gtsvx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, dl : [] c_double, d : [] c_double, du : [] c_double, dlf : [] c_double, df : [] c_double, duf : [] c_double, du2 : [] c_double, ipiv : [] c_int, b : [] c_double, x : [] c_double, ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dgtsvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, dlf, df, duf, du2, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cgtsvx for the type lapack_complex_float.
 */
inline proc gtsvx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, dl : [] complex(64), d : [] complex(64), du : [] complex(64), dlf : [] complex(64), df : [] complex(64), duf : [] complex(64), du2 : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cgtsvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, dlf, df, duf, du2, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zgtsvx for the type lapack_complex_double.
 */
inline proc gtsvx(matrix_order : lapack_memory_order, fact : string, trans : string, n : c_int, dl : [] complex(128), d : [] complex(128), du : [] complex(128), dlf : [] complex(128), df : [] complex(128), duf : [] complex(128), du2 : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zgtsvx(matrix_order, ascii(fact) : c_char, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, dlf, df, duf, du2, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_sgttrf for the type c_float.
 */
inline proc gttrf(n : c_int, dl : [] c_float, d : [] c_float, du : [] c_float, du2 : [] c_float, ipiv : [] c_int): c_int{
  return LAPACKE_sgttrf(n, dl, d, du, du2, ipiv);
}

/* 
Wrapped procedure of LAPACKE_dgttrf for the type c_double.
 */
inline proc gttrf(n : c_int, dl : [] c_double, d : [] c_double, du : [] c_double, du2 : [] c_double, ipiv : [] c_int): c_int{
  return LAPACKE_dgttrf(n, dl, d, du, du2, ipiv);
}

/* 
Wrapped procedure of LAPACKE_cgttrf for the type lapack_complex_float.
 */
inline proc gttrf(n : c_int, dl : [] complex(64), d : [] complex(64), du : [] complex(64), du2 : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_cgttrf(n, dl, d, du, du2, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zgttrf for the type lapack_complex_double.
 */
inline proc gttrf(n : c_int, dl : [] complex(128), d : [] complex(128), du : [] complex(128), du2 : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zgttrf(n, dl, d, du, du2, ipiv);
}

/* 
Wrapped procedure of LAPACKE_sgttrs for the type c_float.
 */
inline proc gttrs(matrix_order : lapack_memory_order, trans : string, n : c_int, dl : [] c_float, d : [] c_float, du : [] c_float, du2 : [] c_float, ipiv : [] c_int, b : [] c_float): c_int{
  return LAPACKE_sgttrs(matrix_order, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, du2, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgttrs for the type c_double.
 */
inline proc gttrs(matrix_order : lapack_memory_order, trans : string, n : c_int, dl : [] c_double, d : [] c_double, du : [] c_double, du2 : [] c_double, ipiv : [] c_int, b : [] c_double): c_int{
  return LAPACKE_dgttrs(matrix_order, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, du2, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgttrs for the type lapack_complex_float.
 */
inline proc gttrs(matrix_order : lapack_memory_order, trans : string, n : c_int, dl : [] complex(64), d : [] complex(64), du : [] complex(64), du2 : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_cgttrs(matrix_order, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, du2, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgttrs for the type lapack_complex_double.
 */
inline proc gttrs(matrix_order : lapack_memory_order, trans : string, n : c_int, dl : [] complex(128), d : [] complex(128), du : [] complex(128), du2 : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zgttrs(matrix_order, ascii(trans) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, dl, d, du, du2, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chbev for the type lapack_complex_float.
 */
inline proc hbev(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, kd : c_int, ab : [] complex(64), w : [] c_float, z : [] complex(64)): c_int{
  return LAPACKE_chbev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhbev for the type lapack_complex_double.
 */
inline proc hbev(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, kd : c_int, ab : [] complex(128), w : [] c_double, z : [] complex(128)): c_int{
  return LAPACKE_zhbev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chbevd for the type lapack_complex_float.
 */
inline proc hbevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, kd : c_int, ab : [] complex(64), w : [] c_float, z : [] complex(64)): c_int{
  return LAPACKE_chbevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhbevd for the type lapack_complex_double.
 */
inline proc hbevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, kd : c_int, ab : [] complex(128), w : [] c_double, z : [] complex(128)): c_int{
  return LAPACKE_zhbevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chbevx for the type lapack_complex_float.
 */
inline proc hbevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, n : c_int, kd : c_int, ab : [] complex(64), q : [] complex(64), vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] complex(64), ifail : [] c_int): c_int{
  return LAPACKE_chbevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_zhbevx for the type lapack_complex_double.
 */
inline proc hbevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, n : c_int, kd : c_int, ab : [] complex(128), q : [] complex(128), vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] complex(128), ifail : [] c_int): c_int{
  return LAPACKE_zhbevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_chbgst for the type lapack_complex_float.
 */
inline proc hbgst(matrix_order : lapack_memory_order, vect : string, uplo : string, ka : c_int, kb : c_int, ab : [] complex(64), bb : [] complex(64), x : [] complex(64)): c_int{
  return LAPACKE_chbgst(matrix_order, ascii(vect) : c_char, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(2).size else x.domain.dim(1).size) : c_int, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhbgst for the type lapack_complex_double.
 */
inline proc hbgst(matrix_order : lapack_memory_order, vect : string, uplo : string, ka : c_int, kb : c_int, ab : [] complex(128), bb : [] complex(128), x : [] complex(128)): c_int{
  return LAPACKE_zhbgst(matrix_order, ascii(vect) : c_char, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(2).size else x.domain.dim(1).size) : c_int, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chbgv for the type lapack_complex_float.
 */
inline proc hbgv(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ka : c_int, kb : c_int, ab : [] complex(64), bb : [] complex(64), w : [] c_float, z : [] complex(64)): c_int{
  return LAPACKE_chbgv(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhbgv for the type lapack_complex_double.
 */
inline proc hbgv(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ka : c_int, kb : c_int, ab : [] complex(128), bb : [] complex(128), w : [] c_double, z : [] complex(128)): c_int{
  return LAPACKE_zhbgv(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chbgvd for the type lapack_complex_float.
 */
inline proc hbgvd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ka : c_int, kb : c_int, ab : [] complex(64), bb : [] complex(64), w : [] c_float, z : [] complex(64)): c_int{
  return LAPACKE_chbgvd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhbgvd for the type lapack_complex_double.
 */
inline proc hbgvd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ka : c_int, kb : c_int, ab : [] complex(128), bb : [] complex(128), w : [] c_double, z : [] complex(128)): c_int{
  return LAPACKE_zhbgvd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chbgvx for the type lapack_complex_float.
 */
inline proc hbgvx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, ka : c_int, kb : c_int, ab : [] complex(64), bb : [] complex(64), q : [] complex(64), vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] complex(64), ifail : [] c_int): c_int{
  return LAPACKE_chbgvx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then q.domain.dim(2).size else q.domain.dim(1).size) : c_int, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_zhbgvx for the type lapack_complex_double.
 */
inline proc hbgvx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, ka : c_int, kb : c_int, ab : [] complex(128), bb : [] complex(128), q : [] complex(128), vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] complex(128), ifail : [] c_int): c_int{
  return LAPACKE_zhbgvx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then q.domain.dim(2).size else q.domain.dim(1).size) : c_int, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_chbtrd for the type lapack_complex_float.
 */
inline proc hbtrd(matrix_order : lapack_memory_order, vect : string, uplo : string, n : c_int, kd : c_int, ab : [] complex(64), d : [] c_float, e : [] c_float, q : [] complex(64)): c_int{
  return LAPACKE_chbtrd(matrix_order, ascii(vect) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, d, e, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhbtrd for the type lapack_complex_double.
 */
inline proc hbtrd(matrix_order : lapack_memory_order, vect : string, uplo : string, n : c_int, kd : c_int, ab : [] complex(128), d : [] c_double, e : [] c_double, q : [] complex(128)): c_int{
  return LAPACKE_zhbtrd(matrix_order, ascii(vect) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, d, e, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_checon for the type lapack_complex_float.
 */
inline proc hecon(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_checon(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_zhecon for the type lapack_complex_double.
 */
inline proc hecon(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_zhecon(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_cheequb for the type lapack_complex_float.
 */
inline proc heequb(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), s : [] c_float, ref scond : c_float, ref amax : c_float): c_int{
  return LAPACKE_cheequb(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_zheequb for the type lapack_complex_double.
 */
inline proc heequb(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), s : [] c_double, ref scond : c_double, ref amax : c_double): c_int{
  return LAPACKE_zheequb(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_cheev for the type lapack_complex_float.
 */
inline proc heev(matrix_order : lapack_memory_order, jobz : string, uplo : string, a : [] complex(64), w : [] c_float): c_int{
  return LAPACKE_cheev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_zheev for the type lapack_complex_double.
 */
inline proc heev(matrix_order : lapack_memory_order, jobz : string, uplo : string, a : [] complex(128), w : [] c_double): c_int{
  return LAPACKE_zheev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_cheevd for the type lapack_complex_float.
 */
inline proc heevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, a : [] complex(64), w : [] c_float): c_int{
  return LAPACKE_cheevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_zheevd for the type lapack_complex_double.
 */
inline proc heevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, a : [] complex(128), w : [] c_double): c_int{
  return LAPACKE_zheevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_cheevr for the type lapack_complex_float.
 */
inline proc heevr(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, a : [] complex(64), vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] complex(64), isuppz : [] c_int): c_int{
  return LAPACKE_cheevr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, isuppz);
}

/* 
Wrapped procedure of LAPACKE_zheevr for the type lapack_complex_double.
 */
inline proc heevr(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, a : [] complex(128), vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] complex(128), isuppz : [] c_int): c_int{
  return LAPACKE_zheevr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, isuppz);
}

/* 
Wrapped procedure of LAPACKE_cheevx for the type lapack_complex_float.
 */
inline proc heevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, a : [] complex(64), vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] complex(64), ifail : [] c_int): c_int{
  return LAPACKE_cheevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_zheevx for the type lapack_complex_double.
 */
inline proc heevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, a : [] complex(128), vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] complex(128), ifail : [] c_int): c_int{
  return LAPACKE_zheevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_chegst for the type lapack_complex_float.
 */
inline proc hegst(matrix_order : lapack_memory_order, itype : c_int, uplo : string, a : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_chegst(matrix_order, itype, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhegst for the type lapack_complex_double.
 */
inline proc hegst(matrix_order : lapack_memory_order, itype : c_int, uplo : string, a : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zhegst(matrix_order, itype, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chegv for the type lapack_complex_float.
 */
inline proc hegv(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, a : [] complex(64), b : [] complex(64), w : [] c_float): c_int{
  return LAPACKE_chegv(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_zhegv for the type lapack_complex_double.
 */
inline proc hegv(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, a : [] complex(128), b : [] complex(128), w : [] c_double): c_int{
  return LAPACKE_zhegv(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_chegvd for the type lapack_complex_float.
 */
inline proc hegvd(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, a : [] complex(64), b : [] complex(64), w : [] c_float): c_int{
  return LAPACKE_chegvd(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_zhegvd for the type lapack_complex_double.
 */
inline proc hegvd(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, a : [] complex(128), b : [] complex(128), w : [] c_double): c_int{
  return LAPACKE_zhegvd(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_chegvx for the type lapack_complex_float.
 */
inline proc hegvx(matrix_order : lapack_memory_order, itype : c_int, jobz : string, range : string, uplo : string, a : [] complex(64), b : [] complex(64), vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] complex(64), ifail : [] c_int): c_int{
  return LAPACKE_chegvx(matrix_order, itype, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_zhegvx for the type lapack_complex_double.
 */
inline proc hegvx(matrix_order : lapack_memory_order, itype : c_int, jobz : string, range : string, uplo : string, a : [] complex(128), b : [] complex(128), vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] complex(128), ifail : [] c_int): c_int{
  return LAPACKE_zhegvx(matrix_order, itype, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_cherfs for the type lapack_complex_float.
 */
inline proc herfs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cherfs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zherfs for the type lapack_complex_double.
 */
inline proc herfs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zherfs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cherfsx for the type lapack_complex_float.
 */
inline proc herfsx(matrix_order : lapack_memory_order, uplo : string, equed : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, s : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_cherfsx(matrix_order, ascii(uplo) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_zherfsx for the type lapack_complex_double.
 */
inline proc herfsx(matrix_order : lapack_memory_order, uplo : string, equed : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, s : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_zherfsx(matrix_order, ascii(uplo) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_chesv for the type lapack_complex_float.
 */
inline proc hesv(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_chesv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhesv for the type lapack_complex_double.
 */
inline proc hesv(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zhesv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chesvx for the type lapack_complex_float.
 */
inline proc hesvx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_chesvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zhesvx for the type lapack_complex_double.
 */
inline proc hesvx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zhesvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_chesvxx for the type lapack_complex_float.
 */
inline proc hesvxx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, ref equed : string, s : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ref rpvgrw : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_chesvxx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_zhesvxx for the type lapack_complex_double.
 */
inline proc hesvxx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, ref equed : string, s : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ref rpvgrw : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_zhesvxx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_chetrd for the type lapack_complex_float.
 */
inline proc hetrd(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), d : [] c_float, e : [] c_float, tau : [] complex(64)): c_int{
  return LAPACKE_chetrd(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, d, e, tau);
}

/* 
Wrapped procedure of LAPACKE_zhetrd for the type lapack_complex_double.
 */
inline proc hetrd(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), d : [] c_double, e : [] c_double, tau : [] complex(128)): c_int{
  return LAPACKE_zhetrd(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, d, e, tau);
}

/* 
Wrapped procedure of LAPACKE_chetrf for the type lapack_complex_float.
 */
inline proc hetrf(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_chetrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zhetrf for the type lapack_complex_double.
 */
inline proc hetrf(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zhetrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_chetri for the type lapack_complex_float.
 */
inline proc hetri(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_chetri(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zhetri for the type lapack_complex_double.
 */
inline proc hetri(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zhetri(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_chetrs for the type lapack_complex_float.
 */
inline proc hetrs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_chetrs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhetrs for the type lapack_complex_double.
 */
inline proc hetrs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zhetrs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chfrk for the type lapack_complex_float.
 */
inline proc hfrk(matrix_order : lapack_memory_order, transr : string, uplo : string, trans : string, alpha : c_float, a : [] complex(64), beta : c_float, c : [] complex(64)): c_int{
  return LAPACKE_chfrk(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (c.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, alpha, a, (a.domain.dim(2).size) : c_int, beta, c);
}

/* 
Wrapped procedure of LAPACKE_zhfrk for the type lapack_complex_double.
 */
inline proc hfrk(matrix_order : lapack_memory_order, transr : string, uplo : string, trans : string, alpha : c_double, a : [] complex(128), beta : c_double, c : [] complex(128)): c_int{
  return LAPACKE_zhfrk(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (c.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, alpha, a, (a.domain.dim(2).size) : c_int, beta, c);
}

/* 
Wrapped procedure of LAPACKE_shgeqz for the type c_float.
 */
inline proc hgeqz(matrix_order : lapack_memory_order, job : string, compq : string, compz : string, ilo : c_int, ihi : c_int, h : [] c_float, t : [] c_float, alphar : [] c_float, alphai : [] c_float, beta : [] c_float, q : [] c_float, z : [] c_float): c_int{
  return LAPACKE_shgeqz(matrix_order, ascii(job) : c_char, ascii(compq) : c_char, ascii(compz) : c_char, (h.domain.dim(1).size) : c_int, ilo, ihi, h, (h.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, alphar, alphai, beta, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dhgeqz for the type c_double.
 */
inline proc hgeqz(matrix_order : lapack_memory_order, job : string, compq : string, compz : string, ilo : c_int, ihi : c_int, h : [] c_double, t : [] c_double, alphar : [] c_double, alphai : [] c_double, beta : [] c_double, q : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dhgeqz(matrix_order, ascii(job) : c_char, ascii(compq) : c_char, ascii(compz) : c_char, (h.domain.dim(1).size) : c_int, ilo, ihi, h, (h.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, alphar, alphai, beta, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chgeqz for the type lapack_complex_float.
 */
inline proc hgeqz(matrix_order : lapack_memory_order, job : string, compq : string, compz : string, ilo : c_int, ihi : c_int, h : [] complex(64), t : [] complex(64), alpha : [] complex(64), beta : [] complex(64), q : [] complex(64), z : [] complex(64)): c_int{
  return LAPACKE_chgeqz(matrix_order, ascii(job) : c_char, ascii(compq) : c_char, ascii(compz) : c_char, (h.domain.dim(1).size) : c_int, ilo, ihi, h, (h.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, alpha, beta, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhgeqz for the type lapack_complex_double.
 */
inline proc hgeqz(matrix_order : lapack_memory_order, job : string, compq : string, compz : string, ilo : c_int, ihi : c_int, h : [] complex(128), t : [] complex(128), alpha : [] complex(128), beta : [] complex(128), q : [] complex(128), z : [] complex(128)): c_int{
  return LAPACKE_zhgeqz(matrix_order, ascii(job) : c_char, ascii(compq) : c_char, ascii(compz) : c_char, (h.domain.dim(1).size) : c_int, ilo, ihi, h, (h.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, alpha, beta, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chpcon for the type lapack_complex_float.
 */
inline proc hpcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), ipiv : [] c_int, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_chpcon(matrix_order, ascii(uplo) : c_char, n, ap, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_zhpcon for the type lapack_complex_double.
 */
inline proc hpcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), ipiv : [] c_int, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_zhpcon(matrix_order, ascii(uplo) : c_char, n, ap, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_chpev for the type lapack_complex_float.
 */
inline proc hpev(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ap : [] complex(64), w : [] c_float, z : [] complex(64)): c_int{
  return LAPACKE_chpev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhpev for the type lapack_complex_double.
 */
inline proc hpev(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ap : [] complex(128), w : [] c_double, z : [] complex(128)): c_int{
  return LAPACKE_zhpev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chpevd for the type lapack_complex_float.
 */
inline proc hpevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ap : [] complex(64), w : [] c_float, z : [] complex(64)): c_int{
  return LAPACKE_chpevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhpevd for the type lapack_complex_double.
 */
inline proc hpevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ap : [] complex(128), w : [] c_double, z : [] complex(128)): c_int{
  return LAPACKE_zhpevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chpevx for the type lapack_complex_float.
 */
inline proc hpevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, n : c_int, ap : [] complex(64), vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] complex(64), ifail : [] c_int): c_int{
  return LAPACKE_chpevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, ap, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_zhpevx for the type lapack_complex_double.
 */
inline proc hpevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, n : c_int, ap : [] complex(128), vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] complex(128), ifail : [] c_int): c_int{
  return LAPACKE_zhpevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, ap, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_chpgst for the type lapack_complex_float.
 */
inline proc hpgst(matrix_order : lapack_memory_order, itype : c_int, uplo : string, n : c_int, ap : [] complex(64), bp : [] complex(64)): c_int{
  return LAPACKE_chpgst(matrix_order, itype, ascii(uplo) : c_char, n, ap, bp);
}

/* 
Wrapped procedure of LAPACKE_zhpgst for the type lapack_complex_double.
 */
inline proc hpgst(matrix_order : lapack_memory_order, itype : c_int, uplo : string, n : c_int, ap : [] complex(128), bp : [] complex(128)): c_int{
  return LAPACKE_zhpgst(matrix_order, itype, ascii(uplo) : c_char, n, ap, bp);
}

/* 
Wrapped procedure of LAPACKE_chpgv for the type lapack_complex_float.
 */
inline proc hpgv(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, n : c_int, ap : [] complex(64), bp : [] complex(64), w : [] c_float, z : [] complex(64)): c_int{
  return LAPACKE_chpgv(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, bp, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhpgv for the type lapack_complex_double.
 */
inline proc hpgv(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, n : c_int, ap : [] complex(128), bp : [] complex(128), w : [] c_double, z : [] complex(128)): c_int{
  return LAPACKE_zhpgv(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, bp, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chpgvd for the type lapack_complex_float.
 */
inline proc hpgvd(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, n : c_int, ap : [] complex(64), bp : [] complex(64), w : [] c_float, z : [] complex(64)): c_int{
  return LAPACKE_chpgvd(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, bp, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhpgvd for the type lapack_complex_double.
 */
inline proc hpgvd(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, n : c_int, ap : [] complex(128), bp : [] complex(128), w : [] c_double, z : [] complex(128)): c_int{
  return LAPACKE_zhpgvd(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, bp, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chpgvx for the type lapack_complex_float.
 */
inline proc hpgvx(matrix_order : lapack_memory_order, itype : c_int, jobz : string, range : string, uplo : string, n : c_int, ap : [] complex(64), bp : [] complex(64), vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] complex(64), ifail : [] c_int): c_int{
  return LAPACKE_chpgvx(matrix_order, itype, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, ap, bp, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_zhpgvx for the type lapack_complex_double.
 */
inline proc hpgvx(matrix_order : lapack_memory_order, itype : c_int, jobz : string, range : string, uplo : string, n : c_int, ap : [] complex(128), bp : [] complex(128), vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] complex(128), ifail : [] c_int): c_int{
  return LAPACKE_zhpgvx(matrix_order, itype, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, ap, bp, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_chprfs for the type lapack_complex_float.
 */
inline proc hprfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), afp : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_chprfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zhprfs for the type lapack_complex_double.
 */
inline proc hprfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), afp : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zhprfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_chpsv for the type lapack_complex_float.
 */
inline proc hpsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_chpsv(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhpsv for the type lapack_complex_double.
 */
inline proc hpsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zhpsv(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chpsvx for the type lapack_complex_float.
 */
inline proc hpsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, ap : [] complex(64), afp : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_chpsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zhpsvx for the type lapack_complex_double.
 */
inline proc hpsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, ap : [] complex(128), afp : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zhpsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_chptrd for the type lapack_complex_float.
 */
inline proc hptrd(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), d : [] c_float, e : [] c_float, tau : [] complex(64)): c_int{
  return LAPACKE_chptrd(matrix_order, ascii(uplo) : c_char, n, ap, d, e, tau);
}

/* 
Wrapped procedure of LAPACKE_zhptrd for the type lapack_complex_double.
 */
inline proc hptrd(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), d : [] c_double, e : [] c_double, tau : [] complex(128)): c_int{
  return LAPACKE_zhptrd(matrix_order, ascii(uplo) : c_char, n, ap, d, e, tau);
}

/* 
Wrapped procedure of LAPACKE_chptrf for the type lapack_complex_float.
 */
inline proc hptrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_chptrf(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zhptrf for the type lapack_complex_double.
 */
inline proc hptrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zhptrf(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_chptri for the type lapack_complex_float.
 */
inline proc hptri(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_chptri(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zhptri for the type lapack_complex_double.
 */
inline proc hptri(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zhptri(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_chptrs for the type lapack_complex_float.
 */
inline proc hptrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_chptrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhptrs for the type lapack_complex_double.
 */
inline proc hptrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zhptrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_shsein for the type c_float.
 */
inline proc hsein(matrix_order : lapack_memory_order, job : string, eigsrc : string, initv : string, chlapack_select : [] c_int, h : [] c_float, wr : [] c_float, wi : [] c_float, vl : [] c_float, vr : [] c_float, mm : c_int, ref m : c_int, ifaill : [] c_int, ifailr : [] c_int): c_int{
  return LAPACKE_shsein(matrix_order, ascii(job) : c_char, ascii(eigsrc) : c_char, ascii(initv) : c_char, chlapack_select, (h.domain.dim(1).size) : c_int, h, (h.domain.dim(2).size) : c_int, wr, wi, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m, ifaill, ifailr);
}

/* 
Wrapped procedure of LAPACKE_dhsein for the type c_double.
 */
inline proc hsein(matrix_order : lapack_memory_order, job : string, eigsrc : string, initv : string, chlapack_select : [] c_int, h : [] c_double, wr : [] c_double, wi : [] c_double, vl : [] c_double, vr : [] c_double, mm : c_int, ref m : c_int, ifaill : [] c_int, ifailr : [] c_int): c_int{
  return LAPACKE_dhsein(matrix_order, ascii(job) : c_char, ascii(eigsrc) : c_char, ascii(initv) : c_char, chlapack_select, (h.domain.dim(1).size) : c_int, h, (h.domain.dim(2).size) : c_int, wr, wi, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m, ifaill, ifailr);
}

/* 
Wrapped procedure of LAPACKE_chsein for the type lapack_complex_float.
 */
inline proc hsein(matrix_order : lapack_memory_order, job : string, eigsrc : string, initv : string, chlapack_select : [] c_int, h : [] complex(64), w : [] complex(64), vl : [] complex(64), vr : [] complex(64), mm : c_int, ref m : c_int, ifaill : [] c_int, ifailr : [] c_int): c_int{
  return LAPACKE_chsein(matrix_order, ascii(job) : c_char, ascii(eigsrc) : c_char, ascii(initv) : c_char, chlapack_select, (h.domain.dim(1).size) : c_int, h, (h.domain.dim(2).size) : c_int, w, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m, ifaill, ifailr);
}

/* 
Wrapped procedure of LAPACKE_zhsein for the type lapack_complex_double.
 */
inline proc hsein(matrix_order : lapack_memory_order, job : string, eigsrc : string, initv : string, chlapack_select : [] c_int, h : [] complex(128), w : [] complex(128), vl : [] complex(128), vr : [] complex(128), mm : c_int, ref m : c_int, ifaill : [] c_int, ifailr : [] c_int): c_int{
  return LAPACKE_zhsein(matrix_order, ascii(job) : c_char, ascii(eigsrc) : c_char, ascii(initv) : c_char, chlapack_select, (h.domain.dim(1).size) : c_int, h, (h.domain.dim(2).size) : c_int, w, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m, ifaill, ifailr);
}

/* 
Wrapped procedure of LAPACKE_shseqr for the type c_float.
 */
inline proc hseqr(matrix_order : lapack_memory_order, job : string, compz : string, ilo : c_int, ihi : c_int, h : [] c_float, wr : [] c_float, wi : [] c_float, z : [] c_float): c_int{
  return LAPACKE_shseqr(matrix_order, ascii(job) : c_char, ascii(compz) : c_char, (h.domain.dim(1).size) : c_int, ilo, ihi, h, (h.domain.dim(2).size) : c_int, wr, wi, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dhseqr for the type c_double.
 */
inline proc hseqr(matrix_order : lapack_memory_order, job : string, compz : string, ilo : c_int, ihi : c_int, h : [] c_double, wr : [] c_double, wi : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dhseqr(matrix_order, ascii(job) : c_char, ascii(compz) : c_char, (h.domain.dim(1).size) : c_int, ilo, ihi, h, (h.domain.dim(2).size) : c_int, wr, wi, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_chseqr for the type lapack_complex_float.
 */
inline proc hseqr(matrix_order : lapack_memory_order, job : string, compz : string, ilo : c_int, ihi : c_int, h : [] complex(64), w : [] complex(64), z : [] complex(64)): c_int{
  return LAPACKE_chseqr(matrix_order, ascii(job) : c_char, ascii(compz) : c_char, (h.domain.dim(1).size) : c_int, ilo, ihi, h, (h.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zhseqr for the type lapack_complex_double.
 */
inline proc hseqr(matrix_order : lapack_memory_order, job : string, compz : string, ilo : c_int, ihi : c_int, h : [] complex(128), w : [] complex(128), z : [] complex(128)): c_int{
  return LAPACKE_zhseqr(matrix_order, ascii(job) : c_char, ascii(compz) : c_char, (h.domain.dim(1).size) : c_int, ilo, ihi, h, (h.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clacgv for the type lapack_complex_float.
 */
inline proc lacgv(n : c_int, x : [] complex(64), incx : c_int): c_int{
  return LAPACKE_clacgv(n, x, incx);
}

/* 
Wrapped procedure of LAPACKE_zlacgv for the type lapack_complex_double.
 */
inline proc lacgv(n : c_int, x : [] complex(128), incx : c_int): c_int{
  return LAPACKE_zlacgv(n, x, incx);
}

/* 
Wrapped procedure of LAPACKE_slacn2 for the type c_float.
 */
inline proc lacn2(n : c_int, v : [] c_float, x : [] c_float, isgn : [] c_int, ref est : c_float, ref kase : c_int, isave : [] c_int): c_int{
  return LAPACKE_slacn2(n, v, x, isgn, est, kase, isave);
}

/* 
Wrapped procedure of LAPACKE_dlacn2 for the type c_double.
 */
inline proc lacn2(n : c_int, v : [] c_double, x : [] c_double, isgn : [] c_int, ref est : c_double, ref kase : c_int, isave : [] c_int): c_int{
  return LAPACKE_dlacn2(n, v, x, isgn, est, kase, isave);
}

/* 
Wrapped procedure of LAPACKE_clacn2 for the type lapack_complex_float.
 */
inline proc lacn2(n : c_int, v : [] complex(64), x : [] complex(64), ref est : c_float, ref kase : c_int, isave : [] c_int): c_int{
  return LAPACKE_clacn2(n, v, x, est, kase, isave);
}

/* 
Wrapped procedure of LAPACKE_zlacn2 for the type lapack_complex_double.
 */
inline proc lacn2(n : c_int, v : [] complex(128), x : [] complex(128), ref est : c_double, ref kase : c_int, isave : [] c_int): c_int{
  return LAPACKE_zlacn2(n, v, x, est, kase, isave);
}

/* 
Wrapped procedure of LAPACKE_slacpy for the type c_float.
 */
inline proc lacpy(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, b : [] c_float): c_int{
  return LAPACKE_slacpy(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dlacpy for the type c_double.
 */
inline proc lacpy(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dlacpy(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clacpy for the type lapack_complex_float.
 */
inline proc lacpy(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_clacpy(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlacpy for the type lapack_complex_double.
 */
inline proc lacpy(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zlacpy(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clacp2 for the type lapack_complex_float.
 */
inline proc lacp2(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, b : [] complex(64)): c_int{
  return LAPACKE_clacp2(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlacp2 for the type lapack_complex_double.
 */
inline proc lacp2(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, b : [] complex(128)): c_int{
  return LAPACKE_zlacp2(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlag2c for the type lapack_complex_double.
 */
inline proc lag2c(matrix_order : lapack_memory_order, a : [] complex(128), sa : [] complex(64)): c_int{
  return LAPACKE_zlag2c(matrix_order, (if matrix_order == lapack_memory_order.row_major then sa.domain.dim(1).size else sa.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sa, (sa.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_slag2d for the type c_float.
 */
inline proc lag2d(matrix_order : lapack_memory_order, sa : [] c_float, a : [] c_double): c_int{
  return LAPACKE_slag2d(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, sa, (sa.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dlag2s for the type c_double.
 */
inline proc lag2s(matrix_order : lapack_memory_order, a : [] c_double, sa : [] c_float): c_int{
  return LAPACKE_dlag2s(matrix_order, (if matrix_order == lapack_memory_order.row_major then sa.domain.dim(1).size else sa.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, sa, (sa.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clag2z for the type lapack_complex_float.
 */
inline proc lag2z(matrix_order : lapack_memory_order, sa : [] complex(64), a : [] complex(128)): c_int{
  return LAPACKE_clag2z(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, sa, (sa.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_slagge for the type c_float.
 */
inline proc lagge(matrix_order : lapack_memory_order, kl : c_int, ku : c_int, d : [] c_float, a : [] c_float, iseed : [] c_int): c_int{
  return LAPACKE_slagge(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, kl, ku, d, a, (a.domain.dim(2).size) : c_int, iseed);
}

/* 
Wrapped procedure of LAPACKE_dlagge for the type c_double.
 */
inline proc lagge(matrix_order : lapack_memory_order, kl : c_int, ku : c_int, d : [] c_double, a : [] c_double, iseed : [] c_int): c_int{
  return LAPACKE_dlagge(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, kl, ku, d, a, (a.domain.dim(2).size) : c_int, iseed);
}

/* 
Wrapped procedure of LAPACKE_clagge for the type lapack_complex_float.
 */
inline proc lagge(matrix_order : lapack_memory_order, kl : c_int, ku : c_int, d : [] c_float, a : [] complex(64), iseed : [] c_int): c_int{
  return LAPACKE_clagge(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, kl, ku, d, a, (a.domain.dim(2).size) : c_int, iseed);
}

/* 
Wrapped procedure of LAPACKE_zlagge for the type lapack_complex_double.
 */
inline proc lagge(matrix_order : lapack_memory_order, kl : c_int, ku : c_int, d : [] c_double, a : [] complex(128), iseed : [] c_int): c_int{
  return LAPACKE_zlagge(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, kl, ku, d, a, (a.domain.dim(2).size) : c_int, iseed);
}

/* 
Wrapped procedure of LAPACKE_slamch for the type c_float.
 */
inline proc lamch(cmach : string): c_float{
  return LAPACKE_slamch(ascii(cmach) : c_char);
}

/* 
Wrapped procedure of LAPACKE_dlamch for the type c_double.
 */
inline proc lamch(cmach : string): c_double{
  return LAPACKE_dlamch(ascii(cmach) : c_char);
}

/* 
Wrapped procedure of LAPACKE_slange for the type c_float.
 */
inline proc lange(matrix_order : lapack_memory_order, norm : string, a : [] c_float): c_float{
  return LAPACKE_slange(matrix_order, ascii(norm) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dlange for the type c_double.
 */
inline proc lange(matrix_order : lapack_memory_order, norm : string, a : [] c_double): c_double{
  return LAPACKE_dlange(matrix_order, ascii(norm) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clange for the type lapack_complex_float.
 */
inline proc lange(matrix_order : lapack_memory_order, norm : string, a : [] complex(64)): c_float{
  return LAPACKE_clange(matrix_order, ascii(norm) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlange for the type lapack_complex_double.
 */
inline proc lange(matrix_order : lapack_memory_order, norm : string, a : [] complex(128)): c_double{
  return LAPACKE_zlange(matrix_order, ascii(norm) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clanhe for the type lapack_complex_float.
 */
inline proc lanhe(matrix_order : lapack_memory_order, norm : string, uplo : string, a : [] complex(64)): c_float{
  return LAPACKE_clanhe(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlanhe for the type lapack_complex_double.
 */
inline proc lanhe(matrix_order : lapack_memory_order, norm : string, uplo : string, a : [] complex(128)): c_double{
  return LAPACKE_zlanhe(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_slansy for the type c_float.
 */
inline proc lansy(matrix_order : lapack_memory_order, norm : string, uplo : string, a : [] c_float): c_float{
  return LAPACKE_slansy(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dlansy for the type c_double.
 */
inline proc lansy(matrix_order : lapack_memory_order, norm : string, uplo : string, a : [] c_double): c_double{
  return LAPACKE_dlansy(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clansy for the type lapack_complex_float.
 */
inline proc lansy(matrix_order : lapack_memory_order, norm : string, uplo : string, a : [] complex(64)): c_float{
  return LAPACKE_clansy(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlansy for the type lapack_complex_double.
 */
inline proc lansy(matrix_order : lapack_memory_order, norm : string, uplo : string, a : [] complex(128)): c_double{
  return LAPACKE_zlansy(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_slantr for the type c_float.
 */
inline proc lantr(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, a : [] c_float): c_float{
  return LAPACKE_slantr(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dlantr for the type c_double.
 */
inline proc lantr(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, a : [] c_double): c_double{
  return LAPACKE_dlantr(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clantr for the type lapack_complex_float.
 */
inline proc lantr(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, a : [] complex(64)): c_float{
  return LAPACKE_clantr(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlantr for the type lapack_complex_double.
 */
inline proc lantr(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, a : [] complex(128)): c_double{
  return LAPACKE_zlantr(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_slarfb for the type c_float.
 */
inline proc larfb(matrix_order : lapack_memory_order, side : string, trans : string, direct : string, storev : string, v : [] c_float, t : [] c_float, c : [] c_float): c_int{
  return LAPACKE_slarfb(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, ascii(direct) : c_char, ascii(storev) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, (t.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dlarfb for the type c_double.
 */
inline proc larfb(matrix_order : lapack_memory_order, side : string, trans : string, direct : string, storev : string, v : [] c_double, t : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dlarfb(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, ascii(direct) : c_char, ascii(storev) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, (t.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clarfb for the type lapack_complex_float.
 */
inline proc larfb(matrix_order : lapack_memory_order, side : string, trans : string, direct : string, storev : string, v : [] complex(64), t : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_clarfb(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, ascii(direct) : c_char, ascii(storev) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, (t.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlarfb for the type lapack_complex_double.
 */
inline proc larfb(matrix_order : lapack_memory_order, side : string, trans : string, direct : string, storev : string, v : [] complex(128), t : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zlarfb(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, ascii(direct) : c_char, ascii(storev) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, (t.domain.dim(1).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_slarfg for the type c_float.
 */
inline proc larfg(n : c_int, ref alpha : c_float, x : [] c_float, incx : c_int, ref tau : c_float): c_int{
  return LAPACKE_slarfg(n, alpha, x, incx, tau);
}

/* 
Wrapped procedure of LAPACKE_dlarfg for the type c_double.
 */
inline proc larfg(n : c_int, ref alpha : c_double, x : [] c_double, incx : c_int, ref tau : c_double): c_int{
  return LAPACKE_dlarfg(n, alpha, x, incx, tau);
}

/* 
Wrapped procedure of LAPACKE_clarfg for the type lapack_complex_float.
 */
inline proc larfg(n : c_int, ref alpha : complex(64), x : [] complex(64), incx : c_int, ref tau : complex(64)): c_int{
  return LAPACKE_clarfg(n, alpha, x, incx, tau);
}

/* 
Wrapped procedure of LAPACKE_zlarfg for the type lapack_complex_double.
 */
inline proc larfg(n : c_int, ref alpha : complex(128), x : [] complex(128), incx : c_int, ref tau : complex(128)): c_int{
  return LAPACKE_zlarfg(n, alpha, x, incx, tau);
}

/* 
Wrapped procedure of LAPACKE_slarft for the type c_float.
 */
inline proc larft(matrix_order : lapack_memory_order, direct : string, storev : string, n : c_int, k : c_int, v : [] c_float, tau : [] c_float, t : [] c_float): c_int{
  return LAPACKE_slarft(matrix_order, ascii(direct) : c_char, ascii(storev) : c_char, n, k, v, (v.domain.dim(2).size) : c_int, tau, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dlarft for the type c_double.
 */
inline proc larft(matrix_order : lapack_memory_order, direct : string, storev : string, n : c_int, k : c_int, v : [] c_double, tau : [] c_double, t : [] c_double): c_int{
  return LAPACKE_dlarft(matrix_order, ascii(direct) : c_char, ascii(storev) : c_char, n, k, v, (v.domain.dim(2).size) : c_int, tau, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clarft for the type lapack_complex_float.
 */
inline proc larft(matrix_order : lapack_memory_order, direct : string, storev : string, n : c_int, k : c_int, v : [] complex(64), tau : [] complex(64), t : [] complex(64)): c_int{
  return LAPACKE_clarft(matrix_order, ascii(direct) : c_char, ascii(storev) : c_char, n, k, v, (v.domain.dim(2).size) : c_int, tau, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlarft for the type lapack_complex_double.
 */
inline proc larft(matrix_order : lapack_memory_order, direct : string, storev : string, n : c_int, k : c_int, v : [] complex(128), tau : [] complex(128), t : [] complex(128)): c_int{
  return LAPACKE_zlarft(matrix_order, ascii(direct) : c_char, ascii(storev) : c_char, n, k, v, (v.domain.dim(2).size) : c_int, tau, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_slarfx for the type c_float.
 */
inline proc larfx(matrix_order : lapack_memory_order, side : string, v : [] c_float, tau : c_float, c : [] c_float, work : [] c_float): c_int{
  return LAPACKE_slarfx(matrix_order, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, v, tau, c, (c.domain.dim(2).size) : c_int, work);
}

/* 
Wrapped procedure of LAPACKE_dlarfx for the type c_double.
 */
inline proc larfx(matrix_order : lapack_memory_order, side : string, v : [] c_double, tau : c_double, c : [] c_double, work : [] c_double): c_int{
  return LAPACKE_dlarfx(matrix_order, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, v, tau, c, (c.domain.dim(2).size) : c_int, work);
}

/* 
Wrapped procedure of LAPACKE_clarfx for the type lapack_complex_float.
 */
inline proc larfx(matrix_order : lapack_memory_order, side : string, v : [] complex(64), tau : complex(64), c : [] complex(64), work : [] complex(64)): c_int{
  return LAPACKE_clarfx(matrix_order, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, v, tau, c, (c.domain.dim(2).size) : c_int, work);
}

/* 
Wrapped procedure of LAPACKE_zlarfx for the type lapack_complex_double.
 */
inline proc larfx(matrix_order : lapack_memory_order, side : string, v : [] complex(128), tau : complex(128), c : [] complex(128), work : [] complex(128)): c_int{
  return LAPACKE_zlarfx(matrix_order, ascii(side) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, v, tau, c, (c.domain.dim(2).size) : c_int, work);
}

/* 
Wrapped procedure of LAPACKE_slarnv for the type c_float.
 */
inline proc larnv(idist : c_int, iseed : [] c_int, n : c_int, x : [] c_float): c_int{
  return LAPACKE_slarnv(idist, iseed, n, x);
}

/* 
Wrapped procedure of LAPACKE_dlarnv for the type c_double.
 */
inline proc larnv(idist : c_int, iseed : [] c_int, n : c_int, x : [] c_double): c_int{
  return LAPACKE_dlarnv(idist, iseed, n, x);
}

/* 
Wrapped procedure of LAPACKE_clarnv for the type lapack_complex_float.
 */
inline proc larnv(idist : c_int, iseed : [] c_int, n : c_int, x : [] complex(64)): c_int{
  return LAPACKE_clarnv(idist, iseed, n, x);
}

/* 
Wrapped procedure of LAPACKE_zlarnv for the type lapack_complex_double.
 */
inline proc larnv(idist : c_int, iseed : [] c_int, n : c_int, x : [] complex(128)): c_int{
  return LAPACKE_zlarnv(idist, iseed, n, x);
}

/* 
Wrapped procedure of LAPACKE_slaset for the type c_float.
 */
inline proc laset(matrix_order : lapack_memory_order, uplo : string, alpha : c_float, beta : c_float, a : [] c_float): c_int{
  return LAPACKE_slaset(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, alpha, beta, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dlaset for the type c_double.
 */
inline proc laset(matrix_order : lapack_memory_order, uplo : string, alpha : c_double, beta : c_double, a : [] c_double): c_int{
  return LAPACKE_dlaset(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, alpha, beta, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_claset for the type lapack_complex_float.
 */
inline proc laset(matrix_order : lapack_memory_order, uplo : string, alpha : complex(64), beta : complex(64), a : [] complex(64)): c_int{
  return LAPACKE_claset(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, alpha, beta, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlaset for the type lapack_complex_double.
 */
inline proc laset(matrix_order : lapack_memory_order, uplo : string, alpha : complex(128), beta : complex(128), a : [] complex(128)): c_int{
  return LAPACKE_zlaset(matrix_order, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, alpha, beta, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_slasrt for the type c_float.
 */
inline proc lasrt(id : string, n : c_int, d : [] c_float): c_int{
  return LAPACKE_slasrt(ascii(id) : c_char, n, d);
}

/* 
Wrapped procedure of LAPACKE_dlasrt for the type c_double.
 */
inline proc lasrt(id : string, n : c_int, d : [] c_double): c_int{
  return LAPACKE_dlasrt(ascii(id) : c_char, n, d);
}

/* 
Wrapped procedure of LAPACKE_slaswp for the type c_float.
 */
inline proc laswp(matrix_order : lapack_memory_order, a : [] c_float, k1 : c_int, k2 : c_int, ipiv : [] c_int, incx : c_int): c_int{
  return LAPACKE_slaswp(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, k1, k2, ipiv, incx);
}

/* 
Wrapped procedure of LAPACKE_dlaswp for the type c_double.
 */
inline proc laswp(matrix_order : lapack_memory_order, a : [] c_double, k1 : c_int, k2 : c_int, ipiv : [] c_int, incx : c_int): c_int{
  return LAPACKE_dlaswp(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, k1, k2, ipiv, incx);
}

/* 
Wrapped procedure of LAPACKE_claswp for the type lapack_complex_float.
 */
inline proc laswp(matrix_order : lapack_memory_order, a : [] complex(64), k1 : c_int, k2 : c_int, ipiv : [] c_int, incx : c_int): c_int{
  return LAPACKE_claswp(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, k1, k2, ipiv, incx);
}

/* 
Wrapped procedure of LAPACKE_zlaswp for the type lapack_complex_double.
 */
inline proc laswp(matrix_order : lapack_memory_order, a : [] complex(128), k1 : c_int, k2 : c_int, ipiv : [] c_int, incx : c_int): c_int{
  return LAPACKE_zlaswp(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, k1, k2, ipiv, incx);
}

/* 
Wrapped procedure of LAPACKE_slatms for the type c_float.
 */
inline proc latms(matrix_order : lapack_memory_order, dist : string, iseed : [] c_int, sym : string, d : [] c_float, mode : c_int, cond : c_float, dmax : c_float, kl : c_int, ku : c_int, pack : string, a : [] c_float): c_int{
  return LAPACKE_slatms(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, ascii(dist) : c_char, iseed, ascii(sym) : c_char, d, mode, cond, dmax, kl, ku, ascii(pack) : c_char, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dlatms for the type c_double.
 */
inline proc latms(matrix_order : lapack_memory_order, dist : string, iseed : [] c_int, sym : string, d : [] c_double, mode : c_int, cond : c_double, dmax : c_double, kl : c_int, ku : c_int, pack : string, a : [] c_double): c_int{
  return LAPACKE_dlatms(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, ascii(dist) : c_char, iseed, ascii(sym) : c_char, d, mode, cond, dmax, kl, ku, ascii(pack) : c_char, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clatms for the type lapack_complex_float.
 */
inline proc latms(matrix_order : lapack_memory_order, dist : string, iseed : [] c_int, sym : string, d : [] c_float, mode : c_int, cond : c_float, dmax : c_float, kl : c_int, ku : c_int, pack : string, a : [] complex(64)): c_int{
  return LAPACKE_clatms(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, ascii(dist) : c_char, iseed, ascii(sym) : c_char, d, mode, cond, dmax, kl, ku, ascii(pack) : c_char, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlatms for the type lapack_complex_double.
 */
inline proc latms(matrix_order : lapack_memory_order, dist : string, iseed : [] c_int, sym : string, d : [] c_double, mode : c_int, cond : c_double, dmax : c_double, kl : c_int, ku : c_int, pack : string, a : [] complex(128)): c_int{
  return LAPACKE_zlatms(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, ascii(dist) : c_char, iseed, ascii(sym) : c_char, d, mode, cond, dmax, kl, ku, ascii(pack) : c_char, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_slauum for the type c_float.
 */
inline proc lauum(matrix_order : lapack_memory_order, uplo : string, n : c_int, a : [] c_float): c_int{
  return LAPACKE_slauum(matrix_order, ascii(uplo) : c_char, n, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dlauum for the type c_double.
 */
inline proc lauum(matrix_order : lapack_memory_order, uplo : string, n : c_int, a : [] c_double): c_int{
  return LAPACKE_dlauum(matrix_order, ascii(uplo) : c_char, n, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_clauum for the type lapack_complex_float.
 */
inline proc lauum(matrix_order : lapack_memory_order, uplo : string, n : c_int, a : [] complex(64)): c_int{
  return LAPACKE_clauum(matrix_order, ascii(uplo) : c_char, n, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zlauum for the type lapack_complex_double.
 */
inline proc lauum(matrix_order : lapack_memory_order, uplo : string, n : c_int, a : [] complex(128)): c_int{
  return LAPACKE_zlauum(matrix_order, ascii(uplo) : c_char, n, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sopgtr for the type c_float.
 */
inline proc opgtr(matrix_order : lapack_memory_order, uplo : string, ap : [] c_float, tau : [] c_float, q : [] c_float): c_int{
  return LAPACKE_sopgtr(matrix_order, ascii(uplo) : c_char, (q.domain.dim(1).size) : c_int, ap, tau, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dopgtr for the type c_double.
 */
inline proc opgtr(matrix_order : lapack_memory_order, uplo : string, ap : [] c_double, tau : [] c_double, q : [] c_double): c_int{
  return LAPACKE_dopgtr(matrix_order, ascii(uplo) : c_char, (q.domain.dim(1).size) : c_int, ap, tau, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sopmtr for the type c_float.
 */
inline proc opmtr(matrix_order : lapack_memory_order, side : string, uplo : string, trans : string, ap : [] c_float, tau : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sopmtr(matrix_order, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, ap, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dopmtr for the type c_double.
 */
inline proc opmtr(matrix_order : lapack_memory_order, side : string, uplo : string, trans : string, ap : [] c_double, tau : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dopmtr(matrix_order, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, ap, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sorgbr for the type c_float.
 */
inline proc orgbr(matrix_order : lapack_memory_order, vect : string, k : c_int, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sorgbr(matrix_order, ascii(vect) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dorgbr for the type c_double.
 */
inline proc orgbr(matrix_order : lapack_memory_order, vect : string, k : c_int, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dorgbr(matrix_order, ascii(vect) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sorghr for the type c_float.
 */
inline proc orghr(matrix_order : lapack_memory_order, n : c_int, ilo : c_int, ihi : c_int, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sorghr(matrix_order, n, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dorghr for the type c_double.
 */
inline proc orghr(matrix_order : lapack_memory_order, n : c_int, ilo : c_int, ihi : c_int, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dorghr(matrix_order, n, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sorglq for the type c_float.
 */
inline proc orglq(matrix_order : lapack_memory_order, k : c_int, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sorglq(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dorglq for the type c_double.
 */
inline proc orglq(matrix_order : lapack_memory_order, k : c_int, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dorglq(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sorgql for the type c_float.
 */
inline proc orgql(matrix_order : lapack_memory_order, k : c_int, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sorgql(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dorgql for the type c_double.
 */
inline proc orgql(matrix_order : lapack_memory_order, k : c_int, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dorgql(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sorgqr for the type c_float.
 */
inline proc orgqr(matrix_order : lapack_memory_order, k : c_int, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sorgqr(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dorgqr for the type c_double.
 */
inline proc orgqr(matrix_order : lapack_memory_order, k : c_int, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dorgqr(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sorgrq for the type c_float.
 */
inline proc orgrq(matrix_order : lapack_memory_order, k : c_int, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sorgrq(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dorgrq for the type c_double.
 */
inline proc orgrq(matrix_order : lapack_memory_order, k : c_int, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dorgrq(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sorgtr for the type c_float.
 */
inline proc orgtr(matrix_order : lapack_memory_order, uplo : string, n : c_int, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_sorgtr(matrix_order, ascii(uplo) : c_char, n, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dorgtr for the type c_double.
 */
inline proc orgtr(matrix_order : lapack_memory_order, uplo : string, n : c_int, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dorgtr(matrix_order, ascii(uplo) : c_char, n, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_sormbr for the type c_float.
 */
inline proc ormbr(matrix_order : lapack_memory_order, vect : string, side : string, trans : string, k : c_int, a : [] c_float, tau : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sormbr(matrix_order, ascii(vect) : c_char, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dormbr for the type c_double.
 */
inline proc ormbr(matrix_order : lapack_memory_order, vect : string, side : string, trans : string, k : c_int, a : [] c_double, tau : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dormbr(matrix_order, ascii(vect) : c_char, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sormhr for the type c_float.
 */
inline proc ormhr(matrix_order : lapack_memory_order, side : string, trans : string, ilo : c_int, ihi : c_int, a : [] c_float, tau : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sormhr(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dormhr for the type c_double.
 */
inline proc ormhr(matrix_order : lapack_memory_order, side : string, trans : string, ilo : c_int, ihi : c_int, a : [] c_double, tau : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dormhr(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sormlq for the type c_float.
 */
inline proc ormlq(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] c_float, tau : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sormlq(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dormlq for the type c_double.
 */
inline proc ormlq(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] c_double, tau : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dormlq(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sormql for the type c_float.
 */
inline proc ormql(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] c_float, tau : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sormql(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dormql for the type c_double.
 */
inline proc ormql(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] c_double, tau : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dormql(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sormqr for the type c_float.
 */
inline proc ormqr(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] c_float, tau : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sormqr(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dormqr for the type c_double.
 */
inline proc ormqr(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] c_double, tau : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dormqr(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sormrq for the type c_float.
 */
inline proc ormrq(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] c_float, tau : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sormrq(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dormrq for the type c_double.
 */
inline proc ormrq(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] c_double, tau : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dormrq(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sormrz for the type c_float.
 */
inline proc ormrz(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, l : c_int, a : [] c_float, tau : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sormrz(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dormrz for the type c_double.
 */
inline proc ormrz(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, l : c_int, a : [] c_double, tau : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dormrz(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sormtr for the type c_float.
 */
inline proc ormtr(matrix_order : lapack_memory_order, side : string, uplo : string, trans : string, a : [] c_float, tau : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sormtr(matrix_order, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dormtr for the type c_double.
 */
inline proc ormtr(matrix_order : lapack_memory_order, side : string, uplo : string, trans : string, a : [] c_double, tau : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dormtr(matrix_order, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_spbcon for the type c_float.
 */
inline proc pbcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_float, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_spbcon(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_dpbcon for the type c_double.
 */
inline proc pbcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_double, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_dpbcon(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_cpbcon for the type lapack_complex_float.
 */
inline proc pbcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(64), anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_cpbcon(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_zpbcon for the type lapack_complex_double.
 */
inline proc pbcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(128), anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_zpbcon(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_spbequ for the type c_float.
 */
inline proc pbequ(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_float, s : [] c_float, ref scond : c_float, ref amax : c_float): c_int{
  return LAPACKE_spbequ(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_dpbequ for the type c_double.
 */
inline proc pbequ(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_double, s : [] c_double, ref scond : c_double, ref amax : c_double): c_int{
  return LAPACKE_dpbequ(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_cpbequ for the type lapack_complex_float.
 */
inline proc pbequ(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(64), s : [] c_float, ref scond : c_float, ref amax : c_float): c_int{
  return LAPACKE_cpbequ(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_zpbequ for the type lapack_complex_double.
 */
inline proc pbequ(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(128), s : [] c_double, ref scond : c_double, ref amax : c_double): c_int{
  return LAPACKE_zpbequ(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_spbrfs for the type c_float.
 */
inline proc pbrfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_float, afb : [] c_float, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_spbrfs(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dpbrfs for the type c_double.
 */
inline proc pbrfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_double, afb : [] c_double, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dpbrfs(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cpbrfs for the type lapack_complex_float.
 */
inline proc pbrfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(64), afb : [] complex(64), b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cpbrfs(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zpbrfs for the type lapack_complex_double.
 */
inline proc pbrfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(128), afb : [] complex(128), b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zpbrfs(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_spbstf for the type c_float.
 */
inline proc pbstf(matrix_order : lapack_memory_order, uplo : string, n : c_int, kb : c_int, bb : [] c_float, ldbb : c_int): c_int{
  return LAPACKE_spbstf(matrix_order, ascii(uplo) : c_char, n, kb, bb, ldbb);
}

/* 
Wrapped procedure of LAPACKE_dpbstf for the type c_double.
 */
inline proc pbstf(matrix_order : lapack_memory_order, uplo : string, n : c_int, kb : c_int, bb : [] c_double, ldbb : c_int): c_int{
  return LAPACKE_dpbstf(matrix_order, ascii(uplo) : c_char, n, kb, bb, ldbb);
}

/* 
Wrapped procedure of LAPACKE_cpbstf for the type lapack_complex_float.
 */
inline proc pbstf(matrix_order : lapack_memory_order, uplo : string, n : c_int, kb : c_int, bb : [] complex(64), ldbb : c_int): c_int{
  return LAPACKE_cpbstf(matrix_order, ascii(uplo) : c_char, n, kb, bb, ldbb);
}

/* 
Wrapped procedure of LAPACKE_zpbstf for the type lapack_complex_double.
 */
inline proc pbstf(matrix_order : lapack_memory_order, uplo : string, n : c_int, kb : c_int, bb : [] complex(128), ldbb : c_int): c_int{
  return LAPACKE_zpbstf(matrix_order, ascii(uplo) : c_char, n, kb, bb, ldbb);
}

/* 
Wrapped procedure of LAPACKE_spbsv for the type c_float.
 */
inline proc pbsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_float, b : [] c_float): c_int{
  return LAPACKE_spbsv(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dpbsv for the type c_double.
 */
inline proc pbsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dpbsv(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cpbsv for the type lapack_complex_float.
 */
inline proc pbsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_cpbsv(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zpbsv for the type lapack_complex_double.
 */
inline proc pbsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zpbsv(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_spbsvx for the type c_float.
 */
inline proc pbsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, kd : c_int, ab : [] c_float, afb : [] c_float, ref equed : string, s : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_spbsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dpbsvx for the type c_double.
 */
inline proc pbsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, kd : c_int, ab : [] c_double, afb : [] c_double, ref equed : string, s : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dpbsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cpbsvx for the type lapack_complex_float.
 */
inline proc pbsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, kd : c_int, ab : [] complex(64), afb : [] complex(64), ref equed : string, s : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cpbsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zpbsvx for the type lapack_complex_double.
 */
inline proc pbsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, kd : c_int, ab : [] complex(128), afb : [] complex(128), ref equed : string, s : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zpbsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, afb, (afb.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_spbtrf for the type c_float.
 */
inline proc pbtrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_float): c_int{
  return LAPACKE_spbtrf(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dpbtrf for the type c_double.
 */
inline proc pbtrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_double): c_int{
  return LAPACKE_dpbtrf(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cpbtrf for the type lapack_complex_float.
 */
inline proc pbtrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(64)): c_int{
  return LAPACKE_cpbtrf(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zpbtrf for the type lapack_complex_double.
 */
inline proc pbtrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(128)): c_int{
  return LAPACKE_zpbtrf(matrix_order, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_spbtrs for the type c_float.
 */
inline proc pbtrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_float, b : [] c_float): c_int{
  return LAPACKE_spbtrs(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dpbtrs for the type c_double.
 */
inline proc pbtrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dpbtrs(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cpbtrs for the type lapack_complex_float.
 */
inline proc pbtrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_cpbtrs(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zpbtrs for the type lapack_complex_double.
 */
inline proc pbtrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, kd : c_int, ab : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zpbtrs(matrix_order, ascii(uplo) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_spftrf for the type c_float.
 */
inline proc pftrf(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] c_float): c_int{
  return LAPACKE_spftrf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_dpftrf for the type c_double.
 */
inline proc pftrf(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] c_double): c_int{
  return LAPACKE_dpftrf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_cpftrf for the type lapack_complex_float.
 */
inline proc pftrf(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] complex(64)): c_int{
  return LAPACKE_cpftrf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_zpftrf for the type lapack_complex_double.
 */
inline proc pftrf(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] complex(128)): c_int{
  return LAPACKE_zpftrf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_spftri for the type c_float.
 */
inline proc pftri(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] c_float): c_int{
  return LAPACKE_spftri(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_dpftri for the type c_double.
 */
inline proc pftri(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] c_double): c_int{
  return LAPACKE_dpftri(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_cpftri for the type lapack_complex_float.
 */
inline proc pftri(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] complex(64)): c_int{
  return LAPACKE_cpftri(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_zpftri for the type lapack_complex_double.
 */
inline proc pftri(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] complex(128)): c_int{
  return LAPACKE_zpftri(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_spftrs for the type c_float.
 */
inline proc pftrs(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] c_float, b : [] c_float): c_int{
  return LAPACKE_spftrs(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dpftrs for the type c_double.
 */
inline proc pftrs(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dpftrs(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cpftrs for the type lapack_complex_float.
 */
inline proc pftrs(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_cpftrs(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zpftrs for the type lapack_complex_double.
 */
inline proc pftrs(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zpftrs(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_spocon for the type c_float.
 */
inline proc pocon(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_spocon(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_dpocon for the type c_double.
 */
inline proc pocon(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_dpocon(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_cpocon for the type lapack_complex_float.
 */
inline proc pocon(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_cpocon(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_zpocon for the type lapack_complex_double.
 */
inline proc pocon(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_zpocon(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_spoequ for the type c_float.
 */
inline proc poequ(matrix_order : lapack_memory_order, a : [] c_float, s : [] c_float, ref scond : c_float, ref amax : c_float): c_int{
  return LAPACKE_spoequ(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_dpoequ for the type c_double.
 */
inline proc poequ(matrix_order : lapack_memory_order, a : [] c_double, s : [] c_double, ref scond : c_double, ref amax : c_double): c_int{
  return LAPACKE_dpoequ(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_cpoequ for the type lapack_complex_float.
 */
inline proc poequ(matrix_order : lapack_memory_order, a : [] complex(64), s : [] c_float, ref scond : c_float, ref amax : c_float): c_int{
  return LAPACKE_cpoequ(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_zpoequ for the type lapack_complex_double.
 */
inline proc poequ(matrix_order : lapack_memory_order, a : [] complex(128), s : [] c_double, ref scond : c_double, ref amax : c_double): c_int{
  return LAPACKE_zpoequ(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_spoequb for the type c_float.
 */
inline proc poequb(matrix_order : lapack_memory_order, a : [] c_float, s : [] c_float, ref scond : c_float, ref amax : c_float): c_int{
  return LAPACKE_spoequb(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_dpoequb for the type c_double.
 */
inline proc poequb(matrix_order : lapack_memory_order, a : [] c_double, s : [] c_double, ref scond : c_double, ref amax : c_double): c_int{
  return LAPACKE_dpoequb(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_cpoequb for the type lapack_complex_float.
 */
inline proc poequb(matrix_order : lapack_memory_order, a : [] complex(64), s : [] c_float, ref scond : c_float, ref amax : c_float): c_int{
  return LAPACKE_cpoequb(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_zpoequb for the type lapack_complex_double.
 */
inline proc poequb(matrix_order : lapack_memory_order, a : [] complex(128), s : [] c_double, ref scond : c_double, ref amax : c_double): c_int{
  return LAPACKE_zpoequb(matrix_order, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_sporfs for the type c_float.
 */
inline proc porfs(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, af : [] c_float, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_sporfs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dporfs for the type c_double.
 */
inline proc porfs(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, af : [] c_double, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dporfs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cporfs for the type lapack_complex_float.
 */
inline proc porfs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), af : [] complex(64), b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cporfs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zporfs for the type lapack_complex_double.
 */
inline proc porfs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), af : [] complex(128), b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zporfs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_sporfsx for the type c_float.
 */
inline proc porfsx(matrix_order : lapack_memory_order, uplo : string, equed : string, a : [] c_float, af : [] c_float, s : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_sporfsx(matrix_order, ascii(uplo) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_dporfsx for the type c_double.
 */
inline proc porfsx(matrix_order : lapack_memory_order, uplo : string, equed : string, a : [] c_double, af : [] c_double, s : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_dporfsx(matrix_order, ascii(uplo) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_cporfsx for the type lapack_complex_float.
 */
inline proc porfsx(matrix_order : lapack_memory_order, uplo : string, equed : string, a : [] complex(64), af : [] complex(64), s : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_cporfsx(matrix_order, ascii(uplo) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_zporfsx for the type lapack_complex_double.
 */
inline proc porfsx(matrix_order : lapack_memory_order, uplo : string, equed : string, a : [] complex(128), af : [] complex(128), s : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_zporfsx(matrix_order, ascii(uplo) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_sposv for the type c_float.
 */
inline proc posv(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, b : [] c_float): c_int{
  return LAPACKE_sposv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dposv for the type c_double.
 */
inline proc posv(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dposv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cposv for the type lapack_complex_float.
 */
inline proc posv(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_cposv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zposv for the type lapack_complex_double.
 */
inline proc posv(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zposv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsposv for the type c_double.
 */
inline proc posv(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, b : [] c_double, x : [] c_double, ref chlapack_iter : c_int): c_int{
  return LAPACKE_dsposv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, chlapack_iter);
}

/* 
Wrapped procedure of LAPACKE_zcposv for the type lapack_complex_double.
 */
inline proc posv(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), b : [] complex(128), x : [] complex(128), ref chlapack_iter : c_int): c_int{
  return LAPACKE_zcposv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, chlapack_iter);
}

/* 
Wrapped procedure of LAPACKE_sposvx for the type c_float.
 */
inline proc posvx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] c_float, af : [] c_float, ref equed : string, s : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_sposvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dposvx for the type c_double.
 */
inline proc posvx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] c_double, af : [] c_double, ref equed : string, s : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dposvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cposvx for the type lapack_complex_float.
 */
inline proc posvx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(64), af : [] complex(64), ref equed : string, s : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cposvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zposvx for the type lapack_complex_double.
 */
inline proc posvx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(128), af : [] complex(128), ref equed : string, s : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zposvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_sposvxx for the type c_float.
 */
inline proc posvxx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] c_float, af : [] c_float, ref equed : string, s : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, ref rpvgrw : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_sposvxx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_dposvxx for the type c_double.
 */
inline proc posvxx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] c_double, af : [] c_double, ref equed : string, s : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, ref rpvgrw : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_dposvxx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_cposvxx for the type lapack_complex_float.
 */
inline proc posvxx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(64), af : [] complex(64), ref equed : string, s : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ref rpvgrw : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_cposvxx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_zposvxx for the type lapack_complex_double.
 */
inline proc posvxx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(128), af : [] complex(128), ref equed : string, s : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ref rpvgrw : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_zposvxx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_spotrf for the type c_float.
 */
inline proc potrf(matrix_order : lapack_memory_order, uplo : string, a : [] c_float): c_int{
  return LAPACKE_spotrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dpotrf for the type c_double.
 */
inline proc potrf(matrix_order : lapack_memory_order, uplo : string, a : [] c_double): c_int{
  return LAPACKE_dpotrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cpotrf for the type lapack_complex_float.
 */
inline proc potrf(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64)): c_int{
  return LAPACKE_cpotrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zpotrf for the type lapack_complex_double.
 */
inline proc potrf(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128)): c_int{
  return LAPACKE_zpotrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_spotri for the type c_float.
 */
inline proc potri(matrix_order : lapack_memory_order, uplo : string, a : [] c_float): c_int{
  return LAPACKE_spotri(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dpotri for the type c_double.
 */
inline proc potri(matrix_order : lapack_memory_order, uplo : string, a : [] c_double): c_int{
  return LAPACKE_dpotri(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cpotri for the type lapack_complex_float.
 */
inline proc potri(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64)): c_int{
  return LAPACKE_cpotri(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zpotri for the type lapack_complex_double.
 */
inline proc potri(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128)): c_int{
  return LAPACKE_zpotri(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_spotrs for the type c_float.
 */
inline proc potrs(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, b : [] c_float): c_int{
  return LAPACKE_spotrs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dpotrs for the type c_double.
 */
inline proc potrs(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dpotrs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cpotrs for the type lapack_complex_float.
 */
inline proc potrs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_cpotrs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zpotrs for the type lapack_complex_double.
 */
inline proc potrs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zpotrs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sppcon for the type c_float.
 */
inline proc ppcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_sppcon(matrix_order, ascii(uplo) : c_char, n, ap, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_dppcon for the type c_double.
 */
inline proc ppcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_dppcon(matrix_order, ascii(uplo) : c_char, n, ap, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_cppcon for the type lapack_complex_float.
 */
inline proc ppcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_cppcon(matrix_order, ascii(uplo) : c_char, n, ap, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_zppcon for the type lapack_complex_double.
 */
inline proc ppcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_zppcon(matrix_order, ascii(uplo) : c_char, n, ap, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_sppequ for the type c_float.
 */
inline proc ppequ(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, s : [] c_float, ref scond : c_float, ref amax : c_float): c_int{
  return LAPACKE_sppequ(matrix_order, ascii(uplo) : c_char, n, ap, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_dppequ for the type c_double.
 */
inline proc ppequ(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, s : [] c_double, ref scond : c_double, ref amax : c_double): c_int{
  return LAPACKE_dppequ(matrix_order, ascii(uplo) : c_char, n, ap, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_cppequ for the type lapack_complex_float.
 */
inline proc ppequ(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), s : [] c_float, ref scond : c_float, ref amax : c_float): c_int{
  return LAPACKE_cppequ(matrix_order, ascii(uplo) : c_char, n, ap, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_zppequ for the type lapack_complex_double.
 */
inline proc ppequ(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), s : [] c_double, ref scond : c_double, ref amax : c_double): c_int{
  return LAPACKE_zppequ(matrix_order, ascii(uplo) : c_char, n, ap, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_spprfs for the type c_float.
 */
inline proc pprfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, afp : [] c_float, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_spprfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dpprfs for the type c_double.
 */
inline proc pprfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, afp : [] c_double, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dpprfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cpprfs for the type lapack_complex_float.
 */
inline proc pprfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), afp : [] complex(64), b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cpprfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zpprfs for the type lapack_complex_double.
 */
inline proc pprfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), afp : [] complex(128), b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zpprfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_sppsv for the type c_float.
 */
inline proc ppsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, b : [] c_float): c_int{
  return LAPACKE_sppsv(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dppsv for the type c_double.
 */
inline proc ppsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dppsv(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cppsv for the type lapack_complex_float.
 */
inline proc ppsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_cppsv(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zppsv for the type lapack_complex_double.
 */
inline proc ppsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zppsv(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sppsvx for the type c_float.
 */
inline proc ppsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, ap : [] c_float, afp : [] c_float, ref equed : string, s : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_sppsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dppsvx for the type c_double.
 */
inline proc ppsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, ap : [] c_double, afp : [] c_double, ref equed : string, s : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dppsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cppsvx for the type lapack_complex_float.
 */
inline proc ppsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, ap : [] complex(64), afp : [] complex(64), ref equed : string, s : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cppsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zppsvx for the type lapack_complex_double.
 */
inline proc ppsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, ap : [] complex(128), afp : [] complex(128), ref equed : string, s : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zppsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_spptrf for the type c_float.
 */
inline proc pptrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float): c_int{
  return LAPACKE_spptrf(matrix_order, ascii(uplo) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_dpptrf for the type c_double.
 */
inline proc pptrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double): c_int{
  return LAPACKE_dpptrf(matrix_order, ascii(uplo) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_cpptrf for the type lapack_complex_float.
 */
inline proc pptrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64)): c_int{
  return LAPACKE_cpptrf(matrix_order, ascii(uplo) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_zpptrf for the type lapack_complex_double.
 */
inline proc pptrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128)): c_int{
  return LAPACKE_zpptrf(matrix_order, ascii(uplo) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_spptri for the type c_float.
 */
inline proc pptri(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float): c_int{
  return LAPACKE_spptri(matrix_order, ascii(uplo) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_dpptri for the type c_double.
 */
inline proc pptri(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double): c_int{
  return LAPACKE_dpptri(matrix_order, ascii(uplo) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_cpptri for the type lapack_complex_float.
 */
inline proc pptri(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64)): c_int{
  return LAPACKE_cpptri(matrix_order, ascii(uplo) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_zpptri for the type lapack_complex_double.
 */
inline proc pptri(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128)): c_int{
  return LAPACKE_zpptri(matrix_order, ascii(uplo) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_spptrs for the type c_float.
 */
inline proc pptrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, b : [] c_float): c_int{
  return LAPACKE_spptrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dpptrs for the type c_double.
 */
inline proc pptrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dpptrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cpptrs for the type lapack_complex_float.
 */
inline proc pptrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_cpptrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zpptrs for the type lapack_complex_double.
 */
inline proc pptrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zpptrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_spstrf for the type c_float.
 */
inline proc pstrf(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, piv : [] c_int, ref rank : c_int, tol : c_float): c_int{
  return LAPACKE_spstrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, piv, rank, tol);
}

/* 
Wrapped procedure of LAPACKE_dpstrf for the type c_double.
 */
inline proc pstrf(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, piv : [] c_int, ref rank : c_int, tol : c_double): c_int{
  return LAPACKE_dpstrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, piv, rank, tol);
}

/* 
Wrapped procedure of LAPACKE_cpstrf for the type lapack_complex_float.
 */
inline proc pstrf(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), piv : [] c_int, ref rank : c_int, tol : c_float): c_int{
  return LAPACKE_cpstrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, piv, rank, tol);
}

/* 
Wrapped procedure of LAPACKE_zpstrf for the type lapack_complex_double.
 */
inline proc pstrf(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), piv : [] c_int, ref rank : c_int, tol : c_double): c_int{
  return LAPACKE_zpstrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, piv, rank, tol);
}

/* 
Wrapped procedure of LAPACKE_sptcon for the type c_float.
 */
inline proc ptcon(n : c_int, d : [] c_float, e : [] c_float, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_sptcon(n, d, e, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_dptcon for the type c_double.
 */
inline proc ptcon(n : c_int, d : [] c_double, e : [] c_double, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_dptcon(n, d, e, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_cptcon for the type lapack_complex_float.
 */
inline proc ptcon(n : c_int, d : [] c_float, e : [] complex(64), anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_cptcon(n, d, e, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_zptcon for the type lapack_complex_double.
 */
inline proc ptcon(n : c_int, d : [] c_double, e : [] complex(128), anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_zptcon(n, d, e, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_spteqr for the type c_float.
 */
inline proc pteqr(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_float, e : [] c_float, z : [] c_float): c_int{
  return LAPACKE_spteqr(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dpteqr for the type c_double.
 */
inline proc pteqr(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_double, e : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dpteqr(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cpteqr for the type lapack_complex_float.
 */
inline proc pteqr(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_float, e : [] c_float, z : [] complex(64)): c_int{
  return LAPACKE_cpteqr(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zpteqr for the type lapack_complex_double.
 */
inline proc pteqr(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_double, e : [] c_double, z : [] complex(128)): c_int{
  return LAPACKE_zpteqr(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sptrfs for the type c_float.
 */
inline proc ptrfs(matrix_order : lapack_memory_order, n : c_int, d : [] c_float, e : [] c_float, df : [] c_float, ef : [] c_float, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_sptrfs(matrix_order, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, df, ef, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dptrfs for the type c_double.
 */
inline proc ptrfs(matrix_order : lapack_memory_order, n : c_int, d : [] c_double, e : [] c_double, df : [] c_double, ef : [] c_double, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dptrfs(matrix_order, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, df, ef, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cptrfs for the type lapack_complex_float.
 */
inline proc ptrfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, d : [] c_float, e : [] complex(64), df : [] c_float, ef : [] complex(64), b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cptrfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, df, ef, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zptrfs for the type lapack_complex_double.
 */
inline proc ptrfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, d : [] c_double, e : [] complex(128), df : [] c_double, ef : [] complex(128), b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zptrfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, df, ef, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_sptsv for the type c_float.
 */
inline proc ptsv(matrix_order : lapack_memory_order, n : c_int, d : [] c_float, e : [] c_float, b : [] c_float): c_int{
  return LAPACKE_sptsv(matrix_order, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dptsv for the type c_double.
 */
inline proc ptsv(matrix_order : lapack_memory_order, n : c_int, d : [] c_double, e : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dptsv(matrix_order, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cptsv for the type lapack_complex_float.
 */
inline proc ptsv(matrix_order : lapack_memory_order, n : c_int, d : [] c_float, e : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_cptsv(matrix_order, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zptsv for the type lapack_complex_double.
 */
inline proc ptsv(matrix_order : lapack_memory_order, n : c_int, d : [] c_double, e : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zptsv(matrix_order, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sptsvx for the type c_float.
 */
inline proc ptsvx(matrix_order : lapack_memory_order, fact : string, n : c_int, d : [] c_float, e : [] c_float, df : [] c_float, ef : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_sptsvx(matrix_order, ascii(fact) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, df, ef, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dptsvx for the type c_double.
 */
inline proc ptsvx(matrix_order : lapack_memory_order, fact : string, n : c_int, d : [] c_double, e : [] c_double, df : [] c_double, ef : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dptsvx(matrix_order, ascii(fact) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, df, ef, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cptsvx for the type lapack_complex_float.
 */
inline proc ptsvx(matrix_order : lapack_memory_order, fact : string, n : c_int, d : [] c_float, e : [] complex(64), df : [] c_float, ef : [] complex(64), b : [] complex(64), x : [] complex(64), ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cptsvx(matrix_order, ascii(fact) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, df, ef, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zptsvx for the type lapack_complex_double.
 */
inline proc ptsvx(matrix_order : lapack_memory_order, fact : string, n : c_int, d : [] c_double, e : [] complex(128), df : [] c_double, ef : [] complex(128), b : [] complex(128), x : [] complex(128), ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zptsvx(matrix_order, ascii(fact) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, df, ef, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_spttrf for the type c_float.
 */
inline proc pttrf(n : c_int, d : [] c_float, e : [] c_float): c_int{
  return LAPACKE_spttrf(n, d, e);
}

/* 
Wrapped procedure of LAPACKE_dpttrf for the type c_double.
 */
inline proc pttrf(n : c_int, d : [] c_double, e : [] c_double): c_int{
  return LAPACKE_dpttrf(n, d, e);
}

/* 
Wrapped procedure of LAPACKE_cpttrf for the type lapack_complex_float.
 */
inline proc pttrf(n : c_int, d : [] c_float, e : [] complex(64)): c_int{
  return LAPACKE_cpttrf(n, d, e);
}

/* 
Wrapped procedure of LAPACKE_zpttrf for the type lapack_complex_double.
 */
inline proc pttrf(n : c_int, d : [] c_double, e : [] complex(128)): c_int{
  return LAPACKE_zpttrf(n, d, e);
}

/* 
Wrapped procedure of LAPACKE_spttrs for the type c_float.
 */
inline proc pttrs(matrix_order : lapack_memory_order, n : c_int, d : [] c_float, e : [] c_float, b : [] c_float): c_int{
  return LAPACKE_spttrs(matrix_order, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dpttrs for the type c_double.
 */
inline proc pttrs(matrix_order : lapack_memory_order, n : c_int, d : [] c_double, e : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dpttrs(matrix_order, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cpttrs for the type lapack_complex_float.
 */
inline proc pttrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, d : [] c_float, e : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_cpttrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zpttrs for the type lapack_complex_double.
 */
inline proc pttrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, d : [] c_double, e : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_zpttrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, d, e, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssbev for the type c_float.
 */
inline proc sbev(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, kd : c_int, ab : [] c_float, w : [] c_float, z : [] c_float): c_int{
  return LAPACKE_ssbev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsbev for the type c_double.
 */
inline proc sbev(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, kd : c_int, ab : [] c_double, w : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dsbev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssbevd for the type c_float.
 */
inline proc sbevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, kd : c_int, ab : [] c_float, w : [] c_float, z : [] c_float): c_int{
  return LAPACKE_ssbevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsbevd for the type c_double.
 */
inline proc sbevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, kd : c_int, ab : [] c_double, w : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dsbevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssbevx for the type c_float.
 */
inline proc sbevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, n : c_int, kd : c_int, ab : [] c_float, q : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] c_float, ifail : [] c_int): c_int{
  return LAPACKE_ssbevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_dsbevx for the type c_double.
 */
inline proc sbevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, n : c_int, kd : c_int, ab : [] c_double, q : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] c_double, ifail : [] c_int): c_int{
  return LAPACKE_dsbevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_ssbgst for the type c_float.
 */
inline proc sbgst(matrix_order : lapack_memory_order, vect : string, uplo : string, ka : c_int, kb : c_int, ab : [] c_float, bb : [] c_float, x : [] c_float): c_int{
  return LAPACKE_ssbgst(matrix_order, ascii(vect) : c_char, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(2).size else x.domain.dim(1).size) : c_int, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsbgst for the type c_double.
 */
inline proc sbgst(matrix_order : lapack_memory_order, vect : string, uplo : string, ka : c_int, kb : c_int, ab : [] c_double, bb : [] c_double, x : [] c_double): c_int{
  return LAPACKE_dsbgst(matrix_order, ascii(vect) : c_char, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(2).size else x.domain.dim(1).size) : c_int, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssbgv for the type c_float.
 */
inline proc sbgv(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ka : c_int, kb : c_int, ab : [] c_float, bb : [] c_float, w : [] c_float, z : [] c_float): c_int{
  return LAPACKE_ssbgv(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsbgv for the type c_double.
 */
inline proc sbgv(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ka : c_int, kb : c_int, ab : [] c_double, bb : [] c_double, w : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dsbgv(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssbgvd for the type c_float.
 */
inline proc sbgvd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ka : c_int, kb : c_int, ab : [] c_float, bb : [] c_float, w : [] c_float, z : [] c_float): c_int{
  return LAPACKE_ssbgvd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsbgvd for the type c_double.
 */
inline proc sbgvd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ka : c_int, kb : c_int, ab : [] c_double, bb : [] c_double, w : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dsbgvd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssbgvx for the type c_float.
 */
inline proc sbgvx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, ka : c_int, kb : c_int, ab : [] c_float, bb : [] c_float, q : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] c_float, ifail : [] c_int): c_int{
  return LAPACKE_ssbgvx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then q.domain.dim(2).size else q.domain.dim(1).size) : c_int, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_dsbgvx for the type c_double.
 */
inline proc sbgvx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, ka : c_int, kb : c_int, ab : [] c_double, bb : [] c_double, q : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] c_double, ifail : [] c_int): c_int{
  return LAPACKE_dsbgvx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (if matrix_order == lapack_memory_order.row_major then q.domain.dim(2).size else q.domain.dim(1).size) : c_int, ka, kb, ab, (ab.domain.dim(2).size) : c_int, bb, (bb.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_ssbtrd for the type c_float.
 */
inline proc sbtrd(matrix_order : lapack_memory_order, vect : string, uplo : string, n : c_int, kd : c_int, ab : [] c_float, d : [] c_float, e : [] c_float, q : [] c_float): c_int{
  return LAPACKE_ssbtrd(matrix_order, ascii(vect) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, d, e, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsbtrd for the type c_double.
 */
inline proc sbtrd(matrix_order : lapack_memory_order, vect : string, uplo : string, n : c_int, kd : c_int, ab : [] c_double, d : [] c_double, e : [] c_double, q : [] c_double): c_int{
  return LAPACKE_dsbtrd(matrix_order, ascii(vect) : c_char, ascii(uplo) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, d, e, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssfrk for the type c_float.
 */
inline proc sfrk(matrix_order : lapack_memory_order, transr : string, uplo : string, trans : string, alpha : c_float, a : [] c_float, beta : c_float, c : [] c_float): c_int{
  return LAPACKE_ssfrk(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (c.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, alpha, a, (a.domain.dim(2).size) : c_int, beta, c);
}

/* 
Wrapped procedure of LAPACKE_dsfrk for the type c_double.
 */
inline proc sfrk(matrix_order : lapack_memory_order, transr : string, uplo : string, trans : string, alpha : c_double, a : [] c_double, beta : c_double, c : [] c_double): c_int{
  return LAPACKE_dsfrk(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (c.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, alpha, a, (a.domain.dim(2).size) : c_int, beta, c);
}

/* 
Wrapped procedure of LAPACKE_sspcon for the type c_float.
 */
inline proc spcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, ipiv : [] c_int, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_sspcon(matrix_order, ascii(uplo) : c_char, n, ap, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_dspcon for the type c_double.
 */
inline proc spcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, ipiv : [] c_int, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_dspcon(matrix_order, ascii(uplo) : c_char, n, ap, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_cspcon for the type lapack_complex_float.
 */
inline proc spcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), ipiv : [] c_int, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_cspcon(matrix_order, ascii(uplo) : c_char, n, ap, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_zspcon for the type lapack_complex_double.
 */
inline proc spcon(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), ipiv : [] c_int, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_zspcon(matrix_order, ascii(uplo) : c_char, n, ap, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_sspev for the type c_float.
 */
inline proc spev(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ap : [] c_float, w : [] c_float, z : [] c_float): c_int{
  return LAPACKE_sspev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dspev for the type c_double.
 */
inline proc spev(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ap : [] c_double, w : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dspev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sspevd for the type c_float.
 */
inline proc spevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ap : [] c_float, w : [] c_float, z : [] c_float): c_int{
  return LAPACKE_sspevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dspevd for the type c_double.
 */
inline proc spevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, n : c_int, ap : [] c_double, w : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dspevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sspevx for the type c_float.
 */
inline proc spevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, n : c_int, ap : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] c_float, ifail : [] c_int): c_int{
  return LAPACKE_sspevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, ap, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_dspevx for the type c_double.
 */
inline proc spevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, n : c_int, ap : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] c_double, ifail : [] c_int): c_int{
  return LAPACKE_dspevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, ap, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_sspgst for the type c_float.
 */
inline proc spgst(matrix_order : lapack_memory_order, itype : c_int, uplo : string, n : c_int, ap : [] c_float, bp : [] c_float): c_int{
  return LAPACKE_sspgst(matrix_order, itype, ascii(uplo) : c_char, n, ap, bp);
}

/* 
Wrapped procedure of LAPACKE_dspgst for the type c_double.
 */
inline proc spgst(matrix_order : lapack_memory_order, itype : c_int, uplo : string, n : c_int, ap : [] c_double, bp : [] c_double): c_int{
  return LAPACKE_dspgst(matrix_order, itype, ascii(uplo) : c_char, n, ap, bp);
}

/* 
Wrapped procedure of LAPACKE_sspgv for the type c_float.
 */
inline proc spgv(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, n : c_int, ap : [] c_float, bp : [] c_float, w : [] c_float, z : [] c_float): c_int{
  return LAPACKE_sspgv(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, bp, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dspgv for the type c_double.
 */
inline proc spgv(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, n : c_int, ap : [] c_double, bp : [] c_double, w : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dspgv(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, bp, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sspgvd for the type c_float.
 */
inline proc spgvd(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, n : c_int, ap : [] c_float, bp : [] c_float, w : [] c_float, z : [] c_float): c_int{
  return LAPACKE_sspgvd(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, bp, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dspgvd for the type c_double.
 */
inline proc spgvd(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, n : c_int, ap : [] c_double, bp : [] c_double, w : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dspgvd(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, n, ap, bp, w, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sspgvx for the type c_float.
 */
inline proc spgvx(matrix_order : lapack_memory_order, itype : c_int, jobz : string, range : string, uplo : string, n : c_int, ap : [] c_float, bp : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] c_float, ifail : [] c_int): c_int{
  return LAPACKE_sspgvx(matrix_order, itype, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, ap, bp, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_dspgvx for the type c_double.
 */
inline proc spgvx(matrix_order : lapack_memory_order, itype : c_int, jobz : string, range : string, uplo : string, n : c_int, ap : [] c_double, bp : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] c_double, ifail : [] c_int): c_int{
  return LAPACKE_dspgvx(matrix_order, itype, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, ap, bp, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_ssprfs for the type c_float.
 */
inline proc sprfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, afp : [] c_float, ipiv : [] c_int, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_ssprfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dsprfs for the type c_double.
 */
inline proc sprfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, afp : [] c_double, ipiv : [] c_int, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dsprfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_csprfs for the type lapack_complex_float.
 */
inline proc sprfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), afp : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_csprfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zsprfs for the type lapack_complex_double.
 */
inline proc sprfs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), afp : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zsprfs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_sspsv for the type c_float.
 */
inline proc spsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, ipiv : [] c_int, b : [] c_float): c_int{
  return LAPACKE_sspsv(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dspsv for the type c_double.
 */
inline proc spsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, ipiv : [] c_int, b : [] c_double): c_int{
  return LAPACKE_dspsv(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cspsv for the type lapack_complex_float.
 */
inline proc spsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_cspsv(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zspsv for the type lapack_complex_double.
 */
inline proc spsv(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zspsv(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sspsvx for the type c_float.
 */
inline proc spsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, ap : [] c_float, afp : [] c_float, ipiv : [] c_int, b : [] c_float, x : [] c_float, ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_sspsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dspsvx for the type c_double.
 */
inline proc spsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, ap : [] c_double, afp : [] c_double, ipiv : [] c_int, b : [] c_double, x : [] c_double, ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dspsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_cspsvx for the type lapack_complex_float.
 */
inline proc spsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, ap : [] complex(64), afp : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_cspsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zspsvx for the type lapack_complex_double.
 */
inline proc spsvx(matrix_order : lapack_memory_order, fact : string, uplo : string, n : c_int, ap : [] complex(128), afp : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zspsvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, afp, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_ssptrd for the type c_float.
 */
inline proc sptrd(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, d : [] c_float, e : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_ssptrd(matrix_order, ascii(uplo) : c_char, n, ap, d, e, tau);
}

/* 
Wrapped procedure of LAPACKE_dsptrd for the type c_double.
 */
inline proc sptrd(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, d : [] c_double, e : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dsptrd(matrix_order, ascii(uplo) : c_char, n, ap, d, e, tau);
}

/* 
Wrapped procedure of LAPACKE_ssptrf for the type c_float.
 */
inline proc sptrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, ipiv : [] c_int): c_int{
  return LAPACKE_ssptrf(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_dsptrf for the type c_double.
 */
inline proc sptrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, ipiv : [] c_int): c_int{
  return LAPACKE_dsptrf(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_csptrf for the type lapack_complex_float.
 */
inline proc sptrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_csptrf(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zsptrf for the type lapack_complex_double.
 */
inline proc sptrf(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zsptrf(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_ssptri for the type c_float.
 */
inline proc sptri(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, ipiv : [] c_int): c_int{
  return LAPACKE_ssptri(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_dsptri for the type c_double.
 */
inline proc sptri(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, ipiv : [] c_int): c_int{
  return LAPACKE_dsptri(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_csptri for the type lapack_complex_float.
 */
inline proc sptri(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_csptri(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zsptri for the type lapack_complex_double.
 */
inline proc sptri(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zsptri(matrix_order, ascii(uplo) : c_char, n, ap, ipiv);
}

/* 
Wrapped procedure of LAPACKE_ssptrs for the type c_float.
 */
inline proc sptrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_float, ipiv : [] c_int, b : [] c_float): c_int{
  return LAPACKE_ssptrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsptrs for the type c_double.
 */
inline proc sptrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] c_double, ipiv : [] c_int, b : [] c_double): c_int{
  return LAPACKE_dsptrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_csptrs for the type lapack_complex_float.
 */
inline proc sptrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_csptrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zsptrs for the type lapack_complex_double.
 */
inline proc sptrs(matrix_order : lapack_memory_order, uplo : string, n : c_int, ap : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zsptrs(matrix_order, ascii(uplo) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sstebz for the type c_float.
 */
inline proc stebz(range : string, order : string, n : c_int, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, d : [] c_float, e : [] c_float, ref m : c_int, ref nsplit : c_int, w : [] c_float, iblock : [] c_int, isplit : [] c_int): c_int{
  return LAPACKE_sstebz(ascii(range) : c_char, ascii(order) : c_char, n, vl, vu, il, iu, abstol, d, e, m, nsplit, w, iblock, isplit);
}

/* 
Wrapped procedure of LAPACKE_dstebz for the type c_double.
 */
inline proc stebz(range : string, order : string, n : c_int, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, d : [] c_double, e : [] c_double, ref m : c_int, ref nsplit : c_int, w : [] c_double, iblock : [] c_int, isplit : [] c_int): c_int{
  return LAPACKE_dstebz(ascii(range) : c_char, ascii(order) : c_char, n, vl, vu, il, iu, abstol, d, e, m, nsplit, w, iblock, isplit);
}

/* 
Wrapped procedure of LAPACKE_sstedc for the type c_float.
 */
inline proc stedc(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_float, e : [] c_float, z : [] c_float): c_int{
  return LAPACKE_sstedc(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dstedc for the type c_double.
 */
inline proc stedc(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_double, e : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dstedc(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cstedc for the type lapack_complex_float.
 */
inline proc stedc(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_float, e : [] c_float, z : [] complex(64)): c_int{
  return LAPACKE_cstedc(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zstedc for the type lapack_complex_double.
 */
inline proc stedc(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_double, e : [] c_double, z : [] complex(128)): c_int{
  return LAPACKE_zstedc(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sstegr for the type c_float.
 */
inline proc stegr(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_float, e : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] c_float, isuppz : [] c_int): c_int{
  return LAPACKE_sstegr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, isuppz);
}

/* 
Wrapped procedure of LAPACKE_dstegr for the type c_double.
 */
inline proc stegr(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_double, e : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] c_double, isuppz : [] c_int): c_int{
  return LAPACKE_dstegr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, isuppz);
}

/* 
Wrapped procedure of LAPACKE_cstegr for the type lapack_complex_float.
 */
inline proc stegr(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_float, e : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] complex(64), isuppz : [] c_int): c_int{
  return LAPACKE_cstegr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, isuppz);
}

/* 
Wrapped procedure of LAPACKE_zstegr for the type lapack_complex_double.
 */
inline proc stegr(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_double, e : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] complex(128), isuppz : [] c_int): c_int{
  return LAPACKE_zstegr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, isuppz);
}

/* 
Wrapped procedure of LAPACKE_sstein for the type c_float.
 */
inline proc stein(matrix_order : lapack_memory_order, n : c_int, d : [] c_float, e : [] c_float, m : c_int, w : [] c_float, iblock : [] c_int, isplit : [] c_int, z : [] c_float, ifailv : [] c_int): c_int{
  return LAPACKE_sstein(matrix_order, n, d, e, m, w, iblock, isplit, z, (z.domain.dim(2).size) : c_int, ifailv);
}

/* 
Wrapped procedure of LAPACKE_dstein for the type c_double.
 */
inline proc stein(matrix_order : lapack_memory_order, n : c_int, d : [] c_double, e : [] c_double, m : c_int, w : [] c_double, iblock : [] c_int, isplit : [] c_int, z : [] c_double, ifailv : [] c_int): c_int{
  return LAPACKE_dstein(matrix_order, n, d, e, m, w, iblock, isplit, z, (z.domain.dim(2).size) : c_int, ifailv);
}

/* 
Wrapped procedure of LAPACKE_cstein for the type lapack_complex_float.
 */
inline proc stein(matrix_order : lapack_memory_order, n : c_int, d : [] c_float, e : [] c_float, m : c_int, w : [] c_float, iblock : [] c_int, isplit : [] c_int, z : [] complex(64), ifailv : [] c_int): c_int{
  return LAPACKE_cstein(matrix_order, n, d, e, m, w, iblock, isplit, z, (z.domain.dim(2).size) : c_int, ifailv);
}

/* 
Wrapped procedure of LAPACKE_zstein for the type lapack_complex_double.
 */
inline proc stein(matrix_order : lapack_memory_order, n : c_int, d : [] c_double, e : [] c_double, m : c_int, w : [] c_double, iblock : [] c_int, isplit : [] c_int, z : [] complex(128), ifailv : [] c_int): c_int{
  return LAPACKE_zstein(matrix_order, n, d, e, m, w, iblock, isplit, z, (z.domain.dim(2).size) : c_int, ifailv);
}

/* 
Wrapped procedure of LAPACKE_sstemr for the type c_float.
 */
inline proc stemr(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_float, e : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, ref m : c_int, w : [] c_float, z : [] c_float, nzc : c_int, isuppz : [] c_int, ref tryrac : c_int): c_int{
  return LAPACKE_sstemr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, m, w, z, (z.domain.dim(2).size) : c_int, nzc, isuppz, tryrac);
}

/* 
Wrapped procedure of LAPACKE_dstemr for the type c_double.
 */
inline proc stemr(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_double, e : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, ref m : c_int, w : [] c_double, z : [] c_double, nzc : c_int, isuppz : [] c_int, ref tryrac : c_int): c_int{
  return LAPACKE_dstemr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, m, w, z, (z.domain.dim(2).size) : c_int, nzc, isuppz, tryrac);
}

/* 
Wrapped procedure of LAPACKE_cstemr for the type lapack_complex_float.
 */
inline proc stemr(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_float, e : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, ref m : c_int, w : [] c_float, z : [] complex(64), nzc : c_int, isuppz : [] c_int, ref tryrac : c_int): c_int{
  return LAPACKE_cstemr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, m, w, z, (z.domain.dim(2).size) : c_int, nzc, isuppz, tryrac);
}

/* 
Wrapped procedure of LAPACKE_zstemr for the type lapack_complex_double.
 */
inline proc stemr(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_double, e : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, ref m : c_int, w : [] c_double, z : [] complex(128), nzc : c_int, isuppz : [] c_int, ref tryrac : c_int): c_int{
  return LAPACKE_zstemr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, m, w, z, (z.domain.dim(2).size) : c_int, nzc, isuppz, tryrac);
}

/* 
Wrapped procedure of LAPACKE_ssteqr for the type c_float.
 */
inline proc steqr(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_float, e : [] c_float, z : [] c_float): c_int{
  return LAPACKE_ssteqr(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsteqr for the type c_double.
 */
inline proc steqr(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_double, e : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dsteqr(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_csteqr for the type lapack_complex_float.
 */
inline proc steqr(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_float, e : [] c_float, z : [] complex(64)): c_int{
  return LAPACKE_csteqr(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zsteqr for the type lapack_complex_double.
 */
inline proc steqr(matrix_order : lapack_memory_order, compz : string, n : c_int, d : [] c_double, e : [] c_double, z : [] complex(128)): c_int{
  return LAPACKE_zsteqr(matrix_order, ascii(compz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssterf for the type c_float.
 */
inline proc sterf(n : c_int, d : [] c_float, e : [] c_float): c_int{
  return LAPACKE_ssterf(n, d, e);
}

/* 
Wrapped procedure of LAPACKE_dsterf for the type c_double.
 */
inline proc sterf(n : c_int, d : [] c_double, e : [] c_double): c_int{
  return LAPACKE_dsterf(n, d, e);
}

/* 
Wrapped procedure of LAPACKE_sstev for the type c_float.
 */
inline proc stev(matrix_order : lapack_memory_order, jobz : string, n : c_int, d : [] c_float, e : [] c_float, z : [] c_float): c_int{
  return LAPACKE_sstev(matrix_order, ascii(jobz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dstev for the type c_double.
 */
inline proc stev(matrix_order : lapack_memory_order, jobz : string, n : c_int, d : [] c_double, e : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dstev(matrix_order, ascii(jobz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sstevd for the type c_float.
 */
inline proc stevd(matrix_order : lapack_memory_order, jobz : string, n : c_int, d : [] c_float, e : [] c_float, z : [] c_float): c_int{
  return LAPACKE_sstevd(matrix_order, ascii(jobz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dstevd for the type c_double.
 */
inline proc stevd(matrix_order : lapack_memory_order, jobz : string, n : c_int, d : [] c_double, e : [] c_double, z : [] c_double): c_int{
  return LAPACKE_dstevd(matrix_order, ascii(jobz) : c_char, n, d, e, z, (z.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sstevr for the type c_float.
 */
inline proc stevr(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_float, e : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] c_float, isuppz : [] c_int): c_int{
  return LAPACKE_sstevr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, isuppz);
}

/* 
Wrapped procedure of LAPACKE_dstevr for the type c_double.
 */
inline proc stevr(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_double, e : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] c_double, isuppz : [] c_int): c_int{
  return LAPACKE_dstevr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, isuppz);
}

/* 
Wrapped procedure of LAPACKE_sstevx for the type c_float.
 */
inline proc stevx(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_float, e : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] c_float, ifail : [] c_int): c_int{
  return LAPACKE_sstevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_dstevx for the type c_double.
 */
inline proc stevx(matrix_order : lapack_memory_order, jobz : string, range : string, n : c_int, d : [] c_double, e : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] c_double, ifail : [] c_int): c_int{
  return LAPACKE_dstevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, n, d, e, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_ssycon for the type c_float.
 */
inline proc sycon(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, ipiv : [] c_int, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_ssycon(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_dsycon for the type c_double.
 */
inline proc sycon(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, ipiv : [] c_int, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_dsycon(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_csycon for the type lapack_complex_float.
 */
inline proc sycon(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int, anorm : c_float, ref rcond : c_float): c_int{
  return LAPACKE_csycon(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_zsycon for the type lapack_complex_double.
 */
inline proc sycon(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int, anorm : c_double, ref rcond : c_double): c_int{
  return LAPACKE_zsycon(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, anorm, rcond);
}

/* 
Wrapped procedure of LAPACKE_ssyequb for the type c_float.
 */
inline proc syequb(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, s : [] c_float, ref scond : c_float, ref amax : c_float): c_int{
  return LAPACKE_ssyequb(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_dsyequb for the type c_double.
 */
inline proc syequb(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, s : [] c_double, ref scond : c_double, ref amax : c_double): c_int{
  return LAPACKE_dsyequb(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_csyequb for the type lapack_complex_float.
 */
inline proc syequb(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), s : [] c_float, ref scond : c_float, ref amax : c_float): c_int{
  return LAPACKE_csyequb(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_zsyequb for the type lapack_complex_double.
 */
inline proc syequb(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), s : [] c_double, ref scond : c_double, ref amax : c_double): c_int{
  return LAPACKE_zsyequb(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, s, scond, amax);
}

/* 
Wrapped procedure of LAPACKE_ssyev for the type c_float.
 */
inline proc syev(matrix_order : lapack_memory_order, jobz : string, uplo : string, a : [] c_float, w : [] c_float): c_int{
  return LAPACKE_ssyev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_dsyev for the type c_double.
 */
inline proc syev(matrix_order : lapack_memory_order, jobz : string, uplo : string, a : [] c_double, w : [] c_double): c_int{
  return LAPACKE_dsyev(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_ssyevd for the type c_float.
 */
inline proc syevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, a : [] c_float, w : [] c_float): c_int{
  return LAPACKE_ssyevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_dsyevd for the type c_double.
 */
inline proc syevd(matrix_order : lapack_memory_order, jobz : string, uplo : string, a : [] c_double, w : [] c_double): c_int{
  return LAPACKE_dsyevd(matrix_order, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_ssyevr for the type c_float.
 */
inline proc syevr(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, a : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] c_float, isuppz : [] c_int): c_int{
  return LAPACKE_ssyevr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, isuppz);
}

/* 
Wrapped procedure of LAPACKE_dsyevr for the type c_double.
 */
inline proc syevr(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, a : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] c_double, isuppz : [] c_int): c_int{
  return LAPACKE_dsyevr(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, isuppz);
}

/* 
Wrapped procedure of LAPACKE_ssyevx for the type c_float.
 */
inline proc syevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, a : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] c_float, ifail : [] c_int): c_int{
  return LAPACKE_ssyevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_dsyevx for the type c_double.
 */
inline proc syevx(matrix_order : lapack_memory_order, jobz : string, range : string, uplo : string, a : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] c_double, ifail : [] c_int): c_int{
  return LAPACKE_dsyevx(matrix_order, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_ssygst for the type c_float.
 */
inline proc sygst(matrix_order : lapack_memory_order, itype : c_int, uplo : string, a : [] c_float, b : [] c_float): c_int{
  return LAPACKE_ssygst(matrix_order, itype, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsygst for the type c_double.
 */
inline proc sygst(matrix_order : lapack_memory_order, itype : c_int, uplo : string, a : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dsygst(matrix_order, itype, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssygv for the type c_float.
 */
inline proc sygv(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, a : [] c_float, b : [] c_float, w : [] c_float): c_int{
  return LAPACKE_ssygv(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_dsygv for the type c_double.
 */
inline proc sygv(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, a : [] c_double, b : [] c_double, w : [] c_double): c_int{
  return LAPACKE_dsygv(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_ssygvd for the type c_float.
 */
inline proc sygvd(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, a : [] c_float, b : [] c_float, w : [] c_float): c_int{
  return LAPACKE_ssygvd(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_dsygvd for the type c_double.
 */
inline proc sygvd(matrix_order : lapack_memory_order, itype : c_int, jobz : string, uplo : string, a : [] c_double, b : [] c_double, w : [] c_double): c_int{
  return LAPACKE_dsygvd(matrix_order, itype, ascii(jobz) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, w);
}

/* 
Wrapped procedure of LAPACKE_ssygvx for the type c_float.
 */
inline proc sygvx(matrix_order : lapack_memory_order, itype : c_int, jobz : string, range : string, uplo : string, n : c_int, a : [] c_float, b : [] c_float, vl : c_float, vu : c_float, il : c_int, iu : c_int, abstol : c_float, ref m : c_int, w : [] c_float, z : [] c_float, ifail : [] c_int): c_int{
  return LAPACKE_ssygvx(matrix_order, itype, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_dsygvx for the type c_double.
 */
inline proc sygvx(matrix_order : lapack_memory_order, itype : c_int, jobz : string, range : string, uplo : string, n : c_int, a : [] c_double, b : [] c_double, vl : c_double, vu : c_double, il : c_int, iu : c_int, abstol : c_double, ref m : c_int, w : [] c_double, z : [] c_double, ifail : [] c_int): c_int{
  return LAPACKE_dsygvx(matrix_order, itype, ascii(jobz) : c_char, ascii(range) : c_char, ascii(uplo) : c_char, n, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, vl, vu, il, iu, abstol, m, w, z, (z.domain.dim(2).size) : c_int, ifail);
}

/* 
Wrapped procedure of LAPACKE_ssyrfs for the type c_float.
 */
inline proc syrfs(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, af : [] c_float, ipiv : [] c_int, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_ssyrfs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dsyrfs for the type c_double.
 */
inline proc syrfs(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, af : [] c_double, ipiv : [] c_int, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dsyrfs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_csyrfs for the type lapack_complex_float.
 */
inline proc syrfs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_csyrfs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zsyrfs for the type lapack_complex_double.
 */
inline proc syrfs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zsyrfs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_ssyrfsx for the type c_float.
 */
inline proc syrfsx(matrix_order : lapack_memory_order, uplo : string, equed : string, a : [] c_float, af : [] c_float, ipiv : [] c_int, s : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_ssyrfsx(matrix_order, ascii(uplo) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_dsyrfsx for the type c_double.
 */
inline proc syrfsx(matrix_order : lapack_memory_order, uplo : string, equed : string, a : [] c_double, af : [] c_double, ipiv : [] c_int, s : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_dsyrfsx(matrix_order, ascii(uplo) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_csyrfsx for the type lapack_complex_float.
 */
inline proc syrfsx(matrix_order : lapack_memory_order, uplo : string, equed : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, s : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_csyrfsx(matrix_order, ascii(uplo) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_zsyrfsx for the type lapack_complex_double.
 */
inline proc syrfsx(matrix_order : lapack_memory_order, uplo : string, equed : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, s : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_zsyrfsx(matrix_order, ascii(uplo) : c_char, ascii(equed) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_ssysv for the type c_float.
 */
inline proc sysv(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, ipiv : [] c_int, b : [] c_float): c_int{
  return LAPACKE_ssysv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsysv for the type c_double.
 */
inline proc sysv(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, ipiv : [] c_int, b : [] c_double): c_int{
  return LAPACKE_dsysv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_csysv for the type lapack_complex_float.
 */
inline proc sysv(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_csysv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zsysv for the type lapack_complex_double.
 */
inline proc sysv(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zsysv(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssysvx for the type c_float.
 */
inline proc sysvx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] c_float, af : [] c_float, ipiv : [] c_int, b : [] c_float, x : [] c_float, ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_ssysvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dsysvx for the type c_double.
 */
inline proc sysvx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] c_double, af : [] c_double, ipiv : [] c_int, b : [] c_double, x : [] c_double, ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dsysvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_csysvx for the type lapack_complex_float.
 */
inline proc sysvx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_csysvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_zsysvx for the type lapack_complex_double.
 */
inline proc sysvx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_zsysvx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_ssysvxx for the type c_float.
 */
inline proc sysvxx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] c_float, af : [] c_float, ipiv : [] c_int, ref equed : string, s : [] c_float, b : [] c_float, x : [] c_float, ref rcond : c_float, ref rpvgrw : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_ssysvxx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_dsysvxx for the type c_double.
 */
inline proc sysvxx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] c_double, af : [] c_double, ipiv : [] c_int, ref equed : string, s : [] c_double, b : [] c_double, x : [] c_double, ref rcond : c_double, ref rpvgrw : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_dsysvxx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_csysvxx for the type lapack_complex_float.
 */
inline proc sysvxx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(64), af : [] complex(64), ipiv : [] c_int, ref equed : string, s : [] c_float, b : [] complex(64), x : [] complex(64), ref rcond : c_float, ref rpvgrw : c_float, berr : [] c_float, n_err_bnds : c_int, err_bnds_norm : [] c_float, err_bnds_comp : [] c_float, nparams : c_int, params : [] c_float): c_int{
  return LAPACKE_csysvxx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_zsysvxx for the type lapack_complex_double.
 */
inline proc sysvxx(matrix_order : lapack_memory_order, fact : string, uplo : string, a : [] complex(128), af : [] complex(128), ipiv : [] c_int, ref equed : string, s : [] c_double, b : [] complex(128), x : [] complex(128), ref rcond : c_double, ref rpvgrw : c_double, berr : [] c_double, n_err_bnds : c_int, err_bnds_norm : [] c_double, err_bnds_comp : [] c_double, nparams : c_int, params : [] c_double): c_int{
  return LAPACKE_zsysvxx(matrix_order, ascii(fact) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, af, (af.domain.dim(2).size) : c_int, ipiv, ascii(equed) : c_char, s, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, rcond, rpvgrw, berr, n_err_bnds, err_bnds_norm, err_bnds_comp, nparams, params);
}

/* 
Wrapped procedure of LAPACKE_ssytrd for the type c_float.
 */
inline proc sytrd(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, d : [] c_float, e : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_ssytrd(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, d, e, tau);
}

/* 
Wrapped procedure of LAPACKE_dsytrd for the type c_double.
 */
inline proc sytrd(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, d : [] c_double, e : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dsytrd(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, d, e, tau);
}

/* 
Wrapped procedure of LAPACKE_ssytrf for the type c_float.
 */
inline proc sytrf(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, ipiv : [] c_int): c_int{
  return LAPACKE_ssytrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_dsytrf for the type c_double.
 */
inline proc sytrf(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, ipiv : [] c_int): c_int{
  return LAPACKE_dsytrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_csytrf for the type lapack_complex_float.
 */
inline proc sytrf(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_csytrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zsytrf for the type lapack_complex_double.
 */
inline proc sytrf(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zsytrf(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_ssytri for the type c_float.
 */
inline proc sytri(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, ipiv : [] c_int): c_int{
  return LAPACKE_ssytri(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_dsytri for the type c_double.
 */
inline proc sytri(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, ipiv : [] c_int): c_int{
  return LAPACKE_dsytri(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_csytri for the type lapack_complex_float.
 */
inline proc sytri(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_csytri(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zsytri for the type lapack_complex_double.
 */
inline proc sytri(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zsytri(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_ssytrs for the type c_float.
 */
inline proc sytrs(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, ipiv : [] c_int, b : [] c_float): c_int{
  return LAPACKE_ssytrs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsytrs for the type c_double.
 */
inline proc sytrs(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, ipiv : [] c_int, b : [] c_double): c_int{
  return LAPACKE_dsytrs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_csytrs for the type lapack_complex_float.
 */
inline proc sytrs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_csytrs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zsytrs for the type lapack_complex_double.
 */
inline proc sytrs(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zsytrs(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_stbcon for the type c_float.
 */
inline proc tbcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, n : c_int, kd : c_int, ab : [] c_float, ref rcond : c_float): c_int{
  return LAPACKE_stbcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, rcond);
}

/* 
Wrapped procedure of LAPACKE_dtbcon for the type c_double.
 */
inline proc tbcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, n : c_int, kd : c_int, ab : [] c_double, ref rcond : c_double): c_int{
  return LAPACKE_dtbcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, rcond);
}

/* 
Wrapped procedure of LAPACKE_ctbcon for the type lapack_complex_float.
 */
inline proc tbcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, n : c_int, kd : c_int, ab : [] complex(64), ref rcond : c_float): c_int{
  return LAPACKE_ctbcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, rcond);
}

/* 
Wrapped procedure of LAPACKE_ztbcon for the type lapack_complex_double.
 */
inline proc tbcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, n : c_int, kd : c_int, ab : [] complex(128), ref rcond : c_double): c_int{
  return LAPACKE_ztbcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, n, kd, ab, (ab.domain.dim(2).size) : c_int, rcond);
}

/* 
Wrapped procedure of LAPACKE_stbrfs for the type c_float.
 */
inline proc tbrfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, kd : c_int, ab : [] c_float, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_stbrfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dtbrfs for the type c_double.
 */
inline proc tbrfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, kd : c_int, ab : [] c_double, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dtbrfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_ctbrfs for the type lapack_complex_float.
 */
inline proc tbrfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, kd : c_int, ab : [] complex(64), b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_ctbrfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_ztbrfs for the type lapack_complex_double.
 */
inline proc tbrfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, kd : c_int, ab : [] complex(128), b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_ztbrfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_stbtrs for the type c_float.
 */
inline proc tbtrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, kd : c_int, ab : [] c_float, b : [] c_float): c_int{
  return LAPACKE_stbtrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dtbtrs for the type c_double.
 */
inline proc tbtrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, kd : c_int, ab : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dtbtrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ctbtrs for the type lapack_complex_float.
 */
inline proc tbtrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, kd : c_int, ab : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_ctbtrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ztbtrs for the type lapack_complex_double.
 */
inline proc tbtrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, kd : c_int, ab : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_ztbtrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, kd, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ab, (ab.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_stfsm for the type c_float.
 */
inline proc tfsm(matrix_order : lapack_memory_order, transr : string, side : string, uplo : string, trans : string, diag : string, alpha : c_float, a : [] c_float, b : [] c_float, ldb : c_int): c_int{
  return LAPACKE_stfsm(matrix_order, ascii(transr) : c_char, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, alpha, a, b, ldb);
}

/* 
Wrapped procedure of LAPACKE_dtfsm for the type c_double.
 */
inline proc tfsm(matrix_order : lapack_memory_order, transr : string, side : string, uplo : string, trans : string, diag : string, alpha : c_double, a : [] c_double, b : [] c_double, ldb : c_int): c_int{
  return LAPACKE_dtfsm(matrix_order, ascii(transr) : c_char, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, alpha, a, b, ldb);
}

/* 
Wrapped procedure of LAPACKE_ctfsm for the type lapack_complex_float.
 */
inline proc tfsm(matrix_order : lapack_memory_order, transr : string, side : string, uplo : string, trans : string, diag : string, alpha : complex(64), a : [] complex(64), b : [] complex(64), ldb : c_int): c_int{
  return LAPACKE_ctfsm(matrix_order, ascii(transr) : c_char, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, alpha, a, b, ldb);
}

/* 
Wrapped procedure of LAPACKE_ztfsm for the type lapack_complex_double.
 */
inline proc tfsm(matrix_order : lapack_memory_order, transr : string, side : string, uplo : string, trans : string, diag : string, alpha : complex(128), a : [] complex(128), b : [] complex(128), ldb : c_int): c_int{
  return LAPACKE_ztfsm(matrix_order, ascii(transr) : c_char, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, alpha, a, b, ldb);
}

/* 
Wrapped procedure of LAPACKE_stftri for the type c_float.
 */
inline proc tftri(matrix_order : lapack_memory_order, transr : string, uplo : string, diag : string, a : [] c_float): c_int{
  return LAPACKE_stftri(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_dtftri for the type c_double.
 */
inline proc tftri(matrix_order : lapack_memory_order, transr : string, uplo : string, diag : string, a : [] c_double): c_int{
  return LAPACKE_dtftri(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_ctftri for the type lapack_complex_float.
 */
inline proc tftri(matrix_order : lapack_memory_order, transr : string, uplo : string, diag : string, a : [] complex(64)): c_int{
  return LAPACKE_ctftri(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_ztftri for the type lapack_complex_double.
 */
inline proc tftri(matrix_order : lapack_memory_order, transr : string, uplo : string, diag : string, a : [] complex(128)): c_int{
  return LAPACKE_ztftri(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a);
}

/* 
Wrapped procedure of LAPACKE_stfttp for the type c_float.
 */
inline proc tfttp(matrix_order : lapack_memory_order, transr : string, uplo : string, n : c_int, arf : [] c_float, ap : [] c_float): c_int{
  return LAPACKE_stfttp(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, n, arf, ap);
}

/* 
Wrapped procedure of LAPACKE_dtfttp for the type c_double.
 */
inline proc tfttp(matrix_order : lapack_memory_order, transr : string, uplo : string, n : c_int, arf : [] c_double, ap : [] c_double): c_int{
  return LAPACKE_dtfttp(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, n, arf, ap);
}

/* 
Wrapped procedure of LAPACKE_ctfttp for the type lapack_complex_float.
 */
inline proc tfttp(matrix_order : lapack_memory_order, transr : string, uplo : string, n : c_int, arf : [] complex(64), ap : [] complex(64)): c_int{
  return LAPACKE_ctfttp(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, n, arf, ap);
}

/* 
Wrapped procedure of LAPACKE_ztfttp for the type lapack_complex_double.
 */
inline proc tfttp(matrix_order : lapack_memory_order, transr : string, uplo : string, n : c_int, arf : [] complex(128), ap : [] complex(128)): c_int{
  return LAPACKE_ztfttp(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, n, arf, ap);
}

/* 
Wrapped procedure of LAPACKE_stfttr for the type c_float.
 */
inline proc tfttr(matrix_order : lapack_memory_order, transr : string, uplo : string, arf : [] c_float, a : [] c_float, lda : c_int): c_int{
  return LAPACKE_stfttr(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (arf.domain.dim(1).size) : c_int, arf, a, lda);
}

/* 
Wrapped procedure of LAPACKE_dtfttr for the type c_double.
 */
inline proc tfttr(matrix_order : lapack_memory_order, transr : string, uplo : string, arf : [] c_double, a : [] c_double, lda : c_int): c_int{
  return LAPACKE_dtfttr(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (arf.domain.dim(1).size) : c_int, arf, a, lda);
}

/* 
Wrapped procedure of LAPACKE_ctfttr for the type lapack_complex_float.
 */
inline proc tfttr(matrix_order : lapack_memory_order, transr : string, uplo : string, arf : [] complex(64), a : [] complex(64), lda : c_int): c_int{
  return LAPACKE_ctfttr(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, arf, a, lda);
}

/* 
Wrapped procedure of LAPACKE_ztfttr for the type lapack_complex_double.
 */
inline proc tfttr(matrix_order : lapack_memory_order, transr : string, uplo : string, arf : [] complex(128), a : [] complex(128), lda : c_int): c_int{
  return LAPACKE_ztfttr(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, arf, a, lda);
}

/* 
Wrapped procedure of LAPACKE_stgevc for the type c_float.
 */
inline proc tgevc(matrix_order : lapack_memory_order, side : string, howmny : string, chlapack_select : [] c_int, s : [] c_float, p : [] c_float, vl : [] c_float, vr : [] c_float, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_stgevc(matrix_order, ascii(side) : c_char, ascii(howmny) : c_char, chlapack_select, (s.domain.dim(1).size) : c_int, s, (s.domain.dim(2).size) : c_int, p, (p.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m);
}

/* 
Wrapped procedure of LAPACKE_dtgevc for the type c_double.
 */
inline proc tgevc(matrix_order : lapack_memory_order, side : string, howmny : string, chlapack_select : [] c_int, s : [] c_double, p : [] c_double, vl : [] c_double, vr : [] c_double, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_dtgevc(matrix_order, ascii(side) : c_char, ascii(howmny) : c_char, chlapack_select, (s.domain.dim(1).size) : c_int, s, (s.domain.dim(2).size) : c_int, p, (p.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m);
}

/* 
Wrapped procedure of LAPACKE_ctgevc for the type lapack_complex_float.
 */
inline proc tgevc(matrix_order : lapack_memory_order, side : string, howmny : string, chlapack_select : [] c_int, s : [] complex(64), p : [] complex(64), vl : [] complex(64), vr : [] complex(64), mm : c_int, ref m : c_int): c_int{
  return LAPACKE_ctgevc(matrix_order, ascii(side) : c_char, ascii(howmny) : c_char, chlapack_select, (s.domain.dim(1).size) : c_int, s, (s.domain.dim(2).size) : c_int, p, (p.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m);
}

/* 
Wrapped procedure of LAPACKE_ztgevc for the type lapack_complex_double.
 */
inline proc tgevc(matrix_order : lapack_memory_order, side : string, howmny : string, chlapack_select : [] c_int, s : [] complex(128), p : [] complex(128), vl : [] complex(128), vr : [] complex(128), mm : c_int, ref m : c_int): c_int{
  return LAPACKE_ztgevc(matrix_order, ascii(side) : c_char, ascii(howmny) : c_char, chlapack_select, (s.domain.dim(1).size) : c_int, s, (s.domain.dim(2).size) : c_int, p, (p.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m);
}

/* 
Wrapped procedure of LAPACKE_stgexc for the type c_float.
 */
inline proc tgexc(matrix_order : lapack_memory_order, wantq : c_int, wantz : c_int, a : [] c_float, b : [] c_float, q : [] c_float, z : [] c_float, ref ifst : c_int, ref ilst : c_int): c_int{
  return LAPACKE_stgexc(matrix_order, wantq, wantz, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int, ifst, ilst);
}

/* 
Wrapped procedure of LAPACKE_dtgexc for the type c_double.
 */
inline proc tgexc(matrix_order : lapack_memory_order, wantq : c_int, wantz : c_int, a : [] c_double, b : [] c_double, q : [] c_double, z : [] c_double, ref ifst : c_int, ref ilst : c_int): c_int{
  return LAPACKE_dtgexc(matrix_order, wantq, wantz, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int, ifst, ilst);
}

/* 
Wrapped procedure of LAPACKE_ctgexc for the type lapack_complex_float.
 */
inline proc tgexc(matrix_order : lapack_memory_order, wantq : c_int, wantz : c_int, a : [] complex(64), b : [] complex(64), q : [] complex(64), z : [] complex(64), ifst : c_int, ilst : c_int): c_int{
  return LAPACKE_ctgexc(matrix_order, wantq, wantz, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int, ifst, ilst);
}

/* 
Wrapped procedure of LAPACKE_ztgexc for the type lapack_complex_double.
 */
inline proc tgexc(matrix_order : lapack_memory_order, wantq : c_int, wantz : c_int, a : [] complex(128), b : [] complex(128), q : [] complex(128), z : [] complex(128), ifst : c_int, ilst : c_int): c_int{
  return LAPACKE_ztgexc(matrix_order, wantq, wantz, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int, ifst, ilst);
}

/* 
Wrapped procedure of LAPACKE_stgsen for the type c_float.
 */
inline proc tgsen(matrix_order : lapack_memory_order, ijob : c_int, wantq : c_int, wantz : c_int, chlapack_select : [] c_int, a : [] c_float, b : [] c_float, alphar : [] c_float, alphai : [] c_float, beta : [] c_float, q : [] c_float, z : [] c_float, ref m : c_int, ref pl : c_float, ref pr : c_float, dif : [] c_float): c_int{
  return LAPACKE_stgsen(matrix_order, ijob, wantq, wantz, chlapack_select, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alphar, alphai, beta, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int, m, pl, pr, dif);
}

/* 
Wrapped procedure of LAPACKE_dtgsen for the type c_double.
 */
inline proc tgsen(matrix_order : lapack_memory_order, ijob : c_int, wantq : c_int, wantz : c_int, chlapack_select : [] c_int, a : [] c_double, b : [] c_double, alphar : [] c_double, alphai : [] c_double, beta : [] c_double, q : [] c_double, z : [] c_double, ref m : c_int, ref pl : c_double, ref pr : c_double, dif : [] c_double): c_int{
  return LAPACKE_dtgsen(matrix_order, ijob, wantq, wantz, chlapack_select, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alphar, alphai, beta, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int, m, pl, pr, dif);
}

/* 
Wrapped procedure of LAPACKE_ctgsen for the type lapack_complex_float.
 */
inline proc tgsen(matrix_order : lapack_memory_order, ijob : c_int, wantq : c_int, wantz : c_int, chlapack_select : [] c_int, a : [] complex(64), b : [] complex(64), alpha : [] complex(64), beta : [] complex(64), q : [] complex(64), z : [] complex(64), ref m : c_int, ref pl : c_float, ref pr : c_float, dif : [] c_float): c_int{
  return LAPACKE_ctgsen(matrix_order, ijob, wantq, wantz, chlapack_select, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alpha, beta, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int, m, pl, pr, dif);
}

/* 
Wrapped procedure of LAPACKE_ztgsen for the type lapack_complex_double.
 */
inline proc tgsen(matrix_order : lapack_memory_order, ijob : c_int, wantq : c_int, wantz : c_int, chlapack_select : [] c_int, a : [] complex(128), b : [] complex(128), alpha : [] complex(128), beta : [] complex(128), q : [] complex(128), z : [] complex(128), ref m : c_int, ref pl : c_double, ref pr : c_double, dif : [] c_double): c_int{
  return LAPACKE_ztgsen(matrix_order, ijob, wantq, wantz, chlapack_select, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, alpha, beta, q, (q.domain.dim(2).size) : c_int, z, (z.domain.dim(2).size) : c_int, m, pl, pr, dif);
}

/* 
Wrapped procedure of LAPACKE_stgsja for the type c_float.
 */
inline proc tgsja(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, k : c_int, l : c_int, a : [] c_float, b : [] c_float, tola : c_float, tolb : c_float, alpha : [] c_float, beta : [] c_float, u : [] c_float, v : [] c_float, q : [] c_float, ref ncycle : c_int): c_int{
  return LAPACKE_stgsja(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, tola, tolb, alpha, beta, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, ncycle);
}

/* 
Wrapped procedure of LAPACKE_dtgsja for the type c_double.
 */
inline proc tgsja(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, k : c_int, l : c_int, a : [] c_double, b : [] c_double, tola : c_double, tolb : c_double, alpha : [] c_double, beta : [] c_double, u : [] c_double, v : [] c_double, q : [] c_double, ref ncycle : c_int): c_int{
  return LAPACKE_dtgsja(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, tola, tolb, alpha, beta, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, ncycle);
}

/* 
Wrapped procedure of LAPACKE_ctgsja for the type lapack_complex_float.
 */
inline proc tgsja(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, k : c_int, l : c_int, a : [] complex(64), b : [] complex(64), tola : c_float, tolb : c_float, alpha : [] c_float, beta : [] c_float, u : [] complex(64), v : [] complex(64), q : [] complex(64), ref ncycle : c_int): c_int{
  return LAPACKE_ctgsja(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, tola, tolb, alpha, beta, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, ncycle);
}

/* 
Wrapped procedure of LAPACKE_ztgsja for the type lapack_complex_double.
 */
inline proc tgsja(matrix_order : lapack_memory_order, jobu : string, jobv : string, jobq : string, k : c_int, l : c_int, a : [] complex(128), b : [] complex(128), tola : c_double, tolb : c_double, alpha : [] c_double, beta : [] c_double, u : [] complex(128), v : [] complex(128), q : [] complex(128), ref ncycle : c_int): c_int{
  return LAPACKE_ztgsja(matrix_order, ascii(jobu) : c_char, ascii(jobv) : c_char, ascii(jobq) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, tola, tolb, alpha, beta, u, (u.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, ncycle);
}

/* 
Wrapped procedure of LAPACKE_stgsna for the type c_float.
 */
inline proc tgsna(matrix_order : lapack_memory_order, job : string, howmny : string, chlapack_select : [] c_int, n : c_int, a : [] c_float, b : [] c_float, vl : [] c_float, vr : [] c_float, s : [] c_float, dif : [] c_float, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_stgsna(matrix_order, ascii(job) : c_char, ascii(howmny) : c_char, chlapack_select, n, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, s, dif, mm, m);
}

/* 
Wrapped procedure of LAPACKE_dtgsna for the type c_double.
 */
inline proc tgsna(matrix_order : lapack_memory_order, job : string, howmny : string, chlapack_select : [] c_int, n : c_int, a : [] c_double, b : [] c_double, vl : [] c_double, vr : [] c_double, s : [] c_double, dif : [] c_double, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_dtgsna(matrix_order, ascii(job) : c_char, ascii(howmny) : c_char, chlapack_select, n, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, s, dif, mm, m);
}

/* 
Wrapped procedure of LAPACKE_ctgsna for the type lapack_complex_float.
 */
inline proc tgsna(matrix_order : lapack_memory_order, job : string, howmny : string, chlapack_select : [] c_int, n : c_int, a : [] complex(64), b : [] complex(64), vl : [] complex(64), vr : [] complex(64), s : [] c_float, dif : [] c_float, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_ctgsna(matrix_order, ascii(job) : c_char, ascii(howmny) : c_char, chlapack_select, n, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, s, dif, mm, m);
}

/* 
Wrapped procedure of LAPACKE_ztgsna for the type lapack_complex_double.
 */
inline proc tgsna(matrix_order : lapack_memory_order, job : string, howmny : string, chlapack_select : [] c_int, n : c_int, a : [] complex(128), b : [] complex(128), vl : [] complex(128), vr : [] complex(128), s : [] c_double, dif : [] c_double, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_ztgsna(matrix_order, ascii(job) : c_char, ascii(howmny) : c_char, chlapack_select, n, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, s, dif, mm, m);
}

/* 
Wrapped procedure of LAPACKE_stgsyl for the type c_float.
 */
inline proc tgsyl(matrix_order : lapack_memory_order, trans : string, ijob : c_int, a : [] c_float, b : [] c_float, c : [] c_float, d : [] c_float, e : [] c_float, f : [] c_float, ref scale : c_float, ref dif : c_float): c_int{
  return LAPACKE_stgsyl(matrix_order, ascii(trans) : c_char, ijob, (a.domain.dim(1).size) : c_int, (b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int, d, (d.domain.dim(2).size) : c_int, e, (e.domain.dim(2).size) : c_int, f, (f.domain.dim(2).size) : c_int, scale, dif);
}

/* 
Wrapped procedure of LAPACKE_dtgsyl for the type c_double.
 */
inline proc tgsyl(matrix_order : lapack_memory_order, trans : string, ijob : c_int, a : [] c_double, b : [] c_double, c : [] c_double, d : [] c_double, e : [] c_double, f : [] c_double, ref scale : c_double, ref dif : c_double): c_int{
  return LAPACKE_dtgsyl(matrix_order, ascii(trans) : c_char, ijob, (a.domain.dim(1).size) : c_int, (b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int, d, (d.domain.dim(2).size) : c_int, e, (e.domain.dim(2).size) : c_int, f, (f.domain.dim(2).size) : c_int, scale, dif);
}

/* 
Wrapped procedure of LAPACKE_ctgsyl for the type lapack_complex_float.
 */
inline proc tgsyl(matrix_order : lapack_memory_order, trans : string, ijob : c_int, a : [] complex(64), b : [] complex(64), c : [] complex(64), d : [] complex(64), e : [] complex(64), f : [] complex(64), ref scale : c_float, ref dif : c_float): c_int{
  return LAPACKE_ctgsyl(matrix_order, ascii(trans) : c_char, ijob, (a.domain.dim(1).size) : c_int, (b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int, d, (d.domain.dim(2).size) : c_int, e, (e.domain.dim(2).size) : c_int, f, (f.domain.dim(2).size) : c_int, scale, dif);
}

/* 
Wrapped procedure of LAPACKE_ztgsyl for the type lapack_complex_double.
 */
inline proc tgsyl(matrix_order : lapack_memory_order, trans : string, ijob : c_int, a : [] complex(128), b : [] complex(128), c : [] complex(128), d : [] complex(128), e : [] complex(128), f : [] complex(128), ref scale : c_double, ref dif : c_double): c_int{
  return LAPACKE_ztgsyl(matrix_order, ascii(trans) : c_char, ijob, (a.domain.dim(1).size) : c_int, (b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int, d, (d.domain.dim(2).size) : c_int, e, (e.domain.dim(2).size) : c_int, f, (f.domain.dim(2).size) : c_int, scale, dif);
}

/* 
Wrapped procedure of LAPACKE_stpcon for the type c_float.
 */
inline proc tpcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, n : c_int, ap : [] c_float, ref rcond : c_float): c_int{
  return LAPACKE_stpcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, n, ap, rcond);
}

/* 
Wrapped procedure of LAPACKE_dtpcon for the type c_double.
 */
inline proc tpcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, n : c_int, ap : [] c_double, ref rcond : c_double): c_int{
  return LAPACKE_dtpcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, n, ap, rcond);
}

/* 
Wrapped procedure of LAPACKE_ctpcon for the type lapack_complex_float.
 */
inline proc tpcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, n : c_int, ap : [] complex(64), ref rcond : c_float): c_int{
  return LAPACKE_ctpcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, n, ap, rcond);
}

/* 
Wrapped procedure of LAPACKE_ztpcon for the type lapack_complex_double.
 */
inline proc tpcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, n : c_int, ap : [] complex(128), ref rcond : c_double): c_int{
  return LAPACKE_ztpcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, n, ap, rcond);
}

/* 
Wrapped procedure of LAPACKE_stprfs for the type c_float.
 */
inline proc tprfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, ap : [] c_float, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_stprfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dtprfs for the type c_double.
 */
inline proc tprfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, ap : [] c_double, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dtprfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_ctprfs for the type lapack_complex_float.
 */
inline proc tprfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, ap : [] complex(64), b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_ctprfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_ztprfs for the type lapack_complex_double.
 */
inline proc tprfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, ap : [] complex(128), b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_ztprfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_stptri for the type c_float.
 */
inline proc tptri(matrix_order : lapack_memory_order, uplo : string, diag : string, n : c_int, ap : [] c_float): c_int{
  return LAPACKE_stptri(matrix_order, ascii(uplo) : c_char, ascii(diag) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_dtptri for the type c_double.
 */
inline proc tptri(matrix_order : lapack_memory_order, uplo : string, diag : string, n : c_int, ap : [] c_double): c_int{
  return LAPACKE_dtptri(matrix_order, ascii(uplo) : c_char, ascii(diag) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_ctptri for the type lapack_complex_float.
 */
inline proc tptri(matrix_order : lapack_memory_order, uplo : string, diag : string, n : c_int, ap : [] complex(64)): c_int{
  return LAPACKE_ctptri(matrix_order, ascii(uplo) : c_char, ascii(diag) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_ztptri for the type lapack_complex_double.
 */
inline proc tptri(matrix_order : lapack_memory_order, uplo : string, diag : string, n : c_int, ap : [] complex(128)): c_int{
  return LAPACKE_ztptri(matrix_order, ascii(uplo) : c_char, ascii(diag) : c_char, n, ap);
}

/* 
Wrapped procedure of LAPACKE_stptrs for the type c_float.
 */
inline proc tptrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, ap : [] c_float, b : [] c_float): c_int{
  return LAPACKE_stptrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dtptrs for the type c_double.
 */
inline proc tptrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, ap : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dtptrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ctptrs for the type lapack_complex_float.
 */
inline proc tptrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, ap : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_ctptrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ztptrs for the type lapack_complex_double.
 */
inline proc tptrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, n : c_int, ap : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_ztptrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, n, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, ap, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_stpttf for the type c_float.
 */
inline proc tpttf(matrix_order : lapack_memory_order, transr : string, uplo : string, n : c_int, ap : [] c_float, arf : [] c_float): c_int{
  return LAPACKE_stpttf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, n, ap, arf);
}

/* 
Wrapped procedure of LAPACKE_dtpttf for the type c_double.
 */
inline proc tpttf(matrix_order : lapack_memory_order, transr : string, uplo : string, n : c_int, ap : [] c_double, arf : [] c_double): c_int{
  return LAPACKE_dtpttf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, n, ap, arf);
}

/* 
Wrapped procedure of LAPACKE_ctpttf for the type lapack_complex_float.
 */
inline proc tpttf(matrix_order : lapack_memory_order, transr : string, uplo : string, n : c_int, ap : [] complex(64), arf : [] complex(64)): c_int{
  return LAPACKE_ctpttf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, n, ap, arf);
}

/* 
Wrapped procedure of LAPACKE_ztpttf for the type lapack_complex_double.
 */
inline proc tpttf(matrix_order : lapack_memory_order, transr : string, uplo : string, n : c_int, ap : [] complex(128), arf : [] complex(128)): c_int{
  return LAPACKE_ztpttf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, n, ap, arf);
}

/* 
Wrapped procedure of LAPACKE_stpttr for the type c_float.
 */
inline proc tpttr(matrix_order : lapack_memory_order, uplo : string, ap : [] c_float, a : [] c_float): c_int{
  return LAPACKE_stpttr(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, ap, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dtpttr for the type c_double.
 */
inline proc tpttr(matrix_order : lapack_memory_order, uplo : string, ap : [] c_double, a : [] c_double): c_int{
  return LAPACKE_dtpttr(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, ap, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ctpttr for the type lapack_complex_float.
 */
inline proc tpttr(matrix_order : lapack_memory_order, uplo : string, ap : [] complex(64), a : [] complex(64)): c_int{
  return LAPACKE_ctpttr(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, ap, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ztpttr for the type lapack_complex_double.
 */
inline proc tpttr(matrix_order : lapack_memory_order, uplo : string, ap : [] complex(128), a : [] complex(128)): c_int{
  return LAPACKE_ztpttr(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, ap, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_strcon for the type c_float.
 */
inline proc trcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, a : [] c_float, ref rcond : c_float): c_int{
  return LAPACKE_strcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, rcond);
}

/* 
Wrapped procedure of LAPACKE_dtrcon for the type c_double.
 */
inline proc trcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, a : [] c_double, ref rcond : c_double): c_int{
  return LAPACKE_dtrcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, rcond);
}

/* 
Wrapped procedure of LAPACKE_ctrcon for the type lapack_complex_float.
 */
inline proc trcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, a : [] complex(64), ref rcond : c_float): c_int{
  return LAPACKE_ctrcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, rcond);
}

/* 
Wrapped procedure of LAPACKE_ztrcon for the type lapack_complex_double.
 */
inline proc trcon(matrix_order : lapack_memory_order, norm : string, uplo : string, diag : string, a : [] complex(128), ref rcond : c_double): c_int{
  return LAPACKE_ztrcon(matrix_order, ascii(norm) : c_char, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, rcond);
}

/* 
Wrapped procedure of LAPACKE_strevc for the type c_float.
 */
inline proc trevc(matrix_order : lapack_memory_order, side : string, howmny : string, chlapack_select : [] c_int, t : [] c_float, vl : [] c_float, vr : [] c_float, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_strevc(matrix_order, ascii(side) : c_char, ascii(howmny) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m);
}

/* 
Wrapped procedure of LAPACKE_dtrevc for the type c_double.
 */
inline proc trevc(matrix_order : lapack_memory_order, side : string, howmny : string, chlapack_select : [] c_int, t : [] c_double, vl : [] c_double, vr : [] c_double, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_dtrevc(matrix_order, ascii(side) : c_char, ascii(howmny) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m);
}

/* 
Wrapped procedure of LAPACKE_ctrevc for the type lapack_complex_float.
 */
inline proc trevc(matrix_order : lapack_memory_order, side : string, howmny : string, chlapack_select : [] c_int, t : [] complex(64), vl : [] complex(64), vr : [] complex(64), mm : c_int, ref m : c_int): c_int{
  return LAPACKE_ctrevc(matrix_order, ascii(side) : c_char, ascii(howmny) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m);
}

/* 
Wrapped procedure of LAPACKE_ztrevc for the type lapack_complex_double.
 */
inline proc trevc(matrix_order : lapack_memory_order, side : string, howmny : string, chlapack_select : [] c_int, t : [] complex(128), vl : [] complex(128), vr : [] complex(128), mm : c_int, ref m : c_int): c_int{
  return LAPACKE_ztrevc(matrix_order, ascii(side) : c_char, ascii(howmny) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, mm, m);
}

/* 
Wrapped procedure of LAPACKE_strexc for the type c_float.
 */
inline proc trexc(matrix_order : lapack_memory_order, compq : string, t : [] c_float, q : [] c_float, ref ifst : c_int, ref ilst : c_int): c_int{
  return LAPACKE_strexc(matrix_order, ascii(compq) : c_char, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, ifst, ilst);
}

/* 
Wrapped procedure of LAPACKE_dtrexc for the type c_double.
 */
inline proc trexc(matrix_order : lapack_memory_order, compq : string, t : [] c_double, q : [] c_double, ref ifst : c_int, ref ilst : c_int): c_int{
  return LAPACKE_dtrexc(matrix_order, ascii(compq) : c_char, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, ifst, ilst);
}

/* 
Wrapped procedure of LAPACKE_ctrexc for the type lapack_complex_float.
 */
inline proc trexc(matrix_order : lapack_memory_order, compq : string, t : [] complex(64), q : [] complex(64), ifst : c_int, ilst : c_int): c_int{
  return LAPACKE_ctrexc(matrix_order, ascii(compq) : c_char, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, ifst, ilst);
}

/* 
Wrapped procedure of LAPACKE_ztrexc for the type lapack_complex_double.
 */
inline proc trexc(matrix_order : lapack_memory_order, compq : string, t : [] complex(128), q : [] complex(128), ifst : c_int, ilst : c_int): c_int{
  return LAPACKE_ztrexc(matrix_order, ascii(compq) : c_char, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, ifst, ilst);
}

/* 
Wrapped procedure of LAPACKE_strrfs for the type c_float.
 */
inline proc trrfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, a : [] c_float, b : [] c_float, x : [] c_float, ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_strrfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_dtrrfs for the type c_double.
 */
inline proc trrfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, a : [] c_double, b : [] c_double, x : [] c_double, ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_dtrrfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_ctrrfs for the type lapack_complex_float.
 */
inline proc trrfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, a : [] complex(64), b : [] complex(64), x : [] complex(64), ferr : [] c_float, berr : [] c_float): c_int{
  return LAPACKE_ctrrfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_ztrrfs for the type lapack_complex_double.
 */
inline proc trrfs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, a : [] complex(128), b : [] complex(128), x : [] complex(128), ferr : [] c_double, berr : [] c_double): c_int{
  return LAPACKE_ztrrfs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, x, (x.domain.dim(2).size) : c_int, ferr, berr);
}

/* 
Wrapped procedure of LAPACKE_strsen for the type c_float.
 */
inline proc trsen(matrix_order : lapack_memory_order, job : string, compq : string, chlapack_select : [] c_int, t : [] c_float, q : [] c_float, wr : [] c_float, wi : [] c_float, ref m : c_int, ref s : c_float, ref sep : c_float): c_int{
  return LAPACKE_strsen(matrix_order, ascii(job) : c_char, ascii(compq) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, wr, wi, m, s, sep);
}

/* 
Wrapped procedure of LAPACKE_dtrsen for the type c_double.
 */
inline proc trsen(matrix_order : lapack_memory_order, job : string, compq : string, chlapack_select : [] c_int, t : [] c_double, q : [] c_double, wr : [] c_double, wi : [] c_double, ref m : c_int, ref s : c_double, ref sep : c_double): c_int{
  return LAPACKE_dtrsen(matrix_order, ascii(job) : c_char, ascii(compq) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, wr, wi, m, s, sep);
}

/* 
Wrapped procedure of LAPACKE_ctrsen for the type lapack_complex_float.
 */
inline proc trsen(matrix_order : lapack_memory_order, job : string, compq : string, chlapack_select : [] c_int, t : [] complex(64), q : [] complex(64), w : [] complex(64), ref m : c_int, ref s : c_float, ref sep : c_float): c_int{
  return LAPACKE_ctrsen(matrix_order, ascii(job) : c_char, ascii(compq) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, w, m, s, sep);
}

/* 
Wrapped procedure of LAPACKE_ztrsen for the type lapack_complex_double.
 */
inline proc trsen(matrix_order : lapack_memory_order, job : string, compq : string, chlapack_select : [] c_int, t : [] complex(128), q : [] complex(128), w : [] complex(128), ref m : c_int, ref s : c_double, ref sep : c_double): c_int{
  return LAPACKE_ztrsen(matrix_order, ascii(job) : c_char, ascii(compq) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, q, (q.domain.dim(2).size) : c_int, w, m, s, sep);
}

/* 
Wrapped procedure of LAPACKE_strsna for the type c_float.
 */
inline proc trsna(matrix_order : lapack_memory_order, job : string, howmny : string, chlapack_select : [] c_int, t : [] c_float, vl : [] c_float, vr : [] c_float, s : [] c_float, sep : [] c_float, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_strsna(matrix_order, ascii(job) : c_char, ascii(howmny) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, s, sep, mm, m);
}

/* 
Wrapped procedure of LAPACKE_dtrsna for the type c_double.
 */
inline proc trsna(matrix_order : lapack_memory_order, job : string, howmny : string, chlapack_select : [] c_int, t : [] c_double, vl : [] c_double, vr : [] c_double, s : [] c_double, sep : [] c_double, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_dtrsna(matrix_order, ascii(job) : c_char, ascii(howmny) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, s, sep, mm, m);
}

/* 
Wrapped procedure of LAPACKE_ctrsna for the type lapack_complex_float.
 */
inline proc trsna(matrix_order : lapack_memory_order, job : string, howmny : string, chlapack_select : [] c_int, t : [] complex(64), vl : [] complex(64), vr : [] complex(64), s : [] c_float, sep : [] c_float, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_ctrsna(matrix_order, ascii(job) : c_char, ascii(howmny) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, s, sep, mm, m);
}

/* 
Wrapped procedure of LAPACKE_ztrsna for the type lapack_complex_double.
 */
inline proc trsna(matrix_order : lapack_memory_order, job : string, howmny : string, chlapack_select : [] c_int, t : [] complex(128), vl : [] complex(128), vr : [] complex(128), s : [] c_double, sep : [] c_double, mm : c_int, ref m : c_int): c_int{
  return LAPACKE_ztrsna(matrix_order, ascii(job) : c_char, ascii(howmny) : c_char, chlapack_select, (t.domain.dim(1).size) : c_int, t, (t.domain.dim(2).size) : c_int, vl, (vl.domain.dim(2).size) : c_int, vr, (vr.domain.dim(2).size) : c_int, s, sep, mm, m);
}

/* 
Wrapped procedure of LAPACKE_strsyl for the type c_float.
 */
inline proc trsyl(matrix_order : lapack_memory_order, trana : string, tranb : string, isgn : c_int, a : [] c_float, b : [] c_float, c : [] c_float, ref scale : c_float): c_int{
  return LAPACKE_strsyl(matrix_order, ascii(trana) : c_char, ascii(tranb) : c_char, isgn, (a.domain.dim(1).size) : c_int, (b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int, scale);
}

/* 
Wrapped procedure of LAPACKE_dtrsyl for the type c_double.
 */
inline proc trsyl(matrix_order : lapack_memory_order, trana : string, tranb : string, isgn : c_int, a : [] c_double, b : [] c_double, c : [] c_double, ref scale : c_double): c_int{
  return LAPACKE_dtrsyl(matrix_order, ascii(trana) : c_char, ascii(tranb) : c_char, isgn, (a.domain.dim(1).size) : c_int, (b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int, scale);
}

/* 
Wrapped procedure of LAPACKE_ctrsyl for the type lapack_complex_float.
 */
inline proc trsyl(matrix_order : lapack_memory_order, trana : string, tranb : string, isgn : c_int, a : [] complex(64), b : [] complex(64), c : [] complex(64), ref scale : c_float): c_int{
  return LAPACKE_ctrsyl(matrix_order, ascii(trana) : c_char, ascii(tranb) : c_char, isgn, (a.domain.dim(1).size) : c_int, (b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int, scale);
}

/* 
Wrapped procedure of LAPACKE_ztrsyl for the type lapack_complex_double.
 */
inline proc trsyl(matrix_order : lapack_memory_order, trana : string, tranb : string, isgn : c_int, a : [] complex(128), b : [] complex(128), c : [] complex(128), ref scale : c_double): c_int{
  return LAPACKE_ztrsyl(matrix_order, ascii(trana) : c_char, ascii(tranb) : c_char, isgn, (a.domain.dim(1).size) : c_int, (b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int, scale);
}

/* 
Wrapped procedure of LAPACKE_strtri for the type c_float.
 */
inline proc trtri(matrix_order : lapack_memory_order, uplo : string, diag : string, a : [] c_float): c_int{
  return LAPACKE_strtri(matrix_order, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dtrtri for the type c_double.
 */
inline proc trtri(matrix_order : lapack_memory_order, uplo : string, diag : string, a : [] c_double): c_int{
  return LAPACKE_dtrtri(matrix_order, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ctrtri for the type lapack_complex_float.
 */
inline proc trtri(matrix_order : lapack_memory_order, uplo : string, diag : string, a : [] complex(64)): c_int{
  return LAPACKE_ctrtri(matrix_order, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ztrtri for the type lapack_complex_double.
 */
inline proc trtri(matrix_order : lapack_memory_order, uplo : string, diag : string, a : [] complex(128)): c_int{
  return LAPACKE_ztrtri(matrix_order, ascii(uplo) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_strtrs for the type c_float.
 */
inline proc trtrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, a : [] c_float, b : [] c_float): c_int{
  return LAPACKE_strtrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dtrtrs for the type c_double.
 */
inline proc trtrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, a : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dtrtrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ctrtrs for the type lapack_complex_float.
 */
inline proc trtrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, a : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_ctrtrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ztrtrs for the type lapack_complex_double.
 */
inline proc trtrs(matrix_order : lapack_memory_order, uplo : string, trans : string, diag : string, a : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_ztrtrs(matrix_order, ascii(uplo) : c_char, ascii(trans) : c_char, ascii(diag) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_strttf for the type c_float.
 */
inline proc trttf(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] c_float, lda : c_int, arf : [] c_float): c_int{
  return LAPACKE_strttf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, lda, arf);
}

/* 
Wrapped procedure of LAPACKE_dtrttf for the type c_double.
 */
inline proc trttf(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] c_double, lda : c_int, arf : [] c_double): c_int{
  return LAPACKE_dtrttf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, lda, arf);
}

/* 
Wrapped procedure of LAPACKE_ctrttf for the type lapack_complex_float.
 */
inline proc trttf(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] complex(64), lda : c_int, arf : [] complex(64)): c_int{
  return LAPACKE_ctrttf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, lda, arf);
}

/* 
Wrapped procedure of LAPACKE_ztrttf for the type lapack_complex_double.
 */
inline proc trttf(matrix_order : lapack_memory_order, transr : string, uplo : string, a : [] complex(128), lda : c_int, arf : [] complex(128)): c_int{
  return LAPACKE_ztrttf(matrix_order, ascii(transr) : c_char, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, lda, arf);
}

/* 
Wrapped procedure of LAPACKE_strttp for the type c_float.
 */
inline proc trttp(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, ap : [] c_float): c_int{
  return LAPACKE_strttp(matrix_order, ascii(uplo) : c_char, (ap.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ap);
}

/* 
Wrapped procedure of LAPACKE_dtrttp for the type c_double.
 */
inline proc trttp(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, ap : [] c_double): c_int{
  return LAPACKE_dtrttp(matrix_order, ascii(uplo) : c_char, (ap.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ap);
}

/* 
Wrapped procedure of LAPACKE_ctrttp for the type lapack_complex_float.
 */
inline proc trttp(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ap : [] complex(64)): c_int{
  return LAPACKE_ctrttp(matrix_order, ascii(uplo) : c_char, (ap.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ap);
}

/* 
Wrapped procedure of LAPACKE_ztrttp for the type lapack_complex_double.
 */
inline proc trttp(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ap : [] complex(128)): c_int{
  return LAPACKE_ztrttp(matrix_order, ascii(uplo) : c_char, (ap.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ap);
}

/* 
Wrapped procedure of LAPACKE_stzrzf for the type c_float.
 */
inline proc tzrzf(matrix_order : lapack_memory_order, a : [] c_float, tau : [] c_float): c_int{
  return LAPACKE_stzrzf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_dtzrzf for the type c_double.
 */
inline proc tzrzf(matrix_order : lapack_memory_order, a : [] c_double, tau : [] c_double): c_int{
  return LAPACKE_dtzrzf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_ctzrzf for the type lapack_complex_float.
 */
inline proc tzrzf(matrix_order : lapack_memory_order, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_ctzrzf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_ztzrzf for the type lapack_complex_double.
 */
inline proc tzrzf(matrix_order : lapack_memory_order, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_ztzrzf(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cungbr for the type lapack_complex_float.
 */
inline proc ungbr(matrix_order : lapack_memory_order, vect : string, k : c_int, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cungbr(matrix_order, ascii(vect) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zungbr for the type lapack_complex_double.
 */
inline proc ungbr(matrix_order : lapack_memory_order, vect : string, k : c_int, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zungbr(matrix_order, ascii(vect) : c_char, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cunghr for the type lapack_complex_float.
 */
inline proc unghr(matrix_order : lapack_memory_order, n : c_int, ilo : c_int, ihi : c_int, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cunghr(matrix_order, n, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zunghr for the type lapack_complex_double.
 */
inline proc unghr(matrix_order : lapack_memory_order, n : c_int, ilo : c_int, ihi : c_int, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zunghr(matrix_order, n, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cunglq for the type lapack_complex_float.
 */
inline proc unglq(matrix_order : lapack_memory_order, k : c_int, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cunglq(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zunglq for the type lapack_complex_double.
 */
inline proc unglq(matrix_order : lapack_memory_order, k : c_int, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zunglq(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cungql for the type lapack_complex_float.
 */
inline proc ungql(matrix_order : lapack_memory_order, k : c_int, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cungql(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zungql for the type lapack_complex_double.
 */
inline proc ungql(matrix_order : lapack_memory_order, k : c_int, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zungql(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cungqr for the type lapack_complex_float.
 */
inline proc ungqr(matrix_order : lapack_memory_order, k : c_int, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cungqr(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zungqr for the type lapack_complex_double.
 */
inline proc ungqr(matrix_order : lapack_memory_order, k : c_int, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zungqr(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cungrq for the type lapack_complex_float.
 */
inline proc ungrq(matrix_order : lapack_memory_order, k : c_int, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cungrq(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zungrq for the type lapack_complex_double.
 */
inline proc ungrq(matrix_order : lapack_memory_order, k : c_int, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zungrq(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cungtr for the type lapack_complex_float.
 */
inline proc ungtr(matrix_order : lapack_memory_order, uplo : string, n : c_int, a : [] complex(64), tau : [] complex(64)): c_int{
  return LAPACKE_cungtr(matrix_order, ascii(uplo) : c_char, n, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_zungtr for the type lapack_complex_double.
 */
inline proc ungtr(matrix_order : lapack_memory_order, uplo : string, n : c_int, a : [] complex(128), tau : [] complex(128)): c_int{
  return LAPACKE_zungtr(matrix_order, ascii(uplo) : c_char, n, a, (a.domain.dim(2).size) : c_int, tau);
}

/* 
Wrapped procedure of LAPACKE_cunmbr for the type lapack_complex_float.
 */
inline proc unmbr(matrix_order : lapack_memory_order, vect : string, side : string, trans : string, k : c_int, a : [] complex(64), tau : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cunmbr(matrix_order, ascii(vect) : c_char, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zunmbr for the type lapack_complex_double.
 */
inline proc unmbr(matrix_order : lapack_memory_order, vect : string, side : string, trans : string, k : c_int, a : [] complex(128), tau : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zunmbr(matrix_order, ascii(vect) : c_char, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cunmhr for the type lapack_complex_float.
 */
inline proc unmhr(matrix_order : lapack_memory_order, side : string, trans : string, ilo : c_int, ihi : c_int, a : [] complex(64), tau : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cunmhr(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zunmhr for the type lapack_complex_double.
 */
inline proc unmhr(matrix_order : lapack_memory_order, side : string, trans : string, ilo : c_int, ihi : c_int, a : [] complex(128), tau : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zunmhr(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, ilo, ihi, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cunmlq for the type lapack_complex_float.
 */
inline proc unmlq(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] complex(64), tau : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cunmlq(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zunmlq for the type lapack_complex_double.
 */
inline proc unmlq(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] complex(128), tau : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zunmlq(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cunmql for the type lapack_complex_float.
 */
inline proc unmql(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] complex(64), tau : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cunmql(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zunmql for the type lapack_complex_double.
 */
inline proc unmql(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] complex(128), tau : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zunmql(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cunmqr for the type lapack_complex_float.
 */
inline proc unmqr(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] complex(64), tau : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cunmqr(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zunmqr for the type lapack_complex_double.
 */
inline proc unmqr(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] complex(128), tau : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zunmqr(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cunmrq for the type lapack_complex_float.
 */
inline proc unmrq(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] complex(64), tau : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cunmrq(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zunmrq for the type lapack_complex_double.
 */
inline proc unmrq(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, a : [] complex(128), tau : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zunmrq(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cunmrz for the type lapack_complex_float.
 */
inline proc unmrz(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, l : c_int, a : [] complex(64), tau : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cunmrz(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zunmrz for the type lapack_complex_double.
 */
inline proc unmrz(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, l : c_int, a : [] complex(128), tau : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zunmrz(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, l, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cunmtr for the type lapack_complex_float.
 */
inline proc unmtr(matrix_order : lapack_memory_order, side : string, uplo : string, trans : string, a : [] complex(64), tau : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cunmtr(matrix_order, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zunmtr for the type lapack_complex_double.
 */
inline proc unmtr(matrix_order : lapack_memory_order, side : string, uplo : string, trans : string, a : [] complex(128), tau : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zunmtr(matrix_order, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cupgtr for the type lapack_complex_float.
 */
inline proc upgtr(matrix_order : lapack_memory_order, uplo : string, ap : [] complex(64), tau : [] complex(64), q : [] complex(64)): c_int{
  return LAPACKE_cupgtr(matrix_order, ascii(uplo) : c_char, (q.domain.dim(1).size) : c_int, ap, tau, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zupgtr for the type lapack_complex_double.
 */
inline proc upgtr(matrix_order : lapack_memory_order, uplo : string, ap : [] complex(128), tau : [] complex(128), q : [] complex(128)): c_int{
  return LAPACKE_zupgtr(matrix_order, ascii(uplo) : c_char, (q.domain.dim(1).size) : c_int, ap, tau, q, (q.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cupmtr for the type lapack_complex_float.
 */
inline proc upmtr(matrix_order : lapack_memory_order, side : string, uplo : string, trans : string, ap : [] complex(64), tau : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cupmtr(matrix_order, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, ap, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zupmtr for the type lapack_complex_double.
 */
inline proc upmtr(matrix_order : lapack_memory_order, side : string, uplo : string, trans : string, ap : [] complex(128), tau : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zupmtr(matrix_order, ascii(side) : c_char, ascii(uplo) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, ap, tau, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_claghe for the type lapack_complex_float.
 */
inline proc laghe(matrix_order : lapack_memory_order, k : c_int, d : [] c_float, a : [] complex(64), iseed : [] c_int): c_int{
  return LAPACKE_claghe(matrix_order, (a.domain.dim(1).size) : c_int, k, d, a, (a.domain.dim(2).size) : c_int, iseed);
}

/* 
Wrapped procedure of LAPACKE_zlaghe for the type lapack_complex_double.
 */
inline proc laghe(matrix_order : lapack_memory_order, k : c_int, d : [] c_double, a : [] complex(128), iseed : [] c_int): c_int{
  return LAPACKE_zlaghe(matrix_order, (a.domain.dim(1).size) : c_int, k, d, a, (a.domain.dim(2).size) : c_int, iseed);
}

/* 
Wrapped procedure of LAPACKE_slagsy for the type c_float.
 */
inline proc lagsy(matrix_order : lapack_memory_order, k : c_int, d : [] c_float, a : [] c_float, iseed : [] c_int): c_int{
  return LAPACKE_slagsy(matrix_order, (a.domain.dim(1).size) : c_int, k, d, a, (a.domain.dim(2).size) : c_int, iseed);
}

/* 
Wrapped procedure of LAPACKE_dlagsy for the type c_double.
 */
inline proc lagsy(matrix_order : lapack_memory_order, k : c_int, d : [] c_double, a : [] c_double, iseed : [] c_int): c_int{
  return LAPACKE_dlagsy(matrix_order, (a.domain.dim(1).size) : c_int, k, d, a, (a.domain.dim(2).size) : c_int, iseed);
}

/* 
Wrapped procedure of LAPACKE_clagsy for the type lapack_complex_float.
 */
inline proc lagsy(matrix_order : lapack_memory_order, k : c_int, d : [] c_float, a : [] complex(64), iseed : [] c_int): c_int{
  return LAPACKE_clagsy(matrix_order, (a.domain.dim(1).size) : c_int, k, d, a, (a.domain.dim(2).size) : c_int, iseed);
}

/* 
Wrapped procedure of LAPACKE_zlagsy for the type lapack_complex_double.
 */
inline proc lagsy(matrix_order : lapack_memory_order, k : c_int, d : [] c_double, a : [] complex(128), iseed : [] c_int): c_int{
  return LAPACKE_zlagsy(matrix_order, (a.domain.dim(1).size) : c_int, k, d, a, (a.domain.dim(2).size) : c_int, iseed);
}

/* 
Wrapped procedure of LAPACKE_slapmr for the type c_float.
 */
inline proc lapmr(matrix_order : lapack_memory_order, forwrd : c_int, x : [] c_float, k : [] c_int): c_int{
  return LAPACKE_slapmr(matrix_order, forwrd, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(1).size else x.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(2).size else x.domain.dim(1).size) : c_int, x, (x.domain.dim(2).size) : c_int, k);
}

/* 
Wrapped procedure of LAPACKE_dlapmr for the type c_double.
 */
inline proc lapmr(matrix_order : lapack_memory_order, forwrd : c_int, x : [] c_double, k : [] c_int): c_int{
  return LAPACKE_dlapmr(matrix_order, forwrd, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(1).size else x.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(2).size else x.domain.dim(1).size) : c_int, x, (x.domain.dim(2).size) : c_int, k);
}

/* 
Wrapped procedure of LAPACKE_clapmr for the type lapack_complex_float.
 */
inline proc lapmr(matrix_order : lapack_memory_order, forwrd : c_int, x : [] complex(64), k : [] c_int): c_int{
  return LAPACKE_clapmr(matrix_order, forwrd, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(1).size else x.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(2).size else x.domain.dim(1).size) : c_int, x, (x.domain.dim(2).size) : c_int, k);
}

/* 
Wrapped procedure of LAPACKE_zlapmr for the type lapack_complex_double.
 */
inline proc lapmr(matrix_order : lapack_memory_order, forwrd : c_int, x : [] complex(128), k : [] c_int): c_int{
  return LAPACKE_zlapmr(matrix_order, forwrd, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(1).size else x.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x.domain.dim(2).size else x.domain.dim(1).size) : c_int, x, (x.domain.dim(2).size) : c_int, k);
}

/* 
Wrapped procedure of LAPACKE_slapy2 for the type c_float.
 */
inline proc lapy2(x : c_float, y : c_float): c_float{
  return LAPACKE_slapy2(x, y);
}

/* 
Wrapped procedure of LAPACKE_dlapy2 for the type c_double.
 */
inline proc lapy2(x : c_double, y : c_double): c_double{
  return LAPACKE_dlapy2(x, y);
}

/* 
Wrapped procedure of LAPACKE_slapy3 for the type c_float.
 */
inline proc lapy3(x : c_float, y : c_float, z : c_float): c_float{
  return LAPACKE_slapy3(x, y, z);
}

/* 
Wrapped procedure of LAPACKE_dlapy3 for the type c_double.
 */
inline proc lapy3(x : c_double, y : c_double, z : c_double): c_double{
  return LAPACKE_dlapy3(x, y, z);
}

/* 
Wrapped procedure of LAPACKE_slartgp for the type c_float.
 */
inline proc lartgp(f : c_float, g : c_float, ref cs : c_float, ref sn : c_float, ref r : c_float): c_int{
  return LAPACKE_slartgp(f, g, cs, sn, r);
}

/* 
Wrapped procedure of LAPACKE_dlartgp for the type c_double.
 */
inline proc lartgp(f : c_double, g : c_double, ref cs : c_double, ref sn : c_double, ref r : c_double): c_int{
  return LAPACKE_dlartgp(f, g, cs, sn, r);
}

/* 
Wrapped procedure of LAPACKE_slartgs for the type c_float.
 */
inline proc lartgs(x : c_float, y : c_float, sigma : c_float, ref cs : c_float, ref sn : c_float): c_int{
  return LAPACKE_slartgs(x, y, sigma, cs, sn);
}

/* 
Wrapped procedure of LAPACKE_dlartgs for the type c_double.
 */
inline proc lartgs(x : c_double, y : c_double, sigma : c_double, ref cs : c_double, ref sn : c_double): c_int{
  return LAPACKE_dlartgs(x, y, sigma, cs, sn);
}

/* 
Wrapped procedure of LAPACKE_cbbcsd for the type lapack_complex_float.
 */
inline proc bbcsd(matrix_order : lapack_memory_order, jobu1 : string, jobu2 : string, jobv1t : string, jobv2t : string, trans : string, m : c_int, theta : [] c_float, phi : [] c_float, u1 : [] complex(64), u2 : [] complex(64), v1t : [] complex(64), v2t : [] complex(64), b11d : [] c_float, b11e : [] c_float, b12d : [] c_float, b12e : [] c_float, b21d : [] c_float, b21e : [] c_float, b22d : [] c_float, b22e : [] c_float): c_int{
  return LAPACKE_cbbcsd(matrix_order, ascii(jobu1) : c_char, ascii(jobu2) : c_char, ascii(jobv1t) : c_char, ascii(jobv2t) : c_char, ascii(trans) : c_char, m, (if matrix_order == lapack_memory_order.row_major then u1.domain.dim(2).size else u1.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then v1t.domain.dim(2).size else v1t.domain.dim(1).size) : c_int, theta, phi, u1, (if matrix_order == lapack_memory_order.row_major then u1.domain.dim(1).size else u1.domain.dim(2).size) : c_int, u2, (u2.domain.dim(2).size) : c_int, v1t, (if matrix_order == lapack_memory_order.row_major then v1t.domain.dim(1).size else v1t.domain.dim(2).size) : c_int, v2t, (v2t.domain.dim(2).size) : c_int, b11d, b11e, b12d, b12e, b21d, b21e, b22d, b22e);
}

/* 
Wrapped procedure of LAPACKE_cheswapr for the type lapack_complex_float.
 */
inline proc heswapr(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), i1 : c_int, i2 : c_int): c_int{
  return LAPACKE_cheswapr(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, i1, i2);
}

/* 
Wrapped procedure of LAPACKE_chetri2 for the type lapack_complex_float.
 */
inline proc hetri2(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_chetri2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_chetri2x for the type lapack_complex_float.
 */
inline proc hetri2x(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int, nb : c_int): c_int{
  return LAPACKE_chetri2x(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, nb);
}

/* 
Wrapped procedure of LAPACKE_chetrs2 for the type lapack_complex_float.
 */
inline proc hetrs2(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_chetrs2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_csyconv for the type lapack_complex_float.
 */
inline proc syconv(matrix_order : lapack_memory_order, uplo : string, way : string, a : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_csyconv(matrix_order, ascii(uplo) : c_char, ascii(way) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_csyswapr for the type lapack_complex_float.
 */
inline proc syswapr(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), i1 : c_int, i2 : c_int): c_int{
  return LAPACKE_csyswapr(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, i1, i2);
}

/* 
Wrapped procedure of LAPACKE_csytri2 for the type lapack_complex_float.
 */
inline proc sytri2(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int): c_int{
  return LAPACKE_csytri2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_csytri2x for the type lapack_complex_float.
 */
inline proc sytri2x(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int, nb : c_int): c_int{
  return LAPACKE_csytri2x(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, nb);
}

/* 
Wrapped procedure of LAPACKE_csytrs2 for the type lapack_complex_float.
 */
inline proc sytrs2(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_csytrs2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cunbdb for the type lapack_complex_float.
 */
inline proc unbdb(matrix_order : lapack_memory_order, trans : string, signs : string, m : c_int, x11 : [] complex(64), x12 : [] complex(64), x21 : [] complex(64), x22 : [] complex(64), theta : [] c_float, phi : [] c_float, taup1 : [] complex(64), taup2 : [] complex(64), tauq1 : [] complex(64), tauq2 : [] complex(64)): c_int{
  return LAPACKE_cunbdb(matrix_order, ascii(trans) : c_char, ascii(signs) : c_char, m, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(1).size else x11.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(2).size else x11.domain.dim(1).size) : c_int, x11, (x11.domain.dim(2).size) : c_int, x12, (x12.domain.dim(2).size) : c_int, x21, (x21.domain.dim(2).size) : c_int, x22, (x22.domain.dim(2).size) : c_int, theta, phi, taup1, taup2, tauq1, tauq2);
}

/* 
Wrapped procedure of LAPACKE_cuncsd for the type lapack_complex_float.
 */
inline proc uncsd(matrix_order : lapack_memory_order, jobu1 : string, jobu2 : string, jobv1t : string, jobv2t : string, trans : string, signs : string, m : c_int, x11 : [] complex(64), x12 : [] complex(64), x21 : [] complex(64), x22 : [] complex(64), theta : [] c_float, u1 : [] complex(64), u2 : [] complex(64), v1t : [] complex(64), v2t : [] complex(64)): c_int{
  return LAPACKE_cuncsd(matrix_order, ascii(jobu1) : c_char, ascii(jobu2) : c_char, ascii(jobv1t) : c_char, ascii(jobv2t) : c_char, ascii(trans) : c_char, ascii(signs) : c_char, m, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(1).size else x11.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(2).size else x11.domain.dim(1).size) : c_int, x11, (x11.domain.dim(2).size) : c_int, x12, (x12.domain.dim(2).size) : c_int, x21, (x21.domain.dim(2).size) : c_int, x22, (x22.domain.dim(2).size) : c_int, theta, u1, (u1.domain.dim(2).size) : c_int, u2, (u2.domain.dim(2).size) : c_int, v1t, (v1t.domain.dim(2).size) : c_int, v2t, (v2t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dbbcsd for the type c_double.
 */
inline proc bbcsd(matrix_order : lapack_memory_order, jobu1 : string, jobu2 : string, jobv1t : string, jobv2t : string, trans : string, m : c_int, theta : [] c_double, phi : [] c_double, u1 : [] c_double, u2 : [] c_double, v1t : [] c_double, v2t : [] c_double, b11d : [] c_double, b11e : [] c_double, b12d : [] c_double, b12e : [] c_double, b21d : [] c_double, b21e : [] c_double, b22d : [] c_double, b22e : [] c_double): c_int{
  return LAPACKE_dbbcsd(matrix_order, ascii(jobu1) : c_char, ascii(jobu2) : c_char, ascii(jobv1t) : c_char, ascii(jobv2t) : c_char, ascii(trans) : c_char, m, (if matrix_order == lapack_memory_order.row_major then u1.domain.dim(2).size else u1.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then v1t.domain.dim(2).size else v1t.domain.dim(1).size) : c_int, theta, phi, u1, (if matrix_order == lapack_memory_order.row_major then u1.domain.dim(1).size else u1.domain.dim(2).size) : c_int, u2, (u2.domain.dim(2).size) : c_int, v1t, (if matrix_order == lapack_memory_order.row_major then v1t.domain.dim(1).size else v1t.domain.dim(2).size) : c_int, v2t, (v2t.domain.dim(2).size) : c_int, b11d, b11e, b12d, b12e, b21d, b21e, b22d, b22e);
}

/* 
Wrapped procedure of LAPACKE_dorbdb for the type c_double.
 */
inline proc orbdb(matrix_order : lapack_memory_order, trans : string, signs : string, m : c_int, x11 : [] c_double, x12 : [] c_double, x21 : [] c_double, x22 : [] c_double, theta : [] c_double, phi : [] c_double, taup1 : [] c_double, taup2 : [] c_double, tauq1 : [] c_double, tauq2 : [] c_double): c_int{
  return LAPACKE_dorbdb(matrix_order, ascii(trans) : c_char, ascii(signs) : c_char, m, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(1).size else x11.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(2).size else x11.domain.dim(1).size) : c_int, x11, (x11.domain.dim(2).size) : c_int, x12, (x12.domain.dim(2).size) : c_int, x21, (x21.domain.dim(2).size) : c_int, x22, (x22.domain.dim(2).size) : c_int, theta, phi, taup1, taup2, tauq1, tauq2);
}

/* 
Wrapped procedure of LAPACKE_dorcsd for the type c_double.
 */
inline proc orcsd(matrix_order : lapack_memory_order, jobu1 : string, jobu2 : string, jobv1t : string, jobv2t : string, trans : string, signs : string, m : c_int, x11 : [] c_double, x12 : [] c_double, x21 : [] c_double, x22 : [] c_double, theta : [] c_double, u1 : [] c_double, u2 : [] c_double, v1t : [] c_double, v2t : [] c_double): c_int{
  return LAPACKE_dorcsd(matrix_order, ascii(jobu1) : c_char, ascii(jobu2) : c_char, ascii(jobv1t) : c_char, ascii(jobv2t) : c_char, ascii(trans) : c_char, ascii(signs) : c_char, m, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(1).size else x11.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(2).size else x11.domain.dim(1).size) : c_int, x11, (x11.domain.dim(2).size) : c_int, x12, (x12.domain.dim(2).size) : c_int, x21, (x21.domain.dim(2).size) : c_int, x22, (x22.domain.dim(2).size) : c_int, theta, u1, (u1.domain.dim(2).size) : c_int, u2, (u2.domain.dim(2).size) : c_int, v1t, (v1t.domain.dim(2).size) : c_int, v2t, (v2t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsyconv for the type c_double.
 */
inline proc syconv(matrix_order : lapack_memory_order, uplo : string, way : string, a : [] c_double, ipiv : [] c_int): c_int{
  return LAPACKE_dsyconv(matrix_order, ascii(uplo) : c_char, ascii(way) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_dsyswapr for the type c_double.
 */
inline proc syswapr(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, i1 : c_int, i2 : c_int): c_int{
  return LAPACKE_dsyswapr(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, i1, i2);
}

/* 
Wrapped procedure of LAPACKE_dsytri2 for the type c_double.
 */
inline proc sytri2(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, ipiv : [] c_int): c_int{
  return LAPACKE_dsytri2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_dsytri2x for the type c_double.
 */
inline proc sytri2x(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, ipiv : [] c_int, nb : c_int): c_int{
  return LAPACKE_dsytri2x(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, nb);
}

/* 
Wrapped procedure of LAPACKE_dsytrs2 for the type c_double.
 */
inline proc sytrs2(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, ipiv : [] c_int, b : [] c_double): c_int{
  return LAPACKE_dsytrs2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sbbcsd for the type c_float.
 */
inline proc bbcsd(matrix_order : lapack_memory_order, jobu1 : string, jobu2 : string, jobv1t : string, jobv2t : string, trans : string, m : c_int, theta : [] c_float, phi : [] c_float, u1 : [] c_float, u2 : [] c_float, v1t : [] c_float, v2t : [] c_float, b11d : [] c_float, b11e : [] c_float, b12d : [] c_float, b12e : [] c_float, b21d : [] c_float, b21e : [] c_float, b22d : [] c_float, b22e : [] c_float): c_int{
  return LAPACKE_sbbcsd(matrix_order, ascii(jobu1) : c_char, ascii(jobu2) : c_char, ascii(jobv1t) : c_char, ascii(jobv2t) : c_char, ascii(trans) : c_char, m, (if matrix_order == lapack_memory_order.row_major then u1.domain.dim(2).size else u1.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then v1t.domain.dim(2).size else v1t.domain.dim(1).size) : c_int, theta, phi, u1, (if matrix_order == lapack_memory_order.row_major then u1.domain.dim(1).size else u1.domain.dim(2).size) : c_int, u2, (u2.domain.dim(2).size) : c_int, v1t, (if matrix_order == lapack_memory_order.row_major then v1t.domain.dim(1).size else v1t.domain.dim(2).size) : c_int, v2t, (v2t.domain.dim(2).size) : c_int, b11d, b11e, b12d, b12e, b21d, b21e, b22d, b22e);
}

/* 
Wrapped procedure of LAPACKE_sorbdb for the type c_float.
 */
inline proc orbdb(matrix_order : lapack_memory_order, trans : string, signs : string, m : c_int, x11 : [] c_float, x12 : [] c_float, x21 : [] c_float, x22 : [] c_float, theta : [] c_float, phi : [] c_float, taup1 : [] c_float, taup2 : [] c_float, tauq1 : [] c_float, tauq2 : [] c_float): c_int{
  return LAPACKE_sorbdb(matrix_order, ascii(trans) : c_char, ascii(signs) : c_char, m, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(1).size else x11.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(2).size else x11.domain.dim(1).size) : c_int, x11, (x11.domain.dim(2).size) : c_int, x12, (x12.domain.dim(2).size) : c_int, x21, (x21.domain.dim(2).size) : c_int, x22, (x22.domain.dim(2).size) : c_int, theta, phi, taup1, taup2, tauq1, tauq2);
}

/* 
Wrapped procedure of LAPACKE_sorcsd for the type c_float.
 */
inline proc orcsd(matrix_order : lapack_memory_order, jobu1 : string, jobu2 : string, jobv1t : string, jobv2t : string, trans : string, signs : string, m : c_int, x11 : [] c_float, x12 : [] c_float, x21 : [] c_float, x22 : [] c_float, theta : [] c_float, u1 : [] c_float, u2 : [] c_float, v1t : [] c_float, v2t : [] c_float): c_int{
  return LAPACKE_sorcsd(matrix_order, ascii(jobu1) : c_char, ascii(jobu2) : c_char, ascii(jobv1t) : c_char, ascii(jobv2t) : c_char, ascii(trans) : c_char, ascii(signs) : c_char, m, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(1).size else x11.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(2).size else x11.domain.dim(1).size) : c_int, x11, (x11.domain.dim(2).size) : c_int, x12, (x12.domain.dim(2).size) : c_int, x21, (x21.domain.dim(2).size) : c_int, x22, (x22.domain.dim(2).size) : c_int, theta, u1, (u1.domain.dim(2).size) : c_int, u2, (u2.domain.dim(2).size) : c_int, v1t, (v1t.domain.dim(2).size) : c_int, v2t, (v2t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssyconv for the type c_float.
 */
inline proc syconv(matrix_order : lapack_memory_order, uplo : string, way : string, a : [] c_float, ipiv : [] c_int): c_int{
  return LAPACKE_ssyconv(matrix_order, ascii(uplo) : c_char, ascii(way) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_ssyswapr for the type c_float.
 */
inline proc syswapr(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, i1 : c_int, i2 : c_int): c_int{
  return LAPACKE_ssyswapr(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, i1, i2);
}

/* 
Wrapped procedure of LAPACKE_ssytri2 for the type c_float.
 */
inline proc sytri2(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, ipiv : [] c_int): c_int{
  return LAPACKE_ssytri2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_ssytri2x for the type c_float.
 */
inline proc sytri2x(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, ipiv : [] c_int, nb : c_int): c_int{
  return LAPACKE_ssytri2x(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, nb);
}

/* 
Wrapped procedure of LAPACKE_ssytrs2 for the type c_float.
 */
inline proc sytrs2(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, ipiv : [] c_int, b : [] c_float): c_int{
  return LAPACKE_ssytrs2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zbbcsd for the type lapack_complex_double.
 */
inline proc bbcsd(matrix_order : lapack_memory_order, jobu1 : string, jobu2 : string, jobv1t : string, jobv2t : string, trans : string, m : c_int, theta : [] c_double, phi : [] c_double, u1 : [] complex(128), u2 : [] complex(128), v1t : [] complex(128), v2t : [] complex(128), b11d : [] c_double, b11e : [] c_double, b12d : [] c_double, b12e : [] c_double, b21d : [] c_double, b21e : [] c_double, b22d : [] c_double, b22e : [] c_double): c_int{
  return LAPACKE_zbbcsd(matrix_order, ascii(jobu1) : c_char, ascii(jobu2) : c_char, ascii(jobv1t) : c_char, ascii(jobv2t) : c_char, ascii(trans) : c_char, m, (if matrix_order == lapack_memory_order.row_major then u1.domain.dim(2).size else u1.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then v1t.domain.dim(2).size else v1t.domain.dim(1).size) : c_int, theta, phi, u1, (if matrix_order == lapack_memory_order.row_major then u1.domain.dim(1).size else u1.domain.dim(2).size) : c_int, u2, (u2.domain.dim(2).size) : c_int, v1t, (if matrix_order == lapack_memory_order.row_major then v1t.domain.dim(1).size else v1t.domain.dim(2).size) : c_int, v2t, (v2t.domain.dim(2).size) : c_int, b11d, b11e, b12d, b12e, b21d, b21e, b22d, b22e);
}

/* 
Wrapped procedure of LAPACKE_zheswapr for the type lapack_complex_double.
 */
inline proc heswapr(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), i1 : c_int, i2 : c_int): c_int{
  return LAPACKE_zheswapr(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, i1, i2);
}

/* 
Wrapped procedure of LAPACKE_zhetri2 for the type lapack_complex_double.
 */
inline proc hetri2(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zhetri2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zhetri2x for the type lapack_complex_double.
 */
inline proc hetri2x(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int, nb : c_int): c_int{
  return LAPACKE_zhetri2x(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, nb);
}

/* 
Wrapped procedure of LAPACKE_zhetrs2 for the type lapack_complex_double.
 */
inline proc hetrs2(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zhetrs2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zsyconv for the type lapack_complex_double.
 */
inline proc syconv(matrix_order : lapack_memory_order, uplo : string, way : string, a : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zsyconv(matrix_order, ascii(uplo) : c_char, ascii(way) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zsyswapr for the type lapack_complex_double.
 */
inline proc syswapr(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), i1 : c_int, i2 : c_int): c_int{
  return LAPACKE_zsyswapr(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, i1, i2);
}

/* 
Wrapped procedure of LAPACKE_zsytri2 for the type lapack_complex_double.
 */
inline proc sytri2(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int): c_int{
  return LAPACKE_zsytri2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv);
}

/* 
Wrapped procedure of LAPACKE_zsytri2x for the type lapack_complex_double.
 */
inline proc sytri2x(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int, nb : c_int): c_int{
  return LAPACKE_zsytri2x(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, nb);
}

/* 
Wrapped procedure of LAPACKE_zsytrs2 for the type lapack_complex_double.
 */
inline proc sytrs2(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zsytrs2(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zunbdb for the type lapack_complex_double.
 */
inline proc unbdb(matrix_order : lapack_memory_order, trans : string, signs : string, m : c_int, x11 : [] complex(128), x12 : [] complex(128), x21 : [] complex(128), x22 : [] complex(128), theta : [] c_double, phi : [] c_double, taup1 : [] complex(128), taup2 : [] complex(128), tauq1 : [] complex(128), tauq2 : [] complex(128)): c_int{
  return LAPACKE_zunbdb(matrix_order, ascii(trans) : c_char, ascii(signs) : c_char, m, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(1).size else x11.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(2).size else x11.domain.dim(1).size) : c_int, x11, (x11.domain.dim(2).size) : c_int, x12, (x12.domain.dim(2).size) : c_int, x21, (x21.domain.dim(2).size) : c_int, x22, (x22.domain.dim(2).size) : c_int, theta, phi, taup1, taup2, tauq1, tauq2);
}

/* 
Wrapped procedure of LAPACKE_zuncsd for the type lapack_complex_double.
 */
inline proc uncsd(matrix_order : lapack_memory_order, jobu1 : string, jobu2 : string, jobv1t : string, jobv2t : string, trans : string, signs : string, m : c_int, x11 : [] complex(128), x12 : [] complex(128), x21 : [] complex(128), x22 : [] complex(128), theta : [] c_double, u1 : [] complex(128), u2 : [] complex(128), v1t : [] complex(128), v2t : [] complex(128)): c_int{
  return LAPACKE_zuncsd(matrix_order, ascii(jobu1) : c_char, ascii(jobu2) : c_char, ascii(jobv1t) : c_char, ascii(jobv2t) : c_char, ascii(trans) : c_char, ascii(signs) : c_char, m, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(1).size else x11.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then x11.domain.dim(2).size else x11.domain.dim(1).size) : c_int, x11, (x11.domain.dim(2).size) : c_int, x12, (x12.domain.dim(2).size) : c_int, x21, (x21.domain.dim(2).size) : c_int, x22, (x22.domain.dim(2).size) : c_int, theta, u1, (u1.domain.dim(2).size) : c_int, u2, (u2.domain.dim(2).size) : c_int, v1t, (v1t.domain.dim(2).size) : c_int, v2t, (v2t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgemqrt for the type c_float.
 */
inline proc gemqrt(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, v : [] c_float, t : [] c_float, c : [] c_float): c_int{
  return LAPACKE_sgemqrt(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, (if matrix_order == lapack_memory_order.row_major then t.domain.dim(1).size else t.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgemqrt for the type c_double.
 */
inline proc gemqrt(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, v : [] c_double, t : [] c_double, c : [] c_double): c_int{
  return LAPACKE_dgemqrt(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, (if matrix_order == lapack_memory_order.row_major then t.domain.dim(1).size else t.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgemqrt for the type lapack_complex_float.
 */
inline proc gemqrt(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, v : [] complex(64), t : [] complex(64), c : [] complex(64)): c_int{
  return LAPACKE_cgemqrt(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, (if matrix_order == lapack_memory_order.row_major then t.domain.dim(1).size else t.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgemqrt for the type lapack_complex_double.
 */
inline proc gemqrt(matrix_order : lapack_memory_order, side : string, trans : string, k : c_int, v : [] complex(128), t : [] complex(128), c : [] complex(128)): c_int{
  return LAPACKE_zgemqrt(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(1).size else c.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then c.domain.dim(2).size else c.domain.dim(1).size) : c_int, k, (if matrix_order == lapack_memory_order.row_major then t.domain.dim(1).size else t.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, c, (c.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgeqrt for the type c_float.
 */
inline proc geqrt(matrix_order : lapack_memory_order, nb : c_int, a : [] c_float, t : [] c_float): c_int{
  return LAPACKE_sgeqrt(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, nb, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgeqrt for the type c_double.
 */
inline proc geqrt(matrix_order : lapack_memory_order, nb : c_int, a : [] c_double, t : [] c_double): c_int{
  return LAPACKE_dgeqrt(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, nb, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgeqrt for the type lapack_complex_float.
 */
inline proc geqrt(matrix_order : lapack_memory_order, nb : c_int, a : [] complex(64), t : [] complex(64)): c_int{
  return LAPACKE_cgeqrt(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, nb, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgeqrt for the type lapack_complex_double.
 */
inline proc geqrt(matrix_order : lapack_memory_order, nb : c_int, a : [] complex(128), t : [] complex(128)): c_int{
  return LAPACKE_zgeqrt(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, nb, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgeqrt2 for the type c_float.
 */
inline proc geqrt2(matrix_order : lapack_memory_order, a : [] c_float, t : [] c_float): c_int{
  return LAPACKE_sgeqrt2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgeqrt2 for the type c_double.
 */
inline proc geqrt2(matrix_order : lapack_memory_order, a : [] c_double, t : [] c_double): c_int{
  return LAPACKE_dgeqrt2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgeqrt2 for the type lapack_complex_float.
 */
inline proc geqrt2(matrix_order : lapack_memory_order, a : [] complex(64), t : [] complex(64)): c_int{
  return LAPACKE_cgeqrt2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgeqrt2 for the type lapack_complex_double.
 */
inline proc geqrt2(matrix_order : lapack_memory_order, a : [] complex(128), t : [] complex(128)): c_int{
  return LAPACKE_zgeqrt2(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_sgeqrt3 for the type c_float.
 */
inline proc geqrt3(matrix_order : lapack_memory_order, a : [] c_float, t : [] c_float): c_int{
  return LAPACKE_sgeqrt3(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dgeqrt3 for the type c_double.
 */
inline proc geqrt3(matrix_order : lapack_memory_order, a : [] c_double, t : [] c_double): c_int{
  return LAPACKE_dgeqrt3(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_cgeqrt3 for the type lapack_complex_float.
 */
inline proc geqrt3(matrix_order : lapack_memory_order, a : [] complex(64), t : [] complex(64)): c_int{
  return LAPACKE_cgeqrt3(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zgeqrt3 for the type lapack_complex_double.
 */
inline proc geqrt3(matrix_order : lapack_memory_order, a : [] complex(128), t : [] complex(128)): c_int{
  return LAPACKE_zgeqrt3(matrix_order, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(1).size else a.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_stpmqrt for the type c_float.
 */
inline proc tpmqrt(matrix_order : lapack_memory_order, side : string, trans : string, l : c_int, v : [] c_float, t : [] c_float, a : [] c_float, b : [] c_float): c_int{
  return LAPACKE_stpmqrt(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, l, (if matrix_order == lapack_memory_order.row_major then t.domain.dim(1).size else t.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dtpmqrt for the type c_double.
 */
inline proc tpmqrt(matrix_order : lapack_memory_order, side : string, trans : string, l : c_int, v : [] c_double, t : [] c_double, a : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dtpmqrt(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, l, (if matrix_order == lapack_memory_order.row_major then t.domain.dim(1).size else t.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ctpmqrt for the type lapack_complex_float.
 */
inline proc tpmqrt(matrix_order : lapack_memory_order, side : string, trans : string, l : c_int, v : [] complex(64), t : [] complex(64), a : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_ctpmqrt(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, l, (if matrix_order == lapack_memory_order.row_major then t.domain.dim(1).size else t.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ztpmqrt for the type lapack_complex_double.
 */
inline proc tpmqrt(matrix_order : lapack_memory_order, side : string, trans : string, l : c_int, v : [] complex(128), t : [] complex(128), a : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_ztpmqrt(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then a.domain.dim(2).size else a.domain.dim(1).size) : c_int, l, (if matrix_order == lapack_memory_order.row_major then t.domain.dim(1).size else t.domain.dim(2).size) : c_int, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dtpqrt for the type c_double.
 */
inline proc tpqrt(matrix_order : lapack_memory_order, l : c_int, nb : c_int, a : [] c_double, b : [] c_double, t : [] c_double): c_int{
  return LAPACKE_dtpqrt(matrix_order, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, l, nb, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ctpqrt for the type lapack_complex_float.
 */
inline proc tpqrt(matrix_order : lapack_memory_order, l : c_int, nb : c_int, a : [] complex(64), b : [] complex(64), t : [] complex(64)): c_int{
  return LAPACKE_ctpqrt(matrix_order, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, l, nb, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ztpqrt for the type lapack_complex_double.
 */
inline proc tpqrt(matrix_order : lapack_memory_order, l : c_int, nb : c_int, a : [] complex(128), b : [] complex(128), t : [] complex(128)): c_int{
  return LAPACKE_ztpqrt(matrix_order, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, l, nb, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_stpqrt2 for the type c_float.
 */
inline proc tpqrt2(matrix_order : lapack_memory_order, l : c_int, a : [] c_float, b : [] c_float, t : [] c_float): c_int{
  return LAPACKE_stpqrt2(matrix_order, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dtpqrt2 for the type c_double.
 */
inline proc tpqrt2(matrix_order : lapack_memory_order, l : c_int, a : [] c_double, b : [] c_double, t : [] c_double): c_int{
  return LAPACKE_dtpqrt2(matrix_order, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ctpqrt2 for the type lapack_complex_float.
 */
inline proc tpqrt2(matrix_order : lapack_memory_order, l : c_int, a : [] complex(64), b : [] complex(64), t : [] complex(64)): c_int{
  return LAPACKE_ctpqrt2(matrix_order, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ztpqrt2 for the type lapack_complex_double.
 */
inline proc tpqrt2(matrix_order : lapack_memory_order, l : c_int, a : [] complex(128), b : [] complex(128), t : [] complex(128)): c_int{
  return LAPACKE_ztpqrt2(matrix_order, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, l, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_stprfb for the type c_float.
 */
inline proc tprfb(matrix_order : lapack_memory_order, side : string, trans : string, direct : string, storev : string, l : c_int, v : [] c_float, t : [] c_float, a : [] c_float, b : [] c_float): c_int{
  return LAPACKE_stprfb(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, ascii(direct) : c_char, ascii(storev) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, (t.domain.dim(1).size) : c_int, l, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dtprfb for the type c_double.
 */
inline proc tprfb(matrix_order : lapack_memory_order, side : string, trans : string, direct : string, storev : string, l : c_int, v : [] c_double, t : [] c_double, a : [] c_double, b : [] c_double): c_int{
  return LAPACKE_dtprfb(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, ascii(direct) : c_char, ascii(storev) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, (t.domain.dim(1).size) : c_int, l, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ctprfb for the type lapack_complex_float.
 */
inline proc tprfb(matrix_order : lapack_memory_order, side : string, trans : string, direct : string, storev : string, l : c_int, v : [] complex(64), t : [] complex(64), a : [] complex(64), b : [] complex(64)): c_int{
  return LAPACKE_ctprfb(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, ascii(direct) : c_char, ascii(storev) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, (t.domain.dim(1).size) : c_int, l, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ztprfb for the type lapack_complex_double.
 */
inline proc tprfb(matrix_order : lapack_memory_order, side : string, trans : string, direct : string, storev : string, l : c_int, v : [] complex(128), t : [] complex(128), a : [] complex(128), b : [] complex(128)): c_int{
  return LAPACKE_ztprfb(matrix_order, ascii(side) : c_char, ascii(trans) : c_char, ascii(direct) : c_char, ascii(storev) : c_char, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(1).size else b.domain.dim(2).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, (t.domain.dim(1).size) : c_int, l, v, (v.domain.dim(2).size) : c_int, t, (t.domain.dim(2).size) : c_int, a, (a.domain.dim(2).size) : c_int, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_ssysv_rook for the type c_float.
 */
inline proc sysv_rook(matrix_order : lapack_memory_order, uplo : string, a : [] c_float, ipiv : [] c_int, b : [] c_float): c_int{
  return LAPACKE_ssysv_rook(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_dsysv_rook for the type c_double.
 */
inline proc sysv_rook(matrix_order : lapack_memory_order, uplo : string, a : [] c_double, ipiv : [] c_int, b : [] c_double): c_int{
  return LAPACKE_dsysv_rook(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_csysv_rook for the type lapack_complex_float.
 */
inline proc sysv_rook(matrix_order : lapack_memory_order, uplo : string, a : [] complex(64), ipiv : [] c_int, b : [] complex(64)): c_int{
  return LAPACKE_csysv_rook(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

/* 
Wrapped procedure of LAPACKE_zsysv_rook for the type lapack_complex_double.
 */
inline proc sysv_rook(matrix_order : lapack_memory_order, uplo : string, a : [] complex(128), ipiv : [] c_int, b : [] complex(128)): c_int{
  return LAPACKE_zsysv_rook(matrix_order, ascii(uplo) : c_char, (a.domain.dim(1).size) : c_int, (if matrix_order == lapack_memory_order.row_major then b.domain.dim(2).size else b.domain.dim(1).size) : c_int, a, (a.domain.dim(2).size) : c_int, ipiv, b, (b.domain.dim(2).size) : c_int);
}

} // ChaLAPACK
