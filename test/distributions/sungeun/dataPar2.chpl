config type myIntType=uint(64);
config const n:myIntType = 3;
const Space = {1:myIntType..1:myIntType,
               1:myIntType..(2:myIntType)*n,
               1:myIntType..n,
               1:myIntType..n,
               1:myIntType..n,
               1:myIntType..n};
var RA, RB: [Space] myIntType;
var A, B: [Space] myIntType;

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
for (ra, a) in zip(RA, A) do
  if ra!=a then err += 1;
if err>0 {
  writeln("Array A does not match (", err, " errors)");
  allerr += err;
  err = 0;
}
for (rb, b) in zip(RB, B) do
  if rb!=b then err += 1;
if err>0 {
  writeln("Array B does not match (", err, " errors)");
  allerr += err;
  err = 0;
}

if allerr==0 then writeln("OK"); else writeln("NO OK");
