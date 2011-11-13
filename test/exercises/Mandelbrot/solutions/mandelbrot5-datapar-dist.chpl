// mandelbrot5-datapar-dist.chpl: Distributed data-parallel implementation
//

use MPlot, BlockDist;

//
// Dimensions of image file
//
config const rows = 201,
             cols = rows;

//
// Maximum number of steps to iterate
//
config const maxSteps = 50;


proc main() {
  //
  // domains and arrays representing the number of steps taken in the
  // calculation (effectively, the image).  Note that Block is
  // actually not a very good choice for Mandelbrot; Cyclic should be
  // better.
  //
  var LocImgSpace = [0..#rows, 0..#cols];
  var ImgSpace = LocImgSpace dmapped Block(rank=2, boundingBox=LocImgSpace);
  var NumSteps: [ImgSpace] int;

  //
  // Compute the image
  //
  NumSteps = compute(ImgSpace);
  //
  // OR:
  //
  //  forall ij in ImgSpace do
  //    NumSteps(ij) = compute(ij);

  // Plot the image
  plot(NumSteps);
}

//
// Compute the pixel value as described in the handout
//
proc compute((x, y)) {
  const c = mapImg2CPlane((x, y));
  
  var z: complex;
  for i in 1..maxSteps {
    z = z*z + c;
    if (abs(z) > 2.0) then
      return i;
  }
  return 0;			
}


// Map an image coordinate to a point in the complex plane.
// Image coordinates are (row, col), with row 0 at the top.
proc mapImg2CPlane((row, col)) {
  const (rmin, rmax) = (-1.5, .5);
  const (imin, imax) = (-1i, 1i);

  return ((rmax - rmin) * col / cols + rmin) +
         ((imin - imax) * row / rows + imax);
}

