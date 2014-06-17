var f = lambda(x:int) { return x + 1; };
writeln(f(10));

var g = lambda() { return 5; };
writeln(g());

writeln((lambda() return 6;)() );
