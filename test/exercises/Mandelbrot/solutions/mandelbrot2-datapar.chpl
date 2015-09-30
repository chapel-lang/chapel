// mandelbrot2-datapar.chpl: Solution for data parallel implementation
//

use MPlot;

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
  // The set of indices over which the image is defined.  Note that
  // 0..#n means "a range with n indices starting at 0", i.e., 0..n-1
  //
  var ImgSpace = {0..#rows, 0..#cols};

  //
  // An array to store the resulting Image.
  //
  var Image: [ImgSpace] int;

  //
  // Compute the image, in parallel
  //
  for (i,j) in ImgSpace do
    Image[i,j] = compute(i,j);
  //
  // OR:
  //
  //  Image = compute(ImgSpace);

  //
  // Plot the image
  //
  plot(Image);
}

//
// Compute the pixel value as described in the handout
//
proc compute(x, y) {
  const c = mapImg2CPlane(x, y);  // convert the pixel coordinates to a complex value

  var z: complex;
  for i in 1..maxSteps {
    z = z*z + c;
    if (abs(z) > 2.0) then
      return i;
  }

  return 0;			
}

//
// Map an image coordinate to a point in the complex plane.
// Image coordinates are (row, col), with row 0 at the top.
//
proc mapImg2CPlane(row, col) {
  const (rmin, rmax) = (-1.5, .5);
  const (imin, imax) = (-1i, 1i);

  return ((rmax - rmin) * col / cols + rmin) +
         ((imin - imax) * row / rows + imax);
}

