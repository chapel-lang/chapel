use printHelp;

var d : domain(real);
var a : [d] real;

d += 3.14;
d += 2.18;

a(3.14) = 1.0;
a(2.18) = 2.0;

writelnSortedByDom(a);
