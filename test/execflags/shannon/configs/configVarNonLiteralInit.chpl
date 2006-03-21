var a: int = 4;
var b: int = 5;
config var x: int = a + b;

writeln("a: ", a);
writeln("b: ", b);
writeln("x is initialized at compile time to a + b");
writeln("x at runtime is ", x);
