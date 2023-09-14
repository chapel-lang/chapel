record R { var x: int; }

var A = [new R(45), new R(33)];

var B = A: R;

writeln(B);
