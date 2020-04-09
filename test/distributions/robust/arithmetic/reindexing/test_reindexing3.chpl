use driver_arrays;

proc foo(TD: domain, A: [TD] int, TA) {
  var errs = 0;
  var offset = if (TD.rank==1) then o5:TD.idxType else fill(TD.rank, o5:TD.idxType);
  [i in TD] A[i] += 1;
  for i in TD do
    if A[i].locale != TA[i+offset].locale {
      writeln("A[",i,"] Incorrect reindex");
      errs += 1;
    }
  return errs;
}

const TD1D: domain(1) = Space1.translate(-o5);
for e in A1D do e = next();
writeln("A1D: ", foo(TD1D, A1D.reindex(TD1D), A1D), " errors");

const TD2D: domain(2) = Space2.translate(-o5, -o5);
for e in A2D do e = next();
writeln("A2D: ", foo(TD2D, A2D.reindex(TD2D), A2D), " errors");

const TD3D: domain(3) = Space3.translate(-o5, -o5, -o5);
for e in A3D do e = next();
writeln("A3D: ", foo(TD3D, A3D.reindex(TD3D), A3D), " errors");

const TD4D: domain(4) = Space4.translate(-o5, -o5, -o5, -o5);
for e in A4D do e = next();
writeln("A4D: ", foo(TD4D, A4D.reindex(TD4D), A4D), " errors");

const TD2D32: domain(2,int(32)) = Space2D32.chpl__unTranslate(o5:int(32), o5:int(32));
for e in A2D32 do e = next();
writeln("A2D32: ", foo(TD2D32, A2D32.reindex(TD2D32), A2D32), " errors");
