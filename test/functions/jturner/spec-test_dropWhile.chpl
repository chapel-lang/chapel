use Functions;
var a:[1..4] int = (1, 2, 3, 4);
writeln(dropWhile(a, lambda(x:int) { return x < 3; }));
