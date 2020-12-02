use Itertools;

var A = [1, 2, 3, 4, 5];
var B = [1.1, 2.2, 3.3, 4.4, 5.5];

writeln(accumulate(A, operations.add));
writeln(accumulate(B, operations.add));
