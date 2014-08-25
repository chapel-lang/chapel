proc foo(x: [], 
        min: x.eltType = min(x.eltType), 
        max: x.eltType = max(x.eltType))
    where isIntegralType(x.eltType) {
  x(1) = min;
  x(2) = max;
  writeln("x is: ", x);
}

var A: [1..2] int(8);
var B: [1..2] int(16);
var C: [1..2] int(32);
var D: [1..2] int(64);

foo(A, 0, 10);
foo(B, 0, 10);
foo(C, 0, 10);
foo(D, 0, 10);

foo(A);
foo(B);
foo(C);
foo(D);
