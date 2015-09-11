// This is an extract of test/distributions/robust/arithmetic/kernels/hpl.chpl
// when run with distType=DistType.replicated.
//
// As of this writing, the failures in this code are non-deterministic.
// Uncommenting the commented-out code and/or increasing 'fac'
// should increase the probability of failure.
//
// It fails under valgrind, perhaps reliably, when run with -nl2.

use ReplicatedDist;

config const blk = 5, fac=10;
const n = blk*fac;

const Dist2D = new dmap(new ReplicatedDist());

proc main() {
  const AbD: domain(2) dmapped Dist2D = {1..n, 1..n};
  var Ab: [AbD] real;

  const ptOp = 1;

  const ptSol = ptOp+blk;
/*
  const replAD: domain(2) dmapped Dist2D = AbD[ptSol.., ptOp..#blk],
        replBD: domain(2) dmapped Dist2D = AbD[ptOp..#blk, ptSol..];
    
  const replA : [replAD] real = Ab[ptSol.., ptOp..#blk],
        replB : [replBD] real = Ab[ptOp..#blk, ptSol..];
*/
  write("start...");

  forall (row,col) in AbD/*[ptSol.., ptSol..]*/ by (blk, blk) {

      const //aBlkD = replAD[row..#blk, ptOp..#blk],
            //bBlkD = replBD[ptOp..#blk, col..#blk],
            cBlkD = AbD[row..#blk, col..#blk];

      dgemm(/*replA(aBlkD),
            replB(bBlkD),
            */Ab(cBlkD));
  }

  writeln("finish");
}

proc dgemm(/*A: [1..blk, 1..blk] real,
           B: [1..blk, 1..blk] real,
           */C: [?D] real)
{ }
