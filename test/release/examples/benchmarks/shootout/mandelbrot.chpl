/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Jacob Nelson, Lydia Duncan, Brad Chamberlain,
   and Ben Harshbarger
   derived from the GNU C version by Greg Buchholz
*/

use DynamicIters, IO;

config const n = 200,              // image size in pixels (n x n)
             maxIter = 50,         // max # of iterations per pixel
             limit = 4.0,          // per-pixel convergence limit
             chunkSize = 1;        // dynamic iterator's chunk size

param bitsPerElt = 8;              // # of bits to store per array element
type eltType = uint(bitsPerElt);   // element type used to store the image


proc main() {
  const xdim = 0..#divceilpos(n, bitsPerElt);  // the compacted x dimension

  var image : [0..#n, xdim] eltType;           // the compacted bitmap image

  forall (y, xelt) in dynamic(image.domain, chunkSize) { // for all elements

    var buff: eltType;                         // a single-element pixel buffer

    for off in 0..#bitsPerElt {                // for each bit in the buffer
      const x = xelt*bitsPerElt + off;         // compute its logical column

      const C = 2.0*x/n - 1.5 +                // the (x,y) pixel as a complex
               (2.0*y/n - 1.0)*1i;             //   (real, imag) value 'C'
      var Z, T: complex;                       // 'complex' helper values

      for 1..maxIter {                         // for the max # of iterations
        if (T.re + T.im > limit) then          // if we haven't converged
          break;

        Z.im = 2.0*Z.re*Z.im + C.im;           // update Z and T
        Z.re = T.re - T.im + C.re;
        T.re = Z.re**2;
        T.im = Z.im**2;
      }

      buff <<= 1;                            // shift the pixel buffer
      if (T.re + T.im <= limit) then         // if 'C' is within the limit,
        buff |= 0x1;                         //   turn the low pixel on
    }

    image[y, xelt] = buff;                   // store the pixel buffer
  }

  //
  // Get a lock-free writer channel on 'stdout', write the file header,
  // and the image array.
  //
  var w = openfd(1).writer(iokind.native, locking=false);

  w.writef("P4\n");
  w.writef("%i %i\n", n, n);
  w.write(image);
}
