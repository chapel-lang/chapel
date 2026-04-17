use PrivateDist;

proc foo(a: [PrivateSpace] real) {
  writeln("In foo");
}

var D: domain(1) dmapped new privateDist();

var A: [PrivateSpace] real;
var B: [D] real;

foo(A);
foo(B);

