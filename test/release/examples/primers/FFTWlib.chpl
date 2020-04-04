// FFTW

/*
  Example usage of the :mod:`FFTW` module in Chapel. This particular file
  demonstrates the single-threaded version of the code. In order to initialize
  FFTW for multithreaded support, see the :mod:`FFTW` documentation.

  If FFTW is in your standard include/library paths, compile this code
  using:

  .. code-block:: text

    chpl testFFTW.chpl

  Otherwise, use the following (where ``$FFTW_DIR`` points to your
  FFTW installation):

  .. code-block:: text

    chpl testFFTW.chpl -I$FFTW_DIR/include -L$FFTW_DIR/lib

  The :mod:`FFTW` module uses the FFTW3 API and currently just implements the
  basic, double-precision interface.  We will assume that the reader
  is familiar with using FFTW; more details are at http://www.fftw.org.

  The code computes a series of 1D, 2D and 3D transforms, exercising
  the complex<->complex and real<->complex transforms (both in- and
  out-of-place). The output of the code should be a series of small
  numbers ( ``<= 10^-13`` ); see ``testFFTW.good`` for example values, though
  it is possible that your values may differ in practice.

  The input data for these tests is in ``arr{1,2,3}d.dat``. The format of
  these files is documented below.
*/

use FFTW;

//
// Config to print out the error values.  Note that values may differ between
// platforms, so turn off for testing.
//
config const printErrors = true;

//
// If we don't print out error values, we'll print a success/failure
// message.  This is the epsilon value against which success should
// be measured.
//
config const epsilon = 10e-13;

//
// Performs the tests and cleanup.
//
proc main() {
  testAllDims();
  cleanup();
}

//
// A helper function that invokes the test for each rank.
//
proc testAllDims() {
  for param d in 1..3 {
    writeln(d, "D");
    runtest(d, "arr"+d:string+"d.dat");
  }
}

//
// This is the main test code, parametrized by the number of dimensions,
// ``ndim``.  ``fn`` is the filename of the file that contains the
// test data.
//
proc runtest(param ndim : int, fn : string) {
  var dims : ndim*int(32);

/* We define a number of different domains below, corresponding to
   complex input/output arrays, real (input)->complex (output)
   out-of-place arrays, and real<->complex in-place arrays.

   The domains are as follows:

     * ``D``: for complex<->complex transforms. This domain is also used
       for the real array in a ``real->complex`` out-of-place transform.

     * ``cD`` : for the complex array in a real<->complex
       out-of-place transform.

     * ``rD`` : for the real array in a real<->complex in-place transform.
       This includes the padding needed by the in-place transform.
       ``D`` is a sub-domain of this, and can be used to extract the real
       array, without padding.

     * ``reD, imD`` : Utility domains that access the real/complex parts
       of the complex array in a real<->complex in-place transform.
*/
  var D : domain(ndim);
  var rD,cD,reD,imD : domain(ndim,int,true);


/* Read in the arrays from the file below. ``A`` and ``B`` are the
   arrays that will be used in the FFTW calls, while ``goodA`` and
   ``goodB`` store the true values.

   Note that ``goodA`` is all real, allowing us to reuse the same
   array for the real<->complex tests.

   The file format is as follows (all little-endian):

     - ``[ndim] int32`` : dimensions of the array

     - ``[narr] real64`` : the real components of ``A`` (imaginary components
       are all zero). ``narr = N_1*N_2*..*N_ndim`` (product of dimensions)

     - ``[narr] complex128`` : the components of ``B``
       (stored as real-imaginary pairs).

     The arrays are all stored in row-major order, as in C.
*/
  var A,B,goodA,goodB : [D] complex(128);
  {
    use IO;
    var f = open(fn,iomode.r).reader(kind=iokind.little);

    // Read in dimensions
    for d in dims {
      f.read(d);
    }

//
// Set the domain ``D``, handling the different rank cases.
//
    select ndim {
      when 1 do D = 0.. #dims(0);
      when 2 do D = {0.. #dims(0), 0.. #dims(1)};
      when 3 do D = {0.. #dims(0), 0.. #dims(1), 0.. #dims(2)};
    }

    // Read in the arrays
    for val in goodA {
      f.read(val.re);
      val.im = 0;
    }
    for val in goodB {
      f.read(val);
    }
    f.close();
    writeln("Data read...");
  }

/* Now set the remaining domains.

   Refer to the FFTW documentation on the storage order for the
   in-place transforms (Sec 2.4 and 4.3.4).
*/
  select ndim {
    when 1 {
      var ldim = dims(0)/2 + 1;
      //
      // Domains for real FFT
      //
      rD = 0.. #(2*ldim); // Padding to do in-place transforms
      cD = 0.. #ldim;
      //
      // Define domains to extract the real and imaginary parts for
      // in-place transforms
      //
      reD = rD[0..(2*ldim-1) by 2];
      imD = rD[1..(2*ldim-1) by 2];
    }
    when 2 {
      //
      // Domains for real FFT
      //
      var ldim = dims(1)/2+1;
      rD = {0.. #dims(0),0.. #(2*ldim)}; // Padding to do in-place transforms
      cD = {0.. #dims(0),0.. #ldim};
      //
      // Define domains to extract the real and imaginary parts for
      // in-place transforms
      //
      reD = rD[..,0..(2*ldim-1) by 2];
      imD = rD[..,1..(2*ldim-1) by 2];
    }
    when 3 {
      //
      // Domains for real FFT
      //
      var ldim = dims(2)/2+1;
      rD = {0.. #dims(0),0.. #dims(1),0.. #(2*ldim)}; // Padding to do in-place transforms
      cD = {0.. #dims(0),0.. #dims(1),0.. #ldim};
      //
      // Define domains to extract the real and imaginary parts for
      // in-place transforms
      //
      reD = rD[..,..,0..(2*ldim-1) by 2];
      imD = rD[..,..,1..(2*ldim-1) by 2];
    }
  }

// FFTW does not normalize inverse transforms, so just compute the
// normalization constant.

  var norm = * reduce dims;

/* We start the FFT tests below. The structure is the same :

     - Define plans for forward and reverse transforms.
     - Execute forward transform ``A -> B``.
     - Compare with ``goodB``.
     - Execute reverse transform ``B -> A`` and normalize.
     - Compare with ``goodA``.
     - Cleanup plans.
*/

/*
.. primers-fftw-complex-complex-out

   complex<->complex out-of-place transform
   ----------------------------------------

   Unlike the basic FFTW interface, we do not have specific 1D/2D/3D
   planner routines.  For the complex <-> complex case, the
   dimensions of the array are inferred automatically.
*/
  var fwd = plan_dft(A, B, FFTW_FORWARD, FFTW_ESTIMATE);
  var rev = plan_dft(B, A, FFTW_BACKWARD, FFTW_ESTIMATE);

  // Test forward and reverse transform
  A = goodA;
  execute(fwd);
  printcmp(B,goodB);
  execute(rev);
  A /= norm;
  printcmp(A,goodA);
  destroy_plan(fwd);
  destroy_plan(rev);

/*
.. primers-fftw-complex-complex-in

  complex <-> complex in-place transform
  --------------------------------------

  This is the same calling sequence as above, but using the
  in-place versions of the routine.
*/
  fwd = plan_dft(A, FFTW_FORWARD, FFTW_ESTIMATE);
  rev = plan_dft(A, FFTW_BACKWARD, FFTW_ESTIMATE);

  // Test forward and reverse transform
  A = goodA;
  execute(fwd);
  printcmp(A,goodB);
  execute(rev);
  A /= norm; // FFTW does an unnormalized transform
  printcmp(A,goodA);
  destroy_plan(fwd);
  destroy_plan(rev);

/*
.. primers-fftw-real-complex-out

  real <-> complex out-of-place transform
  ---------------------------------------

  As with FFTW, these use ``r2c`` and ``c2r`` suffixes to define the
  direction of the transform.

  ``plan_dft_r2c`` and ``plan_dft_c2r`` are overloaded; for the
  out-of-place transforms, they infer the dimensions from the
  sizes of the arrays passed in.
*/
  var rA : [D] real(64); // No padding for an out-of-place transform
  var cB : [cD] complex(128);
  fwd = plan_dft_r2c(rA,cB,FFTW_ESTIMATE);
  rev = plan_dft_c2r(cB,rA,FFTW_ESTIMATE);
  rA[D] = goodA.re;
  execute(fwd);
  printcmp(cB,goodB[cD]);
  execute(rev);
  rA /= norm;
  printcmp(rA[D],goodA.re);
  destroy_plan(fwd);
  destroy_plan(rev);

/*
.. primers-fftw-real-complex-in

  real <-> complex in-place transform
  -----------------------------------

  In this case, the first argument to the planning routines is the
  domain of the *real* array WITHOUT padding (in both ``r2c`` and
  ``c2r`` cases). This breaks the ambiguity of whether the leading
  dimension of the real array is even or odd.

  This design decision was motivated by the fact that the user has
  likely already defined a domain to extract the unpadded real
  array from the fully padded array (or that such a domain is
  intrinsically useful).

  For both the ``r2c`` and ``c2r`` transforms, a real array is passed in.
*/
  var rA2 : [rD] real(64);
  fwd = plan_dft_r2c(D,rA2,FFTW_ESTIMATE);
  rev = plan_dft_c2r(D,rA2,FFTW_ESTIMATE);
  rA2[D] = goodA.re;
  execute(fwd);
  printcmp(rA2[reD],goodB[cD].re); // Check the real and complex parts separately.
  printcmp(rA2[imD],goodB[cD].im);
  execute(rev);
  rA2 /= norm;
  printcmp(rA2[D],goodA.re);
  destroy_plan(fwd);
  destroy_plan(rev);


/*
   This is another real <-> complex in-place transform, except we pass
   in a complex array instead of a real array. This can get a little
   ugly, so we just reverse engineer the previous case.

   Note that we reuse the ``rA2`` and ``cB`` arrays, since they're the
   correct sizes.
*/
  fwd = plan_dft_r2c(D,cB,FFTW_ESTIMATE);
  rev = plan_dft_c2r(D,cB,FFTW_ESTIMATE);

  // Zero out rA2 to ensure that anything extraneous in the padding
  // doesn't get passed in.
  rA2 = 0.0;
  rA2[D] = goodA.re;
  cB.re = rA2[reD]; // Fill the complex array
  cB.im = rA2[imD];
  execute(fwd);
  printcmp(cB.re,goodB[cD].re); // Check the real and complex parts separately.
  printcmp(cB.im,goodB[cD].im);
  execute(rev);
  cB /= norm;

  // Pull everything back out to the real array for simplicity
  rA2 = 0.0;
  rA2[reD] = cB.re;
  rA2[imD] = cB.im;
  printcmp(rA2[D],goodA.re);
  destroy_plan(fwd);
  destroy_plan(rev);
}

//
// Utility function to print the maximum absolute deviation between
// values computed by this code, and "truth". The true values are
// computed using Mathematica v10.
//
proc printcmp(x, y) {
  var err = max reduce abs(x-y);
  if (printErrors) then
    writeln(err);
  else {
    if err < epsilon then
      writeln("SUCCESS: error below threshold");
    else
      writeln("FAILURE: error (", err, ") exceeds epsilon (", epsilon, ")");
  }
}
