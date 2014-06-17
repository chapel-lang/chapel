var A: [1..9, 1..2] int;
var B: [1..3, 1..3, 1..2] int;

var total = + reduce (B + A);
writeln(total);
