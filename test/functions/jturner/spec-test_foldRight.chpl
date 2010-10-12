use Functions;
var a:[1..4] int = (1, 2, 3, 4);
writeln(foldRight(a, 0, lambda(x:int, y:int) { return x - y; }));
