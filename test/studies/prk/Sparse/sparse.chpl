/*
   Chapel's parallel Sparse implementation

   Contributed by Engin Kayraklioglu (GWU)
*/
use BlockDist;
use Time;
use LayoutCS;

param PRKVERSION = "2.17";

config const lsize = 5,
             radius = 2,
             iterations = 10,
             correctness = false,
             scramble = true;

const lsize2 = 2*lsize,
      size = 1<<lsize,
      size2 = size*size,
      stencilSize = 4*radius+1,
      sparsity = stencilSize:real/size2;

// create vector domain
const vectorDom = {0..#size2};

const parentDom = {0..#size2, 0..#size2};
var matrixDom: sparse subdomain(parentDom) dmapped CS();

// temporary index buffer for fast initialization
const indBufDom = {0..#(size2*stencilSize)};
var indBuf: [indBufDom] 2*int;

//initialize sparse domain
for row in 0..#size2 {
  const i = row%size;
  const j = row/size;

  var bufIdx = row*stencilSize;

  indBuf[bufIdx] = (row, reverse(LIN(i,j)));
  for r in 1..radius {
    indBuf[bufIdx+1] = (row, reverse(LIN((i+r)%size,j)));
    indBuf[bufIdx+2] = (row, reverse(LIN((i-r+size)%size,j)));
    indBuf[bufIdx+3] = (row, reverse(LIN(i, (j+r)%size)));
    indBuf[bufIdx+4] = (row, reverse(LIN(i, (j-r+size)%size)));
    bufIdx += 4;
  }
}
matrixDom.bulkAdd(indBuf, preserveInds=false);

//do a sanitiy check to make sure we have created correct numver of
//indicese in the sparse domain
if matrixDom.size != size2*stencilSize then
  halt("Incorrect number of indices created");

var matrix: [matrixDom] real;
[(i,j) in matrixDom] matrix[i,j] = 1.0/(j+1);

var vector: [vectorDom] real;
var result: [vectorDom] real;
vector = 0;
result = 0;

if !correctness {
  // Print information before main loop
  writeln("Parallel Research Kernels Version ", PRKVERSION);
  writeln("Sparse matrix-dense vector multiplication");
  writeln("Max parallelism      = ", here.maxTaskPar);
  writeln("Matrix order         = ", size2);
  writeln("Stencil diameter     = ", 2*radius+1);
  writeln("Sparsity             = ", sparsity);
  writeln("Number of iterations = ", iterations);
  writeln("Indexes are ", if !scramble then "not " else "", "scrambled");
}

var t = new Timer();
for niter in 0..iterations {

  if niter == 1 then t.start();
  [i in vectorDom] vector[i] += i+1;

  forall i in matrix.domain.dim(0) do
    for j in matrix.domain.dimIter(1,i) do
    result[i] += matrix[i,j] * vector[j];
}
t.stop();

// verify the result
const epsilon = 1e-8;
const referenceSum = 0.5 * matrixDom.size * (iterations+1) *
    (iterations+2);
const vectorSum = + reduce result;
if abs(vectorSum-referenceSum) > epsilon then
  halt("VALIDATION FAILED!. Reference sum = ", referenceSum,
      " Vector sum = ", vectorSum);

writeln("Validation successful");

if !correctness {
  const nflop = 2.0*matrixDom.size;
  const avgTime = t.elapsed()/iterations;
  writeln("Rate (MFlops/s): ", 1e-6*nflop/avgTime, " Avg time (s): ",
      avgTime);
}

inline proc LIN(i, j) {
  return (i+(j<<lsize));
}

proc reverse(xx) {
  if !scramble then return xx;

  var x = xx:uint;

  x = ((x >> 1)  & 0x5555555555555555) | ((x << 1)  & 0xaaaaaaaaaaaaaaaa);
  x = ((x >> 2)  & 0x3333333333333333) | ((x << 2)  & 0xcccccccccccccccc);
  x = ((x >> 4)  & 0x0f0f0f0f0f0f0f0f) | ((x << 4)  & 0xf0f0f0f0f0f0f0f0);
  x = ((x >> 8)  & 0x00ff00ff00ff00ff) | ((x << 8)  & 0xff00ff00ff00ff00);
  x = ((x >> 16) & 0x0000ffff0000ffff) | ((x << 16) & 0xffff0000ffff0000);
  x = ((x >> 32) & 0x00000000ffffffff) | ((x << 32) & 0xffffffff00000000);
  return (x>>(64-lsize2)):int;
}
