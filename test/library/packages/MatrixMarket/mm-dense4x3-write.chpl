use MatrixMarket;
use IO;
use Sort;
use List;

var A:[1..4, 1..3] real;
A[1,1] = 1.0;
A[2,1] = 2.0;
A[3,1] = 3.0;
A[4,1] = 4.0;

A[1,2] = 5.0;
A[2,2] = 6.0;
A[3,2] = 7.0;
A[4,2] = 8.0;

A[1,3] = 9.0;
A[2,3] = 10.0;
A[3,3] = 11.0;
A[4,3] = 12.0;

writeln(A);

mmwrite("dense-4x3-write.mtx", A);

var fd = open("dense-4x3-write.mtx", iomode.r);
var lines = new list(string);
var i=1;
for l in fd.lines() {
  // the first two lines are header, not for sorting
  if i <= 2 then
    write(l);
  else
    lines.append(l);
  i += 1;
}
lines.sort();
for l in lines {
  write(l);
}
