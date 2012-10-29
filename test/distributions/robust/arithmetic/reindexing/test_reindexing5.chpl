use driver;
config const shift = o5-1;

const TDom1D: domain(1) dmapped Dist1D = Space1.translate(-shift);
const TDom2D: domain(2) dmapped Dist2D = Space2.translate(-shift,-shift);
const TDom3D: domain(3) dmapped Dist3D = Space3.translate(-shift,-shift,-shift);
const TDom4D: domain(4) dmapped Dist4D = Space4.translate(-shift,-shift,-shift,-shift);
const TDom2D64: domain(2,int(64)) dmapped Dist2D64 = Space2D64.translate(-shift:int(64),-shift:int(64));

var TA1D: [TDom1D] int;
var TA2D: [TDom2D] int;
var TA3D: [TDom3D] int;
var TA4D: [TDom4D] int;
var TA2D64: [TDom2D64] int;

const TD1D: domain(1) = Space1.translate(-o5);
const TD2D: domain(2) = Space2.translate(-o5,-o5);
const TD3D: domain(3) = Space3.translate(-o5,-o5,-o5);
const TD4D: domain(4) = Space4.translate(-o5,-o5,-o5,-o5);
const TD2D64: domain(2,int(64)) = Space2D64.translate(-o5:int(64),-o5:int(64));

proc foo(TD: domain, A: [TD] int, TA) {
  var errs = 0;
  var offset = if (TD.rank==1) then o5-shift else fill(TD.rank, o5-shift);
  for i in {TD} do
    if A[i].locale != TA[i+offset].locale {
      writeln("A[",i,"] Incorrect reindex");
      errs += 1;
    }
  return errs;
}

for e in TA1D do e = next();
writeln("TA1D: ", foo(TD1D, TA1D, TA1D), " errors");

for e in TA2D do e = next();
writeln("TA2D: ", foo(TD2D, TA2D, TA2D), " errors");

for e in TA3D do e = next();
writeln("TA3D: ", foo(TD3D, TA3D, TA3D), " errors");

for e in TA4D do e = next();
writeln("TA4D: ", foo(TD4D, TA4D, TA4D), " errors");

for e in TA2D64 do e = next();
writeln("TA2D64: ", foo(TD2D64, TA2D64, TA2D64), " errors");
