use LinkedLists;

var s1 : LinkedList(int)    = makeList( 2, 3, 4, 5 );
var s2 : LinkedList(string) = makeList( "hot", "dog" );

writeln(s1, s2);

s2.destroy();
s1.destroy();
