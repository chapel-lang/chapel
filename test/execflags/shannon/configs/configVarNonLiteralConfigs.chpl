config var a: int = 4;
config var b: int = 5;
config var x: int = a + b;

writeln("a at compile time is 4");
writeln("b at compile time is 5");
writeln("x is initialized at compile time to a + b");
writeln("x at runtime is ", x);
