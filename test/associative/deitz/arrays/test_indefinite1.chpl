use printHelp;

var d : domain(int);
d.add(2);
d.add(7);

var a : [d] real;

a(2) = 7.2;
a(7) = 2.7;

writelnSortedByDom(a);

writeln(a(2));
writeln(a(7));
