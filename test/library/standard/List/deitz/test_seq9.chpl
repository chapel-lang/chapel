use List;

var s1 : list(int)    = makeList( 2, 3, 4, 5 );
var s2 : list(string) = makeList( "hot", "dog" );

writeln(s1, s2);

s2.destroy();
s1.destroy();
