
var D = {1..9, 1..9};
var SubD = {2..4, 2..4};

var A: [D] real;

fill(A);

proc fill(X: [?D]) {
  forall (i,j) in D do
    X[i,j] = i + j / 10.0;
}

foo(A[SubD]);

proc foo(X) {
  var A: X.type;
  var B: [X.domain] real;
  var C = -X;

  fill(A);
  fill(B);
  
  writeln(A, "\n");
  writeln(B, "\n");
  writeln(C, "\n");

  SubD = {2..5, 2..5};

  writeln(A, "\n");
  writeln(B, "\n");
  writeln(C, "\n");
}
