var a: integer = 4;
var b: integer = 5;
config var x: integer = a + b;

writeln("a: ", a);
writeln("b: ", b);
writeln("x is initialized at compile time to a + b");
writeln("x at runtime is ", x);
