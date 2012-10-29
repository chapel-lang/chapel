use driver_arrays;

proc foo(TD: domain, A: [TD] int, TA) {
  var errs = 0;
  var offset = if (TD.rank==1) then o5 else fill(TD.rank, o5);
  for i in {TD} do
    if A[i] != TA[i+offset] {
      writeln("A[",i,"] Incorrect reindex");
      errs += 1;
    }
  return errs;
}

const TD1D: domain(1) = Space1.translate(-o5);
for e in A1D do e = next();
writeln("A1D: ", foo(TD1D, A1D, A1D), " errors");

const TD2D: domain(2) = Space2.translate(-o5, -o5);
for e in A2D do e = next();
writeln("A2D: ", foo(TD2D, A2D, A2D), " errors");

const TD3D: domain(3) = Space3.translate(-o5, -o5, -o5);
for e in A3D do e = next();
writeln("A3D: ", foo(TD3D, A3D, A3D), " errors");

const TD4D: domain(4) = Space4.translate(-o5, -o5, -o5, -o5);
for e in A4D do e = next();
writeln("A4D: ", foo(TD4D, A4D, A4D), " errors");

const TD2D64: domain(2,int(64)) = Space2D64.translate(-o5:int(64), -o5:int(64));
for e in A2D64 do e = next();
writeln("A2D64: ", foo(TD2D64, A2D64, A2D64), " errors");
