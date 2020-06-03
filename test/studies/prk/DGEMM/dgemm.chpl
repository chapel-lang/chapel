/*
   Chapel's parallel DGEMM implementation

   Contributed by Engin Kayraklioglu (GWU)
*/
use Time;
use BlockDist;
use RangeChunk;

param PRKVERSION = "2.17";

config type dtype = real;

config param useBlockDist = false;

config const order = 10,
             epsilon = 1e-8,
             iterations = 100,
             blockSize = 0,
             debug = false,
             validate = true,
             correctness = false; // being run in start_test


// TODO current logic assumes order is divisible by blockSize. add that
// check

const vecRange = 0..#order;

const matrixSpace = {vecRange, vecRange};
const matrixDom = matrixSpace dmapped if useBlockDist then
                      new dmap(new Block(boundingBox=matrixSpace)) else
                      defaultDist;

var A: [matrixDom] dtype,
    B: [matrixDom] dtype,
    C: [matrixDom] dtype;

forall (i,j) in matrixDom {
  A[i,j] = j;
  B[i,j] = j;
  C[i,j] = 0;
}

const nTasksPerLocale = here.maxTaskPar;

if !correctness {
  writeln("Chapel Dense matrix-matrix multiplication");
  writeln("Max parallelism      =   ", nTasksPerLocale);
  writeln("Matrix order         =   ", order);
  writeln("Blocking factor      =   ", if blockSize>0 then blockSize:string
      else "N/A");
  writeln("Number of iterations =   ", iterations);
  writeln();
}

const refChecksum = (iterations+1) *
    (0.25*order*order*order*(order-1.0)*(order-1.0));

var t = new Timer();

if blockSize == 0 {
  for niter in 0..iterations {
    if niter==1 then t.start();

    forall (i,j) in matrixSpace do
      for k in vecRange do
        C[i,j] += A[i,k] * B[k,j];

  }
  t.stop();
}
else {
  // task-local arrays are necessary for blocked implementation, so
  // using explicit coforalls
  coforall l in Locales with (ref t) {
    on l {
      const bVecRange = 0..#blockSize;
      const blockDom = {bVecRange, bVecRange};
      const localDom = matrixDom.localSubdomain();

      coforall tid in 0..#nTasksPerLocale with (ref t) {
        const myChunk = chunk(localDom.dim(1), nTasksPerLocale, tid);

        var AA: [blockDom] dtype,
            BB: [blockDom] dtype,
            CC: [blockDom] dtype;

        for niter in 0..iterations {
          if here.id==0 && tid==0 && niter==1 then t.start();

          for (jj,kk) in {myChunk by blockSize, vecRange by blockSize} {
            const jMax = min(jj+blockSize-1, myChunk.high);
            const kMax = min(kk+blockSize-1, vecRange.high);
            const jRange = 0..jMax-jj;
            const kRange = 0..kMax-kk;

            // instead of unbounded ranges I was using 0..#blockSize in
            // perf tests, which was running fine with --fast, but
            // causing unequal iter length errors without --fast(if the
            // order is not divisible by
            // blockSize*numLocales*nTasksPerLocale
            for (jB, j) in zip(jj..jMax, 0..) do
              for (kB, k) in zip(kk..kMax, 0..) do
                BB[j,k] = B[kB,jB];

            for ii in localDom.dim(0) by blockSize {
              const iMax = min(ii+blockSize-1, localDom.dim(0).high);
              const iRange = 0..iMax-ii;

              for (iB, i) in zip(ii..iMax, 0..) do
                for (kB, k) in zip(kk..kMax, 0..) do
                  AA[i,k] = A[iB, kB];

              local {
                for cc in CC do
                  cc = 0.0;

                for (k,j,i) in {kRange, jRange, iRange} do
                  CC[i,j] += AA[i,k] * BB[j,k];

                for (iB, i) in zip(ii..iMax, 0..) do
                  for (jB, j) in zip(jj..jMax, 0..) do
                    C[iB,jB] += CC[i,j];
              }
            }
          }
        }
      }
    }
  }
  t.stop();
}

if validate {
  const checksum = + reduce C;
  if abs(checksum-refChecksum)/refChecksum > epsilon then
    halt("VALIDATION FAILED! Reference checksum = ", refChecksum,
                           " Checksum = ", checksum);
  else
    writeln("Validation successful");
}

if !correctness {
  const nflops = 2.0*(order**3);
  const avgTime = t.elapsed()/iterations;
  writeln("Rate(MFlop/s) = ", 1e-6*nflops/avgTime, " Time : ", avgTime);
}
