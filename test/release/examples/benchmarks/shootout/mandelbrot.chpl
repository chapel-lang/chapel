/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Jacob Nelson, Lydia Duncan, Brad Chamberlain, and 
   Ben Harshbarger
*/

use DynamicIters;

config const n = 200,              // the problem size
             maxIter = 50,         // the maximum # of iterations
             limit = 4.0,          // the limit before quitting
             chunkSize = 1;        // the chunk size of the dynamic iterator

param bitsPerElt = 8;              // the # of bits to store per element
type eltType = uint(bitsPerElt);   // the element type to store


proc main() {
  const ydim = 0..#n,                          // the y dimension
        xdim = 0..#divceilpos(n, bitsPerElt);  // the compacted x dimension

  var image : [ydim, xdim] eltType;            // the bitmap image

  forall y in dynamic(ydim, chunkSize) {       // for all rows...
    for xelt in xdim {                         //   for each column element...

      var mask: eltType = 0;                   // zero out the mask

      for off in 0..#bitsPerElt {              // for each bit in the element
        const x = xelt*bitsPerElt + off;       // compute its logical location

        const Cr = 2.0*x/n - 1.5;              // floating point values
        const Ci = 2.0*y/n - 1.0;              //   to compute with
        var Zr, Zi, Tr, Ti = 0.0;

        for 1..maxIter {                       // for the max # of iterations
          if (Tr + Ti > limit) then            // if we haven't converged
            break;
          
          Zi = 2.0*Zr*Zi + Ci;                 // update floating point values
          Zr = Tr - Ti + Cr;
          Tr = Zr**2;
          Ti = Zi**2;
        }

        mask <<= 1;                            // shift the mask
        if (Tr + Ti <= limit) then             // if below the limit,
          mask |= 0x1;                         // set the low bit to 1
      }

      image[y, xelt] = mask;                   // store the computed element
    }
  }

  //
  // get a lock-free writer channel on 'stdout'
  //
  var w = openfd(1).writer(iokind.native, locking=false);

  w.writef("P4\n");                            // write the file header
  w.writef("%i %i\n", n, n);

  w.write(image);                              // write out the image
}
