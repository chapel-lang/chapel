use CGMakeA;

type elemType = real(64);

enum classVals {S, W, A, B, C, D};


const Class = {classVals.S..classVals.D};
const probSizes:   [Class] int = [ 1400, 7000, 14000, 75000, 150000, 150000 ],
      nonZeroes:   [Class] int = [ 7, 8, 11, 13, 15, 21 ],
      shifts:      [Class] int = [ 10, 12, 20, 60, 110, 500 ];

config const probClass = classVals.S;

config const n = probSizes(probClass),
             nonzer = nonZeroes(probClass),
             shift = shifts(probClass);

proc main() {
  const DenseSpace = {1..n, 1..n};
  const MatrixSpace: sparse subdomain(DenseSpace) // dist(CSR);
                   = genAIndsSorted(elemType, n, nonzer, shift);
  var A: [MatrixSpace] elemType;

  for (ind, v) in makea(elemType, n, nonzer, shift) {
    A(ind) += v;
  }

  writeln("MatrixSpace is:\n", MatrixSpace);
  writeln("A is:\n", A);
}
