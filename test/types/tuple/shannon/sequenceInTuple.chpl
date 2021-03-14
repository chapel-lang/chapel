use LinkedLists;

var s1 : LinkedList(int) = makeList( 1, 2, 3 );
var s2 : LinkedList(int) = makeList( 4, 5, 6 );

var s_tuple = (s1, s2);

writeln(s_tuple);

s2.destroy();
s1.destroy();

