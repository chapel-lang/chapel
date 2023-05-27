use CTypes;

var a, b: [0..<10] int;
a = 1;
b = 2;

writeln("a: ", a);
writeln("b: ", b);

writeln("copying a to b...");
c_memmove(c_ptrTo(b), c_ptrTo(a), 10 * c_sizeof(int));
writeln("a: ", a);
writeln("b: ", b);

writeln("setting b[0] to 3...");
b[0] = 3;
writeln("and comparing a and b: ", c_memcmp(c_ptrTo(a), c_ptrTo(b), 10 * c_sizeof(int)));

writeln("copying b to a...");
c_memcpy(c_ptrTo(a), c_ptrTo(b), 10 * c_sizeof(int));
writeln("a: ", a);
writeln("b: ", b);

writeln("setting a to zero...");
c_memset(c_ptrTo(a), 0, 10 * c_sizeof(int));
writeln("a: ", a);
