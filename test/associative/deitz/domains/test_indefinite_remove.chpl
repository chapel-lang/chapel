use printHelp;

var D: domain(real);

D += 2.0;
D += 3.14;
D += 8.26;

var A: [D] string;

A(2.0) = "two";
A(3.14) = "pi";
A(8.26) = "eight point twenty-six";

writelnSortedByDom(A);

D.remove(3.14);

writelnSortedByDom(A);
