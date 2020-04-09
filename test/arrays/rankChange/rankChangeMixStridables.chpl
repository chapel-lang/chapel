config const n = 5;
config const it = 3;

var b: [0..n+1, 0..n+1, 0..1] bool;

b[2..n-1 by 2, 2..n-1, 0] = true;

writeln(b);
