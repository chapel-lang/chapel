use Itertools;

var A = [1, 2, 3, 4, 5];
var B = [true, false, false, true, false];

writeln(accumulate(A, operations.bitwiseAnd));
writeln(accumulate(B, operations.bitwiseAnd));
