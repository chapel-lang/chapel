proc f(x: [?d1] [?d2] real) {
  writeln("Got array of array of real with domains: ", (d1, d2));
}

proc f(x: [?D] real) {
  writeln("Got array of real with domain: ", D);
}

var A: [1..3] real;
var B: [1..3] [1..5] real;

f(A);
f(B);
