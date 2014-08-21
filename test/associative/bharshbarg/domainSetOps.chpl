var x = {"a", "b", "c", "d"};
var y = {"c","d","e","f"};
var z = {"c", "d"};

writeln("x = ", x);
writeln("y = ", y);
writeln("z = ", z);

writeln("Union: x | y = \n\t", x | y);
writeln("Intersection: x & y = \n\t", x & y);
writeln("Difference: x - y = \n\t", x - y);
writeln("XOR: x ^ y = \n\t", x ^ y);
writeln("x - x = ", x - x);
writeln("z is subset of x: ", x.subset(z));
writeln("z is subset of y: ", y.subset(z));

x |= y;
writeln("x |= y: \n\t", x);
x -= z;
writeln("x -= z: \n\t", x);
y &= z;
writeln("y &= z: \n\t", y);
x ^= y;
writeln("x ^= y: \n\t", x);
