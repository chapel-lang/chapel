use samples;
// one defined in samples as new R(1)

var A:[1..2] R;
var B = A;
B[1] = one;
writeln(A[1].x);

// Does this program output 0 or 1?
// In other words, do B[1] and A[1] refer to different array elements?

