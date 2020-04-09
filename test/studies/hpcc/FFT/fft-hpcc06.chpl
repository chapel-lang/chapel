use BitOps;
use Random;
use Time;

use HPCCProblemSize;


param radix = 4;

param numVectors = 2;
type elemType = complex(128);


config const n = computeProblemSize(elemType, numVectors, returnLog2 = true);
const m = 2**n;

config const epsilon = 2.0 ** -51.0,
             threshold = 16.0;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 314159265;

config const printParams = true,
             printArrays = false,
             printStats = true;


proc main() {
  printConfiguration();

  const TwiddleDom: domain(1) = {0..#m/4};
  var Twiddles: [TwiddleDom] elemType;

  const ProblemDom: domain(1) = {0..#m};
  var Z, z: [ProblemDom] elemType;

  initVectors(Twiddles, z);

  const startTime = getCurrentTime();

  Z = conjg(z);
  bitReverseShuffle(Z);
  dfft(Z, Twiddles);

  const execTime = getCurrentTime() - startTime;

  const validAnswer = verifyResults(z, Z, Twiddles);
  printResults(validAnswer, execTime);
}


proc printConfiguration() {
  if (printParams) then printProblemSize(elemType, numVectors, m);
}


proc initVectors(Twiddles, z) {
  computeTwiddles(Twiddles);
  bitReverseShuffle(Twiddles);

  fillRandom(z, seed, algorithm=RNG.NPB);

  if (printArrays) {
    writeln("After initialization, Twiddles is: ", Twiddles, "\n");
    writeln("z is: ", z, "\n");
  }
}


proc computeTwiddles(Twiddles) {
  const numTwdls = Twiddles.size,
        delta = 2.0 * atan(1.0) / numTwdls;

  Twiddles(0) = 1.0;
  Twiddles(numTwdls/2) = let x = cos(delta * numTwdls/2)
                          in (x, x):complex;
  forall i in 1..numTwdls/2-1 {
    const x = cos(delta*i),
          y = sin(delta*i);
    Twiddles(i)            = (x, y):complex;
    Twiddles(numTwdls - i) = (y, x):complex;
  }
}


proc bitReverseShuffle(Vect: [?Dom]) {
  const numBits = log2(Vect.size),
        Perm: [Dom] index(Dom) = [i in Dom] bitReverse(i, revBits = numBits),
        Temp = Vect(Perm);
  Vect = Temp;
}


proc bitReverse(val: ?valType, revBits = 64) {
  param mask = 0x0102040810204080;
  const valReverse64 = bitMatMultOr(mask, bitMatMultOr(val:uint(64), mask)),
        valReverse = rotl(valReverse64, revBits);
  return valReverse: valType;
}


proc dfft(A: [?ADom], W) {
  const numElements = A.size;
  var span = 1;

  for i in 2..log2(numElements)-1 by 2 {
    const m = radix*span,
          m2 = 2*m;

    forall (k,k1) in zip(ADom by m2, 0..) {
      var wk2 = W(k1),
          wk1 = W(2*k1),
          wk3 = (wk1.re - 2 * wk2.im * wk1.im,
                 2 * wk2.im * wk1.re - wk1.im):complex;

      forall j in k..#span do
        butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);

      wk1 = W(2*k1+1);
      wk3 = (wk1.re - 2 * wk2.re * wk1.im,
             2 * wk2.re * wk1.re - wk1.im):complex;
      wk2 *= 1.0i;

      forall j in k+m..#span do
        butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);
    }
    span *= radix;
  }

  if ((span*radix) == numElements) then
    forall j in 0..#span do
      butterfly(1.0, 1.0, 1.0, A[j..j+3*span by span]);
  else
    forall j in 0..#span {
      const a = A(j),
            b = A(j+span);
      A(j)      = a + b;
      A(j+span) = a - b;
    }
}


proc butterfly(wk1, wk2, wk3, inout A:[?D]) {
  const i1 = D.low,
        i2 = i1 + D.stride,
        i3 = i2 + D.stride,
        i4 = i3 + D.stride;
  var x0 = A(i1) + A(i2),
      x1 = A(i1) - A(i2),
      x2 = A(i3) + A(i4),
      x3rot = (A(i3) - A(i4))*1.0i;

  A(i1) = x0 + x2;
  x0 -= x2;
  A(i3) = wk2 * x0;
  x0 = x1 + x3rot;
  A(i2) = wk1 * x0;
  x0 = x1 - x3rot;
  A(i4) = wk3 * x0;
}


proc verifyResults(z, Z, Twiddles) {
  if (printArrays) then writeln("After FFT, Z is: ", Z, "\n");

  Z = conjg(Z) / m;
  bitReverseShuffle(Z);
  dfft(Z, Twiddles);

  if (printArrays) then writeln("After inverse FFT, Z is: ", Z, "\n");

  var maxerr = max reduce sqrt((z.re - Z.re)**2 + (z.im - Z.im)**2);
  maxerr /= (epsilon * n);
  if (printStats) then writeln("error = ", maxerr);

  return (maxerr < threshold);
}


proc printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    writeln("Performance (Gflop/s) = ", (5.0 * m * n) / execTime * 1.0e-9);
  }
}
