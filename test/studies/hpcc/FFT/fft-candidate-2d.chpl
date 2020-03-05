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

  const TwiddleDom = {0:int(64)..#m/4};
  var Twiddles: [TwiddleDom] elemType;

  const ProblemDom = {0:int(64)..#m};
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

proc dfft(A, W) {

  const numElements = A.numElements;
  const halfLogN = (log2(numElements))/2;

  fft2d(A, W, halfLogN, 1);

  transpose(A);

  fft2d(A, W, halfLogN, 2);

  transpose(A);
}

proc fft2d(A, W, steps, phase) {

  const numElements = A.numElements;
  const p = sqrt(numElements):int;

  var span = 1;
  var m, m2, k, k1: int;
  var wk1, wk2, wk3: complex;

  for i in 2..steps-1 by 2 {
    m = 4*span;
    m2 = 2*m;
    if (m2 > p) then break;
    for row in 0..#p {
      for col in 0..#p by m2 {
        k = p*row + col;
        k1 = if (phase == 1) then (k/m2) else (col/m2);
        wk2 = W[k1];
        wk1 = W[2*k1];
        wk3 = (wk1.re - 2 * wk2.im * wk1.im,
                 2 * wk2.im * wk1.re - wk1.im):complex;
        for j in k..#span {
          butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);
        }
        k = p*row + col + m;
        wk1 = W[2*k1+1];
        wk3 = (wk1.re - 2 * wk2.re * wk1.im,
               2 * wk2.re * wk1.re - wk1.im):complex;
        for j in k..#span {
          butterfly(wk1, (-wk2.im, wk2.re):complex, wk3, A[j..j+3*span by span]);
        }
      }
    }
    span *= 4;
  }
  if (phase == 1) {
    for (row, k1) in zip(0..#p by 2, 0..) {
      k = p*row;
      wk2 = W[k1];
      wk1 = W[2*k1];
      wk3 = (wk1.re - 2 * wk2.im * wk1.im,
             2 * wk2.im * wk1.re - wk1.im):complex;
      for j in k..#span {
        butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);
      }
      k += p;
      wk1 = W[2*k1+1];
      wk3 = (wk1.re - 2 * wk2.re * wk1.im,
             2 * wk2.re * wk1.re - wk1.im):complex;
      for j in k..#span {
        butterfly(wk1, (-wk2.im, wk2.re):complex, wk3, A[j..j+3*span by span]);
      }
    }
  }
  else {
    for row in 0..#p {
      k = p*row;
      for j in k..#span {
        butterfly(1.0, 1.0, 1.0, A[j..j+3*span by span]);
      }
    }
  }
}

proc transpose(inout A:[?AD] complex) {
  const n = AD.dim(1).size;
  var p = sqrt(n):int;
  var row, col: index(AD);
  var temp: [AD] complex;

  temp = A;
  for i in AD {
    row = (i/p);
    col = (i%p);
    A(i) = temp(col*p + row);
  }
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
  const numTwdls = Twiddles.numElements,
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
  const numBits = log2(Vect.numElements),
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
    writeln("Performance (Gflop/s) = ", 5 * (m * n / execTime) * 1e-9);
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


