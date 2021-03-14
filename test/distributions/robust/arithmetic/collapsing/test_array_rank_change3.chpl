use driver_arrays;
const c = n4/2;

proc checkMe(i1, i2, i3, i4, A, j1, j2) {
  var err = 0;
  if A4D(i1,i2,i3,i4) != A(j1,j2) then
    err +=1;
  if A4D(i1,i2,i3,i4).locale != A(j1,j2).locale then
    halt("aliased array element on unexpected locale");
  return err;
}

A4D = 1;

ref A212 = A4D(c, c, Dom4D.dim(2), Dom4D.dim(3));
A212 += c;
var errs = 0;
for c3 in Dom4D.dim(2) {
  for c4 in Dom4D.dim(3) {
    errs += checkMe(c,c,c3,c4,A212,c3,c4);
  }
}
writeln("A212: ", errs, " errors");

ref A213 = A4D(c, Dom4D.dim(1), c, Dom4D.dim(3));
A213 += c;
errs = 0;
for c2 in Dom4D.dim(1) {
  for c4 in Dom4D.dim(3) {
    errs += checkMe(c,c2,c,c4,A213,c2,c4);
  }
}
writeln("A213: ", errs, " errors");

ref A214 = A4D(c, Dom4D.dim(1), Dom4D.dim(2), c);
A214 += c;
errs = 0;
for c2 in Dom4D.dim(1) {
  for c3 in Dom4D.dim(2) {
    errs += checkMe(c,c2,c3,c,A214,c2,c3);
  }
}
writeln("A214: ", errs, " errors");

ref A223 = A4D(Dom4D.dim(0), c, c, Dom4D.dim(3));
A223 += c;
errs = 0;
for c1 in Dom4D.dim(0) {
  for c4 in Dom4D.dim(3) {
    errs += checkMe(c1,c,c,c4,A223,c1,c4);
  }
}
writeln("A223: ", errs, " errors");

ref A224 = A4D(Dom4D.dim(0), c, Dom4D.dim(2), c);
A224 += c;
errs = 0;
for c1 in Dom4D.dim(0) {
  for c3 in Dom4D.dim(2) {
    errs += checkMe(c1,c,c3,c,A224,c1,c3);
  }
}
writeln("A224: ", errs, " errors");

ref A234 = A4D(Dom4D.dim(0), Dom4D.dim(1), c, c);
A234 += c;
errs = 0;
for c1 in Dom4D.dim(0) {
  for c2 in Dom4D.dim(1) {
    errs += checkMe(c1,c2,c,c,A234,c1,c2);
  }
}
writeln("A234: ", errs, " errors");

