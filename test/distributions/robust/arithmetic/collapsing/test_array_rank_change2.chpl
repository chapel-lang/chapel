use driver_arrays;
const c = n4/2;

proc checkMe(i1, i2, i3, i4, A, j1, j2, j3) {
  var err = 0;
  if A4D(i1,i2,i3,i4) != A(j1,j2,j3) then
    err +=1;
  if A4D(i1,i2,i3,i4).locale != A(j1,j2,j3).locale then
    halt("aliased array element on unexpected locale");
  return err;
}

A4D = 1;

ref A31 = A4D(c, Dom4D.dim(1), Dom4D.dim(2), Dom4D.dim(3));
A31 += c;
var errs = 0;
for c2 in Dom4D.dim(1) {
  for c3 in Dom4D.dim(2) {
    for c4 in Dom4D.dim(3) {
      errs += checkMe(c,c2,c3,c4,A31,c2,c3,c4);
    }
  }
}
writeln("A31: ", errs, " errors");

ref A32 = A4D(Dom4D.dim(0), c, Dom4D.dim(2), Dom4D.dim(3));
A32 += c;
errs = 0;
for c1 in Dom4D.dim(0) {
  for c3 in Dom4D.dim(2) {
    for c4 in Dom4D.dim(3) {
      errs += checkMe(c1,c,c3,c4,A32,c1,c3,c4);
    }
  }
}
writeln("A32: ", errs, " errors");

ref A33 = A4D(Dom4D.dim(0), Dom4D.dim(1), c, Dom4D.dim(3));
A33 += c;
errs = 0;
for c1 in Dom4D.dim(0) {
  for c2 in Dom4D.dim(1) {
    for c4 in Dom4D.dim(3) {
      errs += checkMe(c1,c2,c,c4,A33,c1,c2,c4);
    }
  }
}
writeln("A33: ", errs, " errors");

ref A34 = A4D(Dom4D.dim(0), Dom4D.dim(1), Dom4D.dim(2), c);
A34 += c;
errs = 0;
for c1 in Dom4D.dim(0) {
  for c2 in Dom4D.dim(1) {
    for c3 in Dom4D.dim(2) {
      errs += checkMe(c1,c2,c3,c,A34,c1,c2,c3);
    }
  }
}
writeln("A34: ", errs, " errors");


