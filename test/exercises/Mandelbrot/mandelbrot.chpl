// mandelbrot.chpl: Starting point for Chapel tutorial Mandelbrot exercises
//
// compile command: chpl mandelbrot.chpl -o mandelbrot
// execute command: ./mandelbrot
//

//
// Use a helper module to create PPM/PGM/PBM image files (use
// ./mandelbrot --help to see configuration options)
//
use MPlot;

//
// Dimensions of image file
//
config const rows = 201,
             cols = rows;

proc main() {
  // The set of indices over which the image is defined.
  var ImgSpace = [0..#rows, 0..#cols];

  //
  // A dummy array just to show that this works.
  //
  var A: [ImgSpace] int = [(i,j) in ImgSpace] i + j;

  // Plot the image
  plot(A);
}
