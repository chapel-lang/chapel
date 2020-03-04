use Itertools;

var A = [1, 2, 3, 4, 5];
var B = [1.1, 2.2, 3.3, 4.4, 5.5];

writeln(accumulate(A, operations.subtract));
writeln(accumulate(B, operations.subtract));
