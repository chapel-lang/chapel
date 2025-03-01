var A = [1, 2, 3];
var B = A: [1..3] int(32);
var C = A: [2..4] real(32);
var D = A: [3..5] string;
var E = A: [4..6] bool;

check(A);
check(B);
check(C);
check(D);
check(E);

proc check(arr) {
  writeln(arr, ": [", arr.domain, "] ", arr.eltType:string);
}
