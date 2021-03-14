var A: [1..9, 1..9] real;
var B: [1..9] real;

proc assignViaZip(X, Y) {
  forall (x,y) in zip(X,Y) do
    x = y;
}

forall (i,j) in A.domain do
  A[i,j] = i + j/10.0;

forall i in B.domain do
  B[i] = i/10.0;

writeln(A, "\n");

assignViaZip(A[1..3, 1..3], A[4..6, 4..6]);
writeln(A, "\n");

assignViaZip(A[1..3, 1..3], A[7..9, 7..9].reindex(4..6, 4..6));
writeln(A, "\n");

assignViaZip(A[2, 1..3], B[1..3]);
writeln(A, "\n");

assignViaZip(A[1..3, 2], B[4..6]);
writeln(A, "\n");


forall (i,j) in A.domain do
  A[i,j] = i + j/10.0;

forall i in B.domain do
  B[i] = i/10.0;

writeln(A, "\n");

assignViaZip(A[4..6, 4..6], A[1..3, 1..3]);
writeln(A, "\n");

assignViaZip(A[7..9, 7..9].reindex(4..6, 4..6), A[1..3, 1..3]);
writeln(A, "\n");

assignViaZip(B[1..3], A[2, 1..3]);
writeln(B, "\n");

assignViaZip(B[4..6], A[1..3, 2]);
writeln(B, "\n");
