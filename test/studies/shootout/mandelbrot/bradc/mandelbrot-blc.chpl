/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

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
  const xdim = 0..#divceilpos(n, bitsPerElt),  // the compacted x dimension
        inv = 2.0 / n,                         // precompute 2/n
        xvals: [0..#n] real = [i in 0..#n] inv*i - 1.5,  // precompute coords
        yvals: [0..#n] real = [i in 0..#n] (inv*i - 1.0);

  var image : [0..#n, xdim] eltType;           // the compacted bitmap image

  forall ((y, xelt), pix) in zip(dynamic(image.domain, chunkSize), image) {

    // TODO: split x/y dimensions in forall loop and hoist the following?
    const ci = yvals[y];
    const xbase = xelt*bitsPerElt;
    // TODO: express in terms of bitsPerElt?
    const cr = (xvals[xbase+0], xvals[xbase+1], xvals[xbase+2], xvals[xbase+3],
                xvals[xbase+4], xvals[xbase+5], xvals[xbase+6], xvals[xbase+7]);

    var Zr, Zi, Tr, Ti: 8*real;

    for 1..maxIter {                       // for the max # of iterations
      Zi = 2.0*Zr*Zi + ci;           // update Z and T
      Zr = Tr - Ti + cr;
      Tr = Zr*Zr;
      Ti = Zi*Zi;

      if (Tr + Ti > limit) then          // if we haven't converged
        break;
    }

    for param i in 0..bitsPerElt-1 {
      if (Tr(i) + Ti(i) <= limit) then       // if 'C' is within the limit,
        pix |= 0x1 << (7-i);                //   turn the low pixel on
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

inline proc +(cr, ci) {
  compilerWarning((cr.type:string) + "," + (ci.type:string));
  return (cr(0)+ci, cr(1)+ci, cr(2)+ci, cr(3)+ci, 
          cr(4)+ci, cr(5)+ci, cr(6)+ci, cr(7)+ci);
}

inline proc >(x, y) {
  for param i in 0..bitsPerElt-1 do
    if x(i) <= y then
      return false;
  return true;
}
