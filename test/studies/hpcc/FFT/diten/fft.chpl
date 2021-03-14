//
// Use standard modules for Bit operations, Random numbers, and Timing
//
use BitOps, Random, Time, BlockDist, CyclicDist;

//
// Use shared user module for computing HPCC problem sizes
//
use HPCCProblemSize;

const radix = 4;               // the radix of this FFT implementation

const numVectors = 2;          // the number of vectors to be stored
type idxType = int(64);        // the type to use as the index to the domains
type elemType = complex(128);  // the element type of the vectors

//
// A configuration constant defining log2(problem size) -- n -- and a
// constant defining the problem size itself -- m
//
config const n = computeProblemSize(elemType, numVectors, returnLog2 = true);
const m = 2**n;

//
// The number of tasks to use per Chapel locale in parallel loops
//
config const tasksPerLocale = here.maxTaskPar;

//
// Configuration constants defining the epsilon and threshold values
// used to verify the result
//
config const epsilon = 2.0 ** -51.0,
             threshold = 16.0;

//
// Configuration constants to indicate whether or not to use a
// pseudo-random seed (based on the clock) or a fixed seed; and to
// specify the fixed seed explicitly
//
config const useRandomSeed = false,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 314159265;

//
// Configuration constants to control what's printed -- benchmark
// parameters, input and output arrays, and/or statistics
//
config const printParams = true,
             printArrays = false,
             printStats = true;

//
// The program entry point
//
proc main() {
  printConfiguration();          // print the problem size
  makeSizeAssertions();
  //
  // TwiddleDom describes the index set used to define the vector of
  // twiddle values and is a 1D domain indexed by 64-bit ints from 0
  // to m/4-1.  Twiddles is the vector of twiddle values.
  //
  //const TwiddleDist = new dmap(new Cyclic(startIdx=0:idxType, tasksPerLocale=tasksPerLocale));
  const TwiddleDist = new dmap(new Block(rank=1, idxType=idxType, boundingBox={0..m/4-1}, targetLocales=Locales));
  const TwiddleDom: domain(1, int(64)) dmapped TwiddleDist = {0..m/4-1};
  var Twiddles: [TwiddleDom] elemType;

  //
  // BlkDom describes the index set used to define the input and
  // output vectors and is also a 1D domain indexed by 64-bit ints
  // from 0 to m-1.  It is distributes the vectors Z and z across the
  // locales using the Block distribution.
  //
  const BlkDist = new dmap(new Block(rank=1, idxType=idxType, boundingBox={0..m-1},
                                     targetLocales=Locales,
                                     dataParTasksPerLocale=tasksPerLocale,
                                     dataParIgnoreRunningTasks=true));
  const BlkDom: domain(1, int(64)) dmapped BlkDist = {0..m-1};
  var Z, z: [BlkDom] elemType;

  const CycDist = new dmap(new Cyclic(startIdx=0:idxType, targetLocales=Locales,
                                      dataParTasksPerLocale=tasksPerLocale,
                                      dataParIgnoreRunningTasks=true));
  const CycDom: domain(1, int(64)) dmapped CycDist = {0..m-1};
  var Zcyc: [CycDom] elemType;

  initVectors(Twiddles, z);            // initialize twiddles and input vector z

  const startTime = getCurrentTime();  // capture the start time

  Z = conjg(z);                        // store the conjugate of z in Z
  bitReverseShuffle(Z);                // permute Z
  dfft(Z, Twiddles, 0);   // compute the Fourier transform block phase
  forall (b, c) in zip(Z, Zcyc) do
    c = b;
  dfft(Zcyc, Twiddles, 1); // compute the Fourier transform cyclic phase
  forall (b, c) in zip(Z, Zcyc) do
    b = c;

  const execTime = getCurrentTime() - startTime;     // store the elapsed time

  const validAnswer = verifyResults(z, Z, Zcyc, Twiddles); // validate the answer
  printResults(validAnswer, execTime);               // print the results
}

//
// compute the discrete fast Fourier transform of a vector A declared
// over domain ADom using twiddle vector W
//
proc dfft(A: [?ADom], W, phase) {
  const numElements = A.size;
  //
  // loop over the phases of the DFT sequentially using custom
  // iterator genDFTStrideSpan that yields the stride and span for
  // each bank of butterfly calculations
  //
  for (str, span) in genDFTStrideSpan(numElements, phase) {
    //
    // loop in parallel over each of the banks of butterflies with
    // shared twiddle factors, zippering with the unbounded range
    // 0.. to get the base twiddle indices
    //
    forall (bankStart, twidIndex) in zip(ADom by 2*span, 0..) {
      //
      // compute the first set of multipliers for the low bank
      //
      var wk2 = W(twidIndex),
          wk1 = W(2*twidIndex),
          wk3 = (wk1.re - 2 * wk2.im * wk1.im,
                 2 * wk2.im * wk1.re - wk1.im):elemType;
      //
      // loop in parallel over the low bank, computing butterflies
      // Note: lo..#num         == lo, lo+1, lo+2, ..., lo+num-1
      //       lo.. by str #num == lo, lo+str, lo+2*str, ... lo+(num-1)*str
      //
      forall lo in bankStart..#str {
        on ADom.dist.idxToLocale(lo) {
          local {
            butterfly(wk1, wk2, wk3, A.localSlice(lo..by str #radix));
          }
        }
      }
      //
      // update the multipliers for the high bank
      //
      wk1 = W(2*twidIndex+1);
      wk3 = (wk1.re - 2 * wk2.re * wk1.im,
             2 * wk2.re * wk1.re - wk1.im):elemType;
      wk2 *= 1.0i;

      //
      // loop in parallel over the high bank, computing butterflies
      //
      forall lo in bankStart+span..#str {
        on ADom.dist.idxToLocale(lo) {
          local {
            butterfly(wk1, wk2, wk3, A.localSlice(lo.. by str #radix));
          }
        }
      }
    }
  }

  if phase == 1 {
    //
    // Do the last set of butterflies...
    //
    const str = radix**log4(numElements-1);
    //
    // ...using the radix-4 butterflies with 1.0 multipliers if the
    // problem size is a power of 4
    //
    if (str*radix == numElements) then
      forall lo in 0..#str do
        butterfly(1.0, 1.0, 1.0, A, lo.. by str #radix);
    //
    // ...otherwise using a simple radix-2 butterfly scheme
    //
    else
      forall lo in 0..#str {
        const a = A(lo),
              b = A(lo+str);
        A(lo)     = a + b;
        A(lo+str) = a - b;
      }
  }
}

//
// this is the radix-4 butterfly routine that takes multipliers wk1,
// wk2, and wk3 and a 4-element array (slice) A.
//
proc butterfly(wk1, wk2, wk3, X:[?D]) {
  const i0 = D.low,
        i1 = i0 + D.stride,
        i2 = i1 + D.stride,
        i3 = i2 + D.stride;
  var x0 = X(i0) + X(i1),
      x1 = X(i0) - X(i1),
      x2 = X(i2) + X(i3),
      x3rot = (X(i2) - X(i3))*1.0i;

  X(i0) = x0 + x2;                   // compute the butterfly in-place on X
  x0 -= x2;
  X(i2) = wk2 * x0;
  x0 = x1 + x3rot;
  X(i1) = wk1 * x0;
  x0 = x1 - x3rot;
  X(i3) = wk3 * x0;
}

proc butterfly(wk1, wk2, wk3, A, rng) {
  var X: [0..#radix] elemType;
  for (x,r) in zip(X, rng) do
    x = A(r);
  butterfly(wk1, wk2, wk3, X);
  for (x,r) in zip(X, rng) do
    A(r) = x;
}

//
// this iterator generates the stride and span values for the phases
// of the DFFT simply by yielding tuples: (radix**i, radix**(i+1))
//
iter genDFTStrideSpan(numElements, phase) {
  const (start, end) =
    if phase == 0 then (1, numLocales:int(64)) else (numLocales, numElements-1);
  var stride = start;
  for i in log4(start)+1..log4(end):int {
    const span = stride * radix;
    yield (stride, span);
    stride = span;
  }
}

//
// Print the problem size
//
proc printConfiguration() {
  if (printParams) {
    if (printStats) then printLocalesTasks(tasksPerLocale=tasksPerLocale);
    printProblemSize(elemType, numVectors, m);
  }
}

proc printLocalesTasks(tasksPerLocale=1) {
  writeln("Number of Locales = ", numLocales);
  writeln("Tasks per locale = ", tasksPerLocale);
}

proc makeSizeAssertions() {
  assert(4**log4(numLocales) == numLocales, "numLocales must be a power of 4");
}

//
// Initialize the twiddle vector and random input vector and
// optionally print them to the console
//
proc initVectors(Twiddles, z) {
  computeTwiddles(Twiddles);
  bitReverseShuffle(Twiddles);

  fillRandom(z, seed, algorithm=RNG.NPB);

  if (printArrays) {
    writeln("After initialization, Twiddles is: ", Twiddles, "\n");
    writeln("z is: ", z, "\n");
  }
}

//
// Compute the twiddle vector values
//
proc computeTwiddles(Twiddles) {
  const numTwdls = Twiddles.size,
        delta = 2.0 * atan(1.0) / numTwdls;

  Twiddles(0) = 1.0;
  Twiddles(numTwdls/2) = let x = cos(delta * numTwdls/2)
                          in (x, x): elemType;
  forall i in 1..numTwdls/2-1 {
    const x = cos(delta*i),
          y = sin(delta*i);
    Twiddles(i)            = (x, y): elemType;
    Twiddles(numTwdls - i) = (y, x): elemType;
  }
}

//
// Perform a permutation of the argument vector by reversing the bits
// of the indices
//
proc bitReverseShuffle(Vect: [?Dom]) {
  const numBits = log2(Vect.size),
        Perm: [Dom] Vect.eltType = [i in Dom] Vect(bitReverse(i, revBits=numBits));
  Vect = Perm;
}

//
// Reverse the low revBits bits of val
//
proc bitReverse(val: ?valType, revBits = 64) {
  param mask = 0x0102040810204080;
  const valReverse64 = bitMatMultOr(mask, bitMatMultOr(val:uint(64), mask)),
        valReverse = rotl(valReverse64, revBits);
  return valReverse: valType;
}

//
// Compute the log base 4 of x
//
proc log4(x) return logBasePow2(x, 2);

//
// verify that the results are correct by reapplying the dfft and then
// calculating the maximum error, comparing against epsilon
//
proc verifyResults(z, Z, Cyc, Twiddles) {
  if (printArrays) then writeln("After FFT, Z is: ", Z, "\n");

  Z = conjg(Z) / m;
  bitReverseShuffle(Z);
  dfft(Z, Twiddles, 0);
  forall (b, c) in zip(Z, Cyc) do
    c = b;
  dfft(Cyc, Twiddles, 1);
  forall (b, c) in zip(Z, Cyc) do
    b = c;

  if (printArrays) then writeln("After inverse FFT, Z is: ", Z, "\n");

  var maxerr = max reduce sqrt((z.re - Z.re)**2 + (z.im - Z.im)**2);
  maxerr /= (epsilon * n);
  if (printStats) then writeln("error = ", maxerr);

  return (maxerr < threshold);
}

//
// print out sucess/failure, the timing, and the Gflop/s value
//
proc printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    writeln("Performance (Gflop/s) = ", 5 * (m * n / execTime) * 1e-9);
  }
}
