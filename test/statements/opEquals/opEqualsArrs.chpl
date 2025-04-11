var A: [1..3, 1..3] int = 1;

A += 1;
writeln(A);

A -= 1;
writeln(A);

A *= 6;
writeln(A);

A /= 2;
writeln(A);

A %= 2;
writeln(A);

A <<= 2;
writeln(A);

A |= 1;
writeln(A);

A >>= 2;
writeln(A);

A ^= 0b111;
writeln(A);

A &= 0b101;
writeln(A);

A **= 2;
writeln(A);

var B: [1..3, 1..3] bool = false;
B &&= true;
writeln(B);

B ||= true;
writeln(B);
