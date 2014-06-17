proc dothing(x:int, y:string) { writeln(y); return x+1; }

var g = dothing;

writeln(g(1, "bob"));
writeln(dothing(1, "bob"));
