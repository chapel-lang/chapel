use driver;
config const shift = o5-1;

const TDom1D: domain(1) dmapped Dist1D = Space1.translate(-shift);
const TDom2D: domain(2) dmapped Dist2D = Space2.translate(-shift,-shift);
const TDom3D: domain(3) dmapped Dist3D = Space3.translate(-shift,-shift,-shift);
const TDom4D: domain(4) dmapped Dist4D = Space4.translate(-shift,-shift,-shift,-shift);
const TDom2D32: domain(2,int(32)) dmapped Dist2D32 = Space2D32.chpl__unTranslate(shift:int(32),shift:int(32));

var TA1D: [TDom1D] int;
var TA2D: [TDom2D] int;
var TA3D: [TDom3D] int;
var TA4D: [TDom4D] int;
var TA2D32: [TDom2D32] int;

const TD1D: domain(1) = Space1.translate(-o5);
const TD2D: domain(2) = Space2.translate(-o5,-o5);
const TD3D: domain(3) = Space3.translate(-o5,-o5,-o5);
const TD4D: domain(4) = Space4.translate(-o5,-o5,-o5,-o5);
const TD2D32: domain(2,int(32)) = Space2D32.chpl__unTranslate(o5:int(32),o5:int(32));

proc foo(TD: domain, A: [TD] int, TA) {
  var errs = 0;
  var offset = if (TD.rank==1) then (o5-shift):TD.idxType else fill(TD.rank, (o5-shift):TD.idxType);
  for i in TD do
    if A[i].locale != TA[i+offset].locale {
      writeln("A[",i,"] Incorrect reindex");
      errs += 1;
    }
  return errs;
}

for e in TA1D do e = next();
writeln("TA1D: ", foo(TD1D, TA1D.reindex(TD1D), TA1D), " errors");

for e in TA2D do e = next();
writeln("TA2D: ", foo(TD2D, TA2D.reindex(TD2D), TA2D), " errors");

for e in TA3D do e = next();
writeln("TA3D: ", foo(TD3D, TA3D.reindex(TD3D), TA3D), " errors");

for e in TA4D do e = next();
writeln("TA4D: ", foo(TD4D, TA4D.reindex(TD4D), TA4D), " errors");

for e in TA2D32 do e = next();
writeln("TA2D32: ", foo(TD2D32, TA2D32.reindex(TD2D32), TA2D32), " errors");
