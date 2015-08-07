use driver_domains;

// test reindexing from distributed domains to non-distributed arrays

proc foo(TD: domain, A: [TD] int, TA) {
  var errs = 0;
  var offset = if (TD.rank==1) then o5:TD.idxType else fill(TD.rank, o5:TD.idxType);
  for i in TD do
    if A[i] != TA[i-offset] {
      writeln("A[",i,"] Incorrect reindex = ", A[i], ", TA[", i+offset, "] = ", TA[i+offset]);
      errs += 1;
    }
  return errs;
}

const TD1D: domain(1) = Space1.translate(-o5);
var TA1D: [TD1D] int;
for e in TA1D do e = next();
writeln("TA1D: ", foo(Dom1D, TA1D.reindex(Dom1D), TA1D), " errors");

const TD2D: domain(2) = Space2.translate(-o5, -o5);
var TA2D: [TD2D] int;
for e in TA2D do e = next();
writeln("TA2D: ", foo(Dom2D, TA2D.reindex(Dom2D), TA2D), " errors");

const TD3D: domain(3) = Space3.translate(-o5, -o5, -o5);
var TA3D: [TD3D] int;
for e in TA3D do e = next();
writeln("TA3D: ", foo(Dom3D, TA3D.reindex(Dom3D), TA3D), " errors");

const TD4D: domain(4) = Space4.translate(-o5, -o5, -o5, -o5);
var TA4D: [TD4D] int;
for e in TA4D do e = next();
writeln("TA4D: ", foo(Dom4D, TA4D.reindex(Dom4D), TA4D), " errors");

const TD2D32: domain(2,int(32)) = Space2D32.chpl__unTranslate(o5:int(32), o5:int(32));
var TA2D32: [TD2D32] int;
for e in TA2D32 do e = next();
writeln("TA2D32: ", foo(Dom2D32, TA2D32.reindex(Dom2D32), TA2D32), " errors");
