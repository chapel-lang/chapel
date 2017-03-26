use rectDomainAPItest;


var D = {1..5, 1..5};

testRectDomainAPI2D("plain-old domain", D, {0..3, 4..6}, (0..6, 2..3), {2..3, 3..4});

var A: [1..6, 1..6] real;
foo("slice array's domain", A[1..5, 1..5]);

var B: [1..5, 1..10, 1..5] real;
foo("rank-change array's domain", B[.., 3, ..]);

var C: [0..4, 0..4] real;
foo("reindex array's domain", C.reindex({1..5, 1..5}));

proc foo(lbl, X: [?D]) {
  testRectDomainAPI2D(lbl, D, {0..3, 4..6}, (0..6, 2..3), {2..3, 3..4});
}