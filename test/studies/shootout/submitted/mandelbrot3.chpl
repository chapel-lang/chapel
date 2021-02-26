/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   derived from the Swift version by Ralph Ganszky
     and the Chapel version by Nelson et al.
*/

use DynamicIters, IO;

config const n = 200,             // image size in pixels (n x n)
             maxIter = 50,        // max # of iterations per pixel
             limit = 4.0,         // per-pixel convergence limit
             chunkSize = 1;       // dynamic iterator's chunk size

param bitsPerElt = 8;             // # of bits to store per array element
type eltType = uint(bitsPerElt);  // element type used to store the image


proc main() {
  const xsize = divceilpos(n, bitsPerElt),  // the compacted x dimension
        imgSpace = {0..#n, 0..#xsize};      // the compacted image size

  var image : [imgSpace] eltType,           // the compacted image
      xval, yval: [0..#n] real;             // pre-computed (x,y) values

  // precompute (x, y) values from the complex plane
  const inv = 2.0 / n;
  forall i in 0..#n {
    xval[i] = inv*i - 1.5;
    yval[i] = inv*i - 1.0;
  }

  // compute the image
  forall (y, xelt) in dynamic(imgSpace, chunkSize) {
    const xbase = xelt*bitsPerElt,
          cr = (xval[xbase+0], xval[xbase+1], xval[xbase+2], xval[xbase+3],
                xval[xbase+4], xval[xbase+5], xval[xbase+6], xval[xbase+7]),
          ci = yval[y];

    var Zr, Zi, Tr, Ti: bitsPerElt*real;

    for 1..maxIter {                      // for the max # of iterations
      Zi = 2.0*Zr*Zi + ci;                // update Z and T
      Zr = Tr - Ti + cr;
      Tr = Zr*Zr;
      Ti = Zi*Zi;

      if (Tr + Ti > limit) then           // break out if we haven't converged
        break;
    }

    // store 'bitsPerElt' pixels compactly into the final image
    var pixval: eltType;
    for param i in 0..<bitsPerElt do
      if (Tr(i) + Ti(i) <= limit) then      // if 'C' is within the limit,
        pixval |= 0x1 << (bitsPerElt-i-1);  // turn the corresponding pixel on

    image[y, xelt] = pixval;
  }

  // Get a lock-free writer channel on 'stdout'
  var w = openfd(1).writer(iokind.native, locking=false);

  // Write the file header and the image array.
  w.writef("P4\n");
  w.writef("%i %i\n", n, n);
  w.write(image);
}

//
// Helper function to compare an 8-tuple and a singleton
//
inline proc >(xs, y) {
  for x in xs do
    if x <= y then
      return false;
  return true;
}
