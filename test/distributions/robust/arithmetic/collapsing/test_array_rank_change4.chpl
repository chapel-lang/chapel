use driver_arrays;
const c = n4/2;

proc checkMe(i1, i2, i3, i4, A, j1) {
  var err = 0;
  if A4D(i1,i2,i3,i4) != A(j1) then
    err +=1;
  if A4D(i1,i2,i3,i4).locale != A(j1).locale then
    halt("aliased array element on unexpected locale");
  return err;
}

A4D = 1;

ref A1234 = A4D(Dom4D.dim(0), c, c, c);
A1234 += c;
var errs = 0;
for c1 in Dom4D.dim(0) {
  errs += checkMe(c1,c,c,c,A1234,c1);
}
writeln("A1234: ", errs, " errors");

ref A1134 = A4D(c, Dom4D.dim(1), c, c);
A1134 += c;
errs = 0;
for c2 in Dom4D.dim(1) {
  errs += checkMe(c,c2,c,c,A1134,c2);
}
writeln("A1134: ", errs, " errors");

ref A1124 = A4D(c, c, Dom4D.dim(2), c);
A1124 += c;
errs = 0;
for c3 in Dom4D.dim(2) {
  errs += checkMe(c,c,c3,c,A1124,c3);
}
writeln("A1124: ", errs, " errors");

ref A1123 = A4D(c, c, c, Dom4D.dim(3));
A1123 += c;
errs = 0;
for c4 in Dom4D.dim(3) {
  errs += checkMe(c,c,c,c4,A1123,c4);
}
writeln("A1123: ", errs, " errors");

