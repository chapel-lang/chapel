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
import DSIUtil._computeChunkStartEnd;

/* Command line arguments. */
config const in_array : string;               /* name of array to read in */
config const in_image : string;                /* name of PNG file to read */
config const outname : string;               /* name of PNG file to write at the end */
config const window_size : real;                  /* the desired area of the neighborhood (in meters^2) */
config const dx : real;                      /* the resolution of the raster image (in meters) */
config const bigInput = false;
config const inputSize = -1;
config const report_times = true;
config const report_detail_times = false;
config const report_checksum = false;
config const write_data = false;
config const verbose_gpu = false;
config const numWorkers = 1;
config const numChunksPerWorker = 1;

extern proc printf(s...);

//var bs = 1;
//var be = 5;

proc convolve_and_calculate(Array: [] real(32), const in centerPoints : ?, locL : ?, locC : ?, locR : ?, ref Output: [] real(64), t: stopwatch) : [] {

  param bs = 0;
  param be = 4;

  var first_point = centerPoints.first[0];
  var last_point = centerPoints.last[0];

  if verbose_gpu then startVerboseGpu();

  @assertOnGpu
  foreach i in centerPoints.dim(1) {
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

  var timeBeforeCoforall: real;
  t.start();

  const radius = (sqrt(window_size) / 2) : int;
  const nx = (radius / dx) : int(16);
  writeln("Distance circle has a radius of ", nx, " points.");
  writeln("inputSize = ", inputSize);

  var (LeftMask, CenterMask, RightMask, Mask_Size) = create_distance_mask(radius, dx, nx);

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

  var Array : [0..#5,0..#y,0..#x] real(32);

  // Read in array
  if inputSize == -1 {
    var f = open(in_array, ioMode.r);
    var r = f.reader(deserializer=new binaryDeserializer(), locking=false);
    for i in 0..#5 {
      for j in 0..#x {
        for k in 0..#y {
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

  const Inner = ImageSpace.expand(-offset);
  const myTargetLocales = reshape(Locales, {1..Locales.size, 1..1});
  const D = blockDist.createDomain(Inner, targetLocales=myTargetLocales);
  var OutputHost : [D] real(64); // D

  if report_times {
    timeBeforeCoforall = t.elapsed();
    writeln("Elapsed time at start of coforall loop: ", timeBeforeCoforall,
            " seconds.");
  }

  writeln("Starting coforall loop.");

  // TODO the triple coforalls could be in a standalone iterator
  coforall loc in Locales do on loc {
    var locOutput = D.localSubdomain();
    var locInput = locOutput.expand(offset);
    var locImage: [0..#5, (...locInput.dims())] real(32);

    locImage = Array[locImage.domain];

    coforall (gpuId, gpu) in zip(here.gpus.domain, here.gpus) do on gpu {
      // Create distance mask
      const locLeftMaskDomain = LeftMask.domain;
      const locCenterMaskDomain = CenterMask.domain;
      const locRightMaskDomain = RightMask.domain;

      coforall taskId in 0..#numWorkers {
        const workerId = gpuId*numWorkers + taskId;

        var commTimer : stopwatch;
        var convolveTimer : stopwatch;

        for chunkId in 0..#numChunksPerWorker {
          const globalChunkId = workerId*numChunksPerWorker + chunkId;

          var (outRowStart, outRowEnd) =
             _computeChunkStartEnd(locOutput.dim(0).size,
                                   loc.gpus.size*numWorkers*numChunksPerWorker,
                                   globalChunkId+1);
          // offset for 1-based support function (_computeChunkStartEnd) and
          // start offset
          outRowStart += locOutput.dim(0).low-1;
          outRowEnd += locOutput.dim(0).low-1;

          const MyInner = {outRowStart..outRowEnd, locOutput.dim(1)};
          var OutputGpu: [MyInner] real(64) = noinit; // D

          const MyInnerExpanded = MyInner.expand(offset);
          const MyArrayDom = {0..#5, (...MyInnerExpanded.dims())};
          var locArray : [MyArrayDom] Array.eltType = noinit;

          commTimer.start();
          locArray = locImage[MyArrayDom];
          commTimer.stop();
          if write_data then writeln(locArray);

          convolveTimer.start();
          convolve_and_calculate(locArray, MyInner, locLeftMaskDomain,
              locCenterMaskDomain, locRightMaskDomain,
              OutputGpu, t);
          convolveTimer.stop();

          commTimer.start();
          OutputHost[MyInner] = OutputGpu;
          commTimer.stop();
        }

        if report_detail_times {
          writef("(Locale %i, GPU %i, Task %i) Convolve time: %r\n", here.id,
                 gpuId, taskId, convolveTimer.elapsed());
          writef("(Locale %i, GPU %i, Task %i) Comm     time: %r\n", here.id,
                 gpuId, taskId, commTimer.elapsed());
        }
      }
    }
  }

  if report_times {
    const now = t.elapsed();
    writeln("Elapsed time at the end of coforall loop: ", now, " seconds.");
    writeln("Convolve time: ", now-timeBeforeCoforall);
  }

  if report_checksum {
      writeln("Checksum: ", + reduce OutputHost);
      if write_data then writeln(OutputHost);
  }

}
