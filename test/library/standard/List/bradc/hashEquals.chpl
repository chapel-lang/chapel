use LinkedLists;

var s: LinkedList(int) = makeList( 1, 2, 3 );

s.append(4);

writeln("s is: ", s);

s.destroy();
