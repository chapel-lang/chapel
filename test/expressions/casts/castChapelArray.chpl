var A = [1, 2, 3];
var B = A: [1..3] int(32);
var C = A: [2..4] real(32);
var D = A: [3..5] string;
var E = A: [4..6] bool;

var A2: [1..2, 1..2] real;
A2[1,1] = 1.1;
A2[1,2] = 1.2;
A2[2,1] = 2.1;
A2[2,2] = 2.2;
var B2 = A2: [0..1, 0..1] int(32);
var C2 = A2: [2..3, 2..3] real(32);
var D2 = A2: [3..4, 3..4] string;
var E2 = A2: [4..5, 4..5] bool;

check(A);
check(B);
check(C);
check(D);
check(E);
check(A2);
check(B2);
check(C2);
check(D2);
check(E2);

proc check(arr) {
  writeln(arr, ": [", arr.domain, "] ", arr.eltType:string);
}
