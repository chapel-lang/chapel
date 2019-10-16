use LinkedLists;

var s1 : LinkedList(int) = makeList( 3, 1, 4, 1, 5, 9 );
var s2 : LinkedList(int);

writeln(s1);

s2 = s1;
s1.append(2);

writeln(s2);
writeln(s1);

s1.destroy();
