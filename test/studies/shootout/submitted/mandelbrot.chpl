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
  const ydim = 0..#n,                          // the image's y dimension
        xdim = 0..#divceilpos(n, bitsPerElt);  // the compacted x dimension

  var image : [ydim, xdim] eltType;            // the compacted bitmap image

  forall y in dynamic(ydim, chunkSize) {       // for all rows...
    for xelt in xdim {                         //   for each column element...

      var buff: eltType;                       // a single-element pixel buffer

      for off in 0..#bitsPerElt {              // for each bit in the buffer
        const x = xelt*bitsPerElt + off;       // compute its logical column

        const Cr = 2.0*x/n - 1.5;              // the (x,y) pixel as a complex
        const Ci = 2.0*y/n - 1.0;              //   (real, imag) value 'C'
        var Zr, Zi, Tr, Ti = 0.0;              // 'complex' helper values

        for 1..maxIter {                       // for the max # of iterations
          if (Tr + Ti > limit) then            // if we haven't converged
            break;
          
          Zi = 2.0*Zr*Zi + Ci;                 // update Z and T
          Zr = Tr - Ti + Cr;
          Tr = Zr**2;
          Ti = Zi**2;
        }

        buff <<= 1;                            // shift the pixel buffer
        if (Tr + Ti <= limit) then             // if 'C' is within the limit,
          buff |= 0x1;                         //   turn the low pixel on
      }

      image[y, xelt] = buff;                   // store the pixel buffer
    }
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
