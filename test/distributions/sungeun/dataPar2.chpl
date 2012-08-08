config const n = 3;
const Space = {1..1,1..2*n,1..n,1..n,1..n,1..n};
var RA, RB: [Space] int;
var A, B: [Space] int;

for i in Space {
  var (i0,i1,i2,i3,i4,i5) = (i);
  RA(i) = (i0+i1)/i2;
  RB(i) += RA(i)+(i3+i4)/i5;
}

writeln("### START ###");
forall i in Space {
  var (i0,i1,i2,i3,i4,i5) = (i);
  A(i) = (i0+i1)/i2;
  B(i) += A(i)+(i3+i4)/i5;
}
writeln("### END ###");

var allerr, err = 0;
for (ra, a) in (RA, A) do
  if ra!=a then err += 1;
if err>0 {
  writeln("Array A does not match (", err, " errors)");
  allerr += err;
  err = 0;
}
for (rb, b) in (RB, B) do
  if rb!=b then err += 1;
if err>0 {
  writeln("Array B does not match (", err, " errors)");
  allerr += err;
  err = 0;
}

if allerr==0 then writeln("OK"); else writeln("NO OK");
