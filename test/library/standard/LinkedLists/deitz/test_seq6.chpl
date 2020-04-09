use LinkedLists;

var s1 : LinkedList(int) = makeList(3, 1, 4, 1, 5, 9);
var s2             = makeList(2, 6, 5);

writeln(s1);
s1.concat(s2);
writeln(s1);

s2.destroy();
s1.destroy();

