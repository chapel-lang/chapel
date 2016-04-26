use List;

var s: list(int) = makeList( 1, 2, 3 );

writeln("s is: ", s);
writeln("s' length is: ", s.length);

s.destroy();

