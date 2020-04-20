use Itertools;

var A = [1, 2, 3, 4, 5];
var B = [true, false, false, true, false];

writeln(accumulate(A, operations.bitwiseXor));
writeln(accumulate(B, operations.bitwiseXor));
