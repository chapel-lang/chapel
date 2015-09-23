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
  // This could have equivalently been written:
  //
  // forall ij in ImgSpace do      // ij is a 2-tuple reprsenting an index from ImgSpace
  //   Image[ij] = ij(1) + ij(2);  // arrays support tuple indices; tuples support detupling

  //
  // Here are some more things that may be helpful:
  //
  // var r = 1.2;            // this is a real floating point variable
  // var i = 3.4i;           // this is an imag[inary] floating point variable
  // var c = 1.2 + 3.4i;     // this is a complex variable
  // var rt = (1.2, 3.4);    // this is a 2-tuple of real values
  // var it = (5.6i, 7.8i);  // this is a 2-tuple of imaginary values
  // var rit = (1.2, 7.8i);  // this is a 2-tuple of (real, imag) type
  // var (r2,i2) = rit;      // tuples may also be de-tupled this way
  //
  // writeln("c is: ", c);   // most anything can be written out using write[ln]()

  //
  // Plot the image
  //
  plot(Image);
}
