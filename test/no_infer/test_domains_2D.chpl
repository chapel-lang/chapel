var x = (0, 0);
config var n : int = 6;

var D : domain(2) = {1..n, 1..n};

writeln(D);

writeln("Iterating over D:");
writeln(D._forall());
