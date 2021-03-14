config const n = 10;

const D = {1..n, 1..n};
const Inner = {1..<n, 1..<n};
var A: [Inner] real;

writeln(D);
writeln(Inner);
writeln(A);
