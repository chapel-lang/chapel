use samples;
// var one = new R(1);

var A:[1..2] R;
var B = A;        // initializes B to a copy of A
B[1] = one;
writeln(A[1]);

