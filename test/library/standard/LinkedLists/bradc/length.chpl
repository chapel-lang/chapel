use LinkedLists;

var s: LinkedList(int) = makeList( 1, 2, 3 );

writeln("s is: ", s);
writeln("s' length is: ", s.size);

s.destroy();

