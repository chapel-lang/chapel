config var a: integer = 4;
config var b: integer = 5;
config var x: integer = a + b;

writeln("a at compile time is 4");
writeln("b at compile time is 5");
writeln("x is initialized at compile time to a + b");
writeln("x at runtime is ", x);
