// Check for memory leaks
use driver_arrays;
config const iters = 10;

proc foo(TD: domain, A: [TD] int, TA, iterNo: int) {
  var errs = 0;
  var offset = if (TD.rank==1) then o5:TD.idxType else fill(TD.rank, o5:TD.idxType);
  [i in TD] A[i] += iterNo;
  for i in TD do
    if A[i] != TA[i+offset] {
      writeln("A[",i,"] Incorrect reindex");
      errs += 1;
    }
  return errs;
}

proc dit(TD: domain, A:[?D]) {
  var errs = 0;
  for iterNo in 1..iters {
    [i in D] A[i] -= iterNo;
    var ei = foo(TD, A.reindex(TD), A, iterNo);
    if (ei != 0) {
      writeln("\tIteration ", iterNo, ": ", ei, " errors");
      errs += ei;
    }
  }
  return errs;
}

const TD1D: domain(1) = Space1.translate(-o5);
for e in A1D do e = next();
writeln("A1D: ", dit(TD1D, A1D), " errors");

const TD2D: domain(2) = Space2.translate(-o5, -o5);
for e in A2D do e = next();
writeln("A2D: ", dit(TD2D, A2D), " errors");

const TD3D: domain(3) = Space3.translate(-o5, -o5, -o5);
for e in A3D do e = next();
writeln("A3D: ", dit(TD3D, A3D), " errors");

const TD4D: domain(4) = Space4.translate(-o5, -o5, -o5, -o5);
for e in A4D do e = next();
writeln("A4D: ", dit(TD4D, A4D), " errors");

const TD2D32: domain(2,int(32)) = Space2D32.chpl__unTranslate(o5:int(32), o5:int(32));
for e in A2D32 do e = next();
writeln("A2D32: ", dit(TD2D32, A2D32), " errors");
