/*
   Chapel's parallel Sparse implementation

   Contributed by Engin Kayraklioglu (GWU)

   Implementation & comments derived from MPI1 version
*/
use BlockDist;
use LayoutCSR;
use ReplicatedDist;
use Time;
use VisualDebug;

param PRKVERSION = "2.17";

/* Distributed version */
config param distributed = CHPL_COMM != 'none';

             /* Logarithmic linear size of grid, s.t. numElements = 4**lsize */
config const lsize = 4,
             /* Designates number of non-zeroes per row (4*r + 1)*/
             radius = 2,
             /* Number of times multiplication is performed */
             iterations = 2,
             /* Convert all linearized grid indices by reversing their bits */
             scramble = true,
             /* Only print correctness string */
             correctness = false;

const lsize2 = 2*lsize,
      size = 1<<lsize,
      size2 = size*size,
      stencilSize = 4*radius+1,
      sparsity = stencilSize:real/size2;

// Locales array setup -- Distributed only
const rowDistLocDom = {0..#numLocales, 0..0};
var rowDistLocArr: [rowDistLocDom] locale;
rowDistLocArr[0..#numLocales, 0] = Locales[0..#numLocales];

// Domains & Distributions
const vectorSpace = {0..#size2},
      matrixSpace = {0..#size2, 0..#size2},
      resultDist = if distributed then new dmap(new Block(vectorSpace))
                   else defaultDist,
      parentDist = new dmap(new Block(matrixSpace,
                                      targetLocales=rowDistLocArr,
                                      sparseLayoutType=CSR)),
      vectorDom = vectorSpace dmapped Replicated(),
      resultDom = vectorSpace dmapped resultDist,
      parentDom = if distributed then matrixSpace dmapped parentDist
                  else matrixSpace,
      matrixDomBlock: sparse subdomain(parentDom),
      matrixDomCSR: sparse subdomain(parentDom) dmapped CSR();

ref matrixDom = if distributed then matrixDomBlock else matrixDomCSR;

// Arrays
var matrix: [matrixDom] real,
    vector: [vectorDom] real,
    result: [resultDom] real;

var t = new Timer();

proc main() {

  initialize();

  printInfo();

  startVdebug('kernel');
  for niter in 0..iterations {

    if niter == 1 then t.start();

    // Update vector (across locales for distributed case)
    coforall loc in Locales do on loc {
      [i in vectorDom] vector[i] += i+1;
    }

    forall i in resultDom {
      var temp = 0.0;
      // TODO: Privatization in SparseBlockDist (this should be local)
      for j in matrixDom.dimIter(2,i) {
        temp += matrix[i,j] * vector[j];
      }
    result[i] += temp;
    }
  }
  t.stop();
  stopVdebug();

  verifyResult();
}


proc initialize() {
  // TODO: Optimize distributed initialization
  t.start();

  // Temporary index buffer for fast initialization
  const indBufDom = {0..#(size2*stencilSize)};
  var indBuf: [indBufDom] 2*int;

  // Initialize sparse domain
  for row in vectorSpace {
    const i = row%size,
          j = row/size;

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
  if distributed {
    matrixDom.bulkAdd(indBuf, preserveInds=false);
  } else {
    matrixDomCSR.bulkAdd(indBuf, preserveInds=false);
  }

  // Sanity check the number of indices in the sparse domain
  if matrixDom.numIndices != size2*stencilSize then
    halt("Incorrect number of indices created");

  // Initialize sparse matrix values
  if distributed {
    [(i,j) in matrixDom] matrix[i,j] = 1.0/(j+1);
  } else {
    [(i,j) in matrixDomCSR] matrix[i,j] = 1.0/(j+1);
  }

  t.stop();
  if VisualDebugOn then writeln('Initialization time: ', t.elapsed());
  t.clear();
}


proc printInfo() {
  // TODO: Match PRK reference version output exactly?
  if !correctness {
    writeln("Parallel Research Kernels Version ", PRKVERSION);
    writeln("Sparse matrix-dense vector multiplication");
    writeln("Max parallelism      = ", here.maxTaskPar);
    writeln("Matrix order         = ", size2);
    writeln("Stencil diameter     = ", 2*radius+1);
    writeln("Sparsity             = ", sparsity);
    writeln("Number of iterations = ", iterations);
    writeln("Indexes are ", if !scramble then "not " else "", "scrambled");
  }
}


proc verifyResult() {
  const epsilon = 1e-8,
        referenceSum = 0.5 * matrixDom.numIndices * (iterations+1) *
                       (iterations+2),
        vectorSum = + reduce result;

  if abs(vectorSum - referenceSum) > epsilon then
    halt("VALIDATION FAILED!. Reference sum = ", referenceSum,
        " Vector sum = ", vectorSum);

  writeln("Validation successful");

  if !correctness {
    const nflop = 2.0*matrixDom.numIndices,
          avgTime = t.elapsed()/iterations;
    writeln("Rate (MFlops/s): ", 1e-6*nflop/avgTime, " Avg time (s): ",
        avgTime);
  }
}


/* Linearize grid index */
inline proc LIN(i, j) {
  return (i+(j<<lsize));
}


/* Code below reverses bits in unsigned integer stored in a 64-bit word.
   Bit reversal is with respect to the largest integer that is going to be
   ranked for the particular run of the code, to make sure the reversal
   constitutes a true permutation. Hence, the final result needs to be shifted
   to the right.
   Example: if largest integer being processed is 0x000000ff = 255 =
   0000...0011111111 (binary), then the unshifted reversal of 0x00000006 = 6 =
   0000...0000000110 (binary) would be 011000000...0000 = 3*2^61, which is
   outside the range of the original sequence 0-255. Setting shift_in_bits to
   2log(256) = 8, the final result is shifted the the right by 64-8=56 bits,
   so we get 000...0001100000 (binary) = 96, which is within the proper range */
proc reverse(xx) {
  if !scramble then return xx;

  var x = xx: uint;

  x = ((x >> 1)  & 0x5555555555555555) | ((x << 1)  & 0xaaaaaaaaaaaaaaaa);
  x = ((x >> 2)  & 0x3333333333333333) | ((x << 2)  & 0xcccccccccccccccc);
  x = ((x >> 4)  & 0x0f0f0f0f0f0f0f0f) | ((x << 4)  & 0xf0f0f0f0f0f0f0f0);
  x = ((x >> 8)  & 0x00ff00ff00ff00ff) | ((x << 8)  & 0xff00ff00ff00ff00);
  x = ((x >> 16) & 0x0000ffff0000ffff) | ((x << 16) & 0xffff0000ffff0000);
  x = ((x >> 32) & 0x00000000ffffffff) | ((x << 32) & 0xffffffff00000000);
  return (x>>(64-lsize2)): int;
}


/* Iterate over NNZ values in dimension ``dim`` at index ``idx`` */
iter SparseBlockDom.dimIter(param dim, idx) {
  // TODO: Contribute back to SparseBlockDist
  // TODO: add prefetch checks for fast iteration
  var targetLocRow = dist.targetLocsIdx((idx, whole.dim(dim).low));
  for l in dist.targetLocales.domain.dim(dim) {
    for idx in locDoms[(targetLocRow[1], l)].mySparseBlock.dimIter(dim, idx) {
      yield idx;
    }
  }
}
