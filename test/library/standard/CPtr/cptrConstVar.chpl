use CPtr;

config const print = false;

var A: [1..10] real;
const B: [1..10] real;
var a: int;
const b: int;

proc noOp(x) {
  if print then
    writeln(x);
}

noOp(c_ptrTo(A));
noOp(c_ptrTo(B));
noOp(c_ptrTo(a));
noOp(c_ptrTo(b));

