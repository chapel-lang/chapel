//use png;
//use mask_iter2;
use distance_mask;
use distance_iter;
use GpuDiagnostics;
use GPU;
use BlockDist;
use Time;
//use IO_Module;
use IO;
use AutoMath;
use LinearAlgebra;

/* Command line arguments. */
config const in_array : string;               /* name of array to read in */
config const in_image : string;                /* name of PNG file to read */
config const outname : string;               /* name of PNG file to write at the end */
config const window_size : real;                  /* the desired area of the neighborhood (in meters^2) */
config const dx : real;                      /* the resolution of the raster image (in meters) */
config const bigInput = false;
config const inputSize = -1;
config const report_times = true;
config const report_checksum = false;
config const write_data = false;
config const verbose_gpu = false;

//var bs = 1;
//var be = 5;

proc convolve_and_calculate(Array: [] real(32), const in centerPoints : ?, locL : ?, locC : ?, locR : ?, ref Output: [] real(64), t: stopwatch) : [] {

  param bs = 1;
  param be = 5;

  var first_point = centerPoints.first[1];
  var last_point = centerPoints.last[1];

  writeln("Before gpu ", centerPoints.dim(0));
  if verbose_gpu then startVerboseGpu();

  
  @assertOnGpu
  foreach i in centerPoints.dim(0) {
    // Only these need to be real(64) in order to guarantee non-negative outputs
    var tmpLL : real(64) = 0;
    var tmpLC : real(64) = 0;
    var tmpLR : real(64) = 0;
    var tmpCC : real(64) = 0;
    var tmpCR : real(64) = 0;
    var tmpRR : real(64) = 0;

    calc_distance(Array, locL, locL, tmpLL, bs, be, i, first_point);
    calc_distance(Array, locL, locC, tmpLC, bs, be, i, first_point);
    calc_distance(Array, locL, locR, tmpLR, bs, be, i, first_point);
    calc_distance(Array, locC, locC, tmpCC, bs, be, i, first_point);
    calc_distance(Array, locC, locR, tmpCR, bs, be, i, first_point);
    calc_distance(Array, locR, locR, tmpRR, bs, be, i, first_point);

    Output.localAccess[first_point,i] = (tmpLL + tmpCC + tmpRR + 2*(tmpLC + tmpLR + tmpCR)); // / (Mask_Size**2);

    //var Output = (tmpLL + tmpCC + tmpRR + 2*(tmpLC + tmpLR + tmpCR));
    var prev = tmpCC + tmpRR + 2*tmpCR;

    for j in (first_point+1)..last_point do {

      tmpLL = 0;
      tmpLC = 0;
      tmpLR = 0;
      tmpCR = 0;
      tmpRR = 0;

      calc_distance(Array, locL, locL, tmpLL, bs, be, i, j);
      calc_distance(Array, locL, locC, tmpLC, bs, be, i, j);
      calc_distance(Array, locL, locR, tmpLR, bs, be, i, j);
      calc_distance(Array, locC, locR, tmpCR, bs, be, i, j);
      calc_distance(Array, locR, locR, tmpRR, bs, be, i, j);

      /* Global GPU memory access causing ~20% slowdown? */
      var current = tmpRR + 2*(tmpLR + tmpCR);
      Output.localAccess[j,i] = (prev + current); // / (Mask_Size**2);

      //Output = prev + current;
      prev = prev + current - tmpLL - 2*(tmpLC + tmpLR);
    }

  }

  if verbose_gpu then stopVerboseGpu();

}

proc main(args: [] string) {

  var t : stopwatch;
  t.start();

  const radius = (sqrt(window_size) / 2) : int;
  const nx = (radius / dx) : int(16);
  writeln("Distance circle has a radius of ", nx, " points.");
  writeln("inputSize = ", inputSize);

  var (LeftMask, CenterMask, RightMask, Mask_Size) = create_distance_mask(radius, dx, nx);

  if report_times then
    writeln("Elapsed time at start of coforall loop: ", t.elapsed(), " seconds.");

  writeln("Starting coforall loop.");

  coforall loc in Locales do on loc {


    const radius = (sqrt(window_size) / 2) : int;
    const nx = (radius / dx) : int(16);
    writeln("Distance circle has a radius of ", nx, " points.");

    var x, y: int;
    if inputSize != -1 {
      x = inputSize;
      y = inputSize;
    }
    else if bigInput {
      x = 15243;
      y = 10073;
    }
    else {
      x = 1000;
      y = 1000;
    }
    const ImageSpace = {0..#y, 0..#x};


    var Array : [1..5,1..y,1..x] real(32);

    // Read in array
    if inputSize == -1 {
      var f = open(in_array, ioMode.r);
      var r = f.reader(deserializer=new binaryDeserializer(), locking=false);
      for i in 1..5 {
        for j in 1..x {
          for k in 1..y {
            var tmp : real;
            r.readBinary(tmp);
            Array[i,k,j] = tmp : real(32);
          }
        }
      }
      r.close();
    }
    else {
      use Random;
      fillRandom(Array, seed=13);
      if write_data then writeln(Array);
    }

    // Create Block distribution of interior of PNG
    const offset = nx+1; // maybe needs to be +1 to account for truncation?

    /*

    The code below can be used to use multiple locales. It is something
    that existed in the original implementation, so we are keeping it for
    now.
     
    const myTargetLocales = reshape(Locales, {1..Locales.size, 1..1});
    const D = Inner dmapped blockDist(Inner, targetLocales=myTargetLocales);

    */
    const Inner = ImageSpace.expand(-offset);
    var OutputHost : [Inner] real(64); // D

    on here.gpus[0] {

      var Output: [Inner] real(64); // D
      const locArrayDomain = Array.domain;
      const locArray : [locArrayDomain] Array.eltType = Array;

      // Create distance mask
      const locLeftMaskDomain = LeftMask.domain;
      const locCenterMaskDomain = CenterMask.domain;
      const locRightMaskDomain = RightMask.domain;

      if report_times then
        writeln("Starting convolution at ", t.elapsed(), ".");

      var t2 : stopwatch;
      t2.start();
      convolve_and_calculate(locArray, Inner, locLeftMaskDomain, locCenterMaskDomain, locRightMaskDomain, Output, t);
      t2.stop();

      if report_times then writeln("Convolve time: ", t2.elapsed());

      if report_checksum {
        on loc {
          OutputHost = Output;
          writeln("Checksum: ", + reduce OutputHost);
          if write_data then writeln(OutputHost);
        }
      }
    }
  }

  if report_times then
    writeln("Elapsed time to finish coforall loop: ", t.elapsed(), " seconds.");

}
