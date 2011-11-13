// mandelbrot-framework.chpl
//
// Starting point for Chapel tutorial exercises
//
// compile command: chpl mandelbrot.chpl -o mand
// execute command: mand
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
