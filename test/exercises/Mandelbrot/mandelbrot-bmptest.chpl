// mandelbrot.chpl: Starting point for Chapel hands-on Mandelbrot exercises
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
// Dimensions of image file.  'config' consts and vars can be set on
// the execution command-line using --rows=401, for example.
//
config const rows = 201,
             cols = rows;

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
  // Use a forall loop to assign the array elements in parallel
  //
  forall (i,j) in ImgSpace do
    Image[i,j] = i+j;

  //
  // Plot the image
  //
  plot(Image);
}


