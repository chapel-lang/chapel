// Check for memory leaks
use driver;
config const iters = 10;

def foo(TD: domain, A: [TD] int, TA, iter: int) {
  var errs = 0;
  var offset = if (TD.rank==1) then o5 else fill(TD.rank, o5);
  [i in TD] A[i] += iter;
  for i in [TD] do
    if A[i] != TA[i+offset] {
      writeln("A[",i,"] Incorrect reindex");
      errs += 1;
    }
  return errs;
}

def dit(D, TD: domain, A:[D]) {
  var errs = 0;
  for iter in 1..iters {
    [i in D] A[i] -= iter;
    var ei = foo(TD, A, A, iter);
    if (ei != 0) {
      writeln("\tIteration ", iter, ": ", ei, " errors");
      errs += ei;
    }
  }
  return errs;
}

const TD1D: domain(1) = Space1 - (o5);
for e in A2D do e = next();
writeln("A1D: ", dit(Dom1D, TD1D, A1D), " errors");

const TD2D: domain(2) = Space2 - (o5, o5);
for e in A2D do e = next();
writeln("A2D: ", dit(Dom2D, TD2D, A2D), " errors");

const TD3D: domain(3) = Space3 - (o5, o5, o5);
for e in A3D do e = next();
writeln("A3D: ", dit(Dom3D, TD3D, A3D), " errors");

const TD4D: domain(4) = Space4 - (o5, o5, o5, o5);
for e in A4D do e = next();
writeln("A4D: ", dit(Dom4D, TD4D, A4D), " errors");

const TD2D64: domain(2,int(64)) = Space2D64 - (o5:int(64), o5:int(64));
for e in A2D64 do e = next();
writeln("A2D64: ", dit(Dom2D64, TD2D64, A2D64), " errors");
