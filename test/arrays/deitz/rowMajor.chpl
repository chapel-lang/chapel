use CPtr;
var D = {1..5, 1..5};
var A: [D] int;

var j = 1;
for i in D {
  A(i) = j;
  j += 1;
}

writeln(A);
const ptr = c_ptrTo(A);
for i in 0..#A.size {
  writeln(ptr[i]);
}
