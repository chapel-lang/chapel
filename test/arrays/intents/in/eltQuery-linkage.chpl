
proc foo(in D : domain(1), in A : [D] ?t) {
  D = {1..D.size*2};
  writeln(A.domain);
}

var D = {1..4};
var A : [D] int;
var B : [D] real;
foo(D, A);
foo(D, B);
