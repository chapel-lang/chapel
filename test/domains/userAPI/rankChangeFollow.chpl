var A: [0..8] real = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0];
const D = {1..9, 1..9};
var B: [1..9, 1..9] real;

proc foo(X, Y) {
  forall (x, y) in zip(X, Y) do
    x = y;
}

proc bar(X, Y) {
  forall (x, y) in zip(X, Y) do
    y = x;
}

foo(B[3, ..], A);
bar(A, B[.., 5]);

writeln(B);
writeln();

foo(A, D[3, ..]);
writeln(A);

bar(D[.., 5], A);
writeln(A);
