use LinkedList;

var s1 : linkedList(int)    = makeList( 2, 3, 4, 5 );
var s2 : linkedList(string) = makeList( "hot", "dog" );

writeln(s1, s2);

s2.destroy();
s1.destroy();
