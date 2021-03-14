var a, b: [1..4] int;

// non-natural alignments need to work
a = 1;
b = 2;

b[1..4 by 2 align 0] = 3;
b[1..3 by 2 align 0] = 5;
writeln(b);

a[2..4 by 2 align 1] = b[1..3 by 2 align 0];
writeln(a);

// likewise, negative strides need to work
a = 1;
b = 2;

b[1..4 by -2] = 3;
writeln(b);

a[1..4 by -2 align 1] = b[1..3 by -2];
writeln(a);

// by contrast, the following works just fine already
a = 1;
b = 2;

b[2..4 by 2 align 0] = 3;
writeln(b);

a[1..3 by 2 align 1] = b[2..4 by 2 align 0];
writeln(a);
