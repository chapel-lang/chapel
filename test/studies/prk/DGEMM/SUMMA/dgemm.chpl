/*
   Chapel's distributed DGEMM implementation using SUMMA
   
   Note - Currently does not perform pipelined communication, 
   which is inherent to SUMMA

   Contributed by Rahul Ghangas (ANU)
*/

use Time;
use BlockDist;
use LinearAlgebra;

config type dtype = real;

config param useBlockDist = false;

config const order = 10,
             epsilon = 1e-8,
             iterations = 100,
             windowSize = 0,
             debug = false,
             validate = true,
             correctness = false; // being run in start_test

const vecRange = 0..#order;
const matrixSpace = {vecRange, vecRange};

const matrixDom = matrixSpace dmapped if useBlockDist then
                      new dmap(new Block(boundingBox=matrixSpace)) else
                      defaultDist;

var A = Matrix(matrixDom, dtype),
    B = Matrix(matrixDom, dtype),
    C = Matrix(matrixDom, dtype);

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
  writeln("Window size          =   ", if windowSize>0 then
      windowSize:string
      else "N/A");
  writeln("Number of iterations =   ", iterations);
  writeln();
}

const refChecksum = (iterations+1) *
    (0.25*order*order*order*(order-1.0)*(order-1.0));

var t = new Timer();

private iter block(indexes, window : int) {
  var lowVal = indexes.low;
  while (true) {
    var highVal = lowVal + window - 1;
    if (highVal >= indexes.high) {
      yield lowVal..indexes.high;
      break;
    } else {
      yield lowVal..highVal;
    }
    lowVal = highVal + 1;
  }
}

if windowSize == 0 {
  for niter in 0..iterations {
    if niter==1 then t.start();

    forall (i,j) in matrixSpace do
      for k in vecRange do
        C[i,j] += A[i,k] * B[k,j];

  }
  t.stop();
}
else {
  for niter in 0..iterations {
    coforall loc in A.targetLocales() with (ref t) {
      on loc {
        if here.id==0 && niter==1 then t.start();
        var localDomainA = A.localSubdomain();
        var localDomainB = B.localSubdomain();
        var localDomainC = C.localSubdomain();
        
        for subArrayChunk in block(A.domain.dim(1), windowSize) {
          var subArrayA : [localDomainA.dim(0), subArrayChunk] A.eltType;
          var subArrayB : [subArrayChunk, localDomainB.dim(1)] B.eltType;

          forall i in localDomainA.dim(0) {
            forall j in subArrayChunk {
              subArrayA[i, j] = A[i, j];
            }
          }
          forall i in subArrayChunk {
            forall j in localDomainB.dim(1) {
              subArrayB[i, j] = B[i, j];
            }
          }
          
          C.localSlice(localDomainC) += dot(subArrayA, subArrayB);
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
    writeln("Validation Successful");
}

if !correctness {
  const nflops = 2.0*(order**3);
  const avgTime = t.elapsed()/iterations;
  writeln("Rate(MFlop/s) = ", 1e-6*nflops/avgTime);
  writeln("Time: ", avgTime);
}
