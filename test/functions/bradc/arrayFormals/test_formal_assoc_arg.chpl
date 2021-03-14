var D: domain(real) = {1.2, 3.4, 5.6};
var D2: domain(real) = {3.4, 5.6, 1.2};
var D3: domain(real) = {9.8, 7.3, 1.4};

var A: [D] real;
var B: [D2] real;
var C: [D3] real;

foo(A);
foo(B);
foo(C);

proc foo(A: [D] real) {
  writeln("In foo!");
}
