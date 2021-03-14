config const n = 3 * 2**30;  // 3 gb is definitely > 2 hugepages
type T = int(8);
var A: [1..n] T;
config const i = n / 2;
config const val = 22:T;
A(i) = val;
writeln(A(i));
